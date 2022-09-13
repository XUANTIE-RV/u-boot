// SPDX-License-Identifier: GPL-2.0+
/*
* Copyright (C) 2017-2021 Alibaba Group Holding Limited
*/

#include <common.h>
#include <asm/io.h>
#include <asm/types.h>
#include <asm/io.h>

#define ADC_APB_CLK_RATE	73728000
#define ADC_BASE		0xFFFFF51000

#define LIGHT_ADC_PHY_CFG			0x00
#define LIGHT_ADC_PHY_CTRL			0x04
#define LIGHT_ADC_PHY_TEST			0x08
#define LIGHT_ADC_OP_CTRL			0x0C
#define LIGHT_ADC_OP_SINGLE_START		0x10
#define LIGHT_ADC_FCLK_CTRL			0x14
#define LIGHT_ADC_START_TIME			0x18
#define LIGHT_ADC_SAMPLE_TIME			0x1C
#define LIGHT_ADC_SAMPLE_DATA			0x20
#define LIGHT_ADC_INT_CTRL1			0x50
#define LIGHT_ADC_INT_CTRL2			0x54
#define LIGHT_ADC_INT_STATUS			0x58
#define LIGHT_ADC_INT_ACTUAL_VALUE_CH0		0x60
#define LIGHT_ADC_INT_ACTUAL_VALUE_CH1		0x64
#define LIGHT_ADC_INT_DELTA_VALUE_CH0		0x90
#define LIGHT_ADC_INT_DELTA_VALUE_CH1		0x94

#define LIGHT_ADC_PHY_CTRL_RST_EN				(0x1 << 4)
#define LIGHT_ADC_PHY_CTRL_ENADC_EN				(0x1 << 0)
#define LIGHT_ADC_OP_SINGLE_START_EN				BIT(0)

#define LIGHT_ADC_OP_CONTINOUS_MODE				(0 << 0)
#define LIGHT_ADC_OP_ONE_SHOT_MODE				(1 << 0)

/* ADC sample data */
/*
 * In Single mode:
 * bit[31:16],reserved;
 * bit[15],data valid;
 * bit[14:12],channel number;
 * bit[11:0],adc conversion data
 *
 * In Continous mode:
 * bit[31],data valid;
 * bit[30:28],channel number;
 * bit[27:16],adc conversion data
 * bit[15],data valid;
 * bit[14:12],channel number;
 * bit[11:0],adc conversion data
*/

#define LIGHT_ADC_SAMPLE_DATA_CH1                      		GENMASK(27, 16)
#define LIGHT_ADC_SAMPLE_DATA_CH1_OFF                      	(16)
#define LIGHT_ADC_SAMPLE_DATA_CH1_VLD				BIT(31)
#define LIGHT_ADC_SAMPLE_CH1_NUMBER				GENMASK(30, 28)
#define LIGHT_ADC_SAMPLE_CH1_NUMBER_OFF				28
#define LIGHT_ADC_SAMPLE_DATA_CH0                      		GENMASK(11, 0)
#define LIGHT_ADC_SAMPLE_DATA_CH0_VLD				BIT(15)
#define LIGHT_ADC_SAMPLE_DATA_CH0_OFF                      	(0)
#define LIGHT_ADC_SAMPLE_CH0_NUMBER				GENMASK(14, 12)
#define LIGHT_ADC_SAMPLE_CH0_NUMBER_OFF				12

static uint32_t g_chan = 0;
static uint32_t g_sample_cnt = 49152;
static uint32_t g_fclk_ctrl = 0x10004;
static uint32_t g_sample_time = 0x10;
static uint32_t g_start_time = 0x160;
static int32_t g_phy_cfg = 0x0;
static uint32_t g_op_ctrl = 0xff000;
static uint32_t g_console_print = 0;

