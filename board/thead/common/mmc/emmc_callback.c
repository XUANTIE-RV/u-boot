/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#include "emmc_callback.h"
#include "emmc_interface.h"

typedef struct {
	emmc_preproc_callback preproc;
	emmc_postproc_callback postproc;
} callback_t;

typedef struct {
	u32 cmd_index;
	callback_t the_callbacks;
} callback_search_table;

current_task_status_t current_task;

static void no_response_preproc(u32 card_num, u32 cmd_index,
                                u32 *cmd_reg, u32 *arg_reg)
{
	UNSET_BITS(*cmd_reg, CMD_ABRT_CMD_BIT);
	UNSET_BITS(*cmd_reg, CMD_RESP_EXP_BIT);
	UNSET_BITS(*cmd_reg, CMD_RESP_LENGTH_BIT);
	SET_BITS(*cmd_reg, CMD_USE_HOLD);
	SET_CARD_NUM(*cmd_reg, card_num);

	if (cmd_index > 200) {
		cmd_index -= 200;
	}

	SET_CMD_INDEX(*cmd_reg, cmd_index);

}

static void no_response_preproc_abrt(u32 card_num, u32 cmd_index,
                                     u32 *cmd_reg, u32 *arg_reg)
{
	no_response_preproc(card_num, cmd_index, cmd_reg, arg_reg);
	SET_BITS(*cmd_reg, CMD_ABRT_CMD_BIT);
}
static void long_response_preproc(u32 card_num, u32 cmd_index,
                                  u32 *cmd_reg, u32 *arg_reg)
{
	UNSET_BITS(*cmd_reg, CMD_ABRT_CMD_BIT);
	SET_BITS(*cmd_reg, CMD_RESP_EXP_BIT);
	SET_BITS(*cmd_reg, CMD_RESP_LENGTH_BIT);
	SET_BITS(*cmd_reg, CMD_CHECK_CRC_BIT);
	SET_BITS(*cmd_reg, CMD_USE_HOLD);
	SET_CARD_NUM(*cmd_reg, card_num);

	if (cmd_index > 200) {
		cmd_index -= 200;
	}

	SET_CMD_INDEX(*cmd_reg, cmd_index);
}

static void short_response_preproc(u32 card_num, u32 cmd_index,
                                   u32 *cmd_reg, u32 *arg_reg)
{
	SET_BITS(*cmd_reg, CMD_RESP_EXP_BIT);
	UNSET_BITS(*cmd_reg, CMD_RESP_LENGTH_BIT);

	if (cmd_index > 200) {
		cmd_index -= 200;
	}

	SET_BITS(*cmd_reg, CMD_USE_HOLD);
	SET_CMD_INDEX(*cmd_reg, cmd_index);
	SET_CARD_NUM(*cmd_reg, card_num);
}
static void short_response_preproc_abrt(u32 card_num, u32 cmd_index,
                                        u32 *cmd_reg, u32 *arg_reg)
{
	short_response_preproc(card_num, cmd_index, cmd_reg, arg_reg);
	SET_BITS(*cmd_reg, CMD_ABRT_CMD_BIT);
}

static void short_response_preproc_with_init(u32 card_num, u32 cmd_index,
        u32 *cmd_reg, u32 *arg_reg)
{
	short_response_preproc(card_num, cmd_index, cmd_reg, arg_reg);
}

static void short_response_block_data_preproc(u32 card_num,
        u32 cmd_index,
        u32 *cmd_reg, u32 *arg_reg)
{
	short_response_preproc(card_num, cmd_index, cmd_reg, arg_reg);
	SET_BITS(*cmd_reg, CMD_DATA_EXP_BIT);

	/*
	Some of MMC/SD cards misbehave (block skip problem) when auto_stop_bit is set for
	a multi block read. So Driver should send the STOP CMD (CMD12) after multi block read
	is complete.
	*/
	UNSET_BITS(*cmd_reg, CMD_SENT_AUTO_STOP_BIT);

	if (cmd_index == CMD17) {
		UNSET_BITS(*cmd_reg, CMD_SENT_AUTO_STOP_BIT);
	}

	SET_BITS(*cmd_reg, CMD_WAIT_PRV_DAT_BIT);
}


