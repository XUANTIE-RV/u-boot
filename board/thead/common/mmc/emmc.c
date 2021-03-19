/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#include <linux/string.h>
#include "../mini_printf.h"
#include "emmc_interface.h"
#include "emmc_callback.h"


#ifndef DWMMC_MAX_FREQ
#define DWMMC_MAX_FREQ      3000000     // default: 3MHz
#endif

#define MMC_FOD_DIVIDER_VALUE   ((HSP_DEFAULT_FREQ % 400000)? \
                                 (HSP_DEFAULT_FREQ / 2 / 400000 + 1): \
                                 (HSP_DEFAULT_FREQ / 2 / 400000))  /* 400KHz */

#define ONE_BIT_BUS_FREQ    (HSP_DEFAULT_FREQ % DWMMC_MAX_FREQ? \
                            (HSP_DEFAULT_FREQ / 2 / DWMMC_MAX_FREQ + 1): \
                            (HSP_DEFAULT_FREQ / 2 / DWMMC_MAX_FREQ)) /* Switching to high-speed mode */


emmc_status_info_t emmc_status_info;
card_info_t card_info;
extern current_task_status_t current_task;

static void emmc_delay(uint32_t value)
{
	uint32_t counter;
	int i;
	volatile int j;

	/* some delay for emmc depends on bsp clock */
	value = (CPU_DEFAULT_FREQ + 59000000) / 60000000 * value;

	for (counter = 0; counter < (value * 5); counter++)
		for (i = 0; i < 100; i++)
			j = i;

	(void)j; // avoid compile warning
}


u32 emmc_send_clock_only_cmd(void)
{
	return emmc_execute_command(CLK_ONLY_CMD | CMD_WAIT_PRV_DAT_BIT, 0);
}

/**
 * Enables all clocks to all card slots in the controller.
 */
u32 emmc_enable_all_clocks(void)
{
	u32 clock_val;

	clock_val = (1 << 1) - 1;
	clock_val |= (1 << 16);
	emmc_set_register(CLKENA, clock_val);
	return emmc_send_clock_only_cmd();
}

/**
 * Disables all clocks to the controller.
 */
u32 emmc_disable_all_clocks(void)
{
	emmc_set_register(CLKENA, 0);
	return emmc_send_clock_only_cmd();
}

u32 emmc_enable_clocks_with_val(u32 val)
{
	emmc_set_register(CLKENA, val);
	return emmc_send_clock_only_cmd();
}

/**
 * Sets the divider for the clock in CIU.
 * This function sets a particular divider to the clock.
 * @param[in] divider The divider value.
 * \return 0 upon success. Error code upon failure.
 */
u32 emmc_set_clk_freq(u32 divider)
{
	u32 orig_clkena;
	u32 retval;

	if (divider > MAX_DIVIDER_VALUE) {
		mini_printf("invalid divider: %d, will set to 0xff\n", divider);
		divider = 0xff;
	}

	/* To make sure we dont disturb enable/disable settings of the cards*/
	orig_clkena = emmc_read_register(CLKENA);

	/* Disable all clocks before changing frequency the of card clocks */
	if ((retval = emmc_disable_all_clocks())) {
		return retval;
	}

	/* Program the clock divider in our case it is divider 0 */
	emmc_clear_bits(CLKDIV, MAX_DIVIDER_VALUE);
	emmc_set_bits(CLKDIV, divider);

	/*Send the command to CIU using emmc_send_clock_only_cmd and enable the clocks in CLKENA register */
	if ((retval = emmc_send_clock_only_cmd())) {
		emmc_enable_clocks_with_val(orig_clkena);
		return retval;
	}

	return emmc_enable_clocks_with_val(orig_clkena);
}


/*
This function is a blocking function. it reads the CMD register for CMD_MAX_RETRIES to
see if the CMD_DONE_BIT is set to 0 by CIU
*/
u32 emmc_poll_cmd_register(void)
{
	u32 num_of_retries = 0;

	while (1) {
		if (num_of_retries > CMD_MAX_RETRIES) {
			mini_printf("CMD register Polling failed");
			return ERRCMDRETRIESOVER;
		}

		if ((emmc_read_register(CMD) & CMD_DONE_BIT) == 0) {
			break;
		}

		num_of_retries++;
		emmc_delay(1);
	}

	return 0;
}

/**
 * This function does the final part of sending commands.
 * It sets the CMD register and CMD ARG register
 */
u32 emmc_execute_command(u32 cmd_register, u32 arg_register)
{
	emmc_set_register(CMDARG, arg_register);
	emmc_set_register(CMD, cmd_register);
	return (emmc_poll_cmd_register());
}

void emmc_send_raw_command(u32 slot, u32 cmd, u32 arg)
{
	u32 buff_cmd;
	buff_cmd = cmd | CMD_DONE_BIT;
	SET_CARD_NUM(buff_cmd, slot);
	mini_printf("send raw command:%d, arg:%d\n", buff_cmd, arg);
	emmc_execute_command(buff_cmd, arg);
	return;
}

/**
 * This function aborts any data transfer that might be happening. This
 * function can be called from interrupt context also since it uses
 * emmc_send_raw_command to dispatch the command on the bus.
 * @param[in] slot The slot to which the command is to be sent.
 */
void emmc_abort_trans_work(u32 slot)
{
	/* Send a raw command */
	emmc_send_raw_command(slot, CMD_USE_HOLD | CMD12 | CMD_RESP_EXP_BIT | CMD_ABRT_CMD_BIT, 0x00010000);
}

/**
 * Read in data from the FIFO in interrupt context.
 * This function reads in the bytes from the fifo . This
 * function is called for a RXDR interrupt or a transfer complete interrupt.
 * @param[in,out] the_task_status The status of the data transfer task.
 * @param[in] the_interrupt_status the flags for which particular interrupts
 */

u32 emmc_read_in_data(current_task_status_t *the_task_status, u32 the_interrupt_status)
{
	u32 fifo_level;
	u8 count = 0;
	u8 *u8_buffer = the_task_status->data_buffer + the_task_status->num_bytes_read;

	fifo_level = (GET_FIFO_COUNT(emmc_read_register(STATUS)));

	while (fifo_level > 0) {
		*((u32 *)(u8_buffer + (count << 2))) = emmc_read_register(FIFODAT);
		//mini_printf("read: %x %d %d\n", *((uint32_t *)(u8_buffer + (count << 2))), the_task_status->num_bytes_read, fifo_level);
		fifo_level--;
		count++;
	}

	the_task_status->num_bytes_read += (count << 2);

	if (the_task_status->blksize * the_task_status->num_of_blocks == the_task_status->num_bytes_read)

		if (the_interrupt_status & INTMSK_FRUN) {
			mini_printf("fifo overflow");
		}

	return 0;

}
/**
 * Function to write out data to the data FIFO.
 * @param[in,out] the_task_status The status of the current transfer in progress.
 * @param[in] the_interrupt_status The value of the interrupt flags.
 */