static void light_adc_reset(void)
{
	u32 tmp;

	tmp = readl((void *)(ADC_BASE + LIGHT_ADC_PHY_CTRL));
	tmp |= LIGHT_ADC_PHY_CTRL_RST_EN;
	writel(tmp, (void *)(ADC_BASE + LIGHT_ADC_PHY_CTRL));
	udelay(10);
	tmp &= ~LIGHT_ADC_PHY_CTRL_RST_EN;
	writel(tmp, (void *)(ADC_BASE + LIGHT_ADC_PHY_CTRL));
}

static void light_adc_hw_init(void)
{
	/* adc_phy_enctr： 0x8e0 */
	writel(0x8e0, (void *)(ADC_BASE + LIGHT_ADC_PHY_TEST));

	writel(g_phy_cfg, (void *)(ADC_BASE + LIGHT_ADC_PHY_CFG));
	writel(g_op_ctrl, (void *)(ADC_BASE + LIGHT_ADC_OP_CTRL));

	writel(g_fclk_ctrl, (void *)(ADC_BASE + LIGHT_ADC_FCLK_CTRL));
	writel(g_start_time, (void *)(ADC_BASE + LIGHT_ADC_START_TIME));
	writel(g_sample_time, (void *)(ADC_BASE + LIGHT_ADC_SAMPLE_TIME));

	/* disable the irq */
	writel(0x3, (void *)(ADC_BASE + LIGHT_ADC_INT_CTRL2));
	writel(0, (void *)(ADC_BASE + LIGHT_ADC_PHY_CTRL));
}

static bool __maybe_unused term_get_char(void)
{
	s32 c;

	c = getc();
#define cESC '\x1b'
	if (c == cESC)
		return true;

	return false;
}

static void light_adc_start_sampling(void)
{
	int cnt = 0;
	int phy_ctrl;
	int cnt2, cnt1;
	int sample_cnt;
	int continous_mode = 0;
	ushort *cont_data = (ushort *)0xd0000000; /* 96K size: 130k(0x20800 ~ 226k(0x38800)*/

	phy_ctrl = readl((void *)(ADC_BASE + LIGHT_ADC_PHY_CTRL));
	phy_ctrl |= LIGHT_ADC_PHY_CTRL_ENADC_EN;
	writel(phy_ctrl, (void *)(ADC_BASE + LIGHT_ADC_PHY_CTRL));

	cnt1 = readl((void *)(ADC_BASE + 0xc4 + (g_chan * 8)));
	if (g_chan < 0 || g_chan > 7)
		printf("non valid chan set, only support 0~7 chan\n");

	if (!(g_op_ctrl & LIGHT_ADC_OP_ONE_SHOT_MODE))
		continous_mode = 1;

	while (cnt < g_sample_cnt) {
		uint ievent;
		uint val = 0, val1 = 0;
		uint chan_number;
		int timeout = 1000000;

		do {
			ievent = readl((void *)(ADC_BASE + LIGHT_ADC_SAMPLE_DATA));

			if (ievent & LIGHT_ADC_SAMPLE_DATA_CH0_VLD) {
				chan_number = (ievent & LIGHT_ADC_SAMPLE_CH0_NUMBER) >> LIGHT_ADC_SAMPLE_CH0_NUMBER_OFF;
				if (chan_number == g_chan) {
					val = (ievent & LIGHT_ADC_SAMPLE_DATA_CH0) >> LIGHT_ADC_SAMPLE_DATA_CH0_OFF;
					if (!continous_mode)
						break;
				}
			}

			if (continous_mode) {
				if (ievent & LIGHT_ADC_SAMPLE_DATA_CH1_VLD) {
					chan_number = (ievent & LIGHT_ADC_SAMPLE_CH1_NUMBER) >> LIGHT_ADC_SAMPLE_CH1_NUMBER_OFF;
					if (chan_number == g_chan) {
						val1 = (ievent & LIGHT_ADC_SAMPLE_DATA_CH1) >> LIGHT_ADC_SAMPLE_DATA_CH1_OFF;
						break;
					}
				}
			}

		} while (timeout--);

		if (timeout <= 0) {
			printf("timeout to read chan sample data\n");

			phy_ctrl = readl((void *)(ADC_BASE + LIGHT_ADC_PHY_CTRL));
			phy_ctrl &= ~LIGHT_ADC_PHY_CTRL_ENADC_EN;
			writel(phy_ctrl, (void *)(ADC_BASE + LIGHT_ADC_PHY_CTRL));
			return;
		}

		cont_data[cnt] = (ushort)val;
		cnt++;
		if (continous_mode) {
			cont_data[cnt] = (ushort)val1;
			cnt++;
		}
	}

	phy_ctrl = readl((void *)(ADC_BASE + LIGHT_ADC_PHY_CTRL));
	phy_ctrl &= ~LIGHT_ADC_PHY_CTRL_ENADC_EN;
	writel(phy_ctrl, (void *)(ADC_BASE + LIGHT_ADC_PHY_CTRL));

	cnt2 = readl((void *)(ADC_BASE + 0xc4 + (g_chan * 8)));

	sample_cnt = cnt2 > cnt1 ? cnt2 - cnt1 : (1 << 24) - cnt1 + cnt2;
	printf("sample_cnt:%d, cnt1:%d, cnt2:%d\n", sample_cnt, cnt1, cnt2);

	if (!g_console_print)
		return;
	cnt = 0;
	while (cnt < g_sample_cnt) {
		printf("%d\n", cont_data[cnt]);
		cnt++;
	}
}

