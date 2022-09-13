// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <asm/io.h>
#include <clk.h>
#include <div64.h>
#include <dm.h>
#include <generic-phy.h>
#include <linux/kernel.h>
#include <regmap.h>
#include <syscon.h>

#include "dw_dsi.h"

/* -------------------------- DPHY Test Code ------------------------ */

#define TC_PLL_DELAY_LP_TX_START_LP11			0x02
    #define TXDINDLYTIME_9_2				GENMASK( 7,  0)
#define TC_PLL_FSM_CNTRL				0x03
#define TC_PLL_PROP_CHARGE_PUMP_CTRL			0x0e
    #define PLL_PROP_CNTRL				GENMASK( 5,  0)
#define TC_PLL_INT_CHARGE_PUMP_CTRL			0x0f
    #define PLL_INT_CNTRL				GENMASK( 5,  0)
#define TC_PLL_VCO_CTRL					0x12
    #define PLL_VCO_CNTRL_OVR_EN			BIT(6)
    #define PLL_VCO_CNTRL_OVR				GENMASK( 5,  0)
#define TC_PLL_GMP_CTRL_DIGITAL_TEST			0x13
    #define PLL_TESTLOCK				BIT(7)
    #define PLL_GMP_CNTRL				GENMASK( 5,  4)
    #define TSTPLLDIG					GENMASK( 3,  1)
        #define TSTPLLDIG_LOCK				0x0
        #define TSTPLLDIG_REFCLK			0x1
        #define TSTPLLDIG_FBCLK				0x2
        #define TSTPLLDIG_INT_CLKIN			0x3
        #define TSTPLLDIG_LOCK_FROM_DETECTOR		0x4
        #define TSTPLLDIG_OUTPUT			0x5
        #define TSTPLLDIG_BUF_VCOCLK			0x6
        #define TSTPLLDIG_UPDATEPLL			0x7
#define TC_PLL_N_DIV_FSM_SIG				0x17
    #define PLL_N_OVR					GENMASK( 3,  0)
#define TC_PLL_M_DIV					0x18
    #define PLL_M_OVR_4_0				GENMASK( 4,  0)
    #define PLL_M_OVR_9_5				GENMASK( 4,  0)
#define TC_PLL_N_AND_M_DIV_CTRL				0x19
    #define PLL_M_OVR_EN				BIT(5)
    #define PLL_N_OVR_EN				BIT(4)
#define TC_PLL_CHARGE_PUMP_BIAS_CTRL			0x1c
    #define PLL_CPBIAS_CNTRL				GENMASK( 6,  0)
#define TC_PLL_ANALOG_PROGRAM_CONTROL			0x1f
    #define PLL_MPLL_PROG_5_0				GENMASK( 5,  0)
    #define PLL_MPLL_PROG_11_6				GENMASK( 5,  0)
    #define PLL_MPLL_PROG_16_12				GENMASK( 4,  0)
#define TC_HS_FREQ_RANGE_OF_OPERATION			0x44
    #define HSFREQRANGE_OVR_EN				BIT(7)
    #define HSFREQRANGE_OVR				GENMASK( 6,  0)
    #define HSFREQRANGE					HSFREQRANGE_OVR
#define TC_PG_LP_BIAS_LANE0				0x4a
    #define PRG_ON_LANE0				BIT(6)
#define TC_SR_FSM_OVR_CNTRL				0xa0
#define TC_SR_DDL_LOOP_CONF				0xa3

/* --------------------------------- VOSYS ------------------------------ */

#define VOSYS_MIPIDSI0_SYSREG				0x74
#define VOSYS_MIPIDSI1_SYSREG				0x78
    #define MIPIDSI_HSFREQRANGE				GENMASK( 9,  3)
    #define MIPIDSI_CFGCLKFREQRANGE			GENMASK(15, 10)

/* --------------------------------- VOSYS End -------------------------- */

#define FCLKIN_FREQ_MIN			2000UL		/* in KHz */
#define FCLKIN_FREQ_MAX			64000UL
#define DATA_RATE_MIN			80000UL
#define DATA_RATE_MAX			2500000UL

