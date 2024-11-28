#ifndef I2C_BITBANG_H
#define I2C_BITBANG_H
#include <avr/io.h>
#include <stdbool.h>
#include "pindefs.h"

//Set pin as output, which will pull line low
#define SCL_LOW (PORTA.DIRSET = (1 << PORTA_I2C_SCL_PIN))
#define SDA_LOW (PORTA.DIRSET = (1 << PORTA_I2C_SDA_PIN))

//Set pin as input, which will pull line high from external pullups
#define SCL_HIGH (PORTA.DIRCLR = (1 << PORTA_I2C_SCL_PIN))
#define SDA_HIGH (PORTA.DIRCLR = (1 << PORTA_I2C_SDA_PIN))

#define SCL_READ (PORTA.IN & (1 << PORTA_I2C_SCL_PIN))
#define SDA_READ (PORTA.IN & (1 << PORTA_I2C_SDA_PIN))


// inline void dly(){asm("nop");};
// void dly();


#endif //I2C_BITBANG_H