u32 emmc_write_out_data(current_task_status_t *the_task_status, u32 the_interrupt_status)
{
	u32 *buffer = (u32 *)(the_task_status->data_buffer + the_task_status->num_bytes_read);
	u32 len;
	u32 the_size = the_task_status->blksize * the_task_status->num_of_blocks - the_task_status->num_bytes_read;

	len = FIFO_WIDTH - (GET_FIFO_COUNT(emmc_read_register(STATUS)));

	while (len > 0) {
		if (!the_size) {
			break;
		}

		if (emmc_read_register(RINTSTS)& INTMSK_DAT_OVER) {
			return 0;
		}

		emmc_set_register(FIFODAT, *buffer);
		len--;
		buffer++;
		the_task_status->num_bytes_read = the_task_status->num_bytes_read + 4;
	}

	return 0;
}

/**
 * Checks a R1 response.
 *
 * @param[in] the_response  The response which is to be checked.
 *
 * \return The error status if an error is found in the response. Else 0.
 */
u32 emmc_check_r1_resp(u32 the_response)
{
	u32 retval = 0;

	if (the_response & R1CS_ERROR_OCCURED_MAP) {
#if (MMC_DEBUG == 1)

		if (the_response & R1CS_ADDRESS_OUT_OF_RANGE) {
			retval = ERRADDRESSRANGE;
		} else if (the_response & R1CS_ADDRESS_MISALIGN) {
			retval = ERRADDRESSMISALIGN;
		} else if (the_response & R1CS_BLOCK_LEN_ERR) {
			retval = ERRBLOCKLEN;
		} else if (the_response & R1CS_ERASE_SEQ_ERR) {
			retval = ERRERASESEQERR;
		} else if (the_response & R1CS_ERASE_PARAM) {
			retval = ERRERASEPARAM;
		} else if (the_response & R1CS_WP_VIOLATION) {
			retval = ERRPROT;
		} else if (the_response & R1CS_CARD_IS_LOCKED) {
			retval = ERRCARDLOCKED;
		} else if (the_response & R1CS_LCK_UNLCK_FAILED) {
			retval = ERRCARDLOCKED;
		} else if (the_response & R1CS_COM_CRC_ERROR) {
			retval = ERRCRC;
		} else if (the_response & R1CS_ILLEGAL_COMMAND) {
			retval = ERRILLEGALCOMMAND;
		} else if (the_response & R1CS_CARD_ECC_FAILED) {
			retval = ERRECCFAILED;
		} else if (the_response & R1CS_CC_ERROR) {
			retval = ERRCCERR;
		} else if (the_response & R1CS_ERROR) {
			retval = ERRUNKNOWN;
		} else if (the_response & R1CS_UNDERRUN) {
			retval = ERRUNDERRUN;
		} else if (the_response & R1CS_OVERRUN) {
			retval = ERROVERRUN;
		} else if (the_response & R1CS_CSD_OVERWRITE) {
			retval = ERRCSDOVERWRITE;
		} else if (the_response & R1CS_WP_ERASE_SKIP) {
			retval = ERRPROT;
		} else if (the_response & R1CS_ERASE_RESET) {
			retval = ERRERASERESET;
		} else if (the_response & R1CS_SWITCH_ERROR) {
			retval = ERRFSMSTATE;
		}

#endif
	}

	if (retval) {
		mini_printf("ERROR\n");
	}

	return retval;
}

/**
 * Resets the data FIFO for the host controller.
 */
void emmc_reset_fifo(void)
{
	emmc_set_bits(CTRL, FIFO_RESET);

	while (emmc_read_register(CTRL) & FIFO_RESET);

	return;
}

u32 emmc_wait_cmd(emmc_postproc_callback the_callback)
{
	u32 status, int_status;

	/* Read the masked interrupt status to see what interrupt has occured */
	int_status = emmc_read_register(RINTSTS);
	status = emmc_read_register(STATUS);

	while (1) {
		int_status = emmc_read_register(RINTSTS);

		if (int_status & INTMSK_CMD_DONE)
			break;
	}

	if (int_status & INTMSK_RTO) {
		emmc_set_register(RINTSTS, 0xFFFFFFFF);
		return ERRCMDRETRIESOVER;
	}

	//mini_printf("int_status:%x, status:%x\n", int_status, status);
	(void)status; // avoid compile warning

	the_callback(&current_task, &int_status);
	emmc_set_register(RINTSTS, 0xFFFFFFFF);
	emmc_reset_fifo();
	return 0;
}

/**
 * This sends the command after taking the lock. The lock is released from ISR.
 * This forms the CMD and CMDARG and calls emmc_execute_command which sends
 * the command and polls for CMD_DONE_BIT to see if it is 0
 * For IDMAC mode of operation, setup the IDMAC related registers, and
 * setup the flag idma_mode_on to indicate the ISR that the present transfer
 * uses IDMAC flow rather than Slave mode flow.
 */
u32 emmc_cmd_to_host(u32 slot, u32 cmd_register, u32 arg_register, u32 *resp_buffer,
                     u8 *data_buffer, emmc_postproc_callback the_callback, u32 flags)
{
	/* update the task status with call back, response buffer,
	data buffer,error_status,cmd_status,bus_corruption_occured,...
	*/
	emmc_set_current_task_status_t(slot, resp_buffer, data_buffer);

	/*Set the CMD_DONE_BIT to initiate the command sending, CIU clears this bit */
	SET_BITS(cmd_register, CMD_DONE_BIT);

	/* Execute the command and wait for the scommand to execute */
	//printf("arg_register = %x cmd_register = %x\n",arg_register,cmd_register);
	emmc_set_register(CMDARG, arg_register);
	emmc_set_register(CMD, cmd_register);
	return emmc_wait_cmd(the_callback);
}