static void short_response_block_data_preproc_noac(u32 card_num,
        u32 cmd_index,
        u32 *cmd_reg,
        u32 *arg_reg)
{
	short_response_block_data_preproc(card_num, cmd_index, cmd_reg, arg_reg);
	UNSET_BITS(*cmd_reg, CMD_SENT_AUTO_STOP_BIT);
}

static void short_response_block_write_preproc(u32 card_num,
        u32 cmd_index,
        u32 *cmd_reg,
        u32 *arg_reg)
{
	short_response_preproc(card_num, cmd_index, cmd_reg, arg_reg);
	SET_BITS(*cmd_reg, CMD_DATA_EXP_BIT);
	SET_BITS(*cmd_reg, CMD_RW_BIT);
	/*
	Some of MMC/SD cards misbehave (block skip problem) when auto_stop_bit is set for
	a multi block write. So Driver should send the STOP CMD (CMD12) after multi block write
	is complete.
	*/
	UNSET_BITS(*cmd_reg, CMD_SENT_AUTO_STOP_BIT);//Just to check with CMD23 instead of autotstopcommand

	if ((cmd_index == CMD24) || (cmd_index == CMD42)) {
		UNSET_BITS(*cmd_reg, CMD_SENT_AUTO_STOP_BIT);
	}

	SET_BITS(*cmd_reg, CMD_WAIT_PRV_DAT_BIT);

}
#ifdef CONFIG_SUPPORT_ALL_EMMC_CMD

static void short_response_stream_write_preproc(u32 card_num,
        u32 cmd_index,
        u32 *cmd_reg,
        u32 *arg_reg)
{
	short_response_block_write_preproc(card_num, cmd_index, cmd_reg, arg_reg);
	SET_BITS(*cmd_reg, CMD_TRANSMODE_BIT);
}

static void short_response_block_write_preproc_noac(u32 card_num,
        u32 cmd_index,
        u32 *cmd_reg,
        u32 *arg_reg)
{
	short_response_block_write_preproc(card_num, cmd_index, cmd_reg, arg_reg);
	UNSET_BITS(*cmd_reg, CMD_SENT_AUTO_STOP_BIT);
}
#endif
static void short_response_rca_preproc(u32 card_num, u32 cmd_index,
                                       u32 *cmd_reg, u32 *arg_reg)
{
	short_response_preproc(card_num, cmd_index, cmd_reg, arg_reg);
	// SET_RCA(*arg_reg, card_info.the_rca);
}

static void long_response_rca_preproc(u32 card_num, u32 cmd_index,
                                      u32 *cmd_reg, u32 *arg_reg)
{
	long_response_preproc(card_num, cmd_index, cmd_reg, arg_reg);
	//SET_RCA(*arg_reg, card_info.the_rca);
}

void short_response_postproc(void *the_data, u32 *interrupt_status)
{
	current_task_status_t *the_task_status = (current_task_status_t *) the_data;

	/* Handle standard interrupt handler */

	/* Read the short response */
	if (the_task_status->resp_buffer) {
		the_task_status->resp_buffer[0] =  emmc_read_register(RESP0);
	}
}

void long_response_postproc(void *the_data, u32 *interrupt_status)
{
	current_task_status_t *the_task_status = (current_task_status_t *) the_data;

	/* Handle standard interrupt handler */

	if (the_task_status->resp_buffer) {
		the_task_status->resp_buffer[0] =  emmc_read_register(RESP0);
		the_task_status->resp_buffer[1] =  emmc_read_register(RESP1);
		the_task_status->resp_buffer[2] =  emmc_read_register(RESP2);
		the_task_status->resp_buffer[3] =  emmc_read_register(RESP3);
	}
}

