/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include "emmc_interface.h"
#include "hardware.h"

#define CEATA_MAP   0xffffffff
static phys_addr_t base_adr = EMMC_BASEADDR;

/*
 * Set the particular bits of the specified register.
 * @param[in] reg	The particular register to which the bits are to be set
 * @param[in] val	The bitmask for the bits which are to be set.
 * return 	The new value of the register
 */
u32 emmc_set_bits(controller_reg_e reg, u32 val)
{
	u32 *reg_addr;
	reg_addr = (u32 *)(base_adr + reg);

	*reg_addr |= val;
	return *reg_addr;
}

/*
 * Clear the particular bits of the specified register.
 * @param[in] reg	The particular register to which the bits are to be cleared.
 * @param[in] val	The bitmask for the bits which are to be cleared.
 * return 	The new value of the register
 */
u32 emmc_clear_bits(controller_reg_e reg, u32 val)
{
	u32 *reg_addr;
	reg_addr = (u32 *)(base_adr + reg);

	*reg_addr &= (~val);
	return *reg_addr;
}

/*
 * Set the value of the specified register.
 * @param[in] reg	The particular register for which values are to be set.
 * @param[in] val	The value.
 * return 	The new value of the register
 */
u32 emmc_set_register(controller_reg_e reg, u32 val)
{
	u32 *reg_addr;
	reg_addr = (u32 *)(base_adr + reg);

	*reg_addr = val;
	return *reg_addr;
}

/*
 * Read the value of the specified register.
 * @param[in] reg	The particular register which is to be read.
 * return 	The value of the register
 */
u32 emmc_read_register(controller_reg_e reg)
{
	u32 *reg_addr;
	u32 retval;

	reg_addr = (u32 *)(base_adr + reg);
	retval = *reg_addr;

	/* if request is to read CDETECT, make sure you return
	   the hacked ;) CEATA_MAP bit */
	if (CDETECT == reg) {
		retval &= CEATA_MAP;
	}

	return retval;
}

