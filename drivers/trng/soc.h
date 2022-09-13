/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     soc.h
 * @brief    CSI Core Peripheral Access Layer Header File for
 *           CSKYSOC Device Series
 * @version  V1.0
 * @date     7. April 2020
 ******************************************************************************/

#ifndef _SOC_H_
#define _SOC_H_

#include <stdint.h>
#include "csi_core.h"
#include "sys_clk.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EHS_VALUE
#define EHS_VALUE               20000000U
#endif

#ifndef ELS_VALUE
#define ELS_VALUE               32768U
#endif

#ifndef IHS_VALUE
#define IHS_VALUE               50000000U
#endif

#ifndef ILS_VALUE
#define ILS_VALUE               32768U
#endif

typedef enum {
    Supervisor_Software_IRQn        =  1U,
    Machine_Software_IRQn           =  3U,
    Supervisor_Timer_IRQn           =  5U,
    CORET_IRQn                      =  7U,
    Supervisor_External_IRQn        =  9U,
    Machine_External_IRQn           =  11U,
    DW_TIMER0_IRQn                  =  16U,
    DW_TIMER1_IRQn                  =  17U,
    DW_TIMER2_IRQn                  =  18U,
    DW_TIMER3_IRQn                  =  19U,
    DW_TIMER4_IRQn                  =  20U,
    DW_TIMER5_IRQn                  =  21U,
    DW_TIMER6_IRQn                  =  22U,
    DW_TIMER7_IRQn                  =  23U,
    DW_GPIO2_IRQn                   =  24U,
    WJ_MBOX_IRQn                    =  28U,
    DW_UART0_IRQn                   =  36U,
    DW_UART1_IRQn                   =  37U,
    DW_UART2_IRQn                   =  38U,
    DW_UART3_IRQn                   =  39U,
    DW_UART4_IRQn                   =  40U,
    DW_UART5_IRQn                   =  41U,
    DW_I2C0_IRQn                    =  44U,
    DW_QSPI0_IRQn                   =  52U,
    DW_QSPI1_IRQn                   =  53U,
    DW_SPI0_IRQn                    =  54U,
    DW_GPIO0_IRQn                   =  56U,
    DW_GPIO1_IRQn                   =  57U,
    DW_EMMC_IRQn                    =  62U,
    DW_SD_IRQn                      =  64U,
    DW_USB_IRQn                     =  68U,
    DW_DMA0_IRQn                    =  70U,
    DW_DMA1_IRQn                    =  71U,
    DW_DMA2_IRQn                    =  72U,
    DW_DMA3_IRQn                    =  73U,
    DW_WDT0_IRQn                    =  111U,
    DW_WDT1_IRQn                    =  112U,
} irqn_type_t;

typedef enum {
    WJ_IOCTL_Wakeupn               =  29U,     /* IOCTOL wakeup */
} wakeupn_type_t;

typedef enum {
    WJ_USB_CLK_MANAGERN            = 28U,
} clk_manager_type_t;

typedef enum {
    WJ_VAD_RX0_L,
    WJ_VAD_RX0_R,
    WJ_VAD_RX1_L,
    WJ_VAD_RX1_R,
    WJ_VAD_RX2_L,
    WJ_VAD_RX2_R,
    WJ_VAD_RX3_L,
    WJ_VAD_RX3_R,
    WJ_I2S0_RX,
    WJ_I2S0_TX,
    WJ_I2S1_RX,
    WJ_I2S1_TX,
    WJ_I2S2_RX,
    WJ_I2S2_TX,
    WJ_I2S_IN_RX0,
    WJ_I2S_IN_RX1,
    WJ_I2S_IN_RX2,
    WJ_I2S_IN_RX3,
    UART_RX,
    UART_TX,
    IIC0_RX,
    IIC0_TX,
    IIC1_RX,
    IIC1_TX,
    SPDIF0_RX,
    SPDIF0_TX,
    SPDIF1_RX,
    SPDIF1_TX,
    TDM_RX0,
    TDM_RX1,
    TDM_RX2,
    TDM_RX3,
    TDM_RX4,
    TDM_RX5,
    TDM_RX6,
    TDM_RX7,
} dman_type_t;

