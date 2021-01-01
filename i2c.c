#include "i2c.h"
#include "hifive1b.h"
#include "sys.h"
#include "interrupt.h"
#include "task.h"

static sem_t __i2c_lock;

#define I2C_BUF_SIZE 32
static struct
{
    sem_t empty;
    uint32_t first, last;
    uint32_t full;
    uint8_t data[I2C_BUF_SIZE];
} i2c_buf;

static void __i2c_buf_enqueue(uint8_t c)
{
    sem_wait(&i2c_buf.empty);
    i2c_buf.data[i2c_buf.last] = c;
    i2c_buf.last = (i2c_buf.last + 1) % I2C_BUF_SIZE;
    i2c_buf.full++;
}

static uint8_t __i2c_buf_dequeue()
{
    // TODO: Assert buffer is non-empty
    char c = i2c_buf.data[i2c_buf.first];
    i2c_buf.first = (i2c_buf.first + 1) % I2C_BUF_SIZE;
    i2c_buf.full--;
    sem_signal(&i2c_buf.empty);
    return c;
}

static void __i2c_irq()
{
    uint32_t status = I2C.csr;

    if (status & I2C_STAT_ACK)
    {
    }
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

void i2c_write(char addr, int count, i2c_write_callback callback, void* arg)
{
    I2C.data = (addr << 1);
    I2C.csr = I2C_CMD_STA;
    I2C.csr = I2C_CMD_WR;

    for (int i = 0; i < count; i++)
    {
        __i2c_buf_enqueue(callback(arg, i));
    }
}

char __i2c_buf_callback(void* arg, int idx)
{
    return *((char*)arg + idx);
}

void i2c_write_buf(char addr, int count, char* buf)
{
    i2c_write(addr, count, __i2c_buf_callback, buf);
}