struct dw_dphy_param {
	u32 data_rate;
	u8  hsfreqrange;
	u8  vco_range;
	u16 clk_lp2hs;
	u16 clk_hs2lp;
	u16 data_lp2hs;
	u16 data_hs2lp;
};

struct dw_dphy_priv {
	struct regmap *regmap;
	struct regmap *vosys_regmap;
	struct clk pix_clk;
	struct clk ref_clk;
	const struct dw_dphy_param *params;
};

#define dw_dphy_fill_params(r, f, v, cl, ch, dl, dh) {	\
	.data_rate	= r,				\
	.hsfreqrange	= f,				\
	.vco_range	= v,				\
	.clk_lp2hs	= cl,				\
	.clk_hs2lp	= ch,				\
	.data_lp2hs	= dl,				\
	.data_hs2lp	= dh				\
}

static const struct dw_dphy_param dphy_param_table[] = {
	dw_dphy_fill_params(80,  0x00, 0x3F, 21, 17, 15, 10),
	dw_dphy_fill_params(90,  0x10, 0x3F, 23, 17, 16, 10),
	dw_dphy_fill_params(100, 0x20, 0x3F, 22, 17, 16, 10),
	dw_dphy_fill_params(110, 0x30, 0x39, 25, 18, 17, 11),
	dw_dphy_fill_params(120, 0x01, 0x39, 26, 20, 18, 11),
	dw_dphy_fill_params(130, 0x11, 0x39, 27, 19, 19, 11),
	dw_dphy_fill_params(140, 0x21, 0x39, 27, 19, 19, 11),
	dw_dphy_fill_params(150, 0x31, 0x39, 28, 20, 20, 12),
	dw_dphy_fill_params(160, 0x02, 0x39, 30, 21, 22, 13),
	dw_dphy_fill_params(170, 0x12, 0x2F, 30, 21, 23, 13),
	dw_dphy_fill_params(180, 0x22, 0x2F, 31, 21, 23, 13),
	dw_dphy_fill_params(190, 0x32, 0x2F, 32, 22, 24, 13),
	dw_dphy_fill_params(205, 0x03, 0x2F, 35, 22, 25, 13),
	dw_dphy_fill_params(220, 0x13, 0x29, 37, 26, 27, 15),
	dw_dphy_fill_params(235, 0x23, 0x29, 38, 28, 27, 16),
	dw_dphy_fill_params(250, 0x33, 0x29, 41, 29, 30, 17),
	dw_dphy_fill_params(275, 0x04, 0x29, 43, 29, 32, 18),
	dw_dphy_fill_params(300, 0x14, 0x29, 45, 32, 35, 19),
	dw_dphy_fill_params(325, 0x25, 0x29, 48, 33, 36, 18),
	dw_dphy_fill_params(350, 0x35, 0x1F, 51, 35, 40, 20),
	dw_dphy_fill_params(400, 0x05, 0x1F, 59, 37, 44, 21),
	dw_dphy_fill_params(450, 0x16, 0x19, 65, 40, 49, 23),
	dw_dphy_fill_params(500, 0x26, 0x19, 71, 41, 54, 24),
	dw_dphy_fill_params(550, 0x37, 0x19, 77, 44, 57, 26),
	dw_dphy_fill_params(600, 0x07, 0x19, 82, 46, 64, 27),
	dw_dphy_fill_params(650, 0x18, 0x19, 87, 48, 67, 28),
	dw_dphy_fill_params(700, 0x28, 0x0F, 94, 52, 71, 29),
	dw_dphy_fill_params(750, 0x39, 0x0F, 99, 52, 75, 31),
	dw_dphy_fill_params(800, 0x09, 0x0F, 105, 55, 82, 32),
	dw_dphy_fill_params(850, 0x19, 0x0F, 110, 58, 85, 32),
	dw_dphy_fill_params(900, 0x29, 0x09, 115, 58, 88, 35),
	dw_dphy_fill_params(950, 0x3A, 0x09, 120, 62, 93, 36),
	dw_dphy_fill_params(1000, 0x0A, 0x09, 128, 63, 99, 38),
	dw_dphy_fill_params(1050, 0x1A, 0x09, 132, 65, 102, 38),
	dw_dphy_fill_params(1100, 0x2A, 0x09, 138, 67, 106, 39),
	dw_dphy_fill_params(1150, 0x3B, 0x09, 146, 69, 112, 42),
	dw_dphy_fill_params(1200, 0x0B, 0x09, 151, 71, 117, 43),
	dw_dphy_fill_params(1250, 0x1B, 0x09, 153, 74, 120, 45),
	dw_dphy_fill_params(1300, 0x2B, 0x09, 160, 73, 124, 46),
	dw_dphy_fill_params(1350, 0x3C, 0x03, 165, 76, 130, 47),
	dw_dphy_fill_params(1400, 0x0C, 0x03, 172, 78, 134, 49),
	dw_dphy_fill_params(1450, 0x1C, 0x03, 177, 80, 138, 49),
	dw_dphy_fill_params(1500, 0x2C, 0x03, 183, 81, 143, 52),
	dw_dphy_fill_params(1550, 0x3D, 0x03, 191, 84, 147, 52),
	dw_dphy_fill_params(1600, 0x0D, 0x03, 194, 85, 152, 52),
	dw_dphy_fill_params(1650, 0x1D, 0x03, 201, 86, 155, 53),
	dw_dphy_fill_params(1700, 0x2E, 0x03, 208, 88, 161, 53),
	dw_dphy_fill_params(1750, 0x3E, 0x03, 212, 89, 165, 53),
	dw_dphy_fill_params(1800, 0x0E, 0x03, 220, 90, 171, 54),
	dw_dphy_fill_params(1850, 0x1E, 0x03, 223, 92, 175, 54),
	dw_dphy_fill_params(1900, 0x2F, 0x03, 231, 91, 180, 55),
	dw_dphy_fill_params(1950, 0x3F, 0x03, 236, 95, 185, 56),
	dw_dphy_fill_params(2000, 0x0F, 0x03, 243, 97, 190, 56),
	dw_dphy_fill_params(2050, 0x40, 0x03, 248, 99, 194, 58),
	dw_dphy_fill_params(2100, 0x41, 0x03, 252, 100, 199, 59),
	dw_dphy_fill_params(2150, 0x42, 0x03, 259, 102, 204, 61),
	dw_dphy_fill_params(2200, 0x43, 0x03, 266, 105, 210, 62),
	dw_dphy_fill_params(2250, 0x44, 0x03, 269, 109, 213, 63),
	dw_dphy_fill_params(2300, 0x45, 0x01, 272, 109, 217, 65),
	dw_dphy_fill_params(2350, 0x46, 0x01, 281, 112, 225, 66),
	dw_dphy_fill_params(2400, 0x47, 0x01, 283, 115, 226, 66),
	dw_dphy_fill_params(2450, 0x48, 0x01, 282, 115, 226, 67),
	dw_dphy_fill_params(2500, 0x49, 0x01, 281, 118, 227, 67),
};

