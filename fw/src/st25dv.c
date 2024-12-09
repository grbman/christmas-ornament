#include "i2c_bitbang.h"
#include "st25dv.h"


bool st25_read_user_reg(uint8_t reg, uint8_t num_regs, uint8_t data[])
{
    uint8_t i;
    i2c_bitbang_start();
    if(i2c_bitbang_tx((ST25DV_USER_REG & 0xFE) | 0x00) == false) // dev Addr, write
    {
        i2c_bitbang_stop();
        return false;
    }
    if(i2c_bitbang_tx(0x00) == false) // Reg Addr MSByte
    {
        i2c_bitbang_stop();
        return false;
    };
    if(i2c_bitbang_tx(reg) == false) // Reg Addr LSByte
    {
        i2c_bitbang_stop();
        return false;
    }
    i2c_bitbang_stop();
    i2c_bitbang_start();
    if(i2c_bitbang_tx((ST25DV_USER_REG & 0xFE) | 0x01) == false) // dev Addr, read
    {
        i2c_bitbang_stop();
        return false;
    }
    for (i = 0; i < num_regs-1; i++)
    {
        if(i2c_bitbang_rx(1, &data[i], 255) == false) // read data
        {
            i2c_bitbang_stop();
            return false;
        }
    }
    if(i2c_bitbang_rx(0, &data[i], 255) == false) // read data
    {
        i2c_bitbang_stop();
        return false;
    }
    
    i2c_bitbang_stop();
    return true;
}