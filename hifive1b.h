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
    uint32_t hfrosc;
    uint32_t hfxosc;
    uint32_t pllcfg;
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
