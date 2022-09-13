/*
 * Copyright (C) 2020 C-SKY Microsystems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <asm/byteorder.h>
#include <asm/io.h>
#include <cpu_func.h>

DECLARE_GLOBAL_DATA_PTR;


extern int disable_slave_cpu(void);
extern int set_slave_cpu_entry(phys_addr_t entry);
extern int enable_slave_cpu(void);


#ifdef CONFIG_CMD_BOOT_SLAVE
int do_bootslave(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	u32 *slave_ddr_base;
	u32 slave_jump_addr;

	if (argc < 2) {
		return CMD_RET_USAGE;
	}

	// hold slave cpu
	disable_slave_cpu();

	slave_ddr_base = (u32*)(uintptr_t)simple_strtoull(argv[1], NULL, 16);
	slave_jump_addr = *slave_ddr_base;
	printf("slave_ddr_base: 0x%p\n", slave_ddr_base);
	printf("slave_jump_addr: 0x%x\n", slave_jump_addr);

	// set slave jump addr
	set_slave_cpu_entry(slave_jump_addr);

	// release slave cpu
	flush_cache((uintptr_t)slave_ddr_base, 0x10000);
	printf("reset slave cpu\n");
	enable_slave_cpu();

	return 0;
}
#endif // CONFIG_CMD_BOOT_SLAVE