/*
This function is used to form the command with the parameters sent to this
- programs the clk divider for the host ip
- handles the sending task to the function emmc_cmd_to_host
@param[in] card_num
@param[in] cmd_index
@param[in] cmd_arg
@param[in] resp_buffer
@param[in] data_buffer
@param[in] flags
*/
u32 emmc_send_serial_command(u32 card_num, u32 cmd_index, u32 cmd_arg,
                             u32 *resp_buffer, u8 *data_buffer, u32 flags)
{
	u32 cmd_register = 0;
	emmc_postproc_callback post_callback = NULL;
	emmc_preproc_callback preproc_fn = NULL;
	u32 arg_register = cmd_arg;

	/* whether post_callback is from table or custom_callback? */

	post_callback = emmc_get_post_callback(cmd_index);

	if (!post_callback) {
		mini_printf("CMD Command not supported CMD = %d\n", cmd_index);
		return ERRNOTSUPPORTED;
	}

	/* whether preproc_callback is from table or custom_preproc? */
	preproc_fn = emmc_get_pre_callback(cmd_index);


	/*execute the preproc_fn to prepare the command: this function populates
	the cmd_register and arg_register*/
	preproc_fn(card_num, cmd_index, &cmd_register, &arg_register);

	/*execute command: this function populates the cmd_register and arg_register*/

	return emmc_cmd_to_host(card_num, cmd_register, arg_register, resp_buffer, data_buffer, post_callback, flags);
}

/**
 * Determines the state in which the card is in.
 * It sends a CMD13 to the card and gets the status out of the R1 response.
 * @param[in] slot The slot for which the status is to be found,
 * @param[out] status The status of the card is populated in this argument.
 * \return Returns 0 on success. Error status upon error.
 */
u32 emmc_get_status_of_card(u32 slot, card_state_e *status)
{
	u32 resp_buffer, retval = 0;

	/* Pick up the status from a R1 response */
	if ((retval = emmc_send_serial_command(slot, CMD13, 0x00010000, &resp_buffer, NULL, 0))) {
		return retval;
	}

	/* We have R1 in the resp buffer , we now check the
	status of the card. If it is not in a a standby
	state we exit.
	*/
	*status = (card_state_e) R1CS_CURRENT_STATE(resp_buffer);
	return 0;
}

/**
 * Puts the card in the trans state.
 * Puts the specifed to card into the trans state. It first ascertains using
 * CMD13 as to whether the card is in standby. Once ascertianed, the card is
 * send CMD7* to put into trans state.
 * @param[in] slot The specified card
 * \return Returns 0 upon success. Error status upon error.
 */
u32 emmc_put_in_trans_state(u32 slot)
{
	u32 retval = 0, resp_buffer;
	card_state_e the_state;

#if !defined(CONFIG_MMC_BROKEN_CD)
	if (emmc_read_register(CDETECT) & (1 << slot)) {
		return ERRCARDNOTCONN;
	}
#endif

	if ((retval = emmc_get_status_of_card(slot, &the_state))) {
		mini_printf("card borked out\n");
		return retval;
	}

	/* If the card already is
	in the trans state, our work
	here is done
	*/
	if (CARD_STATE_TRAN == the_state) {
		card_info.card_state = CARD_STATE_TRAN;
		return 0;
	}

	if (CARD_STATE_STBY != the_state) {
		mini_printf("card error out\n");
		return ERRFSMSTATE;
	}

	card_info.card_state = CARD_STATE_STBY;

	/* Now send the command to send to standby state */
	if ((retval =
	         emmc_send_serial_command(slot, CMD7, 0x00010000,
	                                  &resp_buffer, NULL, 0))) {
		return retval;
	}

	/* This puts the card into trans */
	card_info.card_state = CARD_STATE_TRAN;

	return retval;
}

/**
 * The function tries to set extcsd bits BOOT_PARTITION_ENABLE to boot from the user area,
 *  boot 1 area or boot 2 area
 * @param[in] slot The specified card
 * @param[in] area emmc boot area
 * return Returns 0 upon success. Error status upon error
 */
u32 emmc_select_area(u32 slot, emmc_area_e area)
{
	u32 arg_value = 0;
	u32 resp_buff;
	u32 retval = 0;

	switch (area) {
	case EMMC_BOOT_PARTITION_1:
		arg_value = ((BOOT_PARTITION_ACCESS_1_RW) << 8) | ARG_BOOTCONFIG_ACCESS_WRITE | ARG_BOOTCONFIG_INDEX ;
		break;

	case EMMC_BOOT_PARTITION_2:
		arg_value = ((BOOT_PARTITION_ACCESS_2_RW) << 8) | ARG_BOOTCONFIG_ACCESS_WRITE | ARG_BOOTCONFIG_INDEX ;
		break;

	case EMMC_BOOT_PARTITION_USER:
		arg_value = ARG_BOOTCONFIG_ACCESS_WRITE | ARG_BOOTCONFIG_INDEX ;
		break;

	default:
		return ERROVERRUN;
	}

	if ((retval = emmc_put_in_trans_state(slot))) {
		return retval;
	}

	if ((retval = emmc_send_serial_command(slot, CMD6, arg_value, &resp_buff, NULL, 0))) {
		return retval;
	}

	// emmc_send_serial_command(slot, UNADD_CMD7, 0x00010000, &resp_buff, NULL, 0);

	return 0;
}

