/*
 * Copyright (C) 2018-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <asm/clock.h>
#include <miiphy.h>

void gmac_clk_config(u32 interface)
{
	// release phy by gpio0_0
	*(volatile unsigned int*)(0xbff71000) = 0x1;
	udelay(10000);

#ifdef CONFIG_IS_ASIC
	if (interface == PHY_INTERFACE_MODE_MII) {
		*(volatile unsigned int *)(0xbe83025c) = 0x0; //MII MODE
		*(volatile unsigned int *)(0xbe83031c) = 0x1; //CLK_OUT pad disable
		*(volatile unsigned int *)(0xbff770c0) = 0x18a;
	} else if (interface == PHY_INTERFACE_MODE_RMII) {
		*(volatile unsigned int *)(0xbe83025c) = 0x4; //RMII MODE

		//RMII, External 100Mbps Mode
		*(volatile unsigned int *)(0xbe83031c) = 0x0; //CLK_OUT pad enable

		//50MHz
		*(volatile unsigned int *)(0xbff770cc) = 0x14;
		*(volatile unsigned int *)(0xbff770cc) = 0x80000014;

		//25MHz
		*(volatile unsigned int *)(0xbff770d4) = 0x2;
		*(volatile unsigned int *)(0xbff770d4) = 0x80000002;

		//enable rmii clocks
		*(volatile unsigned int *)(0xbff770c0) = 0xd68;
	} else if (interface == PHY_INTERFACE_MODE_RGMII) {

		//RGMII  mode
		*(volatile  unsigned  int*)(0xbe8302cc)  =  0x20202020;
		*(volatile  unsigned  int*)(0xbe8302cf)  =  0x07072020;
		*(volatile  unsigned  int*)(0xbe8302d4)  =  0x07070707;

		//PHY  Mode  Sel
		*(volatile  unsigned  int*)(0xbe83025c)  =  0x1;  //0:MII/GMII,  1:RGMII,  4:RMII
		//Enable  TX_CLK  PAD  
		*(volatile  unsigned  int*)(0xbe83031c)  =  0x0;  //oen

		//enable  pll_div,  1000/4=250MHz
		*(volatile  unsigned  int*)(0xbff770cc)  =  0x4;
		*(volatile  unsigned  int*)(0xbff770cc)  =  0x4;
		*(volatile  unsigned  int*)(0xbff770cc)  =  0x80000004;
		*(volatile  unsigned  int*)(0xbff770cc)  =  0x80000004;

#if 0
		*(volatile  unsigned  int*)(0xbff770d0)  =  0x2;
		*(volatile  unsigned  int*)(0xbff770d0)  =  0x2;
		*(volatile  unsigned  int*)(0xbff770d0)  =  0x80000002;
		*(volatile  unsigned  int*)(0xbff770d0)  =  0x80000002;
#else
		*(volatile  unsigned  int*)(0xbff770d0)  =  0xa;
		*(volatile  unsigned  int*)(0xbff770d0)  =  0xa;
		*(volatile  unsigned  int*)(0xbff770d0)  =  0x8000000a;
		*(volatile  unsigned  int*)(0xbff770d0)  =  0x8000000a;
#endif
		*(volatile  unsigned  int*)(0xbff770d8)  =  0x28;
		*(volatile  unsigned  int*)(0xbff770d8)  =  0x28;
		*(volatile  unsigned  int*)(0xbff770d8)  =  0x80000028;
		*(volatile  unsigned  int*)(0xbff770d8)  =  0x80000028;

		*(volatile  unsigned  int*)(0xbff770c4)  =  0x1f;//rx
		*(volatile  unsigned  int*)(0xbff770c8)  =  0x1f;//tx

		*(volatile  unsigned  int*)(0xbff770c0)  =  0x13dc;
	}
#endif
}

static void usb_clk_config(void)
{
	// # Setup USB_CLK_CFG(0xFFF770F0)
	// set $USB_CLK_CFG=*0xFFF770F0
	unsigned int usb_clk_cfg = *(volatile unsigned int *)0xbff770f0;
	// # Set ahb_clk_usb_en=1 to enable USB AHB clock
	// set $USB_CLK_CFG |= (1<<17)
	usb_clk_cfg |= (1 << 17);
	// # Set cfg_apb_clk_usb_phy_en=1 to enable APB clock
	// set $USB_CLK_CFG |= (1<<16)
	usb_clk_cfg|= (1 << 16);
	// # Set usb_phy_ref_clk_div_en=1 to enable PHY ref clock
	// set $USB_CLK_CFG |= (1<<7)
	usb_clk_cfg |= (1 << 7);
	// # Set usb_suspend_clk_div_en=1 to enable suspend clock
	// set $USB_CLK_CFG |= (1<<15)
	usb_clk_cfg |= (1 << 15);
	// set *0xFFF770F0=$USB_CLK_CFG
	*(volatile unsigned int *)0xbff770f0 = usb_clk_cfg;
	// # delay 10us at least
	// shell sleep 0.1
	udelay(10);
	// # echo USB_CLK_CFG(0xFFF770F0, should be 0x39899) is:\n
	// # x/wx 0xFFF770F0

	// # Setup PHY internal ref clock (USB_CLK_CFG)
	// set *0xFE83009C=0x1
	*(volatile unsigned int *)0xbe83009c = 0x1;
	// # delay 10us at least
	// shell sleep 0.1
	udelay(10);
	// # echo USB_CLK_CFG(0xFE83009C) is:\n
	// # x/wx 0xFE83009C

	// # Release PHY (USB_PHY_USW_RST_CFG)
	// set *0xFFF78048=0x1
	*(volatile unsigned int *)0xbff78048 = 0x1;
	// # delay 10us at least
	// shell sleep 0.1
	udelay(10);
	// # echo USB_CLK_CFG(0xFFF78048) is:\n
	// # x/wx 0xFFF78048
}

// #################################
// # Define DelayTick function
// #################################
// define DelayTicks
//     set $total = $arg0
//     set $i = 0
//     set $temp = 0
//     while ($i < $total)
//         # Read SRAM which always can be read
//         set $temp = *(0xFE400000 + $i * 4)
//         set $i = $i + 1
//     end
// end
// static void delay_tick(int ticks)
// {
// 	unsigned int temp = 0;
// 	while (ticks--) {
// 		temp = *(volatile unsigned int *)0xbe400000;
// 	}
// }

static void gpu_config(void)
{
	*(volatile unsigned int *)0xbff77070 &= ~((1 << 21) | (1 << 20));
	udelay(100);
	*(volatile unsigned int *)0xbff7709c = 0x00011212;
	udelay(100);
	*(volatile unsigned int *)0xbff77070 |= (1 << 21) | (1 << 20);

	*(volatile unsigned int *)0xbff780a4=0;
	*(volatile unsigned int *)0xbff78094=0;
	*(volatile unsigned int *)0xbff780c0=0;
	udelay(100);

	*(volatile unsigned int *)0xbff78094=1;
	*(volatile unsigned int *)0xbff780c0=1;
	*(volatile unsigned int *)0xbff780a4=1;
	udelay(1000);

	//# x/wx 0x3fff27028 should get 0x20151217
	printf("GPU ChipDate is:0x%08x\n", *(volatile unsigned int *)0xbff27028);
	printf("GPU Frequency is:%dKHz\n", *(volatile unsigned int *)0xbff7713c);
}

static void npu_config(void)
{
	*(volatile unsigned int *)0xbff78044 = 0xff;
	udelay(100);

	printf("NPU ChipDate is:0x%08x\n", *(volatile unsigned int *)0xbff20028);
}

static void dpu_config(void)
{
	//set dpu_pixclk_div_en
	*(volatile unsigned int *)0xbff77098 |= 0x10811212;
	//enable dpu_aclk and dpu_cclk
	*(volatile unsigned int *)0xbff77070 |= (1 << 23 | 1 << 22);
	//set dpu_pixclk_div_en, dpu_pixclk_div_en, ahb_clk_dpu_en, dpu_aclk_div_en, dpu_cclk_div_en
	*(volatile unsigned int *)0xbff77098 |= 0x11811212;
	//dpu rst
	*(volatile unsigned int *)0xbff78090 |= (1 << 0);
	//dpu crst
	*(volatile unsigned int *)0xbff780a0 |= (1 << 0);
	//dpu arst
	*(volatile unsigned int *)0xbff7809c |= (1 << 0);
	//set DPU_DISPLAY_BUF as GPIO
	*(volatile unsigned int *)0xbe830700 |= (1 << 2);
	//Enable LCD 5V output, it also control by "lcd-power" in dts
	*(volatile unsigned int *)0xbff72000 |= (1 << 2);
	*(volatile unsigned int *)0xbff72004 |= (1 << 2);

	printf("DPU ChipDate is:0x%08x\n", *(volatile unsigned int *)0xbff28028);
}

static void vpu_clk_config(void)
{
	// # enable video clock
	// set *0xFFF77074 |= 0xf
	*(volatile unsigned int *)0xbff77074 |= 0xf;
	// # release video reset
	// set *0xFFF78064 |= (1 << 0)
	*(volatile unsigned int *)0xbff78064 |= (1 << 0);
}

// static void pcie_clk_config(void)
// {
// 	// # enable PCIe clock (apb, axi, aux)
// 	// set *0xFFF770E0 |= (1 << 9) | (1 << 8) | (1 << 7)
// 	*(volatile unsigned int *)0xbff770e0 |= (1 << 9) | (1 << 8) | (1 << 7);
// }

static void dmac_clk_config(void)
{
	// # enable DMAC clock
	// set *0xFFF77094 |= (1 << 11)
	*(volatile unsigned int *)0xbff77094 |= (1 << 11);
	// # enable SCE-dmac clock
	// set *0xFFF77104 |= 0x7
	*(volatile unsigned int *)0xbff77070 |= (1 << 13);

	// # de-assert DMAC reset
	// set *0xFFF78050 |= (1 << 0)
	*(volatile unsigned int *)0xbff78050 |= (1 << 0);
	// # de-assert SCE reset
	// set *0xFFF78054 |= 0x11010
	*(volatile unsigned int *)0xbff780c4 |= 0x11010;
}

static void sd_clk_config(void)
{
	// #################################
	// # Enable SD
	// #################################

	// # set SD1_CWPR/GPIO1-15 to GPIO
	// set *0xFE83033C |= (1 << 22)
	*(volatile unsigned int *)0xbe83033c |= (1 << 22);
	// # set SD1_CWPR/GPIO1-15 to output
	// set *0xFFF72004 |= (1 << 15)
	*(volatile unsigned int *)0xbff72004 |= (1 << 15);
	// # set SD1_CWPR/GPIO1-15 to Low(enable power)
	// set *0xFFF72000 &= ~(1 << 15)
	*(volatile unsigned int *)0xbff72000 &= ~(1 << 15);

	// # enable SD clock
	// set *0xFFF77078 = 0x0c800c80
	*(volatile unsigned int *)0xbff77078 = 0x0c800c80;

	// # de-assert SD reset
	// set *0xFFF78058 |= (1 << 16)
	*(volatile unsigned int *)0xbff78058 |= (1 << 16);
}

static void spi_config(void)
{
	// #################################
	// # Set padmux for SPI CS
	// #################################
	// # SPI0: config CS0/CS1 as GPIO
	// set *0xFE830334 |= (1 << 2) | (1 << 4)
	*(volatile unsigned int *)0xbe830334 |= (1 << 2) | (1 << 4);
	// # SPI1: config CS0/CS1 as GPIO
	// set *0xFE830334 |= (1 << 12) | (1 << 14)
	*(volatile unsigned int *)0xbe830334 |= (1 << 12) | (1 << 14);
	// # SFC: config SFC_CSN as GPIO
	// set *0xFE830330 |= (1 << 22)
	*(volatile unsigned int *)0xbe830330 |= (1 << 22);
}

void clock_init(void)
{
	gmac_clk_config(PHY_INTERFACE_MODE_RGMII);

	usb_clk_config();
	gpu_config();
	npu_config();
	dpu_config();
	vpu_clk_config();
	// pcie_clk_config();
	dmac_clk_config();
	sd_clk_config();
	spi_config();
}
