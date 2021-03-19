/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#include <common.h>
#include "../datatype.h"
#include "../spi/spi.h"
#include "../spiflash/spiflash.h"

#define THIS_MODULE MODULE_DEV_SPIFLASH

#define SPI_FLASH_BLOCK_SIZE        0x10000
#define SPI_FLASH_SECTOR_SIZE       0x1000
#define SPI_FLASH_PAGE_SIZE         0x100
#define SPI_FLASH_BASEADDR          0x0
#define IS_SPIFLASH_ADDR(addr) \
            ((addr >= 0x0) && (addr < 0x800000))

s32 spi_norflash_read_status_register(u8 *status)
{
	u8 command = 0x05;

	/* read status register*/
	spi_open(CK_SPI);
	spi_set_ndf(CK_SPI, 0);
	spi_transfer_query(CK_SPI, CK_SPI_CS0, &command, 1, status, 1);

	status++;
	command = 0x35;
	spi_open(CK_SPI);
	spi_set_ndf(CK_SPI, 0);

	spi_transfer_query(CK_SPI, CK_SPI_CS0, &command, 1, status, 1);
	return SUCCESS;
}

s32 spi_norflash_write_status_register(u8 *status)
{
	u8 reg1[2] = {0xff, 0xff};
	u8 command_enable_write = 0x06;
	u8 command[3] = {0x01, 0x0, 0x0};

	spi_open(CK_SPI);
	spi_set_mode(CK_SPI, CK_SPI_TX);

	command[1] = *status;
	command[2] = *(status + 1);

	spi_send_query(CK_SPI, CK_SPI_CS0, &command_enable_write, 1);
	spi_send_query(CK_SPI, CK_SPI_CS0, command, 3);

	while ((reg1[0] & 0x1) == 1) {
		spi_norflash_read_status_register(reg1);
	}

	return SUCCESS;
}

s32 spi_norflash_erase_chip(u8 id)
{
	u8 reg1[2] = {0xff, 0xff};
	u8 reg[2] = {0x00, 0x00};
	u8 command_enable_write = 0x06;
	u8 command_erase_chip = 0x60;

	spi_open(CK_SPI);
	spi_set_mode(CK_SPI, CK_SPI_TX);

	spi_send_query(CK_SPI, CK_SPI_CS0, &command_enable_write, 1);
	while ((reg[0] & 0x2) == 0) {
		spi_norflash_read_status_register(reg);
	}
	spi_set_mode(CK_SPI, CK_SPI_TX);
	spi_send_query(CK_SPI, CK_SPI_CS0, &command_erase_chip, 1);
	while (((reg1[0] & 0x1) == 1) || ((reg1[0] & 0x2) == 0x2)) {
		spi_norflash_read_status_register(reg1);
	}

	return SUCCESS;
}

s32 spi_norflash_erase_block(u8 id, u32 offset, u32 length)
{
	u8 reg1[2] = {0xff, 0xff};
	u8 reg[2] = {0x00, 0x00};
	u8 command_enable_write = 0x06;
	u8 command_erase_block[4] = {0xd8, 0x0, 0x0, 0x0};

	if (length & (SPI_FLASH_BLOCK_SIZE - 1)) {
//        LOG_D_S("erase length is not aligned by block size");
//        LOG_E();
		return FAILURE;
	}
	offset += SPI_FLASH_BASEADDR;
	while (length) {
		spi_open(CK_SPI);
		spi_set_mode(CK_SPI, CK_SPI_TX);

		command_erase_block[1] = (offset >> 16) & 0xff;
		command_erase_block[2] = (offset >> 8) & 0xff;
		command_erase_block[3] = offset & 0xff;

		spi_send_query(CK_SPI, CK_SPI_CS0, &command_enable_write, 1);
		while ((reg[0] & 0x2) == 0) {
			spi_norflash_read_status_register(reg);
		}
		spi_set_mode(CK_SPI, CK_SPI_TX);
		spi_send_query(CK_SPI, CK_SPI_CS0, command_erase_block, 4);
		while (((reg1[0] & 0x1) == 1) || ((reg1[0] & 0x2) == 0x2)) {
			spi_norflash_read_status_register(reg1);
		}
		offset += SPI_FLASH_BLOCK_SIZE;
		length -= SPI_FLASH_BLOCK_SIZE;
	}

	return SUCCESS;
}