/**
 * Sets the voltage for the connected MMC card.
 * This function is modified inorder to incorporate the MMC high capacity card's voltage
 * setting also. OCR_ACCESSMODE_SECTOR field is set in the argument of CMD1 indicating
 * host supports the sector addressing. But the card's addressing capability is validated
 * only after reading the SEC_COUNT field of EXT_CSD register.
 * This will flag an error if the card is
 * not in the IDLE state It does the following --
 *  -# Check if any card is present in the slot
*   -# Check if the card is in the idle state
*   -# Set (CMD1) the OCR to the MMC range.
*   -# If there is a failure in the voltage setting, mark the card as
*   inactive.
* @param[in] slot The slot in which the card is
* \return Returns 0 upon succes and error status upon failure
*/
u32 emmc_set_mmc_voltage_range(u32 slot)
{
	u32 retval = 0, resp_buffer, new_ocr = 0;
	int count = CMD1_RETRY_COUNT;

	/* Check if it is in the correct state */
	if (card_info.card_state != CARD_STATE_IDLE) {
		return ERRFSMSTATE;
	}

	/*the new_ocr is modified to incorporate access mode to indicate sector addressing capability of the host
	The modification to this function should not create any problem to MMC4.1 and earlier cards as OCR_ACCESSMODE_SECTOR
	bit fields are reserverd and card should not care for the value here    */

	new_ocr = OCR_27TO36 | OCR_POWER_UP_STATUS | OCR_ACCESSMODE_SECTOR;
	//new_ocr =  0x40200000;

	count = CMD1_RETRY_COUNT;

	while (count) {
		resp_buffer = 0;
		retval = emmc_send_serial_command(slot, CMD1, new_ocr, &resp_buffer, NULL, 0);

		if ((resp_buffer & OCR_POWER_UP_STATUS) && (!retval)) {
			/* The power up process for the card is over */
			break;
		}

		--count;
		emmc_delay(10);
	}

	if (0 == count) {
		mini_printf("Giving up set voltage after %d\n", CMD1_RETRY_COUNT);
		card_info.card_state = CARD_STATE_INA;
		return ERRHARDWARE;
	} else {
		mini_printf("new_ocr:%x, resp_buffer:%x\n", new_ocr, resp_buffer);
		if ((new_ocr & OCR_27TO36) != OCR_27TO36) {
			card_info.card_state = CARD_STATE_INA;
			return ERRHARDWARE;
		}
	}

	card_info.card_state = CARD_STATE_READY;
	return retval;
}

/**
 * Reads the CID and puts the card to STBY state.
 * This function puts the card into standby state. It does so by reading
 * out the CID using CMD2. It stores the CID for later use and also prints a
 * small informational message on the card details.
 * @param slot The slot number for the card
 * \return Returns 0 on success and error code upon return.
 */
u32 emmc_get_cid(u32 slot)
{
	u32 buffer_reg, retval = 0;
	int count;
	//char product_name[7];
	//int product_revision[2];
	//int month, year;
#if !defined(CONFIG_MMC_BROKEN_CD)
	/* Check if the card is connected */
	buffer_reg = emmc_read_register(CDETECT);

	if (buffer_reg & (1 << slot)) {
		return ERRCARDNOTCONN;
	}
#endif
	(void)buffer_reg; // avoid compile warning

	if (CARD_STATE_READY != card_info.card_state) {
		return ERRFSMSTATE;
	}

	count = CMD2_RETRY_COUNT;

	while (count) {
		retval =
		    emmc_send_serial_command(slot, CMD2, 0, (card_info.the_cid), NULL, 0);

		if (!retval) {
			break;
		}

		count--;
		emmc_delay(100);
	}

	if (0 == count) {
		mini_printf("FAILED TO GET CID OF THE CARD !!\n");
		return ERRHARDWARE;
	} else {
		mini_printf("cid: %x,%x,%x,%x\n",
		            card_info.the_cid[0],
		            card_info.the_cid[1],
		            card_info.the_cid[2],
		            card_info.the_cid[3]);
	}

	/* Print out some  informational message about the card
	always makes for good eye candy
	*/
#if 0

	for (count = 5; count > -1; count--) {
		product_name[5 - count] =
		    card_info.the_cid_bytes[count + 7];
	}

	//product_name[count] = 0;
	product_revision[0] = card_info.the_cid_bytes[6] & 0x0f;
	product_revision[1] = (card_info.the_cid_bytes[6] & 0xf0) >> 4;
	month = (card_info.the_cid_bytes[1] & 0xf0) >> 4;
	year = (card_info.the_cid_bytes[1] & 0x0f) + 1997;
	//PDEBUG("Found Card %s Rev %d.%d (%d/%d)\n",product_name, product_revision[1], product_revision[0], month, year);
#endif
	card_info.card_state = CARD_STATE_IDENT;
	return 0;
}

/**
 * Sets the RCA for the card.
 * This functions sets the RCA for a card at the speficied slot. The flow
 * is as follows
 *  -# Check if the card is inserted.
*   -# Check if the card is in the indentification state.
*   -# Make the rca of the device = slot + 1.
*   -# Mark the card into standby state.
* @param[in] slot The specified slot
* \return 0 upon success. The error status upon error.
*/
u32 emmc_set_rca(u32 slot)
{
	u32 buffer_reg, resp_buffer, retval = 0;
	u32 the_rca;

#if !defined(CONFIG_MMC_BROKEN_CD)
	/* Check if the card is connected */
	buffer_reg = emmc_read_register(CDETECT);

	if (buffer_reg & (1 << slot)) {
		return ERRCARDNOTCONN;
	}
#endif
	(void)buffer_reg; // avoid compile warning

	// PDEBUG("Setting the rca for card %x to %x\n", slot, slot + 1);
	the_rca = (slot + 1) << 16;

	if ((retval =
	         emmc_send_serial_command(slot, CMD3, the_rca, &resp_buffer, NULL, 0))) {
		return retval;
	}

	card_info.card_state = CARD_STATE_STBY;
	card_info.the_rca = slot + 1;
	return 0;
}

/**
 * This function reads and stores the CSD for MMC High capacity cards.
 * This function is used by the initialization routines to evaluate the CSD. The CSD is read
 * and the maximum NSAC and TAAC values are stored away in the card_info data structure
 * for future use to calculate maximum read/write latency. Card capacity is not calculated from
 * CSD register as this information coded in sector count (SEC_COUNT) of EXT_CSD register.
 * @param[in] slot The specified slot.
 * \return 0 upon success. The error status upon error.
 */
u32 emmc_process_MMC_csd(u32 slot)
{
	u32 buffer_reg, retval;
	u32 read_block_size, write_block_size;
	u64 card_size;
	u32 blocknr, blocklen;

#if !defined(CONFIG_MMC_BROKEN_CD)
	buffer_reg = emmc_read_register(CDETECT);

	if (buffer_reg & (1 << slot)) {
		return ERRCARDNOTCONN;
	}
#endif
	(void)buffer_reg; // avoid compile warning

	if ((retval = emmc_send_serial_command(slot, CMD9, 0x00010000, card_info.the_csd, NULL, 0))) {
		return retval;
	}

	/* The CSD is in the bag */

	read_block_size  = 1 << (CSD_READ_BL_LEN((card_info.the_csd)));
	write_block_size = 1 << (CSD_WRT_BL_LEN((card_info.the_csd)));

	/* See section 5.3 of the 4.1 revision of the MMC specs for
	an explanation for the calculation of these values
	*/
	blocknr = (CSD_C_SIZE(card_info.the_csd) + 1) * (1 << (CSD_C_SIZE_MULT(card_info.the_csd) + 2));
	blocklen = read_block_size;
	card_size = (uint64_t)blocknr * (uint64_t)blocklen;
	mini_printf("write_block_size =%d, read_block_size=%d\n", write_block_size, read_block_size);

	/*MMC4.2 High Capacity cards should support 512B read access and write access mandatarily*/

	if (write_block_size < 512) {
		return ERRHARDWARE;
	}

	if (read_block_size < 512) {
		return ERRHARDWARE;
	}

	/* read/write block size */
	card_info.card_write_blksize = (write_block_size > 512) ? 512 : write_block_size;
	card_info.orig_card_read_blksize = read_block_size;
	card_info.card_read_blksize = (read_block_size > 512) ? 512 : read_block_size;
	card_info.orig_card_write_blksize = write_block_size;
	/*
	    card_info.card_size is updated with a dummy value.
	    This is just a hack since emmc_read_write_bytes return an error when used with
	    card_size a non zero value. Note that for MMC4.2 cards the card size information
	    is available in card's ext_csd register
	*/
	card_info.card_size = card_size;
	return 0;
}

