// SPDX-License-Identifier: GPL-2.0+
/*
* Copyright (C) 2017-2021 Alibaba Group Holding Limited
*/

#include <common.h>
#include <asm/io.h>
#include <asm/types.h>
#include <asm/io.h>

static uint32_t g_step_stop = 0x0;

static void term_get_char(void)
{
	s32 c;

	if (!g_step_stop)
		return;

	printf("stop here, if you want to continue please input 'Y' or 'y'\n");
	c = getc();
#define cESC '\x1b'
	while (1) {
		if (c == cESC)
			return;
		if (c == 'y' || c == 'Y')
			return;
		c = getc();
	}
}

void set_audio_pll_freq(int freq)
{
	unsigned int tmp;
#define LIGHT_AONCLK_ADDRBASE	0xfffff46000

	tmp = readl((void *)LIGHT_AONCLK_ADDRBASE + 0x100);
	tmp |= 0x10;
	writel(tmp, (void *)LIGHT_AONCLK_ADDRBASE + 0x100);
	/* aonsys_clk switch to OSC 24MHz */
        udelay(1);

	/* config audio_pll_fout3 to 50Mhz */
	if (freq == 24*1000000) {
		writel(0x41000000, (void *)LIGHT_AONCLK_ADDRBASE + 0x4);
		writel(0x05702301, (void *)LIGHT_AONCLK_ADDRBASE + 0x0);
		udelay(3);
		writel(0x01000000, (void *)LIGHT_AONCLK_ADDRBASE + 0x4);
		readl((void *)LIGHT_AONCLK_ADDRBASE + 0x90);
		readl((void *)LIGHT_AONCLK_ADDRBASE + 0x90);
		//while(!(readl((void *)LIGHT_AONCLK_ADDRBASE + 0x90) & 0x1));
		udelay(20);
	}
	if (freq == 60*1000000) {
		writel(0x41000000, (void *)LIGHT_AONCLK_ADDRBASE + 0x4);
		writel(0x02702301, (void *)LIGHT_AONCLK_ADDRBASE + 0x0);
		udelay(3);
		writel(0x01000000, (void *)LIGHT_AONCLK_ADDRBASE + 0x4);
		readl((void *)LIGHT_AONCLK_ADDRBASE + 0x90);
		readl((void *)LIGHT_AONCLK_ADDRBASE + 0x90);
		//while(!(readl((void *)LIGHT_AONCLK_ADDRBASE + 0x90) & 0x1));
		udelay(20);
	}
	if (freq == 120*1000000) {
		writel(0x41000000, (void *)LIGHT_AONCLK_ADDRBASE + 0x4);
		writel(0x01702301, (void *)LIGHT_AONCLK_ADDRBASE + 0x0);
		udelay(3);
		writel(0x01000000, (void *)LIGHT_AONCLK_ADDRBASE + 0x4);
		readl((void *)LIGHT_AONCLK_ADDRBASE + 0x90);
		readl((void *)LIGHT_AONCLK_ADDRBASE + 0x90);
		//while(!(readl((void *)LIGHT_AONCLK_ADDRBASE + 0x90) & 0x1));
		udelay(20);
	}
	if (freq == 300*1000000) {
		writel(0x41000000, (void *)LIGHT_AONCLK_ADDRBASE + 0x4);
		writel(0x01403201, (void *)LIGHT_AONCLK_ADDRBASE + 0x0);
		udelay(3);
		writel(0x01000000, (void *)LIGHT_AONCLK_ADDRBASE + 0x4);
		readl((void *)LIGHT_AONCLK_ADDRBASE + 0x90);
		readl((void *)LIGHT_AONCLK_ADDRBASE + 0x90);
		//while(!(readl((void *)LIGHT_AONCLK_ADDRBASE + 0x90) & 0x1));
		udelay(20);
	}
	if (freq == 600*1000000) {
		writel(0x41000000, (void *)LIGHT_AONCLK_ADDRBASE + 0x4);
		writel(0x01203201, (void *)LIGHT_AONCLK_ADDRBASE + 0x0);
		udelay(3);
		writel(0x01000000, (void *)LIGHT_AONCLK_ADDRBASE + 0x4);
		readl((void *)LIGHT_AONCLK_ADDRBASE + 0x90);
		readl((void *)LIGHT_AONCLK_ADDRBASE + 0x90);
		//while(!(readl((void *)LIGHT_AONCLK_ADDRBASE + 0x90) & 0x1));
		udelay(20);
	}
	if (freq == 0) {
		writel(0x41000000, (void *)LIGHT_AONCLK_ADDRBASE + 0x4);
		udelay(20);
	}
}

