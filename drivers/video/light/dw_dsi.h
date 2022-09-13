// SPDX-License-Identifier: GPL-2.0+

#ifndef _DW_DSI_H_
#define _DW_DSI_H_

#include <linux/bitops.h>
#include <linux/bitfield.h>

#define DSI_VERSION         0x00

#define DSI_PWR_UP          0x04
#define RESET               0
#define POWERUP             BIT(0)

#define DSI_CLKMGR_CFG          0x08
#define TO_CLK_DIVISION(div)        (((div) & 0xff) << 8)
#define TX_ESC_CLK_DIVISION(div)    ((div) & 0xff)

#define DSI_DPI_VCID            0x0c
#define DPI_VCID(vcid)          ((vcid) & 0x3)

#define DSI_DPI_COLOR_CODING        0x10
#define LOOSELY18_EN            BIT(8)
#define DPI_COLOR_CODING_16BIT_1    0x0
#define DPI_COLOR_CODING_16BIT_2    0x1
#define DPI_COLOR_CODING_16BIT_3    0x2
#define DPI_COLOR_CODING_18BIT_1    0x3
#define DPI_COLOR_CODING_18BIT_2    0x4
#define DPI_COLOR_CODING_24BIT      0x5

#define DSI_DPI_CFG_POL         0x14
#define COLORM_ACTIVE_LOW       BIT(4)
#define SHUTD_ACTIVE_LOW        BIT(3)
#define HSYNC_ACTIVE_LOW        BIT(2)
#define VSYNC_ACTIVE_LOW        BIT(1)
#define DATAEN_ACTIVE_LOW       BIT(0)

#define DSI_DPI_LP_CMD_TIM      0x18
#define OUTVACT_LPCMD_TIME(p)       (((p) & 0xff) << 16)
#define INVACT_LPCMD_TIME(p)        ((p) & 0xff)

#define DSI_DBI_VCID            0x1c
#define DSI_DBI_CFG         0x20
#define DSI_DBI_PARTITIONING_EN     0x24
#define DSI_DBI_CMDSIZE         0x28

#define DSI_PCKHDL_CFG          0x2c
#define EOTP_TX_LP_EN           BIT(5)
#define CRC_RX_EN           BIT(4)
#define ECC_RX_EN           BIT(3)
#define BTA_EN              BIT(2)
#define EOTP_RX_EN          BIT(1)
#define EOTP_TX_EN          BIT(0)

#define DSI_GEN_VCID            0x30

#define DSI_MODE_CFG            0x34
#define ENABLE_VIDEO_MODE       0
#define ENABLE_CMD_MODE         BIT(0)

#define DSI_VID_MODE_CFG        0x38
#define VPG_ORIENTATION         BIT(24)
#define VPG_MODE                BIT(20)
#define VPG_ENABLE              BIT(16)
#define LP_CMD_EN               BIT(15)
#define FRAME_BTA_ACK_EN        BIT(14)
#define LP_HFP_EN               BIT(13)
#define LP_HBP_EN               BIT(12)
#define LP_VACT_EN              BIT(11)
#define LP_VFP_EN               BIT(10)
#define LP_VBP_EN               BIT(9)
#define LP_VSA_EN               BIT(8)
#define VID_MODE_TYPE_NON_BURST_SYNC_PULSES 0x0
#define VID_MODE_TYPE_NON_BURST_SYNC_EVENTS 0x1
#define VID_MODE_TYPE_BURST         0x2
#define VID_MODE_TYPE_MASK          0x3

#define DSI_VID_PKT_SIZE        0x3c
#define VID_PKT_SIZE(p)         ((p) & 0x3fff)

#define DSI_VID_NUM_CHUNKS      0x40
#define VID_NUM_CHUNKS(c)       ((c) & 0x1fff)

#define DSI_VID_NULL_SIZE       0x44
#define VID_NULL_SIZE(b)        ((b) & 0x1fff)

#define DSI_VID_HSA_TIME        0x48
#define DSI_VID_HBP_TIME        0x4c
#define DSI_VID_HLINE_TIME      0x50
#define DSI_VID_VSA_LINES       0x54
#define DSI_VID_VBP_LINES       0x58
#define DSI_VID_VFP_LINES       0x5c
#define DSI_VID_VACTIVE_LINES       0x60
#define DSI_EDPI_CMD_SIZE       0x64

#define DSI_CMD_MODE_CFG        0x68
#define MAX_RD_PKT_SIZE_LP      BIT(24)
#define DCS_LW_TX_LP            BIT(19)
#define DCS_SR_0P_TX_LP         BIT(18)
#define DCS_SW_1P_TX_LP         BIT(17)
#define DCS_SW_0P_TX_LP         BIT(16)
#define GEN_LW_TX_LP            BIT(14)
#define GEN_SR_2P_TX_LP         BIT(13)
#define GEN_SR_1P_TX_LP         BIT(12)
#define GEN_SR_0P_TX_LP         BIT(11)
#define GEN_SW_2P_TX_LP         BIT(10)
#define GEN_SW_1P_TX_LP         BIT(9)
#define GEN_SW_0P_TX_LP         BIT(8)
#define ACK_RQST_EN         BIT(1)
#define TEAR_FX_EN          BIT(0)

