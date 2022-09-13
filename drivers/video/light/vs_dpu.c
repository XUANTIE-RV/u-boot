// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <asm/io.h>
#include <dm.h>
#include <dm/device-internal.h>
#include <panel.h>
#include <video.h>
#include <video_bridge.h>
#include <thead/clock_config.h>

#include "../videomodes.h"

#define FB_MAX_WIDTH		800
#define FB_MAX_HEIGHT		1280

/* dpu registers */
#define AQ_INTR_ACKNOWLEDGE        0x0010
#define AQ_INTR_ENBL               0x0014
#define DC_HW_REVISION             0x0024
#define DC_HW_CHIP_CID             0x0030

#define DC_FRAMEBUFFER_CONFIG           0x1518
#define DC_FRAMEBUFFER_CONFIG_EX        0x1CC0
#define DC_FRAMEBUFFER_SCALE_CONFIG     0x1520
#define DC_FRAMEBUFFER_TOP_LEFT         0x24D8
#define DC_FRAMEBUFFER_BOTTOM_RIGHT     0x24E0
#define DC_FRAMEBUFFER_ADDRESS          0x1400
#define DC_FRAMEBUFFER_U_ADDRESS        0x1530
#define DC_FRAMEBUFFER_V_ADDRESS        0x1538
#define DC_FRAMEBUFFER_STRIDE           0x1408
#define DC_FRAMEBUFFER_U_STRIDE         0x1800
#define DC_FRAMEBUFFER_V_STRIDE         0x1808
#define DC_FRAMEBUFFER_SIZE		0x1810
#define DC_FRAMEBUFFER_SCALE_FACTOR_X       0x1828
#define DC_FRAMEBUFFER_SCALE_FACTOR_Y       0x1830
#define DC_FRAMEBUFFER_H_FILTER_COEF_INDEX  0x1838
#define DC_FRAMEBUFFER_H_FILTER_COEF_DATA   0x1A00
#define DC_FRAMEBUFFER_V_FILTER_COEF_INDEX  0x1A08
#define DC_FRAMEBUFFER_V_FILTER_COEF_DATA   0x1A10
#define DC_FRAMEBUFFER_INIT_OFFSET          0x1A20
#define DC_FRAMEBUFFER_COLOR_KEY            0x1508
#define DC_FRAMEBUFFER_COLOR_KEY_HIGH       0x1510
#define DC_FRAMEBUFFER_CLEAR_VALUE          0x1A18
#define DC_FRAMEBUFFER_COLOR_TABLE_INDEX    0x1818
#define DC_FRAMEBUFFER_COLOR_TABLE_DATA     0x1820
#define DC_FRAMEBUFFER_BG_COLOR             0x1528
#define DC_FRAMEBUFFER_ROI_ORIGIN           0x1CB0
#define DC_FRAMEBUFFER_ROI_SIZE             0x1CB8
#define DC_FRAMEBUFFER_WATER_MARK           0x1CE8
#define DC_FRAMEBUFFER_DEGAMMA_INDEX        0x1D88
#define DC_FRAMEBUFFER_DEGAMMA_DATA         0x1D90
#define DC_FRAMEBUFFER_DEGAMMA_EX_DATA      0x1D98
#define DC_FRAMEBUFFER_YUVTORGB_COEF0       0x1DA0
#define DC_FRAMEBUFFER_YUVTORGB_COEF1       0x1DA8
#define DC_FRAMEBUFFER_YUVTORGB_COEF2       0x1DB0
#define DC_FRAMEBUFFER_YUVTORGB_COEF3       0x1DB8
#define DC_FRAMEBUFFER_YUVTORGB_COEF4       0x1E00
#define DC_FRAMEBUFFER_YUVTORGB_COEFD0      0x1E08
#define DC_FRAMEBUFFER_YUVTORGB_COEFD1      0x1E10
#define DC_FRAMEBUFFER_YUVTORGB_COEFD2      0x1E18
#define DC_FRAMEBUFFER_Y_CLAMP_BOUND        0x1E88
#define DC_FRAMEBUFFER_UV_CLAMP_BOUND       0x1E90
#define DC_FRAMEBUFFER_RGBTORGB_COEF0       0x1E20
#define DC_FRAMEBUFFER_RGBTORGB_COEF1       0x1E28
#define DC_FRAMEBUFFER_RGBTORGB_COEF2       0x1E30
#define DC_FRAMEBUFFER_RGBTORGB_COEF3       0x1E38
#define DC_FRAMEBUFFER_RGBTORGB_COEF4       0x1E40
#define DC_FRAMEBUFFER_BLEND_CONFIG         0x2510
#define DC_FRAMEBUFFER_SRC_GLOBAL_COLOR     0x2500
#define DC_FRAMEBUFFER_DST_GLOBAL_COLOR     0x2508

