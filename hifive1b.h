#ifndef __HIFIVE1B_H__
#define __HIFIVE1B_H__

#include "csr_encoding.h"
#include "stdint.h"
#define BIT(x) (1 << x)

volatile struct __CLINT
{
    uint32_t msip;
    uint32_t reserved0 [4095];
    uint64_t mtimecmp;
    uint32_t reserved1 [8188];
    uint64_t mtime;
} extern CLINT;

#define PLIC_NUM_INTERRUPTS 52
volatile struct __PLIC
{
    #define PLIC_INT_I2C 52
    uint32_t priority[53];
    uint32_t reserved0[971];
    uint32_t pending1;
    uint32_t pending2;
    uint32_t reserved1[1022];
    uint32_t enable1;
    uint32_t enable2;
    uint32_t reserved2[522238];
    uint32_t threshold;
    uint32_t claim_complete;
} extern PLIC;

volatile struct __WDT
{
    uint32_t cfg;
    uint32_t reserved0;
    uint32_t count;
    uint32_t reserved1;
    uint32_t s;
    uint32_t reserved2;
    uint32_t feed;
    uint32_t key;
    uint32_t cmp0;
} extern WDT;

volatile struct __RTC
{
#define RTC_ENALWAYS BIT(12)
    uint32_t cfg;
    uint32_t reserved0;
    uint32_t countlo;
    uint32_t counthi;
    uint32_t s;
    uint32_t reserved1[3];
    uint32_t cmp0;
} extern RTC;

volatile struct __AON
{
    #define LFROSC_ENABLE BIT(30)
    uint32_t lfrosccfg;
    uint32_t reserved0[2];
    uint32_t lfclkmux;
    uint32_t backup[16];
} extern AON;

volatile struct __PRCI
{
    #define HFROSC_DIV_SHIFT 0
    #define HFROSC_TRIM_SHIFT 16
    #define HFROSC_ENABLE BIT(30)
    #define HFROSC_READY BIT(31)
    uint32_t hfrosc;

    #define HFXOSC_ENABLE BIT(30)
    #define HFXOSC_READY BIT(31)
    uint32_t hfxosc;

    #define PLL_R_SHIFT 0
    #define PLL_F_SHIFT 4
    #define PLL_Q_SHIFT 10
    #define PLL_SEL BIT(16)
    #define PLL_REF_SEL BIT(17)
    #define PLL_BYPASS BIT(18)
    #define PLL_LOCK BIT(31)
    uint32_t pllcfg;

    #define PLL_OUTDIV_SHIFT 0
    #define PLL_DIVIDE_BY_1 BIT(8)
    uint32_t plloutdiv;

    uint32_t _reserved0[56];
    uint32_t procmoncfg;
} extern PRCI;


volatile struct __GPIO
{
    uint32_t input_val;
    uint32_t input_en;
    uint32_t output_en;
    uint32_t output_val;
    uint32_t pull_up_enable;
    uint32_t drive_strength;
    uint32_t rise_ie;
    uint32_t rise_ip;
    uint32_t fall_ie;
    uint32_t fall_ip;
    uint32_t high_ie;
    uint32_t high_ip;
    uint32_t low_ie;
    uint32_t low_ip;
    uint32_t iof_en;
    uint32_t iof_sel;
    uint32_t output_xor;
} extern GPIO;

struct __UART
{
    #define UART_FULL BIT(31)
    uint32_t txdata;
    #define UART_EMPTY BIT(31)
    uint32_t rxdata;

    #define UART_TX_EN BIT(0)
    #define UART_TX_NSTOP_1 0
    #define UART_TX_NSTOP_2 BIT(1)
    #define UART_TX_CNT_SHIFT 16
    uint32_t txctrl;

    #define UART_RX_EN BIT(0)
    #define UART_RX_CNT_SHIFT 16
    uint32_t rxctrl;

    #define UART_INT_TXWM BIT(0)
    #define UART_INT_RXWM BIT(1)
    uint32_t ie;
    uint32_t ip;

    uint32_t div;
};

struct __PWM
{
    #define PWM_SCALE_SHIFT 0

