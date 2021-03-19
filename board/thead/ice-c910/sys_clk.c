/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>

void show_sys_clk(void)
{
	unsigned int read = 0;

	read = *(volatile unsigned int*)(0x3fff77120);
	debug("CPU_CLK = %dMHz\n", read / 1000);

	read = *(volatile unsigned int*)(0x3fff77124);
	debug("AXI_CLK = %dMHz\n", read / 1000);

	read = *(volatile unsigned int*)(0x3fff77130);
	debug("AHB_CLK = %dMHz\n", read / 1000);

	read = *(volatile unsigned int*)(0x3fff77140);
	debug("NPU_CLK = %dMHz\n", read / 1000);

	read = *(volatile unsigned int*)(0x3fff7712c);
	debug("DDR_CK = %d MT\n", read / 1000 * 4);
}

void cpu_clk_config(int cpu_freq)
{
	unsigned int read, i;

	for (i = 0; i < 0x40; i++) {
		*(volatile unsigned int*)(0x3fff78040) = 0x0; //reset ddr
	}

	//Wait ALL PLLs lock
	read = *(volatile unsigned int*)(0x3fff77060);
	while ((read & 0x1f) != 0x1f) {
		read = *(volatile unsigned int*)(0x3fff77060);
	}

	//enable dpu_clk
	*(volatile unsigned int*)(0x3fff77098) = 0x01601212;//div en
	*(volatile unsigned int*)(0x3fff77098) = 0x11611212;//pixclk_en, hclk_en

	//enable gpu_clk, 500MHz/500MHz
	*(volatile unsigned int*)(0x3fff7709c) = 0x11212; //gpu_hclk_en, gpu_aclk_div, gpu_cclk_div

	*(volatile unsigned int*)(0x3fff77070) = 0x871;	//AXI BUS 2:1
	read = *(volatile unsigned int*)(0x3fff77070);	//wait
	*(volatile unsigned int*)(0x3fff77070) = 0x879;	//sync
	asm("nop"); //MUST NOT REVISE
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	*(volatile unsigned int*)(0x3fff77090) = 0x1302;
	read = *(volatile unsigned int*)(0x3fff77090); //wait
	*(volatile unsigned int*)(0x3fff77090) = 0x9b0a; //sync
	asm("nop"); //MUST NOT REVISE
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	*(volatile unsigned int*)(0x3fff77070) = 0x809;
	asm("nop"); //MUST NOT REVISE
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	//PLLF, C910/C860 clock
	switch (cpu_freq) {
	case 2000:
		*(volatile unsigned int*)(0x3fff7704c) = 0x0110fa03; //2000MHz, 2000MHz
		break;
	case 1900:
		*(volatile unsigned int*)(0x3fff7704c) = 0x01104f01; //1896MHz, 1896MHz
		break;
	case 1800:
		*(volatile unsigned int*)(0x3fff7704c) = 0x01104b01; //1800MHz, 1800MHz
		break;
	case 1700:
		*(volatile unsigned int*)(0x3fff7704c) = 0x01104701; //1704MHz, 1704MHz
		break;
	case 1600:
		*(volatile unsigned int*)(0x3fff7704c) = 0x0110c803; //1600MHz, 1600MHz
		break;
	case 1500:
		*(volatile unsigned int*)(0x3fff7704c) = 0x01107d02; //1500MHz, 1500MHz
		break;
	case 1400:
		*(volatile unsigned int*)(0x3fff7704c) = 0x01103a01; //1392MHz, 1392MHz
		break;
	case 1300:
		*(volatile unsigned int*)(0x3fff7704c) = 0x01103601; //1296MHz, 1296MHz
		break;
	case 1200: //max legal freq for the ICE
		*(volatile unsigned int*)(0x3fff7704c) = 0x01103201; //1200MHz, 1200MHz
		break;
	case 1100:
		*(volatile unsigned int*)(0x3fff7704c) = 0x01205b01; //2184MHz, 1092MHz
		break;
	case 1000:
		*(volatile unsigned int*)(0x3fff7704c) = 0x0120fa03; //2000MHz, 1000MHz
		break;
	case 900:
		*(volatile unsigned int*)(0x3fff7704c) = 0x01204b01; //1800MHz, 900MHz
		break;
	case 800:
		*(volatile unsigned int*)(0x3fff7704c) = 0x0120c803; //1600MHz, 800MHz
		break;
	case 700:
		*(volatile unsigned int*)(0x3fff7704c) = 0x01203a01; //1392MHz, 696MHz
		break;
	case 600:
		*(volatile unsigned int*)(0x3fff7704c) = 0x01203201; //1200MHz, 600MHz
		break;
	case 500:
		*(volatile unsigned int*)(0x3fff7704c) = 0x0140fa03; //2000MHz, 500MHz
		break;
	}
	*(volatile unsigned int*)(0x3fff77050) = 0x03000000;
	*(volatile unsigned int*)(0x3fff77054) = 0x3;	//reconfig
	read = *(volatile unsigned int*)(0x3fff7704c);	//readback
	//Wait ALL PLLs Lock
	read = *(volatile unsigned int*)(0x3fff77060);
	read = *(volatile unsigned int*)(0x3fff77060);
	read = *(volatile unsigned int*)(0x3fff77060);
	while ((read & 0x3f) != 0x3f) {
		read = *(volatile unsigned int*)(0x3fff77060);
	}

	*(volatile unsigned int*)(0x3fe830470) = 0x11;	//AXI BUS 2:1
	read = *(volatile unsigned int*)(0x3fe830470);	//wait
	*(volatile unsigned int*)(0x3fe830470) = 0x19;	//sync
	asm("nop"); //MUST NOT REVISE
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	*(volatile unsigned int*)(0x3fe830470) = 0x9;	//switch
	asm("nop"); //MUST NOT REVISE
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	*(volatile unsigned int*)(0x3fff77094) = 0xffffffff;

	*(volatile unsigned int*)(0x3fff770a0) = 0xfff;
	*(volatile unsigned int*)(0x3fff78044) = 0xff;

	//SDIO0/1
	*(volatile unsigned int*)(0x3fff77078) = 0x0c800c80;
}