/**
 * This function checks whether the internal buffer for the card is ready
 * to receive any data.
 * @param[in] slot The slot for the card
 */
u32 emmc_is_card_ready_for_data(u32 slot)
{
	u32 resp_buffer, retval = 0;
	int count;

	for (count = 0; count < READY_FOR_DATA_RETRIES; count++) {
		if ((retval =
		         emmc_send_serial_command(slot, CMD13, 0x00010000,
		                                  &resp_buffer, NULL, 0))) {
			return retval;
		}

		// PDEBUG("%s: Got response as 0x%x\n", __FUNCTION__, resp_buffer);
		if (resp_buffer & R1CS_READY_FOR_DATA) {
			break;
		}

		emmc_delay(1);
	}

	if (READY_FOR_DATA_RETRIES == count) {
		return ERRCARDNOTREADY;
	}

	return 0;
}

/**
 * Reads or writes a predetermined number of bytes at a particular address.
 * Flow:
 *  -# Check if the start and end addresses are block aligned.
*   -# Put the card into trans state.
*   -# Set the length of the block using CMD16.
*   -# Reset the FIFO and interrupts.
*   -# Setup the task status for the data transfer.
*   -# Populate the BYTCNT and BLKSIZ registers with the appropriate values.
*   -# For Idmac operation setupt the descriptor by obtaining the physical address
*   -# Fire the CMD18/CMD25 with the start address. This command will wait for
*      the data transfer to complete. The data transfer will occur within interrupt
*      context. See emmc_read_in_data and emmc_write_out_data for details.
*   -# Ensure that the card is back in trans state.
*   -# Execute an unaddressed CMD7 to put the card into standby again.
* data transfer method.
*
* @param[in] slot The slot in which the card is inserted.
* @param[out] resp_buffer The buffer in which the latest response is stored
* @param[out] data_buffer The buffer in which the read data will be put.
* It is the responsiblity of the calling function to ensure that the length
* of the buffer is long enough for the size of data requested.
* @param[in] start Starting address.
* @param[in] end Ending address. If this address is equal to
*  the the start
* @param[in] argreg If a non zero value is specified, the value of the argument
*  of the command is the specified value.
*  address, the transfer will be open ended.
* @param[in] the_copy_func The memcpy function. This allows the caller to
* specify the memory copy function. If this is set to NULL, memcpy is
* used.
* @param[in] the_term_function This is the function which checks whether
* CMD12 is to be sent.
* @param[in] read_or_write 1 denotes a write operation, a 0 denotes a
* read operation
* @param[in] custom_command These 32 bits are used to flag any special conditions
* which might be needed to be conveyed to the read/write command. The structure
* of these 32 bits are as follows.
*	- Bits 0-15 encapsulates an alternative read/write command index. The user
*	might want a different command to be floated on the command line instead of
*	the normal read and write command lines
*	- Bits 16-19 encapsulates the ln2 of the block size which is different from
*	the block size of the card which may be used.
*	- Bits 30-31 are used as flags for executing certain commands. See
*	 #CUSTCOM_DONTSTDBY, #CUSTCOM_DONT_CMD16, #CUSTCOM_DONT_TRANS for
*	 details
*
* \return Returns 0 on successful read and error status
    upon error.
*/
u32 emmc_read_write_bytes(u32 slot, u32 *resp_buffer,
                          u8 *data_buffer, u32 start, u32 end,
                          u32 argreg, u32 read_or_write,
                          u32 custom_command)
{
	u32 retval = 0;
	u32 num_of_blocks, command_to_send, num_of_primary_dwords = 0, the_block_size;
	u32 arg_to_send;

	//mini_printf("custom_command:%d\n", custom_command);

#if !defined(CONFIG_MMC_BROKEN_CD)
	/* Check if the card is inserted */
	if (emmc_read_register(CDETECT) & (1 << slot)) {
		return ERRCARDNOTCONN;
	}
#endif

	/* Set the block size pertinent to the type of operation
	*/
	if (read_or_write) {
		the_block_size = card_info.card_write_blksize;
	} else {
		the_block_size = card_info.card_read_blksize;
	}

	if (start > end) {
		return ERRADDRESSRANGE;
	}

	//mini_printf("read/write: %x,%x,%x,%x\n", start, end, card_info.card_size, the_block_size);

#if CONDIF_SUPPORT_MULTI_BLOCK

	num_of_blocks = (end - start) / the_block_size;

	if ((num_of_blocks & 0x0000ffff) != num_of_blocks) {
		return ERRADDRESSRANGE;
	}
#else
	num_of_blocks = 1;
#endif
	/* One cannot have an open ended transfer with
	no term function specified.
	*/

	if (!(CUSTCOM_DONT_TRANS & custom_command)) {
		/* Check if the card is in standby state and put in trans
		state.
		*/
		if ((retval = emmc_put_in_trans_state(slot))) {
			mini_printf("TRANS STATE FAILED\n");
			goto HOUSEKEEP;
		}
	}

	/* Set the block len */
	if ((!(custom_command & CUSTCOM_DONT_CMD16))
	    && (!(custom_command & CUSTCOM_STREAM_RW))) {
		if ((retval =
		         emmc_send_serial_command(slot, CMD16,
		                                  the_block_size, resp_buffer, NULL, 0))) {
			goto HOUSEKEEP;
		}
	}

	/* Reset internal FIFO */
	emmc_reset_fifo();

	/* Clear the interrupts */
	emmc_set_bits(RINTSTS, 0xfffe);


	if (!(custom_command & CUSTCOM_DONT_BLKSIZ)) {
		emmc_set_register(BLKSIZ, the_block_size);
	}

	emmc_set_register(BYTCNT, (end - start));

	/* Depending on a read or a write, CMD18/CMD25 will be sent out.
	* Also, the FIFO will be filled to its brim for the card to
	* read as soon as it receives the CMD25
	*/
#if CONDIF_SUPPORT_MULTI_BLOCK

	if (read_or_write) {
		command_to_send = CMD25;

		/*Some of MMC and SD cards won't allow Single block write using CMD25. So use CMD24 when number of blocks is equal to 1*/
		if (num_of_blocks == 0x01) {
			command_to_send = CMD24;
		}
	} else {
		command_to_send = CMD18;

		/*Some of MMC and SD cards won't allow Single block read using CMD18. So use CMD17 when number of blocks is equal to 1*/
		if (num_of_blocks == 0x01) {
			command_to_send = CMD17;
		}
	}

#else

	if (read_or_write) {
		command_to_send = CMD24;
	} else {
		command_to_send = CMD17;

	}

#endif

	if (custom_command & CUSTCOM_COMMAND_MSK) {
		command_to_send = custom_command & CUSTCOM_COMMAND_MSK;
	}


	/* The CMD23 is an optional feature which might be used for
	block sized data transfers. The auto stop bit and the
	BYTCNT size register however suffices for the host controller.
	*/
	if (custom_command & CUSTCOM_DO_CMD23) {
		/* Do a command 23 to set the num of blocks */
		if ((retval =
		         emmc_send_serial_command(slot, CMD23, (end - start) / the_block_size,
		                                  resp_buffer, NULL, 0))) {
			goto HOUSEKEEP;
		}
	}

	// PDEBUG("num_of_primary_dwords = %d\n", num_of_primary_dwords);
	/* Set the transfer parameters */
	emmc_set_data_trans_params(slot, data_buffer,
	                           num_of_blocks, num_of_primary_dwords *
	                           0, read_or_write,
	                           the_block_size);

	/* Before we kick off the floating of the data on the data lines
	if it is a write function we check whether the card is ready
	to receive data
	*/
	if (read_or_write) {
		if ((retval = emmc_is_card_ready_for_data(slot))) {
			mini_printf("card  not ready, %d\n", retval);
			goto HOUSEKEEP;
		}
	}

	/* Now kick off the the data read/write command. This command will
	* will schedule this context back in again after the complete
	* transfer.
	*/
	arg_to_send = start / 512 ;

	if ((retval =
	         emmc_send_serial_command(slot, command_to_send,
	                                  arg_to_send, resp_buffer, data_buffer, end - start))) {
		goto HOUSEKEEP;
	}

	/* Wait till the busy bit in the status register gets
	de-asserted for data writes
	*/

	if (read_or_write) {
		while ((emmc_read_register(STATUS)) &  STATUS_DAT_BUSY_BIT);
	}

	/*This is required when we disable the auto_stop_bit in CMD register and send the CMD12 from the driver directly.
	Some of the MMC cards and SD card misbehave (block skip problem) when auto_stop_bit is set in the CMD register.
	So Auto_stop_bit is disabled and the CMD12 (Stop CMD is sent by Host/driver) as a work around for multiblock
	read and write operation.
	*/
#if CONDIF_SUPPORT_MULTI_BLOCK

	if ((command_to_send == CMD25) || (command_to_send == CMD18)) {
		retval = emmc_send_serial_command(slot, CMD12, 0x00010000, resp_buffer, NULL, 0);
	}

#endif

HOUSEKEEP:
	card_info.card_state = CARD_STATE_TRAN;
	return retval;
}
#ifdef CONDIF_SUPPORT_EMMC_EXTCSD

