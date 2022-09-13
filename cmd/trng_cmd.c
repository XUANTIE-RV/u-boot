// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2021 Alibaba Group Holding Limited.
 */

#include <common.h>
#include <autoboot.h>
#include <bootretry.h>
#include <cli.h>
#include <command.h>
#include <console.h>
#include <env.h>
#include <fdtdec.h>
#include <hash.h>
#include <memalign.h>
#include <menu.h>
#include <post.h>
#include <time.h>
#include <u-boot/sha256.h>
#include <bootcount.h>

extern int csi_rng_get_multi_word(uint32_t *data, uint32_t num);
#define CSI_OK 0
void
trng_hexdump(
        const char * szPrefix_p,
        const unsigned int PrintOffset,
        const uint8_t * Buffer_p,
        const unsigned int ByteCount)
{
    unsigned int i;
    char Format[] = "%s %08d:";

    for(i = 0; i < ByteCount; i += 16)
    {
        unsigned int j, Limit;

        // if we do not have enough data for a full line
        if (i + 16 > ByteCount)
            Limit = ByteCount - i;
        else
            Limit = 16;

        // printf(Format, szPrefix_p, PrintOffset + i);

        for (j = 0; j < Limit; j++)
            //printf(" %02X", Buffer_p[i+j]);
            printf("%02X", Buffer_p[i+j]);

        //printf("\n");
    } // for
}

//128M
 #define  RB_TRNG_SYS_TEST_LEN_IN_BYTE (1024 * 1024 * 128)
//#define  RB_TRNG_SYS_TEST_LEN_IN_BYTE (1024)
#define RB_TRNG_SYS_TEST_BLOCK_IN_BYTE (1024)
static uint32_t g_sys_buf[RB_TRNG_SYS_TEST_BLOCK_IN_BYTE] = {0x0};


uint32_t g_rb_trng_reseed_cur = 0;

/* config */
uint32_t g_rb_trng_ctrl = 0;
/* MAX reseed count in block */
uint32_t g_rb_trng_reseed_max = 4195;

/* 1:enable, 0:disable */
uint32_t g_rb_trng_reseed_enable = 1;

/* request times in block */
uint32_t g_rb_trng_request_block = 0;


/* 1:enable, 0:disable */
uint32_t g_rb_trng_auto_detune = 1;

/* EIP76_ALARM_THRESHOLD */
uint32_t g_alarm_threshold = 255;

/* EIP76_SHUTDOWN_THRESHOLD */
uint32_t g_shutdown_threshold = 6;

/* mode 4 in docdument */
uint32_t g_long_request_mode = 0;


static int rb_trng_sys_test(uint32_t times)
{
	printf("===%s, %d\n", __FUNCTION__, __LINE__);
	printf("===%s, %d\n", __FUNCTION__, times);
	printf("===config %s, %x\n", __FUNCTION__, g_rb_trng_ctrl);
	printf("===max %s, %d\n", __FUNCTION__, g_rb_trng_reseed_max);
	printf("===resedd en %s, %d\n", __FUNCTION__, g_rb_trng_reseed_enable);
	printf("===r block %s, %d\n", __FUNCTION__, g_rb_trng_request_block);
	printf("===auto d %s, %d\n", __FUNCTION__, g_rb_trng_auto_detune);
	printf("===alarm threshold %s, %d\n", __FUNCTION__, g_alarm_threshold);
	printf("===shutdown threshold %s, %d\n", __FUNCTION__, g_shutdown_threshold);
	uint32_t ret = trng_init();
	if (ret != CSI_OK) {
		// LOG_INFO("fail to get trng multi data\n");
		// finish(FAIL);
		return ret;
	}
	g_rb_trng_reseed_cur = 0;
	/* delay */
	mdelay(200);


	//CHECK_RET_WITH_RET(ret == CSI_OK, ret);
        printf("===%s, 0x%x\n", "rb_trng_sys_test bytes:", 16 * g_rb_trng_request_block);
	if ((16 * g_rb_trng_request_block % 1024) != 0) {
		printf("===error length must ali to K Byte %s, %d\n", __FUNCTION__, __LINE__);
		return 0;
	}
        for (int i = 0; i < ((16 * g_rb_trng_request_block) /
                             RB_TRNG_SYS_TEST_BLOCK_IN_BYTE);
             i++) {
                ret = csi_rng_get_multi_word(
                    g_sys_buf, RB_TRNG_SYS_TEST_BLOCK_IN_BYTE / 4);
                if (ret != CSI_OK) {
                        // LOG_INFO("fail to get trng multi data\n");
                        // finish(FAIL);
                        return ret;
                }

		memcpy(0x80000000 + i*1024 + (16 * g_rb_trng_request_block) * times, g_sys_buf, 1024);
                // trng_hexdump("", i * RB_TRNG_SYS_TEST_BLOCK_IN_BYTE, g_sys_buf, RB_TRNG_SYS_TEST_BLOCK_IN_BYTE);
        }
        // finish(PASS);
        return 0;
}


