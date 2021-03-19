// SPDX-License-Identifier: GPL-2.0
/*
 * Designware master SPI core controller driver
 *
 * Copyright (C) 2020 linghui.zlh <linghui.zlh@alibaba-inc.com>
 *
 * Very loosely based on the Linux driver:
 * drivers/spi/spi-dw.c, which is:
 * Copyright (c) 2009, Intel Corporation.
 */

#include <common.h>
#include <asm-generic/gpio.h>
#include <clk.h>
#include <dm.h>
#include <errno.h>
#include <malloc.h>
#include <spi.h>
#include <spi-mem.h>
#include <dm.h>
#include <fdtdec.h>
#include <reset.h>
#include <linux/compat.h>
#include <linux/iopoll.h>
#include <asm/io.h>

/* Register offsets */
#define DW_SPI_CTRL0                   0x00
#define DW_SPI_CTRL1                   0x04
#define DW_SPI_SSIENR                  0x08
#define DW_SPI_MWCR                            0x0c
#define DW_SPI_SER                         0x10
#define DW_SPI_BAUDR                   0x14
#define DW_SPI_TXFLTR                  0x18
#define DW_SPI_RXFLTR                  0x1c
#define DW_SPI_TXFLR                   0x20
#define DW_SPI_RXFLR                   0x24
#define DW_SPI_SR                          0x28
#define DW_SPI_IMR                         0x2c
#define DW_SPI_ISR                         0x30
#define DW_SPI_RISR                        0x34
#define DW_SPI_TXOICR                  0x38
#define DW_SPI_RXOICR                  0x3c
#define DW_SPI_RXUICR                  0x40
#define DW_SPI_MSTICR                  0x44
#define DW_SPI_ICR                         0x48
#define DW_SPI_DMACR                   0x4c
#define DW_SPI_DMATDLR                 0x50
#define DW_SPI_DMARDLR                 0x54
#define DW_SPI_IDR                         0x58
#define DW_SPI_VERSION                 0x5c
#define DW_SPI_DR                          0x60
#define DW_SPI_RX_SAMPLE_DLY    0xf0
#define DW_SPI_SPI_CTRLR0       0xf4

/* Bit fields in CTRLR0 */
#define SPI_DFS32_OFFSET        16
#define SPI_DFS32_MASK          (0x1f<<SPI_DFS32_OFFSET)
#define SPI_FRF_OFFSET                 4
#define SPI_FRF_SPI                        0x0
#define SPI_FRF_SSP                        0x1
#define SPI_FRF_MICROWIRE              0x2
#define SPI_FRF_RESV                   0x3
#define SPI_MODE_OFFSET                        6
#define SPI_SCPH_OFFSET                        6
#define SPI_SCOL_OFFSET                        7
#define SPI_TMOD_OFFSET                        8
#define SPI_TMOD_MASK                  (0x3 << SPI_TMOD_OFFSET)
#define        SPI_TMOD_TR                         0x0         /* xmit & recv */
#define SPI_TMOD_TO                        0x1         /* xmit only */
#define SPI_TMOD_RO                        0x2         /* recv only */
#define SPI_TMOD_EPROMREAD             0x3             /* eeprom read mode */

#define SPI_SLVOE_OFFSET               10
#define SPI_SRL_OFFSET                 11
#define SPI_CFS_OFFSET                 12
#define SPI_SPI_FRF_OFFSET      21
#define SPI_SPI_FRF_MASK        (0x3 << SPI_SPI_FRF_OFFSET)
#define SPI_SPI_FRF_STD         0x0
#define SPI_SPI_FRF_DUAL        0x1
#define SPI_SPI_FRF_QUAD        0x2
#define SPI_SPI_FRF_OCTA        0x3

#define SPI_SSTE_OFFSET         24
#define SPI_SSTE_MASK           (1 << SPI_SSTE_OFFSET )

/* Bit fields in SR, 7 bits */
#define SR_MASK                                    GENMASK(6, 0)       /* cover 7 bits */
#define SR_BUSY                                    (1<<0)//BIT(0)
#define SR_TF_NOT_FULL                 BIT(1)
#define SR_TF_EMPT                         BIT(2)
#define SR_RF_NOT_EMPT                 BIT(3)
#define SR_RF_FULL                         BIT(4)
#define SR_TX_ERR                          BIT(5)
#define SR_DCOL                                    BIT(6)