static int dw_dphy_get_param(struct phy *phy)
{
	int i;
	unsigned long fin, data_rate;
	const struct dw_dphy_param *params;
	struct dw_dphy_priv *priv = dev_get_priv(phy->dev);

	fin = DIV_ROUND_UP_ULL(clk_get_rate(&priv->ref_clk), 1000);
	if (fin < FCLKIN_FREQ_MIN || fin > FCLKIN_FREQ_MAX)
		return -EINVAL;

	data_rate = DIV_ROUND_UP_ULL(clk_get_rate(&priv->pix_clk), 1000000) * 24 / 4;
	if (data_rate * 1000 < DATA_RATE_MIN || data_rate * 1000 > DATA_RATE_MAX)
		return -EINVAL;

	for (i = 0; i < ARRAY_SIZE(dphy_param_table); i++) {
		params = &dphy_param_table[i];
		if (data_rate <= params->data_rate)
			break;
	}

	priv->params = params;

	return 0;
}

static int dw_dphy_config_rstz(struct phy *phy, u8 val)
{
	int ret;
	struct dw_dphy_priv *priv = dev_get_priv(phy->dev);
	u32 rstz = FIELD_PREP(PHY_UNRSTZ, !!val);

	ret = regmap_update_bits(priv->regmap, DSI_PHY_RSTZ, PHY_UNRSTZ, rstz);
	if (ret) {
		dev_err(dphy->dev, "config rstz failed : %d\n", ret);
		return  ret;
	}

	return 0;
}