typedef enum {
    PA0         = 0U,
    PA1,
    PA2,
    PA3,
    PA4,
    PA5,
    PA6,
    PA7,
    PA8,
    PA9,
    PA10,
    PA11,
    PA12,
    PA13,
    PA14,
    PA15,
    PA16,
    PA17,
    PA18,
    PA19,
    PA20,
    PA21,
    PA22,
    PA23,
    PA24,
    PA25,
    PA26,
    PA27,
    PA28,
    PA29,
    PA30,
    PA31,
    PB0,
    PB1,
    PB2,
    PB3,
    PB4,
    PB5,
    PB6,
    PB7,
    PB8,
    PB9,
    PB10,
    PB11,
    PB12,
    PB13,
    PB14,
    PB15,
    PB16,
    PB17,
    PB18,
    PB19,
    PB20,
    PB21,
    PB22,
    PB23,
    PB24,
    PB25,
    PB26,
    PB27,
    PB28,
    PB29,
    PB30,
    PB31,
    PC0,
    PC1,
    PC2,
    PC3,
    PC4,
    PC5,
    PC6,
    PC7,
    PC8,
    PC9,
    PC10,
    PC11,
    PC12,
    PC13,
    PC14,
    PC15,
    PC16,
    PC17,
    PC18,
    PC19,
    PC20,
    PC21,
    PC22,
    PC23,
    PC24,
    PC25,
    PC26,
    PC27,
    PC28,
    PC29,
    PC30,
    PC31,
} pin_name_t;


typedef enum {
    PA0_BOOT_SEL0                  =  0U,
    PA1_BOOT_SEL1                  =  0U,
    PA2_BOOT_SEL3                  =  0U,
    PA3_OSC_BOOT                   =  0U,
    PA4_QSPI0_SCK                  =  0U,
    PA5_QSPI0_SSN0                 =  0U,
    PA6_QSPI0_MOSI                 =  0U,
    PA7_QSPI0_MISO                 =  0U,
    PA8_QSPI0_WP                   =  0U,
    PA9_QSPI0_HOLD                 =  0U,
    PA10_QSPI1_SCK                 =  0U,
    PA11_QSPI1_SSN0                =  0U,
    PA11_I2C5_SCL                  =  1U,
    PA11_UART5_TXD                 =  2U,
    PA12_QSPI1_MOSI                =  0U,
    PA12_I2C5_SDA                  =  1U,
    PA12_UART5_RXD                 =  2U,
    PA13_QSPI1_MISO                =  0U,
    PA14_UART5_TXD                 =  2U,
    PA15_QSPI1_HOLD                =  0U,
    PA15_UART5_RXD                 =  2U,
    PA16_SPI_SCK                   =  0U,
    PA16_UART2_TXD                 =  2U,
    PA17_SPI_SSN0                  =  0U,
    PA17_UART2_RXD                 =  2U,
    PA18_SPI_SSN1                  =  0U,
    PA19_SPI_MOSI                  =  0U,
    PA20_SPI_MISO                  =  0U,
    PA21_I2C0_SCL                  =  0U,
    PA22_I2C0_SDA                  =  0U,
    PA23_I2C1_SCL                  =  0U,
    PA24_I2C1_SDA                  =  0U,
    PA25_I2C2_SCL                  =  0U,
    PA26_I2C2_SDA                  =  0U,
    PA27_I2C3_SCL                  =  0U,
    PA28_I2C3_SDA                  =  0U,
    PA29_UART0_RXD                 =  0U,
    PA30_UART0_TXD                 =  0U,
    PA31_UART1_RXD                 =  0U,
    //TO BE CONTINUE

    PIN_FUNC_GPIO                  =  3U,
} pin_func_t;

#define CONFIG_GPIO_NUM             3
#define CONFIG_IRQ_NUM              112
#define CONFIG_DMA_NUM              1

#define DW_USB_BASE                 0xFFE7040000UL
#define DW_USB_SIZE                 0x10000U

#define DW_TIMER0_BASE              0xFFEFC32000UL
#define DW_TIMER0_SIZE              0x14U

#define DW_TIMER1_BASE              (DW_TIMER0_BASE+DW_TIMER0_SIZE)
#define DW_TIMER1_SIZE              DW_TIMER0_SIZE

#define DW_TIMER2_BASE              0xFFFFC33000UL
#define DW_TIMER2_SIZE              DW_TIMER1_SIZE