/* Bit fields in ISR, IMR, RISR, 7 bits */
#define SPI_INT_TXEI                   (1 << 0)   /* tx fifo empty */
#define SPI_INT_TXOI                   (1 << 1)   /* tx fifo overflow */
#define SPI_INT_RXUI                   (1 << 2)   /* rx fifo underfolow */
#define SPI_INT_RXOI                   (1 << 3)   /* rx fifo overflow */
#define SPI_INT_RXFI                   (1 << 4)   /* rx fifo full  */
#define SPI_INT_MSTI                   (1 << 5)   /* multi-master contention interrupt, set only when configured as slave device */

/* Bit fields in SPI_CTRLR0 */
#define SPI_CTRLR0_TRNAS_OFFET       0x0
#define SPI_CTRLR0_TRANS_ISTD_ASTD   (0)       /* both instr and address are sent in stdandard mode */
#define SPI_CTRLR0_TRANS_ISTD_ASPF   (1)       /* instr sent in standard mode , address sent in the mode specified by CTRLR0.SPI_FRF */
#define SPI_CTRLR0_TRANS_ISPF_ASPF   (2)       /* both instr and address sent  in the mode specified by CTRLR0.SPI_FRF */

#define SPI_CTRLR0_ADDR_L_OFFSET     0x2

#define SPI_CTRLR0_INST_L_OFFSET     0x8
#define SPI_CTRLR0_INST_L_0          (0)
#define SPI_CTRLR0_INST_L_4          (1)
#define SPI_CTRLR0_INST_L_8          (2)
#define SPI_CTRLR0_INST_L_16         (3)

#define SPI_CTRLR0_WAIT_CYCLES_OFFSET (11)     /* count in io clks */

#define SPI_CTRLR0_DDR_EN_OFFSET     (16)      /* enable dual-ata rate transfers in dual/quad/octal frame formats of spi */
#define SPI_CTRLR0_DDR_EN_MASK       (1 << SPI_CTRLR0_DDR_EN_OFFSET)

#define SPI_CTRLR0_INST_DDR_EN_OFFSET (17)
#define SPI_CTRLR0_INST_DDR_EN_MASK   (1 << SPI_CTRLR0_INST_DDR_EN_OFFSET

#define SPI_CTRLR0_RXDS_EN_OFFSET    (18)
#define SPI_CTRLR0_RXDS_EN_MASK      (1 << SPI_CTRLR0_RXDS_EN_OFFSET)

#define RX_TIMEOUT                      1000           /* timeout in ms */

struct dw_qspi_platdata {
	s32 frequency;          /* Default clock frequency, -1 for none */
	void __iomem *regs;
};

struct xfer_pre_t {
#define       DW_MAX_CMD_BUF_LEN     32
	u8        xfer_pre[DW_MAX_CMD_BUF_LEN];
	u32       xfer_pre_len;
};

struct dw_qspi_priv {
	void __iomem *regs;
	unsigned int freq;              /* Default frequency */
	unsigned int mode;
	struct clk clk;
	unsigned long bus_clk_rate;

	struct gpio_desc cs_gpio;       /* External chip-select gpio */
	/* used by spi_controller_mem_ops interface */
	struct xfer_pre_t xfer_data_pre;
	u8 cs;                  /* chip select pin */
	u8 tmode;               /* TR/TO/RO/EEPROM */
	u8 type;                /* SPI/SSP/MicroWire */
	u8 n_bytes;     /*  bytes per-word  */
	int len;

	u32 fifo_len;           /* depth of the FIFO buffer */
	void *tx;
	void *tx_end;
	void *rx;
	void *rx_end;

	struct reset_ctl_bulk   resets;
};

static inline u32 dw_read(struct dw_qspi_priv *priv, u32 offset)
{
	return __raw_readl(priv->regs + offset);
}

static inline void dw_write(struct dw_qspi_priv *priv, u32 offset, u32 val)
{
	__raw_writel(val, priv->regs + offset);
}