s32 spi_norflash_erase_sector(u8 id, u32 offset, u32 length)
{
	u8 reg1[2] = {0xff, 0xff};
	u8 reg[2] = {0x00, 0x00};
	u8 command_enable_write = 0x06;
	u8 command_erase_sector[5] = {0x20, 0x0, 0x0, 0x0};

	if (length & (SPI_FLASH_SECTOR_SIZE - 1)) {
		return -1;
	}
	offset += SPI_FLASH_BASEADDR;
	while (length) {
		spi_open(CK_SPI);
		spi_set_mode(CK_SPI, CK_SPI_TX);

		command_erase_sector[1] = (offset >> 16) & 0xff;
		command_erase_sector[2] = (offset >> 8) & 0xff;
		command_erase_sector[3] = offset & 0xff;

		spi_send_query(CK_SPI, CK_SPI_CS0, &command_enable_write, 1);
		while ((reg[0] & 0x2) == 0) {
			spi_norflash_read_status_register(reg);
		}
		spi_set_mode(CK_SPI, CK_SPI_TX);
		spi_send_query(CK_SPI, CK_SPI_CS0, command_erase_sector, 4);
		while (((reg1[0] & 0x1) == 1) || ((reg1[0] & 0x2) == 0x2)) {
			spi_norflash_read_status_register(reg1);
		}
		offset += SPI_FLASH_SECTOR_SIZE;
		length -= SPI_FLASH_SECTOR_SIZE;
	}
	return SUCCESS;

}

s32 spiflash_erase(u8 id, ERASE_TYPE_t type, u32 offset, u32 length)
{
	switch (type) {
	case ERASE_BY_SECTOR:
		spi_norflash_erase_sector(id, offset, length);
		break;
	case ERASE_BY_BLOCK:
		spi_norflash_erase_block(id, offset, length);
		break;
	case ERASE_BY_CHIP:
		spi_norflash_erase_chip(id);
		break;
	default:
		return FAILURE;
	}

	return SUCCESS;
}

s32 spi_norflash_write_page(u8 id, u32 dst_addr, u8 *src_buf, u32 len)
{
	u8 reg1[2] = {0xff, 0xff};
	u8 reg[2] = {0x00, 0x00};
	u8 command_enable_write = 0x06;
	u8 command_page_write[260] = {0x02, 0x0, 0x0, 0x0};
	if (len == 0) {
		return FAILURE;
	}

	spi_open(CK_SPI);
	spi_set_mode(CK_SPI, CK_SPI_TX);
	spi_set_ndf(CK_SPI, len - 1);

	command_page_write[1] = (dst_addr >> 16) & 0xff;
	command_page_write[2] = (dst_addr >> 8) & 0xff;
	command_page_write[3] = dst_addr & 0xff;

	memcpy(&command_page_write[4], src_buf, len);
	spi_send_query(CK_SPI, CK_SPI_CS0, &command_enable_write, 1);
	while ((reg[0] & 0x2) == 0) {
		spi_norflash_read_status_register(reg);
	}
	spi_set_mode(CK_SPI, CK_SPI_TX);
	spi_send_query(CK_SPI, CK_SPI_CS0, command_page_write, len + 4);

	while (((reg1[0] & 0x1) == 1) || ((reg1[0] & 0x2) == 0x2)) {
		spi_norflash_read_status_register(reg1);
	}
	return SUCCESS;
}

