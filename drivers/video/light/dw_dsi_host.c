// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <clk.h>
#include <dsi_host.h>
#include <dm.h>
#include <regmap.h>
#include <syscon.h>

#include "dw_dsi.h"

struct dw_dsi_host_priv {
	struct mipi_dsi_host dsi_host;
	struct mipi_dsi_device *device;
	struct regmap *regmap;
	struct display_timing *timings;
	unsigned int lane_mbps; /* per lane */
	u32 channel;
	unsigned int max_data_lanes;
	const struct mipi_dsi_phy_ops *phy_ops;
};

#define dsi_host_to_priv(h)	container_of(h, struct dw_dsi_host_priv, dsi_host)
#define dsi_write(reg, val)	regmap_write(priv->regmap, reg, val)
#define dsi_read(reg, val)	regmap_read(priv->regmap, reg, val)
#define dsi_read_poll_timeout(reg, val, cond, timeout_ms)		\
	regmap_read_poll_timeout(priv->regmap, reg, val, cond, 1000, timeout_ms)

static int dw_mipi_dsi_host_attach(struct mipi_dsi_host *host,
				   struct mipi_dsi_device *device)
{
	struct dw_dsi_host_priv *priv = dsi_host_to_priv(host);

	if (device->lanes > priv->max_data_lanes)
		return -EINVAL;

	priv->channel = device->channel;

	return 0;
}

static void dw_mipi_message_config(struct dw_dsi_host_priv *priv,
				   const struct mipi_dsi_msg *msg)
{
	bool lpm = msg->flags & MIPI_DSI_MSG_USE_LPM;
	u32 val = 0;

	if (msg->flags & MIPI_DSI_MSG_REQ_ACK)
		val |= ACK_RQST_EN;
	if (lpm)
		val |= CMD_MODE_ALL_LP | MAX_RD_PKT_SIZE_LP;

	dsi_write(DSI_CMD_MODE_CFG, val);
}

static int dw_mipi_dsi_gen_pkt_hdr_write(struct dw_dsi_host_priv *priv,
					 u32 hdr_val)
{
	int ret;
	u32 val, mask;

	ret = dsi_read_poll_timeout(DSI_CMD_PKT_STATUS, val,
				    !(val & GEN_CMD_FULL),
				    CMD_PKT_STATUS_TIMEOUT_US);

	if (ret) {
		dev_err(device, "failed to get available command FIFO\n");
		return ret;
	}

	dsi_write(DSI_GEN_HDR, hdr_val);

	mask = GEN_CMD_EMPTY | GEN_PLD_W_EMPTY;
	ret = dsi_read_poll_timeout(DSI_CMD_PKT_STATUS, val,
				    (val & mask) == mask,
				    CMD_PKT_STATUS_TIMEOUT_US);
	if (ret) {
		dev_err(device, "failed to write command FIFO\n");
		return ret;
	}

	return 0;
}

static int dw_mipi_dsi_write(struct dw_dsi_host_priv *priv,
			     const struct mipi_dsi_packet *packet)
{
	u32 val;
	__le32 word;
	const u8 *tx_buf = packet->payload;
	int len = packet->payload_length, pld_data_bytes = sizeof(u32), ret;

	while (len) {
		if (len < pld_data_bytes) {
			word = 0;
			memcpy(&word, tx_buf, len);
			dsi_write(DSI_GEN_PLD_DATA, le32_to_cpu(word));
			len = 0;
		} else {
			memcpy(&word, tx_buf, pld_data_bytes);
			dsi_write(DSI_GEN_PLD_DATA, le32_to_cpu(word));
			tx_buf += pld_data_bytes;
			len -= pld_data_bytes;
		}

		ret = dsi_read_poll_timeout(DSI_CMD_PKT_STATUS, val,
					    !(val & GEN_PLD_W_FULL),
					    CMD_PKT_STATUS_TIMEOUT_US);
		if (ret) {
			dev_err(device, "failed to get write payload FIFO\n");
			return ret;
		}
	}

	word = 0;
	memcpy(&word, packet->header, sizeof(packet->header));

	return dw_mipi_dsi_gen_pkt_hdr_write(priv, le32_to_cpu(word));
}