static int request_gpio_cs(struct udevice *bus)
{
#if defined(CONFIG_DM_GPIO) && !defined(CONFIG_SPL_BUILD)
	struct dw_qspi_priv *priv = dev_get_priv(bus);
	int ret;

	/* External chip select gpio line is optional */
	ret = gpio_request_by_name(bus, "cs-gpio", 0, &priv->cs_gpio, 0);
	if (ret == -ENOENT)
		return 0;

	if (ret < 0) {
		debug("Error: %d: Can't get %s gpio!\n", ret, bus->name);
		return ret;
	}

	if (dm_gpio_is_valid(&priv->cs_gpio)) {
		dm_gpio_set_dir_flags(&priv->cs_gpio,
		                      GPIOD_IS_OUT | GPIOD_IS_OUT_ACTIVE);
	}

	debug("%s: used external gpio for CS management\n", __func__);
#endif
	return 0;
}

static int dw_qspi_ofdata_to_platdata(struct udevice *bus)
{
	struct dw_qspi_platdata *plat = bus->platdata;

	plat->regs = (struct dw_qspi *)devfdt_get_addr(bus);

	/* Use 500KHz as a suitable default */
	plat->frequency = dev_read_u32_default(bus, "spi-max-frequency",
	                                       1000000);
	debug("%s: regs=%p max-frequency=%d\n", __func__, plat->regs,
	      plat->frequency);

	return request_gpio_cs(bus);
}

static inline void spi_enable_chip(struct dw_qspi_priv *priv, int enable)
{
	dw_write(priv, DW_SPI_SSIENR, (enable ? 1 : 0));
}

/* Restart the controller, disable all interrupts, clean rx fifo */
static void spi_hw_init(struct dw_qspi_priv *priv)
{
	spi_enable_chip(priv, 0);
	dw_write(priv, DW_SPI_IMR, 0xff);
	dw_write(priv, DW_SPI_SER, 0xf);
	spi_enable_chip(priv, 1);

	/*
	 * Try to detect the FIFO depth if not set by interface driver,
	 * the depth could be from 2 to 256 from HW spec
	 */
	if (!priv->fifo_len) {
		u32 fifo;

		for (fifo = 1; fifo < 256; fifo++) {
			dw_write(priv, DW_SPI_TXFLTR, fifo);
			if (fifo != dw_read(priv, DW_SPI_TXFLTR))
				break;
		}

		priv->fifo_len = (fifo == 1) ? 0 : fifo;
		dw_write(priv, DW_SPI_TXFLTR, 0);
	}
	debug("%s: fifo_len=%d\n", __func__, priv->fifo_len);
}

/*
 * We define dw_qspi_get_clk function as 'weak' as some targets
 * (like SOCFPGA_GEN5 and SOCFPGA_ARRIA10) don't use standard clock API
 * and implement dw_qspi_get_clk their own way in their clock manager.
 */
__weak int dw_qspi_get_clk(struct udevice *bus, ulong *rate)
{
	struct dw_qspi_priv *priv = dev_get_priv(bus);
	int ret;

	ret = clk_get_by_index(bus, 0, &priv->clk);
	if (ret)
		return ret;

	ret = clk_enable(&priv->clk);
	if (ret && ret != -ENOSYS && ret != -ENOTSUPP)
		return ret;

	*rate = clk_get_rate(&priv->clk);
	if (!*rate)
		goto err_rate;

	debug("%s: get spi controller clk via device tree: %lu Hz\n",
	      __func__, *rate);

	return 0;

err_rate:
	clk_disable(&priv->clk);
	clk_free(&priv->clk);

	return -EINVAL;
}

static int dw_qspi_reset(struct udevice *bus)
{
	int ret;
	struct dw_qspi_priv *priv = dev_get_priv(bus);

	ret = reset_get_bulk(bus, &priv->resets);
	if (ret) {
		/*
		 * Return 0 if error due to !CONFIG_DM_RESET and reset
		 * DT property is not present.
		 */
		if (ret == -ENOENT || ret == -ENOTSUPP)
			return 0;

		dev_warn(bus, "Can't get reset: %d\n", ret);
		return ret;
	}

	ret = reset_deassert_bulk(&priv->resets);
	if (ret) {
		reset_release_bulk(&priv->resets);
		dev_err(bus, "Failed to reset: %d\n", ret);
		return ret;
	}

	return 0;
}

static int dw_qspi_probe(struct udevice *bus)
{
	struct dw_qspi_platdata *plat = dev_get_platdata(bus);
	struct dw_qspi_priv *priv = dev_get_priv(bus);
	int ret;

	priv->regs = plat->regs;
	priv->freq = plat->frequency;

	ret = dw_qspi_get_clk(bus, &priv->bus_clk_rate);
	if (ret)
		return ret;

	ret = dw_qspi_reset(bus);
	if (ret)
		return ret;

	priv->n_bytes       = 1;
	priv->tmode = SPI_TMOD_TO; /* Tx & Rx */

	/* Basic HW init */
	spi_hw_init(priv);

	return 0;
}