#define LIGHT_AP_PINCTRL1_START		0xffe7f3c000
#define LIGHT_AP_PINCTRL1_END		0xffe7f3c06c
#define LIGHT_AP_PINCTRL2_START		0xffec007000
#define LIGHT_AP_PINCTRL2_END		0xffec00707c
void set_all_ap_io_pu_pd_st_ds(int pu, int pd, int st, int ds)
{
	long k = LIGHT_AP_PINCTRL1_START;
	int val = 0;
	int tmp = 0;

	printf("set all ap io: PU=%d, PD=%d, ST=%d, DS=%d\n", pu, pd, st, ds);

	if (pu == 0 && pd == 0)
		val |= 0;
	else if (pu == 1 && pd == 0)
		val |= 0x30;
	else if (pu == 0 && pd == 1)
		val |= 0x10;
	val |= ((st ? 0x80 : 0x0) | ds);
	tmp = (val << 16) | val;
	while (k <= LIGHT_AP_PINCTRL1_END) {
		writel(tmp, (void *)k);
		k += 0x4;
	}

	k = LIGHT_AP_PINCTRL2_START;
	while (k <= LIGHT_AP_PINCTRL2_END) {
		writel(tmp, (void *)k);
		k += 0x4;
	}
}

static int io_test(cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[])
{
	ulong loop = 0;
	int ret = 0;

	printf("io_test start ....\n");
	if (argc > 1) {
		if (strict_strtoul(argv[1], 16, &loop) < 0)
			return CMD_RET_USAGE;
		g_step_stop = (uint32_t)loop;
	}

	printf("set 24M pll ?\n");
	term_get_char();
	set_audio_pll_freq(24*1000000);    // test clock = 4MHz
	printf("set 60M pll ?\n");
	term_get_char();
	set_audio_pll_freq(60*1000000);    // test clock 10MHz
	printf("set 120M pll ?\n");
	term_get_char();
	set_audio_pll_freq(120*1000000);  // test clock 20MHz
	printf("set 300M pll ?\n");
	term_get_char();
	set_audio_pll_freq(300*1000000);  // test clock 50MHz
	printf("set 600M pll ?\n");
	term_get_char();
	set_audio_pll_freq(600*1000000);  // test clock 100MHz
	printf("set 0M pll ?\n");
	term_get_char();
	set_audio_pll_freq(0);            // PLL shutdown
	printf("io test start ?\n");
	term_get_char();
	set_all_ap_io_pu_pd_st_ds(0, 0, 0, 0);
	set_all_ap_io_pu_pd_st_ds(0, 0, 0, 8);
	set_all_ap_io_pu_pd_st_ds(0, 0, 0, 15);
	set_all_ap_io_pu_pd_st_ds(0, 0, 1, 0);
	set_all_ap_io_pu_pd_st_ds(0, 0, 1, 8);
	set_all_ap_io_pu_pd_st_ds(0, 0, 1, 15);
	set_all_ap_io_pu_pd_st_ds(0, 1, 0, 0);
	set_all_ap_io_pu_pd_st_ds(0, 1, 0, 8);
	set_all_ap_io_pu_pd_st_ds(0, 1, 0, 15);
	set_all_ap_io_pu_pd_st_ds(0, 1, 1, 0);
	set_all_ap_io_pu_pd_st_ds(0, 1, 1, 8);
	set_all_ap_io_pu_pd_st_ds(0, 1, 1, 15);
	set_all_ap_io_pu_pd_st_ds(1, 0, 0, 0);
	set_all_ap_io_pu_pd_st_ds(1, 0, 0, 8);
	set_all_ap_io_pu_pd_st_ds(1, 0, 0, 15);
	set_all_ap_io_pu_pd_st_ds(1, 0, 1, 0);
	set_all_ap_io_pu_pd_st_ds(1, 0, 1, 8);
	set_all_ap_io_pu_pd_st_ds(1, 0, 1, 15);

	return ret;
}

U_BOOT_CMD(
	io_test,	2,	1,	io_test,
	"Light IO test, example: iotest 0x0",
	"[step_stop]"
);
