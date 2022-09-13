// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2002
 * Gerald Van Baren, Custom IDEAS, vanbaren@cideas.com
 */

/*
 * SPI Read/Write Utilities
 */

#include <common.h>
#include <command.h>
#include <dm.h>
#include <errno.h>
#include <spi.h>

/*-----------------------------------------------------------------------
 * Definitions
 */

#ifndef MAX_SPI_BYTES
#define MAX_SPI_BYTES 64	/* Maximum number of bytes we can handle */
#endif

/*
 * Values from last command.
 */
static unsigned int	bus;
static unsigned int	cs;
static unsigned int	mode;
static int          bitlen;
static unsigned int speed = 10000000;
static uchar		dout[MAX_SPI_BYTES];
static uchar		din[MAX_SPI_BYTES];
typedef struct
{
	/* data */
	char*          cmd_str;
	unsigned char  cmd_code;
}ate_cmd_t;

static ate_cmd_t g_ate_cmd_table[] = {
	{"rd.b",0xb1},
	{"rd.w",0xb3},
	{"rd.l",0xb2},
	{"wr.b",0xa1},
	{"wr.w",0xa3},
	{"wr.l",0xa2},
};

static inline int ate_is_write_cmd(unsigned char cmd){
	if(cmd == 0xa1 || cmd == 0xa2 || cmd == 0xa3 ) {
		return 1;
	}
	else {
		return 0;
	}
}

static int ate_get_reg_width(unsigned char cmd)
{
	int len = 0;
	switch (cmd)
	{
	case 0xa1:
	case 0xb1:
		len = 1;
		break;

	case 0xa2:
	case 0xb2:
		len = 4;
		break;

	case 0xa3:
	case 0xb3:
		len = 2;
		break;
	default:
		break;
	}

	return len;
}

static inline int ate_get_write_rsp(unsigned char *rsp, unsigned char *err_code)
{
	/* 0xc1 + err_code*/
	if(rsp == NULL || err_code == NULL)
		return -1;

	*err_code = *(rsp+1);

	return 0;
}

static  int ate_get_read_value(unsigned char *rsp, unsigned int reg_len, unsigned int *val)
{
	unsigned i;
	if(rsp == NULL || val == NULL)
		return -1;

	/*0xc1 + 1/2/4B */
	*val = 0;
	for(i = 0; i < reg_len; i++)
	{
		*val |= *(rsp+1+i) << (i*8);
	}

	return 0;
}

static int ate_build_cmd_frame(unsigned char cmd,unsigned long long reg_addr, unsigned int reg_value)
{
	int spi_frame_index = 0,i,len;

	memset(dout,0,sizeof(dout));
	memset(din,0,sizeof(din));

	/*fill cmd code*/
	dout[spi_frame_index++] = cmd;
	//printf("spi_frame_index: %d\n",spi_frame_index);
	/*fill reg_addr,exp 0xffea000000 -> 0x00 0x00 0x00 0xea 0xff */
	for(i = 0; i < 5; i++){
		dout[spi_frame_index++] = (unsigned char)((reg_addr >> i*8 ) &0xFF);
	}

	//printf("spi_frame_index: %d\n",spi_frame_index);
	/*check whether send reg_value and reg_value length in bytes */
	len = ate_get_reg_width(cmd);

    /* fill reg-value */
	if(ate_is_write_cmd(cmd)) {
		for(i = 0; i < len; i++) {
			dout[spi_frame_index++] = (unsigned char)((reg_value >> i*8 ) &0xFF);
		}
	}

	//printf("spi_frame_index: %d\n",spi_frame_index);
	/*insert 2-byte or 3byte dummy data */
	if(ate_is_write_cmd(cmd)) {
		spi_frame_index +=2;
	}else {
		spi_frame_index +=2;
	}

	//printf("spi_frame_index: %d %d\n",len,spi_frame_index);
	/*pading clk for rsp */
	if(ate_is_write_cmd(cmd)) {
		/*0xc1 + err_code(1B)*/
		spi_frame_index +=2;
	}else {
		/*0xc1 + 1B/2B/4B data */
		spi_frame_index +=len + 1;
	}

	//printf("spi_frame_index: %d\n",spi_frame_index);
   #if 0 
	for(i= 0; i < spi_frame_index; i++){
		if(i == 0)
			printf("spi_tx_frame:");
		printf("%02x ",dout[i]);
		if(i == spi_frame_index-1)
			printf("\n");
	}
   #endif

	return spi_frame_index;
}


static int ate_get_cmd_code(char*pcmd, uchar* cmd)
{
	unsigned int table_len = sizeof(g_ate_cmd_table)/sizeof(g_ate_cmd_table[0]);
    unsigned int i;

	for(i = 0 ; i < table_len; i++){
		if(!strcmp(pcmd,g_ate_cmd_table[i].cmd_str)){
			*cmd = g_ate_cmd_table[i].cmd_code;
			return 0;
		}
	}

	return -1;
}