// static int rb_trng_sys_test_long_request(uint32_t times)
// {
//         printf("===%s, %d\n", __FUNCTION__, __LINE__);
//         printf("===%s, %d\n", __FUNCTION__, times);
//         printf("===config %s, %x\n", __FUNCTION__, g_rb_trng_ctrl);
//         printf("===max %s, %d\n", __FUNCTION__, g_rb_trng_reseed_max);
//         printf("===resedd en %s, %d\n", __FUNCTION__, g_rb_trng_reseed_enable);
//         printf("===r block %s, %d\n", __FUNCTION__, g_rb_trng_request_block);
//         printf("===auto d %s, %d\n", __FUNCTION__, g_rb_trng_auto_detune);
//         printf("===alarm threshold %s, %d\n", __FUNCTION__, g_alarm_threshold);
//         printf("===shutdown threshold %s, %d\n", __FUNCTION__,
//                g_shutdown_threshold);
//         uint32_t ret = trng_init();
//         if (ret != CSI_OK) {
//                 // LOG_INFO("fail to get trng multi data\n");
//                 // finish(FAIL);
//                 return ret;
//         }
//         g_rb_trng_reseed_cur = 0;
//         /* delay */
//         mdelay(200);

//         //CHECK_RET_WITH_RET(ret == CSI_OK, ret);
//         printf("===%s, 0x%x\n",
//                "rb_trng_sys_test bytes:", 16 * g_rb_trng_request_block);
//         if ((16 * g_rb_trng_request_block % 1024) != 0) {
//                 printf("===error length must ali to K Byte %s, %d\n",
//                        __FUNCTION__, __LINE__);
//                 return 0;
//         }
//         for (int i = 0; i < ((16 * g_rb_trng_request_block) /
//                              RB_TRNG_SYS_TEST_BLOCK_IN_BYTE);
//              i++) {
// #if 0
//                 ret = csi_rng_get_multi_word(
//                     g_sys_buf, RB_TRNG_SYS_TEST_BLOCK_IN_BYTE / 4);
//                 if (ret != CSI_OK) {
//                         // LOG_INFO("fail to get trng multi data\n");
//                         // finish(FAIL);
//                         return ret;
//                 }
// #else
//                 /* 1 */
//                 EIP76_Write32(NULL, EIP76_REG_CONTROL,
//                               0x10000 | (g_rb_trng_reseed_max << 20));
//                 /* 2 */
//                 while ((EIP76_Read32(NULL, EIP76_REG_STATUS) & 0x00000001) != 1)
//                         ;
//                 /* 3 read 1k byte data*/
		