static int dw_dphy_config_shutdownz(struct phy *phy, u8 val)
{
	int ret;
	struct dw_dphy_priv *priv = dev_get_priv(phy->dev);
	u32 shutdownz = FIELD_PREP(PHY_UNSHUTDOWNZ, !!val);

	ret = regmap_update_bits(priv->regmap, DSI_PHY_RSTZ,
				 PHY_UNSHUTDOWNZ, shutdownz);
	if (ret) {
		dev_err(dphy->dev, "config shutdownz failed: %d\n", ret);
		return ret;
	}

	return 0;
}

static int dw_dphy_config_enableclk(struct phy *phy, u8 val)
{
	int ret;
	struct dw_dphy_priv *priv = dev_get_priv(phy->dev);
	u32 enableclk = FIELD_PREP(PHY_ENABLECLK, !!val);

	ret = regmap_update_bits(priv->regmap, DSI_PHY_RSTZ,
				 PHY_ENABLECLK, enableclk);
	if (ret)
		dev_err(dphy->dev, "config enableclk failed: %d\n", ret);

	return ret;
}

static int dw_dphy_config_testclr(struct phy *phy, u8 val)
{
	int ret;
	struct dw_dphy_priv *priv = dev_get_priv(phy->dev);
	u32 testclr = FIELD_PREP(PHY_TST_CTRL0_TESTCLR, !!val);

	ret = regmap_update_bits(priv->regmap, DSI_PHY_TST_CTRL0,
				 PHY_TST_CTRL0_TESTCLR, testclr);
	if (ret) {
		dev_err(phy->dev, "config testclr failed: %d\n", ret);
		return ret;
	}

	return 0;
}

static int dw_dphy_config_testclk(struct phy *phy, u8 val)
{
	int ret;
	struct dw_dphy_priv *priv = dev_get_priv(phy->dev);
	u32 testclk = FIELD_PREP(PHY_TST_CTRL0_TESTCLK, !!val);

	ret = regmap_update_bits(priv->regmap, DSI_PHY_TST_CTRL0,
				 PHY_TST_CTRL0_TESTCLK, testclk);
	if (ret) {
		dev_err(phy->dev, "config testclk failed: %d\n", ret);
		return ret;
	}

	return 0;
}

static int dw_dphy_config_testen(struct phy *phy, u8 val)
{
	int ret;
	struct dw_dphy_priv *priv = dev_get_priv(phy->dev);
	u32 testen = FIELD_PREP(PHY_TST_CTRL1_TESTEN, !!val);

	ret = regmap_update_bits(priv->regmap, DSI_PHY_TST_CTRL1,
				 PHY_TST_CTRL1_TESTEN, testen);
	if (ret)
		dev_err(dphy->dev, "config testen failed: %d\n", ret);

	return ret;
}

static int dw_dphy_config_testdin(struct phy *phy, u8 val)
{
	int ret;
	struct dw_dphy_priv *priv = dev_get_priv(phy->dev);
	u32 testdin = FIELD_PREP(PHY_TST_CTRL1_TESTIN, val);

	ret = regmap_update_bits(priv->regmap, DSI_PHY_TST_CTRL1,
				 PHY_TST_CTRL1_TESTIN, testdin);
	if (ret)
		dev_err(dphy->dev, "config testdin failed: %d\n", ret);

	return ret;
}