#define DC_OVERLAY_CONFIG            0x1540
#define DC_OVERLAY_CONFIG_EX         0x2540
#define DC_OVERLAY_SCALE_CONFIG      0x1C00
#define DC_OVERLAY_BLEND_CONFIG      0x1580
#define DC_OVERLAY_TOP_LEFT          0x1640
#define DC_OVERLAY_BOTTOM_RIGHT      0x1680
#define DC_OVERLAY_ADDRESS           0x15C0
#define DC_OVERLAY_U_ADDRESS         0x1840
#define DC_OVERLAY_V_ADDRESS         0x1880
#define DC_OVERLAY_STRIDE            0x1600
#define DC_OVERLAY_U_STRIDE          0x18C0
#define DC_OVERLAY_V_STRIDE          0x1900
#define DC_OVERLAY_SIZE              0x17C0
#define DC_OVERLAY_SCALE_FACTOR_X    0x1A40
#define DC_OVERLAY_SCALE_FACTOR_Y    0x1A80
#define DC_OVERLAY_H_FILTER_COEF_INDEX      0x1AC0
#define DC_OVERLAY_H_FILTER_COEF_DATA       0x1B00
#define DC_OVERLAY_V_FILTER_COEF_INDEX      0x1B40
#define DC_OVERLAY_V_FILTER_COEF_DATA       0x1B80
#define DC_OVERLAY_INIT_OFFSET              0x1BC0
#define DC_OVERLAY_COLOR_KEY                0x1740
#define DC_OVERLAY_COLOR_KEY_HIGH           0x1780
#define DC_OVERLAY_CLEAR_VALUE              0x1940
#define DC_OVERLAY_COLOR_TABLE_INDEX        0x1980
#define DC_OVERLAY_COLOR_TABLE_DATA         0x19C0
#define DC_OVERLAY_SRC_GLOBAL_COLOR         0x16C0
#define DC_OVERLAY_DST_GLOBAL_COLOR         0x1700
#define DC_OVERLAY_ROI_ORIGIN               0x1D00
#define DC_OVERLAY_ROI_SIZE                 0x1D40
#define DC_OVERLAY_WATER_MARK               0x1DC0
#define DC_OVERLAY_DEGAMMA_INDEX            0x2200
#define DC_OVERLAY_DEGAMMA_DATA             0x2240
#define DC_OVERLAY_DEGAMMA_EX_DATA          0x2280
#define DC_OVERLAY_YUVTORGB_COEF0           0x1EC0
#define DC_OVERLAY_YUVTORGB_COEF1           0x1F00
#define DC_OVERLAY_YUVTORGB_COEF2           0x1F40
#define DC_OVERLAY_YUVTORGB_COEF3           0x1F80
#define DC_OVERLAY_YUVTORGB_COEF4           0x1FC0
#define DC_OVERLAY_YUVTORGB_COEFD0          0x2000
#define DC_OVERLAY_YUVTORGB_COEFD1          0x2040
#define DC_OVERLAY_YUVTORGB_COEFD2          0x2080
#define DC_OVERLAY_Y_CLAMP_BOUND            0x22C0
#define DC_OVERLAY_UV_CLAMP_BOUND           0x2300
#define DC_OVERLAY_RGBTORGB_COEF0           0x20C0
#define DC_OVERLAY_RGBTORGB_COEF1           0x2100
#define DC_OVERLAY_RGBTORGB_COEF2           0x2140
#define DC_OVERLAY_RGBTORGB_COEF3           0x2180
#define DC_OVERLAY_RGBTORGB_COEF4           0x21C0