/* Return the max entries we can fill into tx fifo */
static inline u32 tx_max(struct dw_qspi_priv *priv)
{
	u32 tx_left, tx_room, rxtx_gap;

	tx_left = (priv->tx_end - priv->tx) / (priv->n_bytes);
	tx_room = priv->fifo_len - dw_read(priv, DW_SPI_TXFLR);

	/*
	 * Another concern is about the tx/rx mismatch, we
	 * thought about using (priv->fifo_len - rxflr - txflr) as
	 * one maximum value for tx, but it doesn't cover the
	 * data which is out of tx/rx fifo and inside the
	 * shift registers. So a control from sw point of
	 * view is taken.
	 */
	rxtx_gap = ((priv->rx_end - priv->rx) - (priv->tx_end - priv->tx)) /
	           (priv->n_bytes);

	return min3(tx_left, tx_room, (u32)(priv->fifo_len - rxtx_gap));
}

/* Return the max entries we should read out of rx fifo */
static inline u32 rx_max(struct dw_qspi_priv *priv)
{
	u32 rx_left = (priv->rx_end - priv->rx) / (priv->n_bytes);

	return min_t(u32, rx_left, dw_read(priv, DW_SPI_RXFLR));
}

static void dw_writer(struct dw_qspi_priv *priv)
{
	u32 max = tx_max(priv);
	volatile u32 txw = 0;

	while (max--) {
		/* Set the tx word if the transfer's original "tx" is not null */
		if (priv->tx_end - priv->len) {
			if (priv->n_bytes == 1)
				txw = *(u8 *)(priv->tx);
			else if (priv->n_bytes == 2)
				txw = *(u16 *)(priv->tx);
			else
				txw = *(u32 *)(priv->tx);
		}
		dw_write(priv, DW_SPI_DR, txw);
		//debug("%s: tx=0x%02x\n", __func__, txw);
		priv->tx += priv->n_bytes;
	}
}

static void dw_reader(struct dw_qspi_priv *priv)
{
	u32 max = rx_max(priv);
	volatile u32 rxw;

	while (max--) {
		rxw = dw_read(priv, DW_SPI_DR);
		//debug("%s: rx=0x%02x\n", __func__, rxw);

		/* Care about rx if the transfer's original "rx" is not null */
		if (priv->rx_end - priv->len) {
			if (priv->n_bytes == 1)
				*(u8 *)(priv->rx) = rxw;
			else if (priv->n_bytes == 2)
				*(u16 *)(priv->rx) = rxw;
			else {
				*(u32 *)(priv->rx) = rxw;
			}

		}
		priv->rx += priv->n_bytes;
	}
}

/*
 * We define external_cs_manage function as 'weak' as some targets
 * (like MSCC Ocelot) don't control the external CS pin using a GPIO
 * controller. These SoCs use specific registers to control by
 * software the SPI pins (and especially the CS).
 */
__weak void external_cs_manage(struct udevice *dev, bool on)
{
#if defined(CONFIG_DM_GPIO) && !defined(CONFIG_SPL_BUILD)
	struct dw_qspi_priv *priv = dev_get_priv(dev->parent);

	if (!dm_gpio_is_valid(&priv->cs_gpio))
		return;

	dm_gpio_set_value(&priv->cs_gpio, on ? 1 : 0);
#endif
}

static int dw_qspi_wait_bus_idle(struct dw_qspi_priv *priv, u32 timeout_us)
{
	u32 val;
	void __iomem *reg = priv->regs + DW_SPI_SR;
	return readl_poll_timeout(reg, val, !(val & SR_BUSY), timeout_us);
}

static void dw_qspi_build_xfer_pre_portion(struct dw_qspi_priv *priv, const struct spi_mem_op *op)
{
	u32 i = 0, j = 0;

	/* operation code */
	priv->xfer_data_pre.xfer_pre[i++] = op->cmd.opcode;

	/* addr */
	if (op->addr.nbytes) {
		for (j = 0; j < op->addr.nbytes; j++) {
			priv->xfer_data_pre.xfer_pre[i++] = (op->addr.val >> (8 * (op->addr.nbytes - j - 1))) & 0xFF;
		}
	}

	/* dummy */
	if (op->dummy.nbytes) {
		memset(&priv->xfer_data_pre.xfer_pre[i], 0xFF, op->dummy.nbytes);
		i += op->dummy.nbytes;
	}

	priv->xfer_data_pre.xfer_pre_len = i;
	return;
}

