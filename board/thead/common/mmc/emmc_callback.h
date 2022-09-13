/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __ASM_ARCH_EMMC_CALLBACK_H
#define __ASM_ARCH_EMMC_CALLBACK_H

#include "emmc.h"

/* Definitions for cmd status */
#define CMD_STAT_ABSENT         0
#define CMD_STAT_STARTED        1
#define CMD_STATE_READRESP      2
#define CMD_STATE_READDAT       3
#define CMD_STATE_WRITEDAT      4
#define CMD_COMMAND_DONE        5
#define CMD_COMMAND_ABORTING    6
#define CMD_STATE_POLLD         7

emmc_postproc_callback emmc_get_post_callback(u32);
emmc_preproc_callback emmc_get_pre_callback(u32);
void emmc_set_data_trans_params(u32 slot, uint8_t *data_buffer,
                                u32 num_of_blocks, u32 epoch_count,
                                u32 flag, u32 custom_blocksize);

void emmc_set_current_task_status_t(u32 slot, u32 *resp_buffer,
                                    u8 *data_buffer);

#endif /* __ASM_ARCH_EMMC_CALLBACK_H */