#define DC_DISPLAY_DITHER_CONFIG        0x1410
#define DC_DISPLAY_PANEL_CONFIG         0x1418
#define DC_DISPLAY_PANEL_CONFIG_EX      0x2518
#define DC_DISPLAY_DITHER_TABLE_LOW     0x1420
#define DC_DISPLAY_DITHER_TABLE_HIGH    0x1428
#define DC_DISPLAY_H                    0x1430
#define DC_DISPLAY_H_SYNC               0x1438
#define DC_DISPLAY_V                    0x1440
#define DC_DISPLAY_V_SYNC               0x1448
#define DC_DISPLAY_CURRENT_LOCATION     0x1450
#define DC_DISPLAY_GAMMA_INDEX          0x1458
#define DC_DISPLAY_GAMMA_DATA           0x1460
#define DC_DISPLAY_INT                  0x147C
#define DC_DISPLAY_INT_ENABLE           0x1480
#define DC_DISPLAY_DBI_CONFIG           0x1488
#define DC_DISPLAY_GENERAL_CONFIG       0x14B0
#define DC_DISPLAY_DPI_CONFIG           0x14B8
#define DC_DISPLAY_PANEL_START          0x1CCC
#define DC_DISPLAY_DEBUG_COUNTER_SELECT     0x14D0
#define DC_DISPLAY_DEBUG_COUNTER_VALUE      0x14D8
#define DC_DISPLAY_DP_CONFIG                0x1CD0
#define DC_DISPLAY_GAMMA_EX_INDEX           0x1CF0
#define DC_DISPLAY_GAMMA_EX_DATA            0x1CF8
#define DC_DISPLAY_GAMMA_EX_ONE_DATA        0x1D80
#define DC_DISPLAY_RGBTOYUV_COEF0           0x1E48
#define DC_DISPLAY_RGBTOYUV_COEF1           0x1E50
#define DC_DISPLAY_RGBTOYUV_COEF2           0x1E58
#define DC_DISPLAY_RGBTOYUV_COEF3           0x1E60
#define DC_DISPLAY_RGBTOYUV_COEF4           0x1E68
#define DC_DISPLAY_RGBTOYUV_COEFD0          0x1E70
#define DC_DISPLAY_RGBTOYUV_COEFD1          0x1E78
#define DC_DISPLAY_RGBTOYUV_COEFD2          0x1E80

struct vs_dpu_priv {
	fdt_addr_t base;
	struct udevice *disp_dev;
	struct udevice *panel;
	struct udevice *video_bridge;
	struct display_timing timing;
	unsigned int fb_format;
	unsigned int bus_format;
};

static unsigned int dpu_read(struct vs_dpu_priv *priv, unsigned int reg)
{
	return readl((void *)priv->base + reg);
}

static void dpu_write(struct vs_dpu_priv *priv,
		      unsigned int reg, unsigned int val)
{
	writel(val, (void *)priv->base + reg);
}

static void dpu_set_clear(struct vs_dpu_priv *priv, unsigned int reg,
			  unsigned int set, unsigned int clr)
{
	unsigned int value = dpu_read(priv, reg);

	value &= ~clr;
	value |= set;
	dpu_write(priv, reg, value);
}

