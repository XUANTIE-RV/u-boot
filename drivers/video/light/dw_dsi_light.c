// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <clk.h>
#include <dm.h>
#include <dm/lists.h>
#include <dsi_host.h>
#include <generic-phy.h>
#include <mipi_dsi.h>
#include <panel.h>
#include <reset.h>
#include <video.h>
#include <video_bridge.h>

struct light_dw_dsi_priv {
	struct mipi_dsi_device dsi;
	fdt_addr_t base;
	struct udevice *panel;
	struct udevice *dsi_host;
	struct phy dphy;
	struct clk pix_clk;
};

#define dsi_device_to_priv(d)	container_of(d, struct light_dw_dsi_priv, dsi)

static int light_dw_dphy_init(void *priv_data)
{
	int ret;
	struct mipi_dsi_device *dsi = priv_data;
	struct light_dw_dsi_priv *priv = dsi_device_to_priv(dsi);

	ret = generic_phy_reset(&priv->dphy);
	if (ret) {
		pr_err("reset dphy failed: %d\n", ret);
		return ret;
	}

	ret = generic_phy_init(&priv->dphy);
	if (ret) {
		pr_err("init dphy failed: %d\n", ret);
		return ret;
	}

	ret = generic_phy_power_on(&priv->dphy);
	if (ret) {
		pr_err("power on dphy failed: %d\n", ret);
		return ret;
	}

	return 0;
}

static int light_dw_dphy_get_lane_mbps(void *priv_data,
			struct display_timing *timings,
			u32 lanes, u32 format, unsigned int *lane_mbps)
{
	/* TODO */
	*lane_mbps = 450;

	return 0;
}

static void light_dw_dphy_post_set_mode(void *priv_data, unsigned long mode_flags)
{
}

static const struct mipi_dsi_phy_ops light_dw_dphy_ops = {
	.init		= light_dw_dphy_init,
	.get_lane_mbps	= light_dw_dphy_get_lane_mbps,
	.post_set_mode	= light_dw_dphy_post_set_mode,
};

static int light_dw_dsi_attach(struct udevice *dev)
{
	int ret;
	struct display_timing timings;
	struct light_dw_dsi_priv *priv = dev_get_priv(dev);
	struct mipi_dsi_device *dsi = &priv->dsi;
	struct mipi_dsi_panel_plat *mplat;

	ret = uclass_first_device_err(UCLASS_PANEL, &priv->panel);
	if (ret) {
		dev_err(dev, "get panel device err : %d\n", ret);
		return ret;
	}

	mplat = dev_get_platdata(priv->panel);
	mplat->device = dsi;

	ret = panel_get_display_timing(priv->panel, &timings);
	if (ret) {
		dev_err(dev, "failed to get panel timing : %d\n", ret);
		return ret;
	}

	ret = uclass_first_device_err(UCLASS_DSI_HOST, &priv->dsi_host);
	if (ret) {
		dev_err(dev, "No video dsi host detected : %d\n", ret);
		return ret;
	}

	/* TODO: max data lanes */
	ret = dsi_host_init(priv->dsi_host, dsi, &timings,
			    4, &light_dw_dphy_ops);
	if (ret) {
		dev_err(dev, "failed to initialize DW DSI host : %d\n", ret);
		return ret;
	}

	return 0;
}

static int light_dw_dsi_set_backlight(struct udevice *dev, int percent)
{
	int ret;
	struct light_dw_dsi_priv *priv = dev_get_priv(dev);

	ret = panel_enable_backlight(priv->panel);
	if (ret) {
		dev_err(dev, "enable panel %s backlight err : %d\n",
			priv->panel->name, ret);
		return ret;
	}

	ret = dsi_host_enable(priv->dsi_host);
	if (ret) {
		dev_err(dev, "failed to enable DW DSI host : %d\n", ret);
		return ret;
	}

	return 0;
}

struct video_bridge_ops light_dw_dsi_ops = {
	.attach		= light_dw_dsi_attach,
	.set_backlight	= light_dw_dsi_set_backlight,
};

static int light_dw_dsi_ofdata_to_platdata(struct udevice *dev)
{
	int ret;
	struct light_dw_dsi_priv *priv = dev_get_priv(dev);

	ret = generic_phy_get_by_name(dev, "dphy", &priv->dphy);
	if (ret) {
		dev_err(dev, "Unable to get MIPI dphy: %d\n", ret);
		return ret;
	}

	ret = clk_get_by_name(dev, "pix-clk", &priv->pix_clk);
	if (ret) {
		dev_err(dev, "failed to get pixel clock: %d\n", ret);
		return ret;
	}

	return 0;
}

static int light_dw_dsi_probe(struct udevice *dev)
{
	/* TODO: enable power & clk */

	/* TODO: get max lanes from dtb or driver data */

	return 0;
}

static int light_dw_dsi_remove(struct udevice *dev)
{
	return 0;
}

static const struct udevice_id light_dw_dsi_ids[] = {
	{ .compatible = "thead,light-dsi-bridge" },
	{ /* sentinel */ }
};

U_BOOT_DRIVER(light_dw_dsi) = {
	.name			= "light-dw-dsi",
	.id			= UCLASS_VIDEO_BRIDGE,
	.of_match		= light_dw_dsi_ids,
	.probe			= light_dw_dsi_probe,
	.remove			= light_dw_dsi_remove,
	.ops			= &light_dw_dsi_ops,
	.ofdata_to_platdata	= light_dw_dsi_ofdata_to_platdata,
	.priv_auto_alloc_size	= sizeof(struct light_dw_dsi_priv),
};