/**
 * Read in the Ext CSD.
 * Reads in the Ext CSD for the card and stores it in the card_info structure.
 * @param[in] slot The slot for the card for which the EXT CSD is to be read
 */
u32 emmc_process_extcsd(u32 slot)
{
	u32 i = 0;
	u8 *the_extcsd_buffer = card_info.the_extcsd_bytes;
	u32 resp[4], retval;
	u64 capacity;


#if !defined(CONFIG_MMC_BROKEN_CD)
	if (emmc_read_register(CDETECT) & (1 << slot)) {
		return ERRCARDNOTCONN;
	}
#endif

	/* (10 - 1) << 16 is 512 , the size of the extcsd register */
	retval = emmc_read_write_bytes(slot, resp, the_extcsd_buffer, 0, 512, 0, 0,
	                               CMD8 | 10 << CUSTOM_BLKSIZE_SHIFT | CUSTCOM_DONT_CMD16);

	if (retval != 0)
		return retval;

	for (i = 0; i < 512 / 4 ; i += 4)
		mini_printf("Ext_csd[%d . . .] = %x  %x  %x  %x\n", i * 4,
		            the_extcsd_buffer[i + 0], the_extcsd_buffer[i + 1],
		            the_extcsd_buffer[i + 2], the_extcsd_buffer[i + 3]);

	card_info.card_boot_size = ECSD_BOOT_SIZE_MULTI(the_extcsd_buffer) << 17;
	card_info.card_rpmb_size = ECSD_RPMB_SIZE_MULTI(the_extcsd_buffer) << 17;

	capacity = ((uint32_t)(the_extcsd_buffer[212])) |
	           ((uint32_t)(the_extcsd_buffer[213]) << 8) |
	           ((uint32_t)(the_extcsd_buffer[214]) << 16) |
	           ((uint32_t)(the_extcsd_buffer[215]) << 24);
	capacity *= card_info.card_read_blksize;
	card_info.card_size = capacity;

	return retval;
}
#endif
/**
 * Reset the mmc card to ready state.
 * The MMC card reset is different from MMC4.1 and earlier cards
 * in that sence, the cards support sector addressing and this needs to properly handled.
 * This function resets the card and then brings it up back to the ready state.
 * For the exact process in which this is to be done, refer to Figure 17 of the
 * MMC card standards version 4.2 The steps followed in this function are as
 * follows
 *	-# Check if the card is present.
*	-# Send a CMD0 to reset the card.
*	-# Set the clock frequency to f<sub>OD</sub> (300-400KHz).
*	-# Enable open drain pullup on the command line.
*	-# Setup the OCR register for the card. This process is different from the older
*        way of setting the operating voltage range. Refer emmc_set_mmc_voltage_range
*        for details. This moves the card to ready.
*	-# Poll for the CID  for the system. This moves the card to ident
*	-# Assign a rca if the CID is retrieved. This pushes the card to stby.
*	-# Read in the Ext CSD.
*	-# Compute the card capacity from EXT_CSD register.
*	-# Check whether the card can be set to a wide bus mode and sets it so.
* @param[in] slot The slot in which the card is placed.
* \return Returns 0 upon success and the error status upon failure.
* \callgraph
*/
u32 emmc_reset_mmc_card(u32 slot)
{
	u32 buffer_reg;
	u32 version;
	u32 retval = 0;

#if !defined(CONFIG_MMC_BROKEN_CD)
	/* Check if the card is connected */
	buffer_reg = emmc_read_register(CDETECT);

	if (buffer_reg & (1 << slot)) {
		return ERRCARDNOTCONN;
	}
#endif

	/* Fod the clock and OD the bus since we start enumerating now */
	emmc_set_bits(CTRL, ENABLE_OD_PULLUP);
	card_info.divider_val = MMC_FOD_DIVIDER_VALUE;

	emmc_set_clk_freq(card_info.divider_val);

	/* Reset the card. Since we really dont know as to from where the call has been made */
	if ((retval = emmc_send_serial_command(slot, CMD0, 0, NULL, NULL, 0))) {
		goto HOUSEKEEP;
	}

	/*After reset we know that card state is IDEL */
	card_info.card_state = CARD_STATE_IDLE;
	emmc_delay(10);

	/*Card state is IDLE. Set the mmc voltage range on the card */
	if ((retval = emmc_set_mmc_voltage_range(slot))) {
		goto HOUSEKEEP;
	}

	/*Card is in READY stae. Can now extract the CID */
	if ((retval = emmc_get_cid(slot))) {
		goto HOUSEKEEP;
	}

	/*Card is in IDENT state. Now finally set the RCA */
	if ((retval = emmc_set_rca(slot))) {
		goto HOUSEKEEP;
	}

	/* set the divider value for data transfer. Now pull down the OD_PULLUP */
	card_info.divider_val = ONE_BIT_BUS_FREQ;
	emmc_clear_bits(CTRL, ENABLE_OD_PULLUP);
	emmc_delay(10);

	emmc_set_clk_freq(card_info.divider_val);
	/*Card is in CARD_STATE_STBY. So we get the CSD Register and store it */
	if ((retval = emmc_process_MMC_csd(slot))) {
		/* Switch off the card */
		goto HOUSEKEEP;
	}

	// After CMD9 the card is in STANDBY state. Before sending ACMD6, Card should be brought to trans state
	// by sending CMD7

	// PDEBUG("Sending CMD7 to bring the card to TRANS state\n");
	if ((retval =  emmc_send_serial_command(slot, CMD7, 0x00010000, &buffer_reg, NULL, 0))) {
		return retval;
	}

	card_info.card_state = CARD_STATE_TRAN;

	version  = (card_info.the_csd[3] >> 26) & 0xf;
	mini_printf("mmc version:%d\n", version);
	switch (version) {
	case 0:
		card_info.version = MMC_VERSION_1_2;
		break;
	case 1:
		card_info.version = MMC_VERSION_1_4;
		break;
	case 2:
		card_info.version = MMC_VERSION_2_2;
		break;
	case 3:
		card_info.version = MMC_VERSION_3;
		break;
	case 4:
		card_info.version = MMC_VERSION_4;
		break;
	default:
		card_info.version = MMC_VERSION_1_2;
		break;
	}

	if (card_info.version >= MMC_VERSION_4) {
#ifdef CONDIF_SUPPORT_EMMC_EXTCSD
		if ((retval = emmc_process_extcsd(slot))) {
			return 1;
		}
#endif
	} else {
		card_info.card_boot_size = 0;
		card_info.card_rpmb_size = 0;
	}

HOUSEKEEP:
	//PDEBUG("Returning %x\n", retval);
	return retval;
}