static int dw_dphy_write(struct phy *phy, u8 test_code, u8 *test_data, u8 len)
{
	int i;

	/* set the desired test code */
	dw_dphy_config_testclk(phy, 1);
	dw_dphy_config_testdin(phy, test_code);
	dw_dphy_config_testen(phy, TESTEN_WRITEADDR);
	dw_dphy_config_testclk(phy, 0);

	/* enter the necessary test data */
	dw_dphy_config_testen(phy, TESTEN_WRITEDATA);
	for (i = 0; i < len; i++) {
		dw_dphy_config_testdin(phy, test_data[i]);
		dw_dphy_config_testclk(phy, 1);
		dw_dphy_config_testclk(phy, 0);

		dev_info(phy->dev, "%s: test_code = %#x, len = %u, data[%d] = %#x\n",
			 __func__, test_code, len, i, test_data[i]);
	}

	return 0;
}

static int dw_dphy_pll_config(struct phy *phy,
		const struct dw_dphy_param *params)
{
	u8 data[8];

	data[0] = FIELD_PREP(PLL_PROP_CNTRL, params->data_rate >= 1150 ? 0xE : 0x8);
	dw_dphy_write(phy, TC_PLL_PROP_CHARGE_PUMP_CTRL, data, 1);

	data[0] = FIELD_PREP(PLL_CPBIAS_CNTRL, 0x10);
	dw_dphy_write(phy, TC_PLL_CHARGE_PUMP_BIAS_CTRL, data, 1);

	return 0;
}

static int dw_dphy_init(struct phy *phy)
{
	int ret;
	u8 data[8];
	u32 hsfreqrange, cfgclkfreqrange;
	u32 phy_if_cfg, phy_tmr_lpclk_cfg, phy_tmr_cfg;
	struct dw_dphy_priv *priv = dev_get_priv(phy->dev);

	ret = dw_dphy_get_param(phy);
	if (ret) {
		dev_err(phy->dev, "failed to get params: %d\n", ret);
		return ret;
	}

	/* configure hsfreqrange and cfgclkfreqrange */
	hsfreqrange = FIELD_PREP(MIPIDSI_HSFREQRANGE, priv->params->hsfreqrange);
	ret = regmap_update_bits(priv->vosys_regmap, VOSYS_MIPIDSI0_SYSREG,
				 MIPIDSI_HSFREQRANGE, hsfreqrange);
	if (ret) {
		dev_err(phy->dev, "config dsi0 hsfreqrange failed: %d\n", ret);
		return ret;
	}

	cfgclkfreqrange = FIELD_PREP(MIPIDSI_CFGCLKFREQRANGE, (24 - 17) * 4);
	ret = regmap_update_bits(priv->vosys_regmap, VOSYS_MIPIDSI0_SYSREG,
				 MIPIDSI_CFGCLKFREQRANGE, cfgclkfreqrange);
	if (ret) {
		dev_err(dphy->dev, "config dsi0 cfgclkfreqrange failed: %d\n", ret);
		return ret;
	}

	/* disable slew rate calibration */
	data[0] = 0x0;
	dw_dphy_write(phy, TC_SR_DDL_LOOP_CONF, data, 1);
	data[0] = 0x1;
	dw_dphy_write(phy, TC_SR_FSM_OVR_CNTRL, data, 1);

	/* configure prg_on_lane0 */
	data[0] = FIELD_PREP(PRG_ON_LANE0, 1);
	dw_dphy_write(phy, TC_PG_LP_BIAS_LANE0, data, 1);

	dw_dphy_pll_config(phy, priv->params);

	phy_if_cfg = FIELD_PREP(N_LANES, 4 - 1) |
		     FIELD_PREP(PHY_STOP_WAIT_TIME, 0x2);
	regmap_write(priv->regmap, DSI_PHY_IF_CFG, phy_if_cfg);

	/* config clock & data lane timers */
	phy_tmr_lpclk_cfg = FIELD_PREP(PHY_CLKHS2LP_TIME, priv->params->clk_hs2lp) |
			    FIELD_PREP(PHY_CLKLP2HS_TIME, priv->params->clk_lp2hs);
	regmap_write(priv->regmap, DSI_PHY_TMR_LPCLK_CFG, phy_tmr_lpclk_cfg);

	phy_tmr_cfg = FIELD_PREP(PHY_HS2LP_TIME, priv->params->data_hs2lp) |
		      FIELD_PREP(PHY_LP2HS_TIME, priv->params->data_lp2hs);
	regmap_write(priv->regmap, DSI_PHY_TMR_CFG, phy_tmr_cfg);

	regmap_write(priv->regmap, DSI_PHY_TMR_RD_CFG,
		     FIELD_PREP(MAX_RD_TIME, 10000));

	return 0;
}