void no_response_postproc(void *the_data, u32 *interrupt_status)
{
	current_task_status_t *the_task_status = (current_task_status_t *) the_data;

	/* Handle standard interrupt handler */

	/* Check if there is any error */
	if (the_task_status->error_status) {
		the_task_status->cmd_status = CMD_STAT_ABSENT;
		return;
	}

	return;
}

static void r1_r6_response_postproc(void *the_data, u32 *interrupt_status)
{
	u32 r1_check_val;
	current_task_status_t *the_task_status = (current_task_status_t *) the_data;

	short_response_postproc(the_data, interrupt_status);

	if (the_task_status->error_status) {
		the_task_status->bus_corruption_occured = 1;
		the_task_status->error_status = 0;
	}

	if (the_task_status->resp_buffer) {

		r1_check_val = the_task_status->resp_buffer[0];

		the_task_status->error_status = emmc_check_r1_resp(r1_check_val);
	}

	return;
}

static void r1_response_postproc(void *the_data, u32 *interrupt_status)
{
	current_task_status_t *the_task_status = (current_task_status_t *) the_data;
	short_response_postproc(the_data, interrupt_status);

	if (the_task_status->error_status) {
		return;
	}

	if (the_task_status->resp_buffer) {
		the_task_status->error_status = emmc_check_r1_resp(the_task_status->resp_buffer[0]);
	}

	return;
}

static void r1b_response_postproc(void *the_data, u32 *interrupt_status)
{
	r1_response_postproc(the_data, interrupt_status);

	while ((emmc_read_register(STATUS)) & STATUS_DAT_BUSY_BIT);

	return;
}

#ifdef CONFIG_SUPPORT_ALL_EMMC_CMD
static void r1_response_write_bstst_postproc(void *the_data,
        u32 *interrupt_status)
{
	current_task_status_t *the_task_status = (current_task_status_t *) the_data;
	u32 cmd_status;

	//The following variables are required for IDMAC mode interrupt handling

	r1_response_postproc(the_data, interrupt_status);

	if (ERRENDBITERR == the_task_status->error_status) {
		the_task_status->error_status = 0;
	}


	cmd_status = the_task_status->cmd_status;

	while (1) {
		*interrupt_status = emmc_read_register(RINTSTS);

		if (cmd_status != CMD_STATE_WRITEDAT) {
			the_task_status->cmd_status = CMD_STATE_WRITEDAT;

			if (*interrupt_status & INTMSK_DAT_OVER) {
				*interrupt_status &= (*interrupt_status & ~INTMSK_DAT_OVER);
			}
		}

		if ((*interrupt_status & INTMSK_TXDR)) {
			emmc_write_out_data(the_task_status, INTMSK_TXDR);
		}

		if (*interrupt_status & INTMSK_DAT_OVER) {
			the_task_status->cmd_status = CMD_COMMAND_DONE;
			break;
		}
	}

	return;
}
#endif
static void r1b_response_write_data_postproc(void *the_data,
        u32 *interrupt_status)
{
	current_task_status_t *the_task_status = (current_task_status_t *) the_data;

	//The following variables are required for IDMAC mode interrupt handling
	r1b_response_postproc(the_data, interrupt_status);

	if (ERRENDBITERR == the_task_status->error_status) {
		the_task_status->bus_corruption_occured = 1;
		the_task_status->error_status = 0;
	}

	while (1) {
		*interrupt_status = emmc_read_register(RINTSTS);

		if ((*interrupt_status & INTMSK_TXDR)) {
			emmc_write_out_data(the_task_status, INTMSK_TXDR);
		}

		if (*interrupt_status & INTMSK_DAT_OVER) {
			the_task_status->cmd_status = CMD_COMMAND_DONE;
			break;
		}
	}

	return;
}