static void ate_spi_config(unsigned int bus, unsigned int cs, unsigned mode, unsigned int speed)
{
	bus = bus;
	cs  = cs;
	mode = mode;
    speed = speed;
	return;
}

static int do_spi_xfer(int bus, int cs ,int bprint_rx)
{
	struct spi_slave *slave;
	int ret = 0;
	int j;

#ifdef CONFIG_DM_SPI
	char name[30], *str;
	struct udevice *dev;

	snprintf(name, sizeof(name), "generic_%d:%d", bus, cs);
	str = strdup(name);
	if (!str)
		return -ENOMEM;
	ret = spi_get_bus_and_cs(bus, cs, speed, mode, "spi_generic_drv",
				 str, &dev, &slave);
	if (ret)
		return ret;
#else
	slave = spi_setup_slave(bus, cs, speed, mode);
	if (!slave) {
		printf("Invalid device %d:%d\n", bus, cs);
		return -EINVAL;
	}
#endif

	ret = spi_claim_bus(slave);
	if (ret)
		goto done;
	printf("out:\n");
	if(bprint_rx)
	{
		for (j = 0; j < ((bitlen + 7) / 8); j++)
			printf("%02X", dout[j]);
		printf("\n");
	}
	ret = spi_xfer(slave, bitlen, dout, din,
		       SPI_XFER_BEGIN | SPI_XFER_END);
#ifndef CONFIG_DM_SPI
	/* We don't get an error code in this case */
	if (ret)
		ret = -EIO;
#endif
	if (ret) {
		printf("Error %d during SPI transaction\n", ret);
	} else {
		printf("in:\n");
		if(bprint_rx)
		{
			for (j = 0; j < ((bitlen + 7) / 8); j++)
				printf("%02X", din[j]);
			printf("\n");
		}
	}

done:
	spi_release_bus(slave);
#ifndef CONFIG_DM_SPI
	spi_free_slave(slave);
#endif

	return ret;
}

/*
 * SPI read/write
 *
 * Syntax:
 *   spi {dev} {num_bits} {dout}
 *     {dev} is the device number for controlling chip select (see TBD)
 *     {num_bits} is the number of bits to send & receive (base 10)
 *     {dout} is a hexadecimal string of data to send
 * The command prints out the hexadecimal string received via SPI.
 */

int do_spi (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char  *cp = 0;
	uchar tmp;
	int   j;

	/*
	 * We use the last specified parameters, unless new ones are
	 * entered.
	 */

	if ((flag & CMD_FLAG_REPEAT) == 0)
	{
		if (argc >= 2) {
			mode = CONFIG_DEFAULT_SPI_MODE;
			bus = simple_strtoul(argv[1], &cp, 10);
			if (*cp == ':') {
				cs = simple_strtoul(cp+1, &cp, 10);
			} else {
				cs = bus;
				bus = CONFIG_DEFAULT_SPI_BUS;
			}
			if (*cp == '.')
				mode = simple_strtoul(cp+1, NULL, 10);
		}
		if (argc >= 3)
			bitlen = simple_strtoul(argv[2], NULL, 10);
		if (argc >= 4) {
			cp = argv[3];
			for(j = 0; *cp; j++, cp++) {
				tmp = *cp - '0';
				if(tmp > 9)
					tmp -= ('A' - '0') - 10;
				if(tmp > 15)
					tmp -= ('a' - 'A');
				if(tmp > 15) {
					printf("Hex conversion error on %c\n", *cp);
					return 1;
				}
				if((j % 2) == 0)
					dout[j / 2] = (tmp << 4);
				else
					dout[j / 2] |= tmp;
			}
		}
	}

	if ((bitlen < 0) || (bitlen >  (MAX_SPI_BYTES * 8))) {
		printf("Invalid bitlen %d\n", bitlen);
		return 1;
	}

	if (do_spi_xfer(bus, cs,1))
		return 1;

	return 0;
}

static char ate_cmd_help [] = {
	"ate_spi speed wr{.b .w .l} addr data \n"
	"ate_spi speed rd{.b .w .l} addr\n"
	"{speed} is baud rate of spi, uinit in hz\n" 
	"{ rd{.b .w .l}| wr{.b .w .l} } is the reading or writing reg operation\n"
	"{addr}  is chip reg addr\n"
	"{data}  is the data while opertion is writing\n"
};
/*
 * SPI read/write
 *
 * Syntax:
 *   ate_spi {dev} {rd|wr} {b|w|l} {addr} {data}
 *     {dev} is the device number for controlling chip select (see TBD)
 *     {rd|wr} is the reading or writing reg operation
 *     {b|w|l} is the width of reg value
 *     {addr}  is chip reg addr
 *     {data}  is the data while opertion is writing
 * The command prints out the hexadecimal string received via SPI.
 */

