#pragma once

typedef char (*i2c_buf_callback)(void*, int);

void i2c_init();
void i2c_write(char addr, int count, i2c_buf_callback callback, void* arg);