static bool dw_qspi_can_xfer_32bits_frame(const struct spi_mem_op *op)
{
	bool ret = false;

	if (op->data.buswidth > 1 && op->data.nbytes && !(op->data.nbytes & 0x3) && !(op->data.nbytes & 0x3)) {
		if (op->data.dir == SPI_MEM_DATA_OUT && !((unsigned long)(op->data.buf.out) & 0x03)) {
			ret = true;
		} else if (op->data.dir == SPI_MEM_DATA_IN && !((unsigned long)(op->data.buf.in) & 0x03)) {
			ret = true;
		}
	}

	return ret;
}

static int dw_qspi_exec_op(struct spi_slave *slave, const struct spi_mem_op *op)
{
	struct udevice      *bus = slave->dev->parent;
	struct dw_qspi_priv  *priv = dev_get_priv(bus);
	u32    cr0 = 0, spi_cr0 = 0;
	u32 addr_bits_len, dummy_bits_len;
	int ret;
	//unsigned long flag;

	/*disable spi core */
	spi_enable_chip(priv, 0);
	/*build pre-xfer data portion*/
	dw_qspi_build_xfer_pre_portion(priv, op);

	/*
	 * default cr0 register setting
	 * SSI_TYPE-----> SPI_FRF_SPI
	 * CPOL&CPHA----> SPI_CPOL=SPI_CPHA = 0
	 * FRAME_SIZE---> 8bits
	 * SPI_TMOD_OFFSET SPI_TMOD_TO
	 */
	cr0 = (priv->type << SPI_FRF_OFFSET) |
	      (0 << SPI_MODE_OFFSET) | \
	      (priv->tmode << SPI_TMOD_OFFSET) | \
	      ((8 - 1) << SPI_DFS32_OFFSET) ;
	priv->tmode = SPI_TMOD_TO;
	if (op->data.dir == SPI_MEM_DATA_OUT || op->data.dir == SPI_MEM_NO_DATA) {
		priv->tmode = SPI_TMOD_TO;
	} else if (op->data.dir == SPI_MEM_DATA_IN && op->data.buswidth > 1) {
		priv->tmode = SPI_TMOD_RO;
		u32 len = op->data.nbytes;
		if (dw_qspi_can_xfer_32bits_frame(op)) {
			len >>= 2;
		}
		dw_write(priv, DW_SPI_CTRL1, len - 1);
	}
	cr0 &= ~SPI_TMOD_MASK;
	cr0 |= (priv->tmode << SPI_TMOD_OFFSET);

	/* init config spi_cr0 if use non-standard spi mode */
	if (op->data.buswidth > 1) {
		/*
		 * trans_type = both instruction and address are sent in standard mode
		 * instruction bits length is 8bits
		 *
		 */
		spi_cr0 = (SPI_CTRLR0_TRANS_ISTD_ASTD << SPI_CTRLR0_TRNAS_OFFET) | \
		          (SPI_CTRLR0_INST_L_8 << SPI_CTRLR0_INST_L_OFFSET);

		addr_bits_len = (op->addr.nbytes << 3) >> 2;
		dummy_bits_len = op->dummy.nbytes << 3;
		spi_cr0 |= (addr_bits_len << SPI_CTRLR0_ADDR_L_OFFSET);
		spi_cr0 |= (dummy_bits_len << SPI_CTRLR0_WAIT_CYCLES_OFFSET);
		dw_write(priv, DW_SPI_SPI_CTRLR0, spi_cr0);


		switch (op->data.buswidth) {
		case 2:
			cr0 |= (SPI_SPI_FRF_DUAL << SPI_SPI_FRF_OFFSET);
			break;
		case 4:
			cr0 |= (SPI_SPI_FRF_QUAD << SPI_SPI_FRF_OFFSET);
		default:
			break;
		}
	}

	/*check whether can xfer through 32bit-frame size, then update cr0 */
	if (dw_qspi_can_xfer_32bits_frame(op)) {
		cr0 &= ~SPI_DFS32_MASK;
		cr0 |= ((32 - 1) << SPI_DFS32_OFFSET);
	}
	dw_write(priv, DW_SPI_CTRL0, cr0);
	/*  for poll mode just disable all interrupts */
	external_cs_manage(slave->dev, false);
	dw_write(priv, DW_SPI_IMR, 0xff);
	printf("cmd:0x%02x  cr0 %08x\n", op->cmd.opcode, cr0);
	debug("#1:cr0 %08x cr1 %08x spi_cr0 %08x \n", dw_read(priv, DW_SPI_CTRL0), dw_read(priv, DW_SPI_CTRL1), dw_read(priv, DW_SPI_SPI_CTRLR0));
	/* transfer data_pre portion(cmd+addr+dummy) */
	spi_enable_chip(priv, 1);

	/* pre portion is sent with spi-standard mode and data-frame size is 8bit frame */
	priv->n_bytes = 1;
	if (op->data.nbytes && op->data.buswidth > 1) {
		/*!!!note: in quad mode, dw-ssi can't be interrupt during sending pre portion and data portion
		 * otherwise, the timing won't be expected
		 */
		//local_irq_save(flag);
		dw_write(priv, DW_SPI_DR, op->cmd.opcode);
		if (op->addr.nbytes) {
			dw_write(priv, DW_SPI_DR, op->addr.val);
		}
	} else {
		priv->tx = (void *)priv->xfer_data_pre.xfer_pre;
		priv->tx_end = priv->tx  + priv->xfer_data_pre.xfer_pre_len;
		priv->len = priv->xfer_data_pre.xfer_pre_len;
		do {
			dw_writer(priv);
		} while (priv->tx_end > priv->tx);
	}

	/* transfer data portion if needs */
	if (op->data.dir == SPI_MEM_DATA_OUT) {
		priv->tx = (void*) op->data.buf.out;
		priv->tx_end = priv->tx + op->data.nbytes;
		priv->len = op->data.nbytes;
		if (dw_qspi_can_xfer_32bits_frame(op)) {
			priv->n_bytes = 4;
		}
		do {
			dw_writer(priv);
		} while (priv->tx_end > priv->tx);
		if (op->data.nbytes && op->data.buswidth > 1) {
			//local_irq_restore(flag);
		}
	} else if (op->data.dir == SPI_MEM_DATA_IN) {
		/* if data portion use standard mode,pre transfer mode is tx, need set rx mode  */
		if (op->data.buswidth == 1) {
			ret = dw_qspi_wait_bus_idle(priv, 10000);
			if (ret) {
				debug("wait bus idle time out \n");
				return ret;
			}
			spi_enable_chip(priv, 0);
			cr0 = dw_read(priv, DW_SPI_CTRL0);
			cr0 &= ~SPI_TMOD_MASK;
			cr0 |= (SPI_TMOD_RO << SPI_TMOD_OFFSET);
			dw_write(priv, DW_SPI_CTRL0, cr0);
			dw_write(priv, DW_SPI_CTRL1, op->data.nbytes - 1);
			debug("#2:cr0 %08x cr1 %08x spi_cr0 %08x \n", dw_read(priv, DW_SPI_CTRL0), dw_read(priv, DW_SPI_CTRL1), dw_read(priv, DW_SPI_SPI_CTRLR0));
			spi_enable_chip(priv, 1);
			dw_write(priv, DW_SPI_DR, 0);
			priv->rx = op->data.buf.in;
			priv->rx_end = priv->rx + op->data.nbytes;
			priv->len = op->data.nbytes;
			do {
				dw_reader(priv);
				//cpu_relax();
			} while (priv->rx_end > priv->rx);
		}
		/* non-standard mode */
		else {

			priv->rx = op->data.buf.in;
			priv->rx_end = priv->rx + op->data.nbytes;
			priv->len = op->data.nbytes;
			if (dw_qspi_can_xfer_32bits_frame(op)) {
				priv->n_bytes = 4;
			}
			do {
				dw_reader(priv);
				//cpu_relax();
			} while (priv->rx_end > priv->rx);
			if (op->data.nbytes && op->data.buswidth > 1) {
				//local_irq_restore(flag);
			}
		}

	}

	ret = dw_qspi_wait_bus_idle(priv, 1000);
	if (ret) {
		debug("wait bus idle timeout \n");
		return ret;
	}
	if (dw_read(priv, DW_SPI_RISR) & (SPI_INT_RXOI | SPI_INT_RXUI)) {
		debug("###rx err %02x \n", dw_read(priv, DW_SPI_RISR));
		return -1;
	}
	external_cs_manage(slave->dev, true);
	return 0;
}