static int vs_dpu_crtc_setup(struct udevice *dev)
{
	unsigned int display_h, display_h_sync;
	unsigned int display_v, display_v_sync;
	unsigned int htotal, hsync_start, hsync_end;
	unsigned int vtotal, vsync_start, vsync_end;
	struct vs_dpu_priv *priv = dev_get_priv(dev);
	struct display_timing *timing = &priv->timing;

	/* TODO: only support panel0, DPI and layer0 now */
	dpu_set_clear(priv, DC_DISPLAY_DP_CONFIG, 0, BIT(3));
	dpu_write(priv, DC_DISPLAY_DPI_CONFIG, 5);

	hsync_start = timing->hactive.typ + timing->hfront_porch.typ;
	hsync_end   = hsync_start + timing->hsync_len.typ;
	htotal      = hsync_end + timing->hback_porch.typ;

	vsync_start = timing->vactive.typ + timing->vfront_porch.typ;
	vsync_end   = vsync_start + timing->vsync_len.typ;
	vtotal      = vsync_end + timing->vback_porch.typ;

	display_h = timing->hactive.typ | (htotal << 16);
	dpu_write(priv, DC_DISPLAY_H, display_h);

	display_h_sync = hsync_start | (hsync_end << 15) | BIT(31) | BIT(30);
	dpu_write(priv, DC_DISPLAY_H_SYNC, display_h_sync);

	display_v = timing->vactive.typ | (vtotal << 16);
	dpu_write(priv, DC_DISPLAY_V, display_v);

	display_v_sync = vsync_start | (vsync_end << 15) | BIT(31) | BIT(30);
	dpu_write(priv, DC_DISPLAY_V_SYNC, display_v_sync);

	dpu_set_clear(priv, DC_DISPLAY_PANEL_CONFIG, BIT(12) | BIT(0) | BIT(4), BIT(16));
	dpu_set_clear(priv, DC_DISPLAY_PANEL_START, BIT(0), BIT(3));

	return 0;
}

static int vs_dpu_plane_setup(struct udevice *dev)
{
	struct vs_dpu_priv *priv = dev_get_priv(dev);
	struct display_timing *timing = &priv->timing;
	struct video_uc_platdata *plat = dev_get_uclass_platdata(dev);

	/* disable shadow register */
	dpu_set_clear(priv, DC_FRAMEBUFFER_CONFIG_EX, 0, BIT(12));
	dpu_set_clear(priv, DC_DISPLAY_PANEL_CONFIG_EX, BIT(0), 0);

	/* config alpha blending */
	dpu_write(priv, DC_FRAMEBUFFER_SRC_GLOBAL_COLOR, 0xff000000);
	dpu_write(priv, DC_FRAMEBUFFER_DST_GLOBAL_COLOR, 0xff000000);
	dpu_write(priv, DC_FRAMEBUFFER_BLEND_CONFIG, 0x3450);

	dpu_write(priv, DC_FRAMEBUFFER_ADDRESS, plat->base);
	dpu_write(priv, DC_FRAMEBUFFER_STRIDE, timing->hactive.typ * 4);
	dpu_write(priv, DC_FRAMEBUFFER_SIZE, timing->hactive.typ | (timing->vactive.typ << 15));
	dpu_write(priv, DC_FRAMEBUFFER_CONFIG, 5 << 26);
	dpu_set_clear(priv, DC_FRAMEBUFFER_CONFIG_EX, BIT(13), 0);
	dpu_write(priv, DC_FRAMEBUFFER_TOP_LEFT, 0x0);
	dpu_write(priv, DC_FRAMEBUFFER_BOTTOM_RIGHT, timing->hactive.typ | (timing->vactive.typ << 15));

	/* enable shadow register */
	dpu_set_clear(priv, DC_FRAMEBUFFER_CONFIG_EX, BIT(12), 0);
	dpu_set_clear(priv, DC_DISPLAY_PANEL_CONFIG_EX, 0, BIT(0));

	return 0;
}

static int vs_dpu_init(struct udevice *dev)
{
	int ret;

	ret = vs_dpu_crtc_setup(dev);
	if (ret)
		return ret;

	ret = vs_dpu_plane_setup(dev);
	if (ret)
		return ret;

	return 0;
}

static int vs_dpu_video_bind(struct udevice *dev)
{
	struct video_uc_platdata *plat = dev_get_uclass_platdata(dev);

	/* maximum fb size to be allocated later */
	plat->size = FB_MAX_WIDTH * FB_MAX_HEIGHT *
		     ((1 << VIDEO_BPP32) >> 3);

	return 0;
}

