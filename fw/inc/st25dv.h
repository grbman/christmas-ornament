#ifndef ST25DV_H
#define ST25DV_H

#include <avr/io.h>
#include <stdbool.h>
#include "pindefs.h"

#define ST25DV_USER_REG 0xA6

bool st25_read_user_reg(uint8_t reg, uint8_t num_regs, uint8_t data[]);

#endif //ST25DV_H