static int dw_qspi_set_speed(struct udevice *bus, uint speed)
{
	struct dw_qspi_platdata *plat = bus->platdata;
	struct dw_qspi_priv *priv = dev_get_priv(bus);
	u16 clk_div;
	debug("bus_clk %ld plat->freq %d speed %d \n", priv->bus_clk_rate, plat->frequency, speed);
	/* Disable controller before writing control registers */
	spi_enable_chip(priv, 0);
	(void)(speed);
	/* clk_div doesn't support odd number */
	clk_div = priv->bus_clk_rate / plat->frequency;
	clk_div = (clk_div + 1) & 0xfffe;
	dw_write(priv, DW_SPI_BAUDR, clk_div);

	/* Enable controller after writing control registers */
	spi_enable_chip(priv, 1);

	priv->freq = plat->frequency;
	debug("%s: regs=%p speed=%d clk_div=%d\n", __func__, priv->regs,
	      priv->freq, clk_div);

	return 0;
}

static int dw_qspi_set_mode(struct udevice *bus, uint mode)
{
	struct dw_qspi_priv *priv = dev_get_priv(bus);

	/*
	 * Can't set mode yet. Since this depends on if rx, tx, or
	 * rx & tx is requested. So we have to defer this to the
	 * real transfer function.
	 */
	priv->mode = mode;
	debug("%s: regs=%p, mode=%d\n", __func__, priv->regs, priv->mode);

	return 0;
}