#define DW_TIMER3_BASE              (DW_TIMER2_BASE+DW_TIMER2_SIZE)
#define DW_TIMER3_SIZE              DW_TIMER2_SIZE

#define DW_UART0_BASE               0xFFE7014000UL
#define DW_UART0_SIZE               0x4000U

#define DW_UART1_BASE               0xFFE7018000UL
#define DW_UART1_SIZE               0x4000U

#define DW_UART2_BASE               0xFFEC000000UL
#define DW_UART2_SIZE               0x4000U

#define DW_UART3_BASE               0xFFEC008000UL
#define DW_UART3_SIZE               0x4000U

#define DW_UART4_BASE               0xFFFC004000UL
#define DW_UART4_SIZE               0x4000U

#define DW_UART5_BASE               0xFFFC024000UL
#define DW_UART5_SIZE               0x4000U

#define DW_GPIO0_BASE               0xFFE7004000UL
#define DW_GPIO0_SIZE               0x4000U

#define DW_GPIO1_BASE               0xFFE7008000UL
#define DW_GPIO1_SIZE               0x4000U

#define DW_GPIO2_BASE               0xFFE7010000UL
#define DW_GPIO2_SIZE               0x4000U

#define DW_WDT_BASE                 0xFFEFC30000UL
#define DW_WDT_BASE_SZIE            0x1000U

#define DW_DMA_BASE                 0xFFEFC00000UL
#define DW_DMA_BASE_SZIE            0x4000U

#define WJ_IOC_BASE                 0xFFEF01C000UL
#define WJ_IOC_SIZE                 0x4000U

#define WJ_CPR_BASE                 0xFFCB000000UL
#define WJ_CPR_BASE_SIZE            0x1000000U

#define DW_SPI0_BASE                0xFFF700C000UL
#define DW_SPI0_BASE_SIZE           0x10000U

#define DW_QSPI0_BASE              0xFFE8000000UL
#define DW_QSPI0_BASE_SIZE         0x10000U

#define DW_QSPI1_BASE              0xFFEA000000UL
#define DW_QSPI1_BASE_SIZE         0x10000U

#define DW_I2C0_BASE               0xFFE701C000UL
#define DW_I2C0_BASE_SIZE          0x4000U

#define WJ_MBOX_BASE               0xFFFFC38000UL
#define WJ_MBOX_SIZE               0x1000U

#define WJ_MBOX1_BASE              0xFFFFC48000UL
#define WJ_MBOX1_SIZE              0x1000U

#define DW_EMMC_BASE               0xFFE7080000UL
#define DW_EMMC_SIZE               0x1000U

#define DW_SD_BASE                 0xFFE7090000UL
#define DW_SD_SIZE                 0x1000U

#define RB_RNG_BASE                0xFFFF300000UL
#define RB_RNG_SIZE                0x10000U
#define CONFIG_MAILBOX_CHANNEL_NUM  4U

#define CONFIG_RTC_FAMILY_D
#define CONFIG_DW_AXI_DMA_8CH_NUM_CHANNELS

#define SOC_OM_ADDRBASE             0xFFEF018010
#define SOC_OSC_BOOT_ADDRBASE       0xFFEF010314
#define SOC_INTERNAL_SRAM_BASEADDR  0xFFE0000000
#ifdef CONFIG_IS_MPW
#define SOC_INTERNAL_SRAM_SIZE      (256 * 1024)    //256KB
#else
#define SOC_INTERNAL_SRAM_SIZE      (1536 * 1024)   //1.5MB
#endif
#define SOC_BROM_BASE_ADDRESS       0xFFFFD00000

#define RB_EIP150B_BASE            0xFFFF300000UL
#define RB_EIP150B_SIZE            0x10000U



#define CONFIG_OTP_BASE_ADDR        0   // FIXME:
#define CONFIG_OTP_BANK_SIZE        (8 * 1024)

#define bootsel() \
    ({ unsigned int __v = (*(volatile uint32_t *) (0xFFEF018010)); __v&0x7; })

#define osc_bootsel() \
    ({ unsigned int __v = (*(volatile uint32_t *) (0xFFEF010314)); __v&0x1; })

#ifdef __cplusplus
}
#endif

#endif  /* _SOC_H_ */