static int vs_dpu_video_probe(struct udevice *dev)
{
	struct video_uc_platdata *plat = dev_get_uclass_platdata(dev);
	struct video_priv *uc_priv = dev_get_uclass_priv(dev);
	struct vs_dpu_priv *priv = dev_get_priv(dev);
	u32 bpp = VIDEO_BPP32;
	int ret;

	dev_info(dev, "%s() plat: base 0x%lx, size 0x%x\n",
		 __func__, plat->base, plat->size);

	priv->base = dev_read_addr(dev);
	if (priv->base == FDT_ADDR_T_NONE) {
		dev_err(dev, "VS DPU base address is not found\n");
		ret = -EINVAL;
		goto error;
	}

	/* TODO: make sure apb clock is on */

	ret = uclass_first_device_err(UCLASS_PANEL, &priv->panel);
	if (ret) {
		dev_err(dev, "LCD panel cannot be found : %d\n", ret);
		goto error;
	}

	ret = panel_get_display_timing(priv->panel, &priv->timing);
	if (ret) {
		ret = ofnode_decode_display_timing(dev_ofnode(dev),
				0, &priv->timing);
		if (ret) {
			dev_err(dev, "failed to get panel timing : %d\n", ret);
			goto error;
		}
	}

	if (IS_ENABLED(CONFIG_VIDEO_BRIDGE)) {
		ret = uclass_first_device_err(UCLASS_VIDEO_BRIDGE,
					  &priv->video_bridge);
		if (ret) {
			dev_err(dev, "no video bridge for vs dpu : %d\n", ret);
			goto error;
		}

		ret = video_bridge_attach(priv->video_bridge);
		if (ret) {
			dev_err(dev, "vs dpu fail to attach video bridge : %d\n", ret);
			goto error;
		}

		/* enable video bridge */
		ret = video_bridge_set_backlight(priv->video_bridge, 100);
		if (ret) {
			dev_err(dev, "video bridge fail to set backlight : %d\n", ret);
			goto error;
		}
	} else { /* TODO: need improve */
		ret = -ENOENT;
		goto error;
	}

	/* TODO: initialize dpu */
	ret = vs_dpu_init(dev);
	if (ret) {
		dev_err(dev, "initialize dpu failed : %d\n", ret);
		goto error;
	}

	/* TODO: get pixel format */

	uc_priv->bpix  = bpp;
	uc_priv->xsize = priv->timing.hactive.typ;
	uc_priv->ysize = priv->timing.vactive.typ;

	video_set_flush_dcache(dev, true);

	return 0;

error:
	ap_mipi_dsi0_clk_endisable(false);
	ap_dpu_clk_endisable(false);
	return ret;
}

static int vs_dpu_video_remove(struct udevice *dev)
{
	int ret;
	struct vs_dpu_priv *priv = dev_get_priv(dev);

	ret = device_remove(priv->panel, DM_REMOVE_NORMAL);
	if (ret) {
		printf("remove panel device failed: %d\n", ret);
		return ret;
	}

	/* reset dpu */
	writel(0x0, (void *)0xffef528004);
	writel(0x7, (void *)0xffef528004);

	return 0;
}

static const struct udevice_id vs_dpu_video_ids[] = {
	{ .compatible = "verisilicon,dc8200" },
	{ /* sentinel */ }
};

U_BOOT_DRIVER(vs_dpu_video) = {
	.name	= "vs_dpu_video",
	.id	= UCLASS_VIDEO,
	.of_match = vs_dpu_video_ids,
	.bind	= vs_dpu_video_bind,
	.probe	= vs_dpu_video_probe,
	.remove = vs_dpu_video_remove,
	.flags	= DM_FLAG_PRE_RELOC | DM_FLAG_OS_PREPARE,
	.priv_auto_alloc_size = sizeof(struct vs_dpu_priv),
};
