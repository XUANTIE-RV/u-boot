// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <command.h>
#include <net.h>
#include <asm/io.h>
#include <dm.h>
#include <fdt_support.h>
#include <fdtdec.h>
#include <asm/csr.h>

enum board_type {
	NT_DTS = 0,
	T_DTS,
	UBOOT_DTS,
};

long t_start_address;
long t_size;
long nt_start_address;
long nt_size;
long t_dtb_address;
long nt_dtb_address;
struct pmp {
	long start;
	long end;
	int r, w, x;
};
struct pmp pmp_configs[32];
static int total_pmp = 0;
static int setup_nt_pmp_array(long start, long size)
{
	if (total_pmp == 31) {
		printf("PMP entries are full!!!\n");
		return -1;
	}

	pmp_configs[total_pmp].start = start;
	pmp_configs[total_pmp].end = start + size;

	total_pmp++;

	return 0;
}

static void dump_nt_pmp_array(void)
{
	printf("total pmp number: %d\n", total_pmp);
	for (int i = 0; i < total_pmp; i++) {
		printf("pmp[%d]: 0x%lx ~ 0x%lx\n",
			i, pmp_configs[i].start, pmp_configs[i].end);
	}
}

static void setup_nt_pmp_configs(void)
{
	long pmp_entry = PMP_BASE_ADDR + csr_read(CSR_MHARTID) * 0x4000 + 0x100;
	long pmp_cfg = PMP_BASE_ADDR + csr_read(CSR_MHARTID) * 0x4000 + 0x0;

	for (int i = 0, j = 0; i < total_pmp; i++) {
		writel(pmp_configs[i].start >> 12, (void *)(pmp_entry + j * 4));
		j++;
		writel(pmp_configs[i].end >> 12, (void *)(pmp_entry + j * 4));
		j++;
	}

	for (int k = 0; k < total_pmp; k++) {
		int x, y;
		x = k / 4;
		y = k % 4;

		/* pmp_configs[0] must be memory */
		if (k == 0)
			writel(readl((void *)(pmp_cfg + x * 4)) | 0x87 << y * 8, (void *)(pmp_cfg + x * 4));
		else
			writel(readl((void *)(pmp_cfg + x * 4)) | 0x83 << y * 8, (void *)(pmp_cfg + x * 4));
	}

	/* Set default pmp all allow but for T
	 * So NT can't use it either
	 */
	writel(0xc7, (void *)(pmp_cfg + 0x20));

	sync_is();
}

static int parse_memory(const void *blob, int t)
{
	int node;
	fdt32_t *reg;
	long address, size;

	node = fdt_path_offset(blob, "/memory");
	if (node < 0)
		return -ENOENT;

	reg = (fdt32_t *)fdt_getprop(blob, node, "reg", 0);
	address = fdt_translate_address(blob, node, reg);
	reg += 2;
	size = fdt_translate_address(blob, node, reg);
	if (t == T_DTS) {
		t_start_address = address;
		t_size = size;
		printf("t_start_address: 0x%lx\n", t_start_address);
	} else if (t == NT_DTS) {
		nt_start_address = address;
		nt_size = size;
		if (setup_nt_pmp_array(address, size))
			return -EINVAL;
		printf("nt_start_address: 0x%lx\n", nt_start_address);
	}

	return 0;
}

/*
int irq_no[100] = {56, 57, 58, 59, 16, 17, 18, 19, 20, 21, 22, 23, 36, 38, 39, 40, 41,
					44, 54, 52, 24, 25, 74, 68, 27, 150, 66, 62, 64};
*/
int i = 0;
int irq_no[255] = {0};
static int setup_t_plic(const void *blob)
{
	int x, y;
	int i;

	writel(0x40000000, (void *)(PLIC_BASE_ADDR + 0x1ffff8));

	for (i = 0; i < 255 && irq_no[i]; i++) {
		printf("T irq_no: %d\n", irq_no[i]);
		x = irq_no[i] % 32;
		y = irq_no[i] / 32;

		writel(1 << x, (void *)(PLIC_BASE_ADDR + 0x1fe000 + y * 4));
	}

	/* Enable & Lock AMP */
	writel(0xc0000000, (void *)(PLIC_BASE_ADDR + 0x1ffff8));

	return 0;
}