void ddr_clk_config(int ddr_freq)
{
	unsigned int read;

	//DDRC clock
	switch (ddr_freq) {
	case 3192:
		*(volatile unsigned int*)(0x3fff77020) = 0x01208502; //798MHz, 3192MT
		break;
	case 3008:
		*(volatile unsigned int*)(0x3fff77020) = 0x01305e01; //752MHz, 3008MT
		break;
	case 2912:
		*(volatile unsigned int*)(0x3fff77020) = 0x01305b01; //728MHz, 2912MT
		break;
	case 2816:
		*(volatile unsigned int*)(0x3fff77020) = 0x01305801; //704MHz, 2816MT
		break;
	case 2720:
		*(volatile unsigned int*)(0x3fff77020) = 0x01305501; //680MHz, 2720MT
		break;
	case 2656:
		*(volatile unsigned int*)(0x3fff77020) = 0x01305301; //664MHz, 2656MT
		break;
	case 2592:
		*(volatile unsigned int*)(0x3fff77020) = 0x01305101; //648MHz, 2592MT
		break;
	case 2496:
		*(volatile unsigned int*)(0x3fff77020) = 0x01304e01; //624MHz, 2496MT
		break;
	case 2400:
		*(volatile unsigned int*)(0x3fff77020) = 0x01406401; //600MHz, 2400MT
		break;
	case 2184:
		*(volatile unsigned int*)(0x3fff77020) = 0x01405b01; //546MHz, 2184MT
		break;
	case 2133:
		*(volatile unsigned int*)(0x3fff77020) = 0x01405801; //528MHz, 2112MT
		break;
	case 2000:
		*(volatile unsigned int*)(0x3fff77020) = 0x01607d01; //500MHz, 2000MT
		break;
	case 1800:
		*(volatile unsigned int*)(0x3fff77020) = 0x01404b01; //450MHz, 1800MT
		break;
	case 1600:
		*(volatile unsigned int*)(0x3fff77020) = 0x01606401; //400MHz, 1600MT
		break;
	case 1392:
		*(volatile unsigned int*)(0x3fff77020) = 0x01605701; //348MHz, 1392MT
		break;
	default:
		*(volatile unsigned int*)(0x3fff77020) = 0x01604b01; //300MHz, 1200MT
		break;
	}

	*(volatile unsigned int*)(0x3fff77024) = 0x03000000;
	*(volatile unsigned int*)(0x3fff77028) = 0x3;	//reconfig
	read = *(volatile unsigned int*)(0x3fff77020);	//readback
	//Wait ALL PLLs Lock
	read = *(volatile unsigned int*)(0x3fff77060);
	read = *(volatile unsigned int*)(0x3fff77060);
	read = *(volatile unsigned int*)(0x3fff77060);
	while ((read & 0x1f) != 0x1f) {
		read = *(volatile unsigned int*)(0x3fff77060);
	}

	//enable ddr_axi clocks
	*(volatile unsigned int*)(0x3fff78040) = 0x0;
	*(volatile unsigned int*)(0x3fff78040) = 0x0;
	*(volatile unsigned int*)(0x3fff78040) = 0x0;
	*(volatile unsigned int*)(0x3fff78040) = 0x0;
	*(volatile unsigned int*)(0x3fff77108) = 0x1ff;
	*(volatile unsigned int*)(0x3fff77108) = 0x1ff;
	*(volatile unsigned int*)(0x3fff77108) = 0x1ff;
	*(volatile unsigned int*)(0x3fff78040) = 0x0;
	*(volatile unsigned int*)(0x3fff78040) = 0x0;
	*(volatile unsigned int*)(0x3fff78040) = 0x0;
	*(volatile unsigned int*)(0x3fff78040) = 0x0;
	*(volatile unsigned int*)(0x3fff78040) = 0x9;
	*(volatile unsigned int*)(0x3fff78040) = 0x9;

	//disable wdg
	*(volatile unsigned int*)(0x3fff78000) = 0x5ada7200;
	*(volatile unsigned int*)(0x3fff78010) = 0x0;
}
