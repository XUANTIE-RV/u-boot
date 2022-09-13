/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#ifndef __ASM_SPIFLASH_H__
#define __ASM_SPIFLASH_H__

#include "../datatype.h"

typedef enum ERASE_TYPE_e {
	ERASE_BY_SECTOR,
	ERASE_BY_BLOCK,
	ERASE_BY_CHIP
} ERASE_TYPE_t;

s32 spiflash_init(void);

/*
* read the spiflash id.
* Parameters:
*   id: the pointer to the id buffer.
* return: SUCCESS or FAILURE.
*/
s32 spiflash_read_id(u8 *id);
/*
* read the spiflash status register.
* Parameters:
*   status: the pointer to the status buffer.
* return: SUCCESS or FAILURE.
*/
s32 spi_norflash_read_status_register(u8 *status);

/*
* write the spiflash status register.
* Parameters:
*   status: the pointer to the status buffer.
* return: SUCCESS or FAILURE.
*/
s32 spi_norflash_write_status_register(u8 *status);

/*
* read data from the spiflash.
* Parameters:
*   src_addr: the start address to be read.
*   dst_buf: the pointer to the data buffer.
*   len: the read data length.
* return: SUCCESS or FAILURE.
*/
s32 spiflash_read(u8 id, u32 offset, u8 *buf, u32 length, u32 * retlen);

/*
* erase the chip.
* Parameters:
*   None.
* return: SUCCESS or FAILURE.
*/
s32 spi_norflash_erase_chip(u8 id);

/*
* erase the spiflash by block.
* Parameters:
*   src_addr: the start address to be erased.
* return: SUCCESS or FAILURE.
*/
s32 spi_norflash_erase_block(u8 id, u32 offset, u32 length);

/*
* erase the spiflash by sector.
* Parameters:
*   src_addr: the start address to be erased.
* return: SUCCESS or FAILURE.
*/
s32 spi_norflash_erase_sector(u8 id, u32 offset, u32 length);

s32 spiflash_erase(u8 id, ERASE_TYPE_t type, u32 offset, u32 length);
/*
* write data to the spiflash by page.
* Parameters:
*   dst_addr: the start address to be write.
*   src_buf: the pointer to the data buffer.
*   len: the write data length.
* return: SUCCESS or FAILURE.
*/
s32 spi_norflash_write_page(u8 id, u32 dst_addr, u8 *src_buf, u32 len);

s32 spiflash_write(u8 id, u32 offset, const u8 *buf, u32 length, u32 *retlen);
/*
* program data to the spiflash.
* Parameters:
*   dst_addr: the start address to be program.
*   src_buf: the pointer to the data buffer.
*   len: the program data length.
* return: SUCCESS or FAILURE.
*/
//int spi_norflash_program(u32 dst_addr, u8 *src_buf, u8 len);
#endif /* __ASM_SPIFLASH_H__ */