/**
 * This function reads a single block on the device for any memory/storage
 * device that might be attached to the host controller. This is the interface
 * any block device driver might use for accessing the devices on the other
 * side of the host controller.
 * @param[in] slot The slot number for the device.
 * @param[in] start_sect The sector to read in.
 * @param[in,out] buffer The data buffer to transfer.
 * @param[in] read_or_write 1 = write operation, 0 = read operation.
 * @param[in] sect_size
 * \return Returns 0 upon success or error code upon error.
 * \note The size of the buffer should be enough for the requested transfer.
 *  The caller of the function is responsible for the size.
 */

u32 emmc_read_write_block(u32 slot, u32 start_sect, u8 *buffer,
                          u32 num_of_sects, u32 read_or_write, u32 sect_size)
{
	u32 retval = 0;
	u32 resp_buff[4];

	retval = emmc_read_write_bytes(slot, resp_buff, buffer,
	                               start_sect * sect_size,
	                               start_sect * sect_size + sect_size * num_of_sects, 0,
	                               read_or_write, 0);
	return retval;
}
#ifdef EMMC_SUPPORT_GET_CARD

/**
 * Determine the card type in the slot.
 * This function determines the card type in the slot. The steps for doing so
 * are as follows:
 *	-# Send ACMD41 + CMD55 combo to the slot. If a response is received,
*	it is a SD card.
*	-# If not, it is a MMC card.
*
*	@param[in] slot The index of the slot in which the card is in.
*	return Returns the card type found.
*/
card_type_e emmc_get_card_type(u32 slot)
{
	u32 buffer_reg, retval;

#if !defined(CONFIG_MMC_BROKEN_CD)
	/*Read the CDETECT bit 0 => card connected. Note this is not true for CEATA so you find a hack in the emmc_read_register() */
	buffer_reg = emmc_read_register(CDETECT);

	if ((buffer_reg & (1 << slot))) {
		return NONE_TYPE;
	}
#endif

	/*
	    Clear the CTYPE register bit for of IP. This bit indicates whether the card connected is 8/4/1 bit card
	*/
	emmc_clear_bits(CTYPE, (1 << slot));

	/* Lets Issue ACMD41 to see whether it is SDMEM. CMD55 should preced and ACMD command
	If nonzero response to CMD55 => the card is not an SD type so move to detect whether it is MMC?
	*/

	if ((retval = emmc_send_serial_command(slot, CMD55, 0, NULL, NULL, 0))) {
		goto CONT_MMC;
	}

	/*
	    CMD55 is successful, so send ACMD41 to get the OCR from SD card. If success Declare a SD card
	*/
	//PDEBUG("Sending ACMD41 to slot %x\n", slot);
	retval =  emmc_send_serial_command(slot, ACMD41, 0, NULL, NULL, 0);

	if (!retval) {
		/* Found an SD card */
		return SD_TYPE;
	}

	if (retval != ERRRESPTIMEOUT) {
		return ERRTYPE;
	} else {
		mini_printf("ACMD41 has timed out\n");
	}

	/* Not an SD .. May be MMC type? */

CONT_MMC:
	/*
	This is required only for Bus topology. But For the sake of commonness do it.
	*/
	emmc_set_bits(CTRL, ENABLE_OD_PULLUP);
	card_info.divider_val = MMC_FOD_DIVIDER_VALUE;

	if ((retval = emmc_send_serial_command(slot, CMD0, 0, NULL, NULL, 0))) {
		return ERRTYPE;
	}

	return MMC_TYPE;
}
#endif