static int dw_qspi_remove(struct udevice *bus)
{
	struct dw_qspi_priv *priv = dev_get_priv(bus);
	int ret;

	ret = reset_release_bulk(&priv->resets);
	if (ret)
		return ret;

#if CONFIG_IS_ENABLED(CLK)
	ret = clk_disable(&priv->clk);
	if (ret)
		return ret;

	ret = clk_free(&priv->clk);
	if (ret)
		return ret;
#endif
	return 0;
}

static int dw_qspi_check_buswidth(u8 width)
{
	switch (width) {
	case 1:
	case 2:
	case 4:
		return 0;
	}

	return -ENOTSUPP;
}
static bool dw_qspi_supports_op(struct spi_slave *slave, const struct spi_mem_op *op)
{
	//struct udevice      *bus = slave->dev->parent;
	//struct dw_qspi_priv  *priv = dev_get_priv(bus);
	int ret = 0;
	u32 temp_len;
	/* check buswidth */
	if (op->cmd.buswidth != 1 || \
	    (op->addr.nbytes && op->addr.buswidth != 1) || \
	    (op->dummy.nbytes && op->dummy.buswidth != 1)) {
		return false;
	}
	if (op->data.nbytes)
		ret |= dw_qspi_check_buswidth(op->data.buswidth);

	if (ret) {
		return false;
	}
	/* check addr bits length */
	temp_len = op->addr.nbytes << 3;
	if (op->data.nbytes && op->data.buswidth > 4 && temp_len > 60) {
		return false;
	}

	return true;
}

static const struct spi_controller_mem_ops dw_qspi_mem_ops = {
	.exec_op = dw_qspi_exec_op,
	.supports_op = dw_qspi_supports_op,
};

static const struct dm_spi_ops dw_qspi_ops = {
	//.xfer         = dw_qspi_xfer,
	.set_speed      = dw_qspi_set_speed,
	.set_mode       = dw_qspi_set_mode,
	.mem_ops    = &dw_qspi_mem_ops,
	/*
	 * cs_info is not needed, since we require all chip selects to be
	 * in the device tree explicitly
	 */
};

static const struct udevice_id dw_qspi_ids[] = {
	{ .compatible = "snps,dw-apb-ssi-quad" },
	{ }
};

U_BOOT_DRIVER(dw_qspi) = {
	.name = "dw_qspi",
	.id = UCLASS_SPI,
	.of_match = dw_qspi_ids,
	.ops = &dw_qspi_ops,
	.ofdata_to_platdata = dw_qspi_ofdata_to_platdata,
	.platdata_auto_alloc_size = sizeof(struct dw_qspi_platdata),
	.priv_auto_alloc_size = sizeof(struct dw_qspi_priv),
	.probe = dw_qspi_probe,
	.remove = dw_qspi_remove,
};