static void r1_response_write_data_postproc(void *the_data, u32 *interrupt_status)
{
	current_task_status_t *the_task_status = (current_task_status_t *) the_data;

	//The following variables are required for IDMAC mode interrupt handling

	r1_response_postproc(the_data, interrupt_status);

	if (ERRENDBITERR == the_task_status->error_status) {
		the_task_status->bus_corruption_occured = 1;
		the_task_status->error_status = 0;
	}

	// The interrupts are interpreted as IDMAC interrupts for data transfer commands in IDMAC mode
	while (1) {
		*interrupt_status = emmc_read_register(RINTSTS);

		if ((*interrupt_status & INTMSK_TXDR)) {
			emmc_write_out_data(the_task_status, INTMSK_TXDR);
			emmc_set_register(RINTSTS, INTMSK_TXDR);
		}

		if (*interrupt_status & INTMSK_DAT_OVER) {
			the_task_status->cmd_status = CMD_COMMAND_DONE;
			break;
		}
	}

	return;
}
#if 0
static void r1_response_read_bstst_postproc(void *the_data,
        u32 *interrupt_status)
{
	current_task_status_t *the_task_status = (current_task_status_t *) the_data;
	u32 cmd_status;
	//The following variables are required for IDMAC mode interrupt handling

	r1_response_postproc(the_data, interrupt_status);

	if ((ERRDCRC == the_task_status->error_status) || (ERRENDBITERR == the_task_status->error_status)) {
		the_task_status->error_status = 0;
	}

	if (the_task_status->error_status) {
		return;
	}

	if (!(the_task_status->error_status)) {
		the_task_status->cmd_status = CMD_STATE_READDAT;
	}

	cmd_status = the_task_status->cmd_status;

	if (CMD_STATE_READDAT == cmd_status) {
		while (1) {
			*interrupt_status = emmc_read_register(RINTSTS);

			if ((*interrupt_status & INTMSK_RXDR)) {
				emmc_read_in_data(the_task_status, INTMSK_RXDR);
			}

			if ((*interrupt_status & INTMSK_DAT_OVER)) {
				emmc_read_in_data(the_task_status, INTMSK_DAT_OVER);
			}

			if ((*interrupt_status & INTMSK_DAT_OVER) && !(the_task_status->error_status)) {
				the_task_status->cmd_status = CMD_COMMAND_DONE;
				break;
			}
		}
	}

	return;
}
#endif
static void r1_response_read_data_postproc(void *the_data, u32 *interrupt_status)
{
	current_task_status_t *the_task_status = (current_task_status_t *) the_data;
	//u32 cmd_status;

	r1_response_postproc(the_data, interrupt_status);

	if (ERRDCRC == the_task_status->error_status) {
		the_task_status->bus_corruption_occured = 1;
		the_task_status->error_status = 0;
	}

	the_task_status->cmd_status = CMD_STATE_READDAT;

	while (1) {
		*interrupt_status = emmc_read_register(RINTSTS);
		// mini_printf("*interrupt_status =%x,error_status =%x\n",*interrupt_status,the_task_status->error_status);

		if ((*interrupt_status & INTMSK_RXDR)) {
			emmc_read_in_data(the_task_status, INTMSK_RXDR);
			emmc_set_register(RINTSTS, INTMSK_RXDR);
		}

		if ((*interrupt_status & INTMSK_DAT_OVER)) {
			emmc_read_in_data(the_task_status, INTMSK_DAT_OVER);
			emmc_set_register(RINTSTS, INTMSK_DAT_OVER);
		}

		if ((*interrupt_status & INTMSK_DAT_OVER)) {
			the_task_status->cmd_status = CMD_COMMAND_DONE;
			break;
		}
		if ((*interrupt_status & INTMSK_SBE)) {
			emmc_set_register(RINTSTS, INTMSK_SBE);
		}
	}

	return;
}

#ifdef CONFIG_SUPPORT_ALL_EMMC_CMD

static void short_response_sd_app_specific_data(u32 card_num,
        u32 cmd_index,
        u32 *cmd_reg,
        u32 *arg_reg)
{
	// emmc_send_serial_command(card_num, CMD55, 0, NULL, NULL, 0);
	short_response_preproc(card_num, cmd_index, cmd_reg, arg_reg);
	SET_BITS(*cmd_reg, CMD_WAIT_PRV_DAT_BIT);

	return;
}