static int dw_mipi_dsi_read(struct dw_dsi_host_priv *priv,
			    const struct mipi_dsi_msg *msg)
{
	u32 val;
	u8 *buf = msg->rx_buf;
	int i, j, ret, len = msg->rx_len;

	/* Wait end of the read operation */
	ret = dsi_read_poll_timeout(DSI_CMD_PKT_STATUS, val,
				    !(val & GEN_RD_CMD_BUSY),
				    CMD_PKT_STATUS_TIMEOUT_US);
	if (ret) {
		dev_err(device, "Timeout during read operation\n");
		return ret;
	}

	for (i = 0; i < len; i += 4) {
		/* Read fifo must not be empty before all bytes are read */
		ret = dsi_read_poll_timeout(DSI_CMD_PKT_STATUS, val,
					    !(val & GEN_PLD_R_EMPTY),
					    CMD_PKT_STATUS_TIMEOUT_US);
		if (ret) {
			dev_err(device, "Read payload FIFO is empty\n");
			return ret;
		}

		ret = dsi_read(DSI_GEN_PLD_DATA, &val);
		if (unlikely(ret))
			return ret;

		for (j = 0; j < 4 && j + i < len; j++)
			buf[i + j] = val >> (8 * j);
	}

	return 0;
}

static ssize_t dw_mipi_dsi_host_transfer(struct mipi_dsi_host *host,
				     const struct mipi_dsi_msg *msg)
{
	struct dw_dsi_host_priv *priv = dsi_host_to_priv(host);
	struct mipi_dsi_packet packet;
	int ret, nb_bytes;

	ret = mipi_dsi_create_packet(&packet, msg);
	if (ret) {
		dev_err(dev, "failed to create packet: %d\n", ret);
		return ret;
	}

	dw_mipi_message_config(priv, msg);

	ret = dw_mipi_dsi_write(priv, &packet);
	if (ret)
		return ret;

	if (msg->rx_buf && msg->rx_len) {
		ret = dw_mipi_dsi_read(priv, msg);
		if (ret)
			return ret;
		nb_bytes = msg->rx_len;
	} else {
		nb_bytes = packet.size;
	}

	return nb_bytes;
}

static const struct mipi_dsi_host_ops dw_mipi_dsi_host_ops = {
	.attach		= dw_mipi_dsi_host_attach,
	.transfer	= dw_mipi_dsi_host_transfer,
};

static void dw_mipi_dsi_dpi_config(struct dw_dsi_host_priv *priv)
{
	u32 dpi_cfg_pol = 0, dpi_color_coding = 0;
	struct mipi_dsi_device *device = priv->device;

	dsi_write(DSI_DPI_VCID, DPI_VCID(priv->channel));

	switch (device->format) {
	case MIPI_DSI_FMT_RGB888:
		dpi_color_coding = DPI_COLOR_CODING_24BIT;
		break;
	case MIPI_DSI_FMT_RGB666:
		dpi_color_coding = DPI_COLOR_CODING_18BIT_2 | LOOSELY18_EN;
		break;
	case MIPI_DSI_FMT_RGB666_PACKED:
		dpi_color_coding = DPI_COLOR_CODING_18BIT_1;
		break;
	case MIPI_DSI_FMT_RGB565:
		dpi_color_coding = DPI_COLOR_CODING_16BIT_1;
		break;
	}
	dsi_write(DSI_DPI_COLOR_CODING, dpi_color_coding);

	/* TODO: negotiate bus flags for DE polarity */
	if (priv->timings->flags & DISPLAY_FLAGS_VSYNC_LOW)
		dpi_cfg_pol |= VSYNC_ACTIVE_LOW;
	if (priv->timings->flags & DISPLAY_FLAGS_HSYNC_LOW)
		dpi_cfg_pol |= HSYNC_ACTIVE_LOW;
	dsi_write(DSI_DPI_CFG_POL, dpi_cfg_pol);

	/*
	 * TODO dw drv improvements
	 * largest packet sizes during hfp or during vsa/vpb/vfp
	 * should be computed according to byte lane, lane number and only
	 * if sending lp cmds in high speed is enable (PHY_TXREQUESTCLKHS)
	 */
	dsi_write(DSI_DPI_LP_CMD_TIM, OUTVACT_LPCMD_TIME(4) | INVACT_LPCMD_TIME(4));
}