#define CMD_MODE_ALL_LP         (MAX_RD_PKT_SIZE_LP | \
                     DCS_LW_TX_LP | \
                     DCS_SR_0P_TX_LP | \
                     DCS_SW_1P_TX_LP | \
                     DCS_SW_0P_TX_LP | \
                     GEN_LW_TX_LP | \
                     GEN_SR_2P_TX_LP | \
                     GEN_SR_1P_TX_LP | \
                     GEN_SR_0P_TX_LP | \
                     GEN_SW_2P_TX_LP | \
                     GEN_SW_1P_TX_LP | \
                     GEN_SW_0P_TX_LP)

#define DSI_GEN_HDR         0x6c
#define DSI_GEN_PLD_DATA        0x70

#define DSI_CMD_PKT_STATUS      0x74
#define GEN_RD_CMD_BUSY         BIT(6)
#define GEN_PLD_R_FULL          BIT(5)
#define GEN_PLD_R_EMPTY         BIT(4)
#define GEN_PLD_W_FULL          BIT(3)
#define GEN_PLD_W_EMPTY         BIT(2)
#define GEN_CMD_FULL            BIT(1)
#define GEN_CMD_EMPTY           BIT(0)

#define DSI_TO_CNT_CFG          0x78
#define HSTX_TO_CNT(p)          (((p) & 0xffff) << 16)
#define LPRX_TO_CNT(p)          ((p) & 0xffff)

#define DSI_HS_RD_TO_CNT        0x7c
#define DSI_LP_RD_TO_CNT        0x80
#define DSI_HS_WR_TO_CNT        0x84
#define DSI_LP_WR_TO_CNT        0x88
#define DSI_BTA_TO_CNT          0x8c

#define DSI_LPCLK_CTRL          0x94
#define AUTO_CLKLANE_CTRL       BIT(1)
#define PHY_TXREQUESTCLKHS      BIT(0)

#define DSI_PHY_TMR_LPCLK_CFG       0x98
#define PHY_CLKHS2LP_TIME	GENMASK(25, 16)
#define PHY_CLKLP2HS_TIME	GENMASK( 9,  0)

#define DSI_PHY_TMR_CFG         0x9c
#define PHY_HS2LP_TIME		GENMASK(25, 16)
#define PHY_LP2HS_TIME		GENMASK( 9,  0)

#define DSI_PHY_RSTZ            0xa0
#define PHY_DISFORCEPLL         0
#define PHY_ENFORCEPLL          BIT(3)
#define PHY_DISABLECLK          0
#define PHY_ENABLECLK           BIT(2)
#define PHY_RSTZ            0
#define PHY_UNRSTZ          BIT(1)
#define PHY_SHUTDOWNZ           0
#define PHY_UNSHUTDOWNZ         BIT(0)

#define DSI_PHY_IF_CFG          0xa4
#define PHY_STOP_WAIT_TIME	GENMASK(15,  8)
#define N_LANES			GENMASK( 1,  0)

#define DSI_PHY_ULPS_CTRL       0xa8
#define DSI_PHY_TX_TRIGGERS     0xac

#define DSI_PHY_STATUS          0xb0
#define PHY_STOP_STATE_CLK_LANE     BIT(2)
#define PHY_LOCK            BIT(0)

#define DSI_PHY_TST_CTRL0	0xb4
#define PHY_TST_CTRL0_TESTCLK	BIT(1)
#define PHY_TST_CTRL0_TESTCLR	BIT(0)

#define DSI_PHY_TST_CTRL1	0xb8
#define PHY_TST_CTRL1_TESTEN	BIT(16)
    #define TESTEN_WRITEADDR	1
    #define TESTEN_WRITEDATA	0
#define PHY_TST_CTRL1_TESTOUT	GENMASK(15,  8)
#define PHY_TST_CTRL1_TESTIN	GENMASK( 7,  0)

#define DSI_INT_ST0         0xbc
#define DSI_INT_ST1         0xc0
#define DSI_INT_MSK0            0xc4
#define DSI_INT_MSK1            0xc8

#define DSI_PHY_TMR_RD_CFG      0xf4
#define MAX_RD_TIME		GENMASK(14,  0)

#define PHY_STATUS_TIMEOUT_US       25
#define CMD_PKT_STATUS_TIMEOUT_US   25

#define MAX_LANE_COUNT          4

#endif