static int parse_soc(const void *blob, int t)
{
	int node, device, irq;
	fdt32_t *intc, *reg;
	const char *status;
	long address, size;

	node = fdt_path_offset(blob, "/soc");
	if (node < 0)
		return -ENOENT;

	printf("%s device ================\n", t == T_DTS ? "T" : "NT");
	for (device = fdt_first_subnode(blob, node);
		device >= 0; device = fdt_next_subnode(blob, device)) {
		if (device == -FDT_ERR_NOTFOUND)
			return -ENOENT;

		printf("name: %s\n", fdt_get_name(blob, device, NULL));
		status = (char *)fdt_getprop(blob, device, "status", NULL);
		if (status)
			printf("\tstatus: %s\n", status);

		intc = (fdt32_t *)fdt_getprop(blob, device, "interrupts", 0);
		if (intc) {
			irq = fdt_read_number(intc, 1);
			printf("\tirq_no: %d\n", irq);
			if (t == T_DTS) {
				irq_no[i] = irq;
				i++;
			}
		}

		reg = (fdt32_t *)fdt_getprop(blob, device, "reg", 0);
		if (reg) {
			address = fdt_translate_address(blob, device, reg);
			printf("\taddress: 0x%lx\n", address);
			reg += 2;
			size = fdt_translate_address(blob, device, reg);
			printf("\tsize: 0x%lx\n", size);
			if ((t == NT_DTS) && setup_nt_pmp_array(address, size))
				return -EINVAL;
		}
	}

	return 0;
}

static int parse_and_set_iopmp(const void *blob, int t)
{
	int node, device;
	fdt32_t *reg, *range;
	long base_addr, start, end, size;

	node = fdt_path_offset(blob, "/iopmp");
	if (node < 0)
		return -ENOENT;

	printf("%s iopmp ================\n", t == T_DTS ? "T" : "NT");
	for (device = fdt_first_subnode(blob, node);
		device >= 0; device = fdt_next_subnode(blob, device)) {
		if (device == -FDT_ERR_NOTFOUND)
			return -ENOENT;

		printf("name: %s\n", fdt_get_name(blob, device, NULL));
		reg = (fdt32_t *)fdt_getprop(blob, device, "reg", 0);
		range = (fdt32_t *)fdt_getprop(blob, device, "range", 0);
		if (reg && range) {
			base_addr = fdt_translate_address(blob, device, reg);
			printf("\tbase_addr: 0x%lx\n", base_addr);
			start = fdt_translate_address(blob, device, range);
			printf("\tstart: 0x%lx\n", start);
			range += 2;
			size = fdt_translate_address(blob, device, range);
			printf("\tsize: 0x%lx\n", size);
			end = start + size;
			if ((t == NT_DTS)) {
				writel(start >> 12, (void *)(base_addr + 0x280));
				writel(end >> 12, (void *)(base_addr + 0x284));
				writel(0x3, (void *)(base_addr + 0x80));
			}
		}
	}

	return 0;
}

static long boot_addr[4];
static long boot_addr_chk[4];

static void boot_t_core(void)
{
	void (*fly)(long, long);

	fly = (void *)t_start_address;

	if (csr_read(CSR_MHARTID) == 0)
		goto boot;

	csr_write(CSR_SMPEN, 0x1);
	csr_write(CSR_MCOR, 0x70013);
	csr_write(CSR_MCCR2, 0xe0010009);
	csr_write(CSR_MHCR, 0x11ff);
	csr_write(CSR_MXSTATUS, 0x638000);
	csr_write(CSR_MHINT, 0x16e30c);

boot:
	/* Set this for locking it */
	csr_write(CSR_MTEE, 0xff);

	fly(0xdeadbeef, t_dtb_address);
}

static void boot_nt_core(void)
{
	void (*fly)(long, long);

	fly = (void *)nt_start_address;

	setup_nt_pmp_configs();

	csr_write(CSR_SMPEN, 0x1);
	csr_write(CSR_MCOR, 0x70013);
	csr_write(CSR_MCCR2, 0xe0010009);
	csr_write(CSR_MHCR, 0x11ff);
	csr_write(CSR_MXSTATUS, 0x638000);
	csr_write(CSR_MHINT, 0x16e30c);

	//csr_write(CSR_MTEE, 0x00); /* Do it in opensbi */

	fly(0xdeadbeef, nt_dtb_address);
}

static int parse_cpu(const void *blob, int t)
{
	int node, cpu, core;
	fdt32_t *reg;
	const char *status;

	node = fdt_path_offset(blob, "/cpus");
	if (node < 0)
		return -ENOENT;

	for (cpu = fdt_first_subnode(blob, node);
		cpu >= 0; cpu = fdt_next_subnode(blob, cpu)) {

		reg = (fdt32_t *)fdt_getprop(blob, cpu, "reg", 0);
		core = fdt_read_number(reg, 1);
		printf("core %d  ", core);

		status = fdt_getprop(blob, cpu, "status", NULL);
		if (t == T_DTS) {
			if (!strcmp(status, "okay")) {
				printf("T world\n");
				boot_addr[core] = (long)&boot_t_core;
			} else if (!strcmp(status, "disabled")) {
				printf("NT world\n");
				boot_addr[core] = (long)&boot_nt_core;
			} else {
				printf("Incorrect DTS! Not okay nor disabled\n");
				return -EINVAL;
			}
		} else if (t == NT_DTS) {
			if (!strcmp(status, "okay")) {
				printf("NT world\n");
				boot_addr_chk[core] = (long)&boot_nt_core;
			} else if (!strcmp(status, "disabled")) {
				printf("T world\n");
				boot_addr_chk[core] = (long)&boot_t_core;
			} else {
				printf("Incorrect DTS! Not okay nor disabled\n");
				return -EINVAL;
			}
		}
	}

	return 0;
}