static void short_response_sd_data_app_specific_data(u32 card_num,
        u32 cmd_index,
        u32 *cmd_reg,
        u32 *arg_reg)
{

	short_response_sd_app_specific_data(card_num, cmd_index, cmd_reg, arg_reg);
	SET_BITS(*cmd_reg, CMD_DATA_EXP_BIT);

	return;
}
#endif
/**
 * Command Index callback table.
 * This is a table sorted by command index for the command indices and
 * their corresponding callbacks. This table allows easy manipulation of
 * new command addition and thier handling logic.
 * \todo Use this table for a minimal perfect hashing rather than a
 * binary search <i>(use gperf ?)</i>
 */
static const callback_search_table the_callback_table[] = {
	{CMD0, {no_response_preproc_abrt, no_response_postproc}},
	{CMD1, {short_response_preproc_with_init, short_response_postproc}},
	{CMD2, {long_response_preproc, long_response_postproc}},
	{CMD3, {short_response_preproc, r1_r6_response_postproc}},
	{CMD6, {short_response_preproc, r1b_response_postproc}},
	{CMD7, {short_response_rca_preproc, r1b_response_postproc}},
	{CMD8, {short_response_block_data_preproc_noac, r1_response_read_data_postproc}},
	{CMD9, {long_response_rca_preproc, long_response_postproc}},
	{CMD12, {short_response_preproc_abrt, r1b_response_postproc}},
	{CMD13, {short_response_rca_preproc, r1_response_postproc}},
	{CMD16, {short_response_preproc, r1_response_postproc}},
	{CMD17, {short_response_block_data_preproc, r1_response_read_data_postproc}},
	{CMD23, {short_response_preproc, r1_response_postproc}},
	{CMD24, {short_response_block_write_preproc, r1_response_write_data_postproc}},
	{CMD42, {short_response_block_write_preproc, r1b_response_write_data_postproc}},
	{UNADD_CMD7, {no_response_preproc, no_response_postproc}},
#ifdef CONDIF_SUPPORT_MULTI_BLOCK
	{CMD18, {short_response_block_data_preproc, r1_response_read_data_postproc}},
	{CMD25, {short_response_block_write_preproc, r1_response_write_data_postproc}},
#endif
#ifdef EMMC_SUPPORT_GET_CARD
	{CMD55, {short_response_rca_preproc, short_response_postproc}},
	{ACMD41, {short_response_preproc, short_response_postproc}},
#endif
#ifdef CONFIG_SUPPORT_ALL_EMMC_CMD
	{CMD5, {short_response_preproc, short_response_postproc}},
	{CMD14, {short_response_block_data_preproc_noac, r1_response_read_bstst_postproc}},
	{CMD15, {no_response_preproc, no_response_postproc}},
	{CMD19, {short_response_block_write_preproc_noac, r1_response_write_bstst_postproc}},
	{CMD32, {short_response_preproc, r1_response_postproc}},
	{CMD33, {short_response_preproc, r1_response_postproc}},
	{CMD35, {short_response_preproc, r1_response_postproc}},
	{CMD36, {short_response_preproc, r1_response_postproc}},
	{CMD38, {short_response_preproc, r1b_response_postproc}},
	{ACMD51, {short_response_sd_data_app_specific_data, r1_response_read_data_postproc}},
	{CMD20, {short_response_stream_write_preproc, r1_response_write_data_postproc}},
	{ACMD6, {short_response_sd_app_specific_data, r1_response_postproc}},
#endif
};

/**
 * Finds the set of callbacks for the command index.
 * Performs a binary search on the statically defined the_callback_table.
 * @param[in] cmd_index The command index.
 * \return Returns the pointer to the callbacks if found. Returns NULL if not
 * found.
 * \todo This function has to be converted to a minimal perfect hash table search.
 * \callgraph
 */