static void dw_mipi_dsi_video_mode_config(struct dw_dsi_host_priv *priv)
{
	u32 vid_mode_cfg = 0;
	struct mipi_dsi_device *device = priv->device;

	/* configure low-power transitions */
	vid_mode_cfg |= LP_HFP_EN	|
			LP_HBP_EN	|
			LP_VACT_EN	|
			LP_VFP_EN	|
			LP_VBP_EN	|
			LP_VSA_EN;

        /* configure commands transfer in low power mode */
        vid_mode_cfg |= LP_CMD_EN;

        /* configure burs-mode or non-burst mode */
        if (device->mode_flags & MIPI_DSI_MODE_VIDEO_BURST)
                vid_mode_cfg |= VID_MODE_TYPE_BURST;
        else if (device->mode_flags & MIPI_DSI_MODE_VIDEO_SYNC_PULSE)
                vid_mode_cfg |= VID_MODE_TYPE_NON_BURST_SYNC_PULSES;
        else
                vid_mode_cfg |= VID_MODE_TYPE_NON_BURST_SYNC_EVENTS;

 #if 0
        /* enable colorbar mode */
        vid_mode_cfg |= VPG_ENABLE;
 #endif

        dsi_write(DSI_VID_MODE_CFG, vid_mode_cfg);
}

/* Get lane byte clock cycles. */
static u32 dw_mipi_dsi_get_hcomponent_lbcc(struct dw_dsi_host_priv *priv,
					   u32 hcomponent)
{
	u32 frac, lbcc;
	struct display_timing *timings = priv->timings;

	lbcc = hcomponent * priv->lane_mbps * 1000 / 8;

	frac = lbcc % (timings->pixelclock.typ / 1000);
	lbcc = lbcc / (timings->pixelclock.typ / 1000);
	if (frac)
		lbcc++;

	return lbcc;
}

static void dw_mipi_dsi_timing_config(struct dw_dsi_host_priv *priv)
{
	u32 htotal, hsa, hbp, lbcc;
	struct display_timing *timings = priv->timings;

	htotal = timings->hactive.typ + timings->hfront_porch.typ +
		 timings->hback_porch.typ + timings->hsync_len.typ;
	hsa = timings->hsync_len.typ;
	hbp = timings->hback_porch.typ;

	/* horizontal */
	lbcc = dw_mipi_dsi_get_hcomponent_lbcc(priv, htotal);
	dsi_write(DSI_VID_HLINE_TIME, lbcc);

	lbcc = dw_mipi_dsi_get_hcomponent_lbcc(priv, hsa);
	dsi_write(DSI_VID_HSA_TIME, lbcc);

	lbcc = dw_mipi_dsi_get_hcomponent_lbcc(priv, hbp);
	dsi_write(DSI_VID_HBP_TIME, lbcc);

	dsi_write(DSI_VID_PKT_SIZE, VID_PKT_SIZE(timings->hactive.typ));

	/* vertical */
	dsi_write(DSI_VID_VACTIVE_LINES, timings->vactive.typ);
	dsi_write(DSI_VID_VSA_LINES, timings->vsync_len.typ);
	dsi_write(DSI_VID_VFP_LINES, timings->vfront_porch.typ);
	dsi_write(DSI_VID_VBP_LINES, timings->vback_porch.typ);
}

static void dw_mipi_dsi_packet_handler_config(struct dw_dsi_host_priv *priv)
{
	u32 pkthdl_cfg;

	pkthdl_cfg = EOTP_TX_LP_EN | EOTP_TX_EN | BTA_EN;
	dsi_write(DSI_PCKHDL_CFG, pkthdl_cfg);
}

