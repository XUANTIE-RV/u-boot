/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#include "spi.h"
#include "spi_internal.h"

/*
* Enable spi, and config the spi by default:
*     baudrate -
*     transfer mode - transmit&receive
*     serial clock polarity - low
*     serial clock phase - middle
* Set the SPI irqhandler and register it into interrupt controller. Disable
* the interrupts. Set the opening sign in CK_SPIInfo.
* Parameters:
*   id - SPI ID
*   handler -
*   bfast -
* return: SUCCESS or FAILURE.
*/
s32 spi_open(
    enum_spi_device_t spiid
)
{
	volatile pckstruct_spi_reg_t reg;

	reg = (pckstruct_spi_reg_t)(spiid);
	/* It is impossible to write to this register when the SSI is enabled.*/
	reg->SPIENR = CK_SPI_DISABLE;
	reg->IMR = CK_SPI_INT_DISABLE;
	/* config the spi defaultly as below.*/
	spi_set_mode(spiid, CK_SPI_EERX);
	spi_set_polarity(spiid, CK_SPI_CLOCK_POLARITY_HIGH);
	spi_set_phase(spiid, CK_SPI_CLOCK_PHASE_START);
	spi_set_datawidth(spiid, CK_SPI_DataSize_8);
	spi_set_transfer_threshold(spiid, 0x8, 0xa);
	spi_set_baudrate(spiid, 50);

	return SUCCESS;
}

/*
* Unable SPI, also, unregiste the SPI irq. Clear the opening sign.
* Parameters:
*   id - SPI ID
* return: SUCCESS or FAILURE.
*/
s32 spi_close(enum_spi_device_t spiid)
{
	volatile pckstruct_spi_reg_t reg;

	reg = (pckstruct_spi_reg_t)(spiid);
	reg->SPIENR = CK_SPI_DISABLE;     /* disable the spi */
	return SUCCESS;
}

/*
* Set the transfer mode of spi.
* Parameters:
*   id - SPI ID
*   mode - has three modes: transmit&receive, transmit only, and receive only.
* return: SUCCESS or FAILURE.
*/
s32 spi_set_mode(
    enum_spi_device_t spiid,
    enum_spi_mode_t mode
)
{
	volatile pckstruct_spi_reg_t reg;

	reg = (pckstruct_spi_reg_t)(spiid);
	/* It is impossible to write to this register when the SSI is enabled.*/
	/* we can set the TMOD to config transfer mode as below:
	*     TMOD_BIT9  TMOD_BIT8      transfer mode
	*         0          0         transmit & receive
	*         0          1           transmit only
	*         1          0           receive only
	*         1          1             reserved
	*/
	switch (mode) {
	case CK_SPI_TXRX:
		reg->CTRLR0 &= (~CK_SPI_TMOD_BIT8);
		reg->CTRLR0 &= (~CK_SPI_TMOD_BIT9);
		break;

	case CK_SPI_TX:
		reg->CTRLR0 |= CK_SPI_TMOD_BIT8;
		reg->CTRLR0 &= (~CK_SPI_TMOD_BIT9);
		break;

	case CK_SPI_RX:
		reg->CTRLR0 &= (~CK_SPI_TMOD_BIT8);
		reg->CTRLR0 |= CK_SPI_TMOD_BIT9;
		break;

	default:
		reg->CTRLR0 |= CK_SPI_TMOD_BIT8;
		reg->CTRLR0 |= CK_SPI_TMOD_BIT9;
		break;
	}
	return SUCCESS;
}


/*
* Set the serial clock polarity.
* Parameters:
*   id - SPI ID
*   polarity - serial clock polarity.
* return: SUCCESS or FAILURE.
*/
s32 spi_set_polarity(
    enum_spi_device_t spiid,
    enum_spi_polarity_t polarity
)
{
	volatile pckstruct_spi_reg_t reg;

	reg = (pckstruct_spi_reg_t)(spiid);
	/* To config the polarity, we can set the SCPOL bit(CTRLR0[7]) as below:
	*     0 - inactive state of serial clock is low
	*     1 - inactive state of serial clock is high
	*/
	switch (polarity) {
	case CK_SPI_CLOCK_POLARITY_LOW:
		reg->CTRLR0 &= (~CK_SPI_POLARITY);
		break;

	case CK_SPI_CLOCK_POLARITY_HIGH:
		reg->CTRLR0 |= CK_SPI_POLARITY;
		break;

	default:
		break;
	}
	return SUCCESS;
}

/*
* Set serial clock phase. When SCPH(CTRLR0[6])=0, data are captured on the
* first edge of the serial clock. When SCPH=1, the serial clock starts
* toggling one cycle after the slave select line is activated, and data
* are capture one the second edge of the serial clock.
* Parameters:
*   id - SPI ID
*   phase - serial clock phase
* return: SUCCESS or FAILURE.
*/
s32 spi_set_phase(
    enum_spi_device_t spiid,
    enum_spi_phase_t phase
)
{
	volatile pckstruct_spi_reg_t reg;

	reg = (pckstruct_spi_reg_t)(spiid);
	switch (phase) {
	case CK_SPI_CLOCK_PHASE_MIDDLE:
		reg->CTRLR0 &= (~CK_SPI_PHASE);
		break;

	case CK_SPI_CLOCK_PHASE_START:
		reg->CTRLR0 |= CK_SPI_PHASE;
		break;

	default:
		break;
	}
	return SUCCESS;
}