static int adc_sampling(cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[])
{
	ulong chan, sample_cnt, fclk_ctrl, sample_time, start_time, phy_cfg, op_ctrl, console_print;
	int ret = 0;

	if (argc > 1) {
		if (strict_strtoul(argv[1], 16, &chan) < 0)
			return CMD_RET_USAGE;
		g_chan = (uint32_t)chan;
	}

	if (argc > 2) {
		if (strict_strtoul(argv[2], 16, &sample_cnt) < 0)
			return CMD_RET_USAGE;
		g_sample_cnt = (uint32_t)sample_cnt;
	}

	if (argc > 3) {
		if (strict_strtoul(argv[3], 16, &fclk_ctrl) < 0)
			return CMD_RET_USAGE;
		g_fclk_ctrl = (uint32_t)fclk_ctrl;
	}

	if (argc > 4) {
		if (strict_strtoul(argv[4], 16, &sample_time) < 0)
			return CMD_RET_USAGE;
		g_sample_time = (uint32_t)sample_time;
	}

	if (argc > 5) {
		if (strict_strtoul(argv[5], 16, &start_time) < 0)
			return CMD_RET_USAGE;
		g_start_time = (uint32_t)start_time;
	}

	if (argc > 6) {
		if (strict_strtoul(argv[6], 16, &phy_cfg) < 0)
			return CMD_RET_USAGE;
		g_phy_cfg = (uint32_t)phy_cfg;
	}

	if (argc > 7) {
		if (strict_strtoul(argv[7], 16, &op_ctrl) < 0)
			return CMD_RET_USAGE;
		g_op_ctrl = (uint32_t)op_ctrl;
	}

	if (argc > 8) {
		if (strict_strtoul(argv[8], 16, &console_print) < 0)
			return CMD_RET_USAGE;
		g_console_print = (uint32_t)console_print;
	}

	printf("adc_sampling 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x\n",
	       g_chan, g_sample_cnt, g_fclk_ctrl, g_sample_time, g_start_time, g_phy_cfg, g_op_ctrl, g_console_print);

	light_adc_reset();
	light_adc_hw_init();
	light_adc_start_sampling();

	return ret;
}

U_BOOT_CMD(
	adc_sampling,	9,	1,	adc_sampling,
	"Light ADC sampling, example: adc_sampling 0x0 100 0x10004 0x10 0x160 0x0 0x1000 1",
	"[chan sample_cnt fclk_ctrl sample_time satrt_time phy_cfg op_ctrl console_print]"
);