    #define PWM_STICKY BIT(8)
    #define PWM_ZEROCMP BIT(9)
    #define PWM_DEGLITCH BIT(10)

    #define PWM_ENALWAYS BIT(12)
    #define PWM_ENONESHOT BIT(13)

    #define PWM_CMP0CENTER BIT(16)
    #define PWM_CMP1CENTER BIT(17)
    #define PWM_CMP2CENTER BIT(18)
    #define PWM_CMP3CENTER BIT(19)

    #define PWM_CMP0GANG BIT(24)
    #define PWM_CMP1GANG BIT(25)
    #define PWM_CMP2GANG BIT(26)
    #define PWM_CMP3GANG BIT(27)

    #define PWM_CMP0IP BIT(28)
    #define PWM_CMP1IP BIT(29)
    #define PWM_CMP2IP BIT(30)
    #define PWM_CMP3IP BIT(31)
    #define PWM_IP_BITS (BIT(28) | BIT(29) | BIT(30) | BIT(31))
    uint32_t cfg;

    uint32_t reserved0;

    uint32_t count;

    uint32_t reserved1;

    uint32_t s;

    uint32_t reserved2[3];

    uint32_t cmp0, cmp1, cmp2, cmp3;
};
volatile extern struct __PWM PWM0;
volatile extern struct __PWM PWM1;
volatile extern struct __PWM PWM2;

struct __SPI
{
    uint32_t sckdiv;
    #define SPI_SCKMODE_PHASE BIT(0)
    #define SPI_SCKMODE_POLARITY BIT(1)
    uint32_t sckmode;

    uint32_t reserved0[2];

    uint32_t csid;
    uint32_t csdef;
    #define SPI_CSMODE_AUTO 0
    #define SPI_CSMODE_HOLD 2
    #define SPI_CSMODE_OFF 3
    uint32_t csmode;

    uint32_t reserved1[3];

    uint32_t delay0;
    uint32_t delay1;

    uint32_t reserved2[4];

    #define SPI_FMT_PROTO_SINGLE 0
    #define SPI_FMT_PROTO_DUAL 1
    #define SPI_FMT_PROTO_QUAD 2
    #define SPI_FMT_LSBFIRST BIT(2)
    #define SPI_FMT_MSBFIRST 0
    #define SPI_FMT_DIR_RX 0
    #define SPI_FMT_DIR_TX BIT(3)
    #define SPI_FMT_LEN_SHIFT 16
    uint32_t fmt;

    uint32_t reserved3;

    #define SPI_TXDATA_FULL BIT(31)
    uint32_t txdata;
    #define SPI_RXDATA_EMPTY BIT(31)
    uint32_t rxdata;

    uint32_t txmark;
    uint32_t rxmark;

    uint32_t reserved4[2];

    #define SPI_FCTRL_EN BIT(0)
    uint32_t fctrl;

    #define SPI_FFMT_CMD_EN BIT(0)
    #define SPI_FFMT_ADDR_LEN_SHIFT 1
    #define SPI_FFMT_PAD_CNT_SHIFT 4
    #define SPI_FFMT_CMD_PROTO_SHIFT 8
    #define SPI_FFMT_ADDR_PROTO_SHIFT 10
    #define SPI_FFMT_DATA_PROTO_SHIFT 12
    #define SPI_FFMT_CMD_CODE_SHIFT 16
    #define SPI_FFMT_PAD_CODE_SHIFT 24
    uint32_t ffmt;

    uint32_t reserved5[2];
    
    #define SPI_INT_TXWM BIT(0)
    #define SPI_INT_RXWM BIT(1)
    uint32_t ie;
    uint32_t ip;
};
volatile extern struct __SPI QSPI0;
volatile extern struct __SPI  SPI1;
volatile extern struct __SPI  SPI2;

volatile struct
{
    uint32_t prescale_lo;
    uint32_t prescale_hi;
    uint32_t control;
    uint32_t transmit;
    uint32_t receive;
    uint32_t command_status;
} extern I2C;

#endif // __HIFIVE1B_H__