int do_ate_spi (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int   ret,frame_len;
	unsigned char      cmd = 0;
	unsigned long long reg_addr = 0;
	unsigned int       reg_value = 0;

	/*
	 * We use the last specified parameters, unless new ones are
	 * entered.
	 */

	if ((flag & CMD_FLAG_REPEAT) == 0)
	{
		/* parser argv */

		if(argc < 4){
			printf("cmd err \n");
			printf("%s",ate_cmd_help);
		}

		/*get speed */
		speed = simple_strtoul(argv[1],NULL,10);
		if(speed < 1000000) {
			printf("speed need bigger than 1000000 hz\n");
			return -1;
		}

	    /*
	     *  cs = cs0
	     *  mode = mode0
	     *  speed = 1000000(default)
	     *  bus = 0
	     */
		ate_spi_config(0,0,0,10000000);

		/*get cmd code*/
		ret = ate_get_cmd_code(argv[2],&cmd);
		if(ret){
			printf("unknow cmd code \n");
			printf("%s",ate_cmd_help);
			return ret;
		}

		/* get reg_addr */
		reg_addr = simple_strtoull(argv[3],NULL,16);

		if(ate_is_write_cmd(cmd)){
			reg_value = simple_strtoul(argv[4],NULL,16);
		}

	}

	/*build cmd frame */
	frame_len = ate_build_cmd_frame(cmd,reg_addr,reg_value);
	if(frame_len < 0)
		return -1;

	bitlen = frame_len<<3;
	/*send cmd frame and receive result */
	if (do_spi_xfer(bus, cs,0))
		return 1;
	/* get rsp */
	if(ate_is_write_cmd(cmd)){
		unsigned char err_code = 0xde;
		ret = ate_get_write_rsp(&din[frame_len-2],&err_code);
		printf("0x%02x\n",err_code);
		//printf(" frame_len %d  ret = %d rsp = %02x\n",frame_len,ret,err_code);
	}else {
		unsigned int val = 0;
		unsigned int reg_len = 0;
		reg_len = ate_get_reg_width(cmd);
		ate_get_read_value(&din[frame_len-reg_len-1],reg_len,&val);
		if(reg_len == 1){
			printf("0x%02x \n",val);
		}

		if(reg_len == 2){
			printf("0x%04x \n",val);
		}

		if(reg_len == 4){
			printf("0x%08x \n",val);
		}
	}
	return 0;
}

/***************************************************/

int spi_tst_cfg (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char  *cp = 0;

	if(argc != 3){
		return -1;
	}

	speed = simple_strtoul(argv[1],&cp,10);
	mode  = simple_strtoul(argv[2],&cp,10);

	return 0;
}

int spi_tst_xfer (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	char  *cp = 0;
	uchar tmp;
	int   j;

	/*
	 * We use the last specified parameters, unless new ones are
	 * entered.
	 */

	if ((flag & CMD_FLAG_REPEAT) == 0)
	{
			bus = simple_strtoul(argv[1], &cp, 10);
			cs =  simple_strtoul(argv[2], &cp, 10);
			bitlen = simple_strtoul(argv[3], NULL, 10);
			cp = argv[4];
			for(j = 0; *cp; j++, cp++) {
				tmp = *cp - '0';
				if(tmp > 9)
					tmp -= ('A' - '0') - 10;
				if(tmp > 15)
					tmp -= ('a' - 'A');
				if(tmp > 15) {
					printf("Hex conversion error on %c\n", *cp);
					return 1;
				}
				if((j % 2) == 0)
					dout[j / 2] = (tmp << 4);
				else
					dout[j / 2] |= tmp;
			}
	}

	if ((bitlen < 0) || (bitlen >  (MAX_SPI_BYTES * 8))) {
		printf("Invalid bitlen %d\n", bitlen);
		return -1;
	}

	if (do_spi_xfer(bus, cs,1))
		return -1;

	return 0;
}

U_BOOT_CMD(
	sspi,	5,	1,	do_spi,
	"SPI utility command",
	"[<bus>:]<cs>[.<mode>] <bit_len> <dout> - Send and receive bits\n"
	"<bus>     - Identifies the SPI bus\n"
	"<cs>      - Identifies the chip select\n"
	"<mode>    - Identifies the SPI mode to use\n"
	"<bit_len> - Number of bits to send (base 10)\n"
	"<dout>    - Hexadecimal string that gets sent"
);

U_BOOT_CMD(
	spi_tst_xfer,	5,	1,	spi_tst_xfer,
	"do spi xfer op",
	"spi_tst_xfer bus(n) cs(n) bits_len(nbyte*8) data(e.g:aabbccddee...)\n"
);

U_BOOT_CMD(
	spi_tst_cfg,	3,	1,	spi_tst_cfg,
	"config spi speed and mode",
    "spi_tst_cfg speed(hz) mode(0/1/2/3)\n"
);

U_BOOT_CMD(
	ate_spi,	5,	1,	do_ate_spi,
	"ate_spi test command",
	ate_cmd_help
);