static const callback_t *emmc_lookup_callback_table(u32 cmd_index)
{
	u32 num_commands = (sizeof(the_callback_table) / sizeof(callback_search_table)) -  1;
	u32 left, right;
	u32 present_index;

	left = 0;
	right = num_commands;

	while (left <= right) {
		present_index = left + (right - left) / 2;

		if (the_callback_table[present_index].cmd_index ==  cmd_index) {
			return &(the_callback_table[present_index].the_callbacks);
		} else if (cmd_index >  the_callback_table[present_index].cmd_index) {
			left = present_index + 1;
		} else {
			right = present_index - 1;
		}
	}

	return NULL;
}

/** Looks up the table for a post processing callback for the cmd.
 * This function looks up the has table of function pointers to locate the
 * appropriate postprocessing callback for the index.
 * @param[in] cmd_index	The command which is to be sent on the bus.
 * \return The function pointer to the post processing function.
 * \callgraph
 */
emmc_postproc_callback emmc_get_post_callback(u32 cmd_index)
{
	emmc_postproc_callback retval = NULL;
	const callback_t *the_callbacks;
	the_callbacks = emmc_lookup_callback_table(cmd_index);

	if (!the_callbacks) {
		return retval;
	}

	retval = the_callbacks->postproc;
	return retval;
}

/** Looks up the table for a pre processing callback for the cmd.
 * This function looks up the has table of function pointers to locate the
 * appropriate postprocessing callback for the index.
 * @param[in] cmd_index. The command which is to be sent on the bus.
 * \return The function pointer to the pre processing function.
 */
emmc_preproc_callback emmc_get_pre_callback(u32 cmd_index)
{
	emmc_preproc_callback retval = NULL;
	const callback_t *the_callbacks;
	the_callbacks = emmc_lookup_callback_table(cmd_index);

	if (!the_callbacks) {
		return retval;
	}

	retval = the_callbacks->preproc;
	return retval;
}

/**
 * This function sets up the transfer parameters.
 * the much of the data other than slot and data_buffer used only in case of slave mode of operation.
 * For IDMAC mode of operation they just maintained to make the flow same as that of slave mode.
 * The command specific paramters  are set in the current_task structure.
 * The parameters set here are
 *	- The epoch count indicating the number of bytes read (for read operation) before entering this function. For write operation,
*        it indicates the number of bytes already written in to the FIFO.
*     - num_of_blocks of data to be read or writen to FIFO.
*     - Function pointer to copy_func. This is just a place holder if a different function to be used for data transfer.
        If any valid pointer is passed here, it should not block as this function will be invoked from ISR top half.
*     - Function pointer to terminate function. This is just a place holder not used at present.
        If any valid pointer is passed here, it should not block as this function will be invoked from ISR top half.
*	- The slot number to which the command is intended to.
*	- data buffer for data transfer commands. This variable is used by ISR to invoke IDMAC functions or slave mode functions.
*     - Block size for the current data transfer command
* This fucntion updates the current_task structure by disabling the device interrupts.
* \return returns void.
* \callgraph
*/
void emmc_set_data_trans_params(u32 slot, u8 *data_buffer,
                                u32 num_of_blocks, u32 epoch_count, u32 flag,
                                u32 custom_blocksize)
{
	current_task.num_of_blocks = num_of_blocks;
	current_task.slot_num = slot;
	current_task.data_buffer = data_buffer;
	current_task.num_bytes_read = epoch_count;
	current_task.blksize = custom_blocksize;
	// emmc_set_bits(CTRL, INT_ENABLE);
}

void emmc_set_current_task_status_t(u32 slot, u32 *resp_buffer,
                                    u8 *data_buffer)
{
	current_task.resp_buffer = resp_buffer;
	current_task.data_buffer = data_buffer;
	current_task.slot_num = slot;
	/* If the error status is not updated, it would mean that the command has
	timed out and no response has been actually received from the card.
	*/
	current_task.error_status = 0;
	current_task.cmd_status = CMD_STAT_STARTED;
	current_task.bus_corruption_occured = 0;
}