s32 spiflash_write(u8 id, u32 offset, const u8 *buf, u32 length, u32 *retlen)
{
	u32 first_page_space;
	u8 *p = (u8 *)buf;
	u32 page_num;
	u8 i;

	offset += SPI_FLASH_BASEADDR;
	first_page_space = SPI_FLASH_PAGE_SIZE - (offset & (SPI_FLASH_PAGE_SIZE - 1));
	if (length < first_page_space) {
		first_page_space = length;
	}
	spi_norflash_write_page(id, offset, p, first_page_space);
	offset += first_page_space;
	p += first_page_space;
	length -= first_page_space;

	page_num = length >> 8;
	for (i = 0; i < page_num; i++) {
		spi_norflash_write_page(id, offset, p, SPI_FLASH_PAGE_SIZE);
		offset += SPI_FLASH_PAGE_SIZE;
		p += SPI_FLASH_PAGE_SIZE;
		length -= SPI_FLASH_PAGE_SIZE;
	}
	spi_norflash_write_page(id, offset, p, length);
	return SUCCESS;
}

s32 spiflash_read_id(u8 *id)
{
	u8 command[4] = {0x90, 0x0, 0x0, 0x0};
	/* read the ID*/
	spi_open(CK_SPI);
	spi_set_ndf(CK_SPI, 1);

	spi_transfer_query(CK_SPI, CK_SPI_CS0, command, 4, id, 2);
	return SUCCESS;
}

s32 spiflash_read(u8 id, u32 offset, u8* buf, u32 length, u32* retlen)
{
	u8 command[4] = {0x3, 0x0, 0x0, 0x00};

	offset += SPI_FLASH_BASEADDR;
	spi_open(CK_SPI);
	spi_set_ndf(CK_SPI, length - 1);

	command[1] = (offset >> 16) & 0xff;
	command[2] = (offset >> 8) & 0xff;
	command[3] = offset & 0xff;

	spi_transfer_query(CK_SPI, CK_SPI_CS0, command, 4, buf, length);
	return SUCCESS;
}
#if 0
int spi_norflash_program(u32 dst_addr, u8 *src_buf, u32 len)
{
	u32 i;
	u32 block_addr;
	u32 sector_addr;
	u32 page_num;
	u32 first_page_space;
	u8 *p = src_buf;
	u32 length;

	if (!IS_SPIFLASH_ADDR(dst_addr) || !IS_SPIFLASH_ADDR(dst_addr + len) || src_buf == NULL) {
		return -1;
	}

	length = len;
	/* erase the region */
	if (length > SPI_FLASH_BLOCK_SIZE) {
		block_addr = dst_addr;
		while (block_addr < (dst_addr + length)) {
			spi_norflash_erase_block(block_addr);
			block_addr += SPI_FLASH_BLOCK_SIZE;
		}
		spi_norflash_erase_block(dst_addr + length);


	} else if (length < SPI_FLASH_BLOCK_SIZE && length > SPI_FLASH_SECTOR_SIZE) {
		sector_addr = dst_addr;
		while (sector_addr < (dst_addr + length)) {
			spi_norflash_erase_sector(sector_addr);
			sector_addr += SPI_FLASH_SECTOR_SIZE;
		}
		spi_norflash_erase_sector(dst_addr + length);

	} else {
		spi_norflash_erase_chip();
	}
	first_page_space = SPI_FLASH_PAGE_SIZE - (dst_addr & (SPI_FLASH_PAGE_SIZE - 1));
	spi_norflash_write_page(dst_addr, p, first_page_space);
	dst_addr += first_page_space;
	p += first_page_space;
	length -= first_page_space;

	page_num = length >> 8;
	for (i = 0; i < page_num - 1; i++) {
		spi_norflash_write_page(dst_addr, p, SPI_FLASH_PAGE_SIZE);
		dst_addr += SPI_FLASH_PAGE_SIZE;
		p += SPI_FLASH_PAGE_SIZE;
		length -= SPI_FLASH_PAGE_SIZE;
	}
	spi_norflash_write_page(dst_addr, p, length);

	return 0;
}
#endif