/*
* Set the data frame size. The data size is determined by DFS(CTRLR0[3:0])
* as below:
*              DFS Value               Description
*               0011                  4-bit serial data transfer
*               0100                  5-bit serial data transfer
*               ...                       ...
*               1111                  16-bit serial data transfer
* and the 0000 to 0010 are reserved.
* Parameters:
*   id - SPI ID
*   datawidth - data frame size
* return: SUCCESS or FAILURE.
*/
s32 spi_set_datawidth(
    enum_spi_device_t spiid,
    enum_spi_datawidth_t datawidth
)
{
	volatile pckstruct_spi_reg_t reg;
	u16 temp;

	reg = (pckstruct_spi_reg_t)(spiid);
	temp = reg->CTRLR0;
	temp &= 0xfff0;  /* temp has the value of CTRLR0 with DFS being cleared.*/
	temp |= (datawidth);/* get the final CTRLR0 after datawidth config. */
	reg->CTRLR0 = temp;  /* write CTRLR0 */
	return SUCCESS;
}

/* Set the baudrate. The frequency of the sclk_out is derived from the
* following equation:
*   fsclk_out = fssi_clk / SCKDV
* where SCKDV is any even value between 2 and 65534.
* Parameters:
*   id - SPI ID
*   baudrate - transfer speed to be set
* return: SUCCESS or FAILURE.
*/
s32 spi_set_baudrate(
    enum_spi_device_t spiid,
    u32 baudrate
)
{
	volatile pckstruct_spi_reg_t reg;
	reg = (pckstruct_spi_reg_t)(spiid);
	reg->BAUDR =  baudrate;
	return SUCCESS;
}

u8 spi_transfer_query(
    enum_spi_device_t spiid,
    enum_spi_slave_t slave,
    u8 *ptx_buffer,
    u8 tx_length,
    u8 *prx_buffer,
    u32 rx_length
)
{
	volatile u32 i;
	volatile pckstruct_spi_reg_t reg;

	reg = (pckstruct_spi_reg_t)(spiid);
	reg->SPIENR = CK_SPI_ENABLE; /* enable SPI */
	for (i = 0; i < tx_length; i++) { /* transmit datas in transmit-buffer */
		reg->DR = *ptx_buffer;
		ptx_buffer++;
	}
	reg->SER = slave;   /* disable all cs */
	while ((reg->SR & 0x4) == 0);

	for (i = 0; i < rx_length; i++) {
		while ((reg->SR & 0x8) == 0);
		*prx_buffer = reg->DR;
		prx_buffer++;
	}
	reg->SER = 0;
	reg->SPIENR = CK_SPI_DISABLE;
	return SUCCESS;
}

/*
* Disable the corresponding SPI, and release the slave that has been selected.
* Parameters;
*   id - SPI ID
* return:
*/
s32 spi_disable(enum_spi_device_t spiid)
{
	volatile pckstruct_spi_reg_t reg;

	reg = (pckstruct_spi_reg_t)(spiid);
	reg->SPIENR = CK_SPI_DISABLE;

	return SUCCESS;
}

s32 spi_enable(enum_spi_device_t spiid)
{
	volatile pckstruct_spi_reg_t reg;

	reg = (pckstruct_spi_reg_t)(spiid);
	reg->SPIENR = CK_SPI_ENABLE;

	return SUCCESS;
}


s32 spi_set_ndf(enum_spi_device_t spiid, u32 num)
{
	volatile pckstruct_spi_reg_t reg;
	reg = (pckstruct_spi_reg_t)(spiid);
	reg->SPIENR = 0;
	reg->CTRLR1 = num;

	return SUCCESS;
}



u8 spi_enable_slave(enum_spi_device_t spiid, enum_spi_slave_t slave)
{
	volatile pckstruct_spi_reg_t reg;
	reg = (pckstruct_spi_reg_t)(spiid);

	reg->SER |= slave;

	return SUCCESS;
}

u8 spi_disable_slave(enum_spi_device_t spiid, enum_spi_slave_t slave)
{
	volatile pckstruct_spi_reg_t reg;
	reg = (pckstruct_spi_reg_t)(spiid);

	reg->SER &= ~slave;

	return SUCCESS;
}


u32 spi_send_query(
    enum_spi_device_t spiid,
    enum_spi_slave_t slave,
    u8 *ptxbuffer,
    u16 txlength)
{
	u32 i;
	volatile pckstruct_spi_reg_t reg;
	reg = (pckstruct_spi_reg_t)(spiid);
	reg->SPIENR = CK_SPI_ENABLE; /* enable SPI */

	if (txlength > 34) {
		for (i = 0; i < 34; i++) {
			reg->DR = *ptxbuffer;
			ptxbuffer++;
		}
		txlength -= 34;
		reg->SER = slave;
		for (i = 0; i < txlength; i++) { /* transmit datas in transmit-buffer */
			while (reg->TXFLR >= 30);
			reg->DR = *ptxbuffer;
			ptxbuffer++;
		}
		while ((reg->SR & CK_SPI_BUSY));
	}

	else {
		for (i = 0; i < txlength; i++) { /* transmit datas in transmit-buffer */
			while (reg->TXFLR >= 30);
			reg->DR = *ptxbuffer;
			ptxbuffer++;
		}
		reg->SER = slave;
		while ((reg->SR & 0x04) == 0);
		while ((reg->SR & CK_SPI_BUSY));
	}
	reg->SER = 0;
	reg->SPIENR = CK_SPI_DISABLE;
	return SUCCESS;
}


u8 spi_set_transfer_threshold(enum_spi_device_t spiid, u8 txftlr, u8 rxftlr)
{
	volatile pckstruct_spi_reg_t reg;
	reg = (pckstruct_spi_reg_t)(spiid);
	reg->RXFTLR = rxftlr;
	reg->TXFTLR = txftlr;

	return SUCCESS;
}