u32 emmc_set_bus_width(u32 slot)
{
	u32 arg_value = 0;
	u32 resp_buff;
	u32 retval = 0;

	arg_value = 2 << 8   | ARG_BUSWIDTH_ACCESS_WRITE | ARG_BUSWIDTH_INDEX ;

	if ((retval = emmc_put_in_trans_state(slot))) {
		return retval;
	}

	if ((retval = emmc_send_serial_command(slot, CMD6, arg_value, &resp_buff, NULL, 0))) {
		return retval;
	}

	emmc_set_bits(CTYPE, 1 << (slot + 16));

	emmc_send_serial_command(slot, UNADD_CMD7, 0x00010000, &resp_buff, NULL, 0);

	return 0;
}

u32 emmc_host_init(card_info_t *emmc_card_info)
{
	u32 buffer_reg = 0; /* multipurpose buffer register */
	u32 num_of_cards;
	u32 retval = 0;
	u32 slot_num;
	card_type_e card_type;

	//PDEBUG("Starting emmc_init\n");

	/* Zero the global emmc_status_info structure which keeps info about the host controller ip */
	memset(&emmc_status_info, 0, sizeof(emmc_status_info));

	/* Power up only those cards/devices which are connected
	    - Shut-down the card/device once wait for some time
	    - Enable the power to the card/Device. wait for some time
	*/
	buffer_reg = 1;
	emmc_clear_bits(PWREN, buffer_reg);
	emmc_delay(100);    /*some SDIO cards need more time to power up so changed from 10 to 1000*/
	emmc_set_register(PWREN, buffer_reg);
	emmc_delay(100);

	/* Befor proceeding further lets reset the host controller IP */
	emmc_set_register(CTRL, CTRL_RESET | FIFO_RESET | DMA_RESET);
	emmc_delay(100);
	while (emmc_read_register(CTRL) & (CTRL_RESET | FIFO_RESET | DMA_RESET)) {
		mini_printf("Fail-reset, try again!!!\n");
		emmc_delay(20);
	}

	/* Now make CTYPE to default i.e, all the cards connected will work in 1 bit mode initially*/
	buffer_reg = 0xffffffff;
	emmc_clear_bits(CTYPE, buffer_reg);

	/* No. of cards supported by the IP */
	buffer_reg = emmc_read_register(HCON);
	num_of_cards = HCON_NUM_CARDS(buffer_reg);
	emmc_status_info.num_of_cards = num_of_cards;
	mini_printf("num_of_cards: %d\n", num_of_cards);

	/* disable interrupt */

	emmc_set_register(RINTSTS, 0xffffffff);
	emmc_clear_bits(CTRL, INT_ENABLE);

	/* Set Data and Response timeout to Maximum Value */
	emmc_set_register(TMOUT, 0xffffffff);

	/* Enable the clocks to the all connected cards/drives
	- Note this command is to CIU of host controller ip
	- the command is not sent on the command bus
	- it emplys the polling method to accomplish the task
	- it also emplys wait prev data complete in CMD register
	*/
	emmc_enable_all_clocks();

	/* Set the card Debounce to allow the CDETECT fluctuations to settle down */
	emmc_set_register(DEBNCE, DEFAULT_DEBNCE_VAL);

	/* Update the watermark levels to half the fifo depth
	- while reset bitsp[27..16] contains FIFO Depth
	- Setup Tx Watermark
	- Setup Rx Watermark */
	emmc_set_bits(FIFOTH,  0x20060007);
	emmc_status_info.fifo_depth = 7 * 2;

	slot_num = num_of_cards - 1;

	emmc_clear_bits(CTYPE, ((1 << slot_num) | (1 << (slot_num + 16))));
	//emmc_set_bits(CTYPE, 1 << slot_num);
#ifdef EMMC_SUPPORT_GET_CARD
	card_type = emmc_get_card_type(slot_num);
#else
	card_type = MMC_TYPE;
#endif

	switch (card_type) {
#ifdef EMMC_SUPPORT_GET_CARD

	case SD_TYPE:
		mini_printf(" not suuport SD %d\n", slot_num);
		break;
#endif

	case MMC_TYPE: {
		retval = emmc_reset_mmc_card(slot_num);

		if (retval) {
			mini_printf("MMC reset returned the error %d, slot_num: %d\n", retval, slot_num);
			break;
		}
		//emmc_set_bus_width(slot_num);
		emmc_select_area(slot_num, EMMC_BOOT_PARTITION_1);
		if (emmc_card_info) {
			memcpy(emmc_card_info, &card_info, sizeof(card_info_t));
		}

		mini_printf("The card_boot_size is %d\n", card_info.card_boot_size);
		mini_printf("The card_size is %x\n", card_info.card_size);

		return 0;
	}

	default:
		mini_printf("BAD CARD FOUND AT SLOT %d\n", slot_num);
	}

	return ERRCARDNOTFOUND;
}

void emmc_emmc_read(u8 slot_id, u32 from, u32 len, u8 *buf)
{
	emmc_read_write_block(slot_id, from, buf, 1, 0, 512);
}

void emmc_emmc_write(u8 slot_id, u32 to, u32 len, const u8 *buf)
{
	emmc_read_write_block(slot_id, to, (u8 *)buf, 1, 1, 512);
}

#define EMMC_FLASH_PAGE_SIZE   512       // 512B