//                 int *p = g_sys_buf;
//                 for (int i = 0; i < (1024 / 16); i++) {
//                         *p = EIP76_Read32(NULL, EIP76_REG_OUTPUT_0);
//                         p++;
//                         *p = EIP76_Read32(NULL, EIP76_REG_OUTPUT_1);
//                         p++;
//                         *p = EIP76_Read32(NULL, EIP76_REG_OUTPUT_2);
//                         p++;
//                         *p = EIP76_Read32(NULL, EIP76_REG_OUTPUT_3);
//                         p++;
//                 }
//                 /* 4 */
//                 /* 5 */
//                 /* 6 */
//                 /* 7 */
//                 /* 8 */
//                 /* 9 */
//                 /* 10 */
//                 /* 11 */
//                 /* 12*/
//                 /* 13 */
// #endif
//                 memcpy(0x80000000 + i * 1024 +
//                            (16 * g_rb_trng_request_block) * times,
//                        g_sys_buf, 1024);
//                 // trng_hexdump("", i * RB_TRNG_SYS_TEST_BLOCK_IN_BYTE, g_sys_buf, RB_TRNG_SYS_TEST_BLOCK_IN_BYTE);
//         }
//         // finish(PASS);
//         return 0;
// }

extern int rb_trng_sys_test_entropy();

static int do_rb_trng_sampling(cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[])
{
	ulong ctrl, times;
	ulong reseed_max, reseed_enable, request_block, auto_detune, alarm_threshold, shutdown_threshold;
	ulong entropy_mode = 0;
        ulong long_request_mode = 0;
	int ret;

	ctrl = 0x0011034C;
	times = 1;

	if (argc > 1)
		if (strict_strtoul(argv[1], 16, &ctrl) < 0)
			return CMD_RET_USAGE;

	if (argc > 2)
		if (strict_strtoul(argv[2], 16, &times) < 0)
			return CMD_RET_USAGE;

	if (argc > 3) {
		if (strict_strtoul(argv[3], 16, &reseed_max) < 0)
			return CMD_RET_USAGE;
		g_rb_trng_reseed_max = (uint32_t)reseed_max;
	}

	if (argc > 4) {
		if (strict_strtoul(argv[4], 16, &reseed_enable) < 0)
			return CMD_RET_USAGE;
		g_rb_trng_reseed_enable = (uint32_t)reseed_enable;
	}

	if (argc > 5) {
		if (strict_strtoul(argv[5], 16, &request_block) < 0)
			return CMD_RET_USAGE;
		g_rb_trng_request_block = (uint32_t)request_block;
	}

	if (argc > 6) {
		if (strict_strtoul(argv[6], 16, &auto_detune) < 0)
			return CMD_RET_USAGE;
		g_rb_trng_auto_detune = (uint32_t)auto_detune;
	}

	if (argc > 7) {
		if (strict_strtoul(argv[7], 16, &alarm_threshold) < 0)
			return CMD_RET_USAGE;
		g_alarm_threshold = (uint32_t)alarm_threshold;
	}

	if (argc > 8) {
		if (strict_strtoul(argv[8], 16, &shutdown_threshold) < 0)
			return CMD_RET_USAGE;
		g_shutdown_threshold = (uint32_t)shutdown_threshold;
	}

	if (argc > 9) {
		if (strict_strtoul(argv[9], 16, &entropy_mode) < 0)
			return CMD_RET_USAGE;
	}

        if (argc > 10) {
                if (strict_strtoul(argv[10], 16, &long_request_mode) < 0)
                        return CMD_RET_USAGE;
        }

        printf(
            "Sampling TRNG raw data with ctrl:%08lx, sampling times:%08lx:\n",
            ctrl, times);
        g_rb_trng_ctrl = (uint32_t)ctrl;
        for (int i = 0; i < times; i++) {
                /* TODO laoduan add switch to rb_trng_sys_test or  rb_trng_sys_test_entropy */
                if (!entropy_mode) {
                        if (long_request_mode) {
                                g_long_request_mode = 1;
                        }
                        rb_trng_sys_test(i);

                }
                else
                        rb_trng_sys_test_entropy();

        }

        return 0;
}

U_BOOT_CMD(rb_trng_sampling, 11, 1, do_rb_trng_sampling,
           "Light MPW TRNG sampling",
           "[ctrl times reseed_max reseed_enable request_block auto_detune "
           "alarm_threshold shutdown_threshold mode long_request_mode]");

