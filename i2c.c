#include "i2c.h"
#include "hifive1b.h"
#include "sys.h"
#include "interrupt.h"
#include "task.h"

static sem_t __i2c_lock;

static void __i2c_irq()
{
    

}

void i2c_init()
{
    // 100khz - i2c master chip internally adds one and multiplies by 5
    uint32_t prescale = (cpu_freq / (5 * 100000)) - 1;
    I2C.control = 0;
    I2C.prescale_lo = prescale & 0xff;
    I2C.prescale_hi = (prescale & 0xff00) >> 8;

    plic_enable(PLIC_INT_I2C, 7);
    plic_set_handler(PLIC_INT_I2C, __i2c_irq);
    GPIO.iof_en |= I2C_SDA_PIN & I2C_SCL_PIN;
    GPIO.iof_sel &= ~(I2C_SDA_PIN & I2C_SCL_PIN);
    
    I2C.control = I2C_CTR_EN | I2C_CTR_IEN;
}

void i2c_write(char addr, int count, i2c_buf_callback callback, void* arg)
{

}