static int dw_dphy_power_on(struct phy *phy)
{
	int ret;
	u32 phy_status;
	struct dw_dphy_priv *priv = dev_get_priv(phy->dev);

	dw_dphy_config_enableclk(phy, PHY_ENABLECLK);
	dw_dphy_config_shutdownz(phy, PHY_UNSHUTDOWNZ);
	dw_dphy_config_rstz(phy, PHY_UNRSTZ);

	/* wait phy lock */
	ret = regmap_read_poll_timeout(priv->regmap, DSI_PHY_STATUS,
				       phy_status, phy_status & PHY_LOCK,
				       1000, PHY_STATUS_TIMEOUT_US);
	if (ret) {
		dev_err(phy->dev, "wait phy lock state timeout\n");
		return ret;
	}

	/* wati clock lane stop */
	ret = regmap_read_poll_timeout(priv->regmap, DSI_PHY_STATUS,
				       phy_status, phy_status & PHY_STOP_STATE_CLK_LANE,
				       1000, PHY_STATUS_TIMEOUT_US);
	if (ret) {
		dev_err(phy->dev, "wait clock lane stop state timeout\n");
		return ret;
	}

	return 0;
}

static int dw_dphy_power_off(struct phy *phy)
{
	dw_dphy_config_rstz(phy, PHY_RSTZ);
	dw_dphy_config_enableclk(phy, PHY_DISABLECLK);
	dw_dphy_config_shutdownz(phy, PHY_SHUTDOWNZ);

	return 0;
}

static int dw_dphy_reset(struct phy *phy)
{
	dw_dphy_config_rstz(phy, PHY_RSTZ);
	dw_dphy_config_shutdownz(phy, PHY_SHUTDOWNZ);

	dw_dphy_config_testclr(phy, 1);
	dw_dphy_config_testclr(phy, 0);

	return 0;
}

static int dw_dphy_exit(struct phy *phy)
{
	return 0;
}

static int dw_dphy_ofdata_to_platdata(struct udevice *dev)
{
	int ret;
	struct dw_dphy_priv *priv = dev_get_priv(dev);

	priv->regmap = syscon_regmap_lookup_by_phandle(dev, "regmap");
	if (IS_ERR_OR_NULL(priv->regmap))
		return -ENODEV;

	priv->vosys_regmap = syscon_regmap_lookup_by_phandle(dev, "vosys-regmap");
	if (IS_ERR_OR_NULL(priv->vosys_regmap))
		return -ENODEV;

	ret = clk_get_by_name(dev, "pix-clk", &priv->pix_clk);
	if (ret) {
		dev_err(dev, "failed to get pix clk: %d\n", ret);
		return ret;
	}

	ret = clk_get_by_name(dev, "ref-clk", &priv->ref_clk);
	if (ret) {
		dev_err(dev, "failed to get ref clock: %d\n", ret);
		return ret;
	}

	return 0;
}

static int dw_dphy_probe(struct udevice *dev)
{
	return 0;
}

static struct phy_ops dw_dphy_ops = {
	.init		= dw_dphy_init,
	.power_on	= dw_dphy_power_on,
	.power_off	= dw_dphy_power_off,
	.exit		= dw_dphy_exit,
	.reset		= dw_dphy_reset,
};

static const struct udevice_id dw_dphy_ids[] = {
	{ .compatible = "synopsys,dw-dphy" },
	{ /* sentinel */ }
};

U_BOOT_DRIVER(dw_dphy)  = {
	.name		= "phy_dw_dphy",
	.id		= UCLASS_PHY,
	.of_match	= dw_dphy_ids,
	.ops		= &dw_dphy_ops,
	.probe		= dw_dphy_probe,
	.ofdata_to_platdata   = dw_dphy_ofdata_to_platdata,
	.priv_auto_alloc_size = sizeof(struct dw_dphy_priv),
};