static void dw_mipi_dsi_command_mode_config(struct dw_dsi_host_priv *priv)
{
	u32 cmd_mode_cfg;

	/*
	 * TODO dw drv improvements
	 * compute high speed transmission counter timeout according
	 * to the timeout clock division (TO_CLK_DIVISION) and byte lane...
	 */
	dsi_write(DSI_TO_CNT_CFG, HSTX_TO_CNT(1000) | LPRX_TO_CNT(1000));
	/*
	 * TODO dw drv improvements
	 * the Bus-Turn-Around Timeout Counter should be computed
	 * according to byte lane...
	 */
	dsi_write(DSI_BTA_TO_CNT, 0xd00);

	/* TODO use low power mode to transfer
	 * commands in command mode in default
	 */
	cmd_mode_cfg = MAX_RD_PKT_SIZE_LP   |
		       DCS_LW_TX_LP         |
		       DCS_SR_0P_TX_LP      |
		       DCS_SW_1P_TX_LP      |
		       DCS_SW_0P_TX_LP      |
		       GEN_LW_TX_LP         |
		       GEN_SR_2P_TX_LP      |
		       GEN_SR_1P_TX_LP      |
		       GEN_SR_0P_TX_LP      |
		       GEN_SW_2P_TX_LP      |
		       GEN_SW_1P_TX_LP      |
		       GEN_SW_0P_TX_LP;
	dsi_write(DSI_CMD_MODE_CFG, cmd_mode_cfg);
}

static int dsi_mode_vrefresh(struct display_timing *timings)
{
	int refresh = 0;
	unsigned int calc_val;
	u32 htotal = timings->hactive.typ + timings->hfront_porch.typ +
		     timings->hback_porch.typ + timings->hsync_len.typ;
	u32 vtotal = timings->vactive.typ + timings->vfront_porch.typ +
		     timings->vback_porch.typ + timings->vsync_len.typ;

	if (htotal > 0 && vtotal > 0) {
		calc_val = timings->pixelclock.typ;
		calc_val /= htotal;
		refresh = (calc_val + vtotal / 2) / vtotal;
	}

	return refresh;
}

/*
 * The controller should generate 2 frames before
 * preparing the peripheral.
 */
static void dw_mipi_dsi_wait_for_two_frames(struct display_timing *timings)
{
	int refresh, two_frames;

	refresh = dsi_mode_vrefresh(timings);
	two_frames = DIV_ROUND_UP(1000, refresh) * 2;
	mdelay(two_frames);
}

static void dw_mipi_dsi_set_mode(struct dw_dsi_host_priv *priv,
                                 unsigned long mode_flags)
{
	u32 mode_cfg;

	mode_cfg = mode_flags & MIPI_DSI_MODE_VIDEO ?
		   ENABLE_VIDEO_MODE : ENABLE_CMD_MODE;
	dsi_write(DSI_MODE_CFG, mode_cfg);
}

static void dw_mipi_dsi_power_config(struct dw_dsi_host_priv *priv,
				     bool on)
{
	dsi_write(DSI_PWR_UP, on ? POWERUP : RESET);
}

static void dw_mipi_dsi_set_esc_div(struct dw_dsi_host_priv *priv)
{
	u8 esc_clk;
	u32 esc_clk_division;

	esc_clk = 20; /* 20MHz */

	/*
	 * The maximum permitted escape clock is 20MHz and it is derived from
	 * lanebyteclk, which is running at "lane_link_rate / 8".  Thus we want:
	 *
	 *     (lane_link_rate >> 3) / esc_clk_division < 20
	 * which is:
	 *     (lane_link_rate >> 3) / 20 > esc_clk_division
	 */
	esc_clk_division = (priv->lane_mbps >> 3) / esc_clk + 1;

	/*
	 * TODO dw drv improvements
	 * timeout clock division should be computed with the
	 * high speed transmission counter timeout and byte lane...
	 */
	dsi_write(DSI_CLKMGR_CFG, TO_CLK_DIVISION(10) |
		TX_ESC_CLK_DIVISION(esc_clk_division));
}