int check_cpu(void)
{
	if ((boot_addr[0] != boot_addr_chk[0]) ||
		(boot_addr[1] != boot_addr_chk[1]) ||
		(boot_addr[2] != boot_addr_chk[2]) ||
		(boot_addr[3] != boot_addr_chk[3]))
		return -1;
	return 0;
}

static int parse_dtb(const void *blob_t, const void *blob_nt)
{
	int ret = 0;

	parse_memory(blob_t, T_DTS);
	parse_memory(blob_nt, NT_DTS);

	parse_soc(blob_t, T_DTS);
	parse_soc(blob_nt, NT_DTS);

	parse_and_set_iopmp(blob_nt, NT_DTS);

	ret = parse_cpu(blob_t, T_DTS);
	ret |= parse_cpu(blob_nt, NT_DTS);
	ret |= check_cpu();

	if (ret)
		return -EINVAL;

	return 0;
}

static int boot_buddies(void)
{
	printf("cpu 0 ---0x%lx\n", boot_addr[0]);
	printf("cpu 1 ---0x%lx\n", boot_addr[1]);
	printf("cpu 2 ---0x%lx\n", boot_addr[2]);
	printf("cpu 3 ---0x%lx\n", boot_addr[3]);

	writel(boot_addr[1] & 0xffffffff, (void *)(0xffff018000 + 0x58));
	writel(boot_addr[1] >> 32,        (void *)(0xffff018000 + 0x5c));
	writel(0b00111, (void *)(0xffff014000 + 0x04));
	udelay(50000);

	writel(boot_addr[2] & 0xffffffff, (void *)(0xffff018000 + 0x60));
	writel(boot_addr[2] >> 32,        (void *)(0xffff018000 + 0x64));
	writel(0b01111, (void *)(0xffff014000 + 0x04));
	udelay(50000);

	writel(boot_addr[3] & 0xffffffff, (void *)(0xffff018000 + 0x68));
	writel(boot_addr[3] >> 32,        (void *)(0xffff018000 + 0x6c));
	writel(0b11111, (void *)(0xffff014000 + 0x04));

	return 0;
}

extern int check_for_good(char *name, long addr);
static int parse_args(ulong *addr, char *const argv[])
{
	addr[1] = simple_strtoul(argv[1], NULL, 16);
	printf("linux: 0x%lx\n", addr[1]);
	if (check_for_good("linux", addr[1]) < 0)
		return -1;

	addr[2] = simple_strtoul(argv[2], NULL, 16);
	printf("rootfs: 0x%lx\n", addr[2]);
	if (check_for_good("rootfs", addr[1]) < 0)
		return -1;

	addr[3] = simple_strtoul(argv[3], NULL, 16);
	t_dtb_address = addr[3];
	printf("t dtb: 0x%lx\n", addr[3]);
	if (check_for_good("t.dtb", addr[1]) < 0)
		return -1;

	addr[4] = simple_strtoul(argv[4], NULL, 16);
	nt_dtb_address = addr[4];
	printf("nt dtb: 0x%lx\n", addr[4]);
	if (check_for_good("nt.dtb", addr[1]) < 0)
		return -1;

	return 0;
}

#define BOOTCODE_SIZE    4
char tee_bootcode[BOOTCODE_SIZE] = {0x73, 0x50, 0x40, 0x7f};
int light_boot(int argc, char * const argv[])
{
	ulong	addr[5];
	void (*entry)(int, long);

	if (argc < 2) {
		printf("argc: %d\n", argc);
		printf("args not match!\n");
		return CMD_RET_USAGE;
	}

	if (parse_args(addr, argv) < 0) {
		printf("parse args failed!\n");
		return CMD_RET_USAGE;
	}

	if (parse_dtb((const void *)addr[3], (const void *)addr[4]) < 0) {
		printf("parse dtb failed!\n");
		return CMD_RET_USAGE;
	}

	setup_t_plic((const void *)addr[3]);

	dump_nt_pmp_array();

	memcpy((void *)nt_start_address, tee_bootcode, BOOTCODE_SIZE);

	boot_buddies();

	/* Set cpu 0's start address */
	addr[0] = boot_addr[0];

	entry = (void (*)(int, long))addr[0];
	entry(0, addr[3]);

	return 0;
}
