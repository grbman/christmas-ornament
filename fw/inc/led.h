#ifndef LED_H
#define LED_H
#include <avr/io.h>
#include "pindefs.h"

#define LED1_G_CMP TCA0.SPLIT.LCMP0 //WO0
#define LED1_R_CMP TCA0.SPLIT.LCMP1 //WO1
#define LED1_B_CMP TCA0.SPLIT.LCMP2 //WO2
#define LED2_R_CMP TCA0.SPLIT.HCMP0 //WO3
#define LED2_B_CMP TCA0.SPLIT.HCMP1 //WO4
#define LED2_G_CMP TCA0.SPLIT.HCMP2 //WO5

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} led_t;


#endif //LED_H