#pragma once

void i2c_init();

// This function provides 
typedef char (*i2c_write_callback)(void* /* passthrough */, int /* index */);

void i2c_write(char addr, int count, i2c_write_callback callback, void* arg);
void i2c_write_buf(char addr, int count, char* buf);
