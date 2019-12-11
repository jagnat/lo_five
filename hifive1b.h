#include "stdint.h"
#define BIT(x) (1 << x)

volatile struct __PLIC
{
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
    uint32_t out_xor;
} extern GPIO;

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