static int dw_mipi_dsi_host_init(struct udevice *dev)
{
	int ret;
	struct dw_dsi_host_priv *priv = dev_get_priv(dev);
	struct mipi_dsi_device *device = priv->device;
	const struct mipi_dsi_phy_ops *phy_ops = priv->phy_ops;
	struct display_timing *timings = priv->timings;

	ret = phy_ops->get_lane_mbps(device, timings, device->lanes,
				     device->format, &priv->lane_mbps);
	if (ret) {
		dev_err(dev, "Phy get_lane_mbps() failed\n");
		return ret;
	}

	dw_mipi_dsi_dpi_config(priv);
	dw_mipi_dsi_video_mode_config(priv);
	dw_mipi_dsi_timing_config(priv);
	dw_mipi_dsi_packet_handler_config(priv);
	dw_mipi_dsi_command_mode_config(priv);
	dw_mipi_dsi_set_esc_div(priv);

	ret = phy_ops->init(device);
	if (ret) {
		dev_err(dev, "Phy init() failed: %d\n", ret);
		return ret;
	}

	dw_mipi_dsi_wait_for_two_frames(timings);

	/* Switch to cmd mode for panel-bridge pre_enable & panel prepare */
	dw_mipi_dsi_set_mode(priv, 0);
	dw_mipi_dsi_power_config(priv, true);

	return 0;
}

static int dw_dsi_host_init(struct udevice *dev,
			    struct mipi_dsi_device *device,
			    struct display_timing *timings,
			    unsigned int max_data_lanes,
			    const struct mipi_dsi_phy_ops *phy_ops)
{
	int ret;
	struct dw_dsi_host_priv *priv = dev_get_priv(dev);

	if (!device)
		return -ENODEV;

	if (!phy_ops->init || !phy_ops->get_lane_mbps) {
		dev_err(device->dev, "Phy not properly configured\n");
		return -ENODEV;
	}

	priv->phy_ops = phy_ops;
	priv->max_data_lanes = max_data_lanes;
	priv->device = device;
	priv->dsi_host.ops = &dw_mipi_dsi_host_ops;
	device->host = &priv->dsi_host;
	priv->timings = timings;

	/* initialize dsi host and dphy */
	ret = dw_mipi_dsi_host_init(dev);
	if (ret) {
		dev_err(device->dev, "init dw dsi host failed: %d\n", ret);
		return ret;
	}

	return 0;
}

static void dw_mipi_dsi_lpclk_config(struct dw_dsi_host_priv *priv,
				     bool hsclk)
{
	u32 lpclk_ctrl = 0;
	struct mipi_dsi_device *device = priv->device;

	lpclk_ctrl |= hsclk ? PHY_TXREQUESTCLKHS : 0;
	lpclk_ctrl |= device->mode_flags & MIPI_DSI_CLOCK_NON_CONTINUOUS ?
					   AUTO_CLKLANE_CTRL : 0;

        dsi_write(DSI_LPCLK_CTRL, lpclk_ctrl);
}

static int dw_dsi_host_enable(struct udevice *dev)
{
	struct dw_dsi_host_priv *priv = dev_get_priv(dev);

	/* Switch to video mode for panel-bridge enable & panel enable */
	dw_mipi_dsi_set_mode(priv, MIPI_DSI_MODE_VIDEO);
	dw_mipi_dsi_lpclk_config(priv, true);

	return 0;
}

static int dw_dsi_host_disable(struct udevice *dev)
{
	return 0;
}

struct dsi_host_ops dw_dsi_host_ops = {
	.init		= dw_dsi_host_init,
	.enable		= dw_dsi_host_enable,
	.disable	= dw_dsi_host_disable,
};

static int dw_dsi_host_ofdata_to_platdata(struct udevice *dev)
{
	struct dw_dsi_host_priv *priv = dev_get_priv(dev);

	priv->regmap = syscon_regmap_lookup_by_phandle(dev, "regmap");
	return IS_ERR_OR_NULL(priv->regmap) ? -ENODEV : 0;
}

static int dw_dsi_host_probe(struct udevice *dev)
{
	return 0;
}

static int dw_dsi_host_remove(struct udevice *dev)
{
	return 0;
}

static const struct udevice_id dw_dsi_host_ids[] = {
	{ .compatible = "synopsys,dw-mipi-dsi" },
	{ /* sentinel */ }
};

U_BOOT_DRIVER(dw_dsi_host) = {
	.name			= "dw_dsi_host",
	.id			= UCLASS_DSI_HOST,
	.of_match		= dw_dsi_host_ids,
	.probe			= dw_dsi_host_probe,
	.remove			= dw_dsi_host_remove,
	.ops			= &dw_dsi_host_ops,
	.ofdata_to_platdata	= dw_dsi_host_ofdata_to_platdata,
	.priv_auto_alloc_size	= sizeof(struct dw_dsi_host_priv),
};
