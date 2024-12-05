#include "led.h"

void led_init()
{
    /* hard reset TCA0 */
    TCA0.SPLIT.CTRLESET = 0xF;

    // enable split mode
    TCA0.SPLIT.CTRLD = TCA_SPLIT_SPLITM_bm;

    // set periods
    TCA0.SPLIT.HPER = 255;
    TCA0.SPLIT.LPER = 255;

    /* set compare values */
    LED1_R_CMP = 100;
    LED1_G_CMP = 100;
    LED1_B_CMP = 100;

    LED2_R_CMP = 100;
    LED2_G_CMP = 100;
    LED2_B_CMP = 100;

    //enable waveform output
    TCA0.SPLIT.CTRLB = TCA_SPLIT_LCMP0EN_bm | TCA_SPLIT_LCMP1EN_bm | TCA_SPLIT_LCMP2EN_bm | 
                        TCA_SPLIT_HCMP0EN_bm | TCA_SPLIT_HCMP1EN_bm | TCA_SPLIT_HCMP2EN_bm;

    // enable TCA 
    TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV1_gc | TCA_SPLIT_ENABLE_bm;

    // invert outputs
    PORTB.PIN0CTRL = PORT_INVEN_bm;
    PORTB.PIN1CTRL = PORT_INVEN_bm;
    PORTB.PIN2CTRL = PORT_INVEN_bm;

    PORTA.PIN3CTRL = PORT_INVEN_bm;
    PORTA.PIN4CTRL = PORT_INVEN_bm;
    PORTA.PIN5CTRL = PORT_INVEN_bm;

    //PORT B Direction for D1 (LED1)
    PORTB.DIR |= (1 << PORTB_LED1_R_PIN) | (1 << PORTB_LED1_G_PIN) | (1 << PORTB_LED1_B_PIN);

    //PORT A Direction for D2 (LED2)
    PORTA.DIR |= (1 << PORTA_LED2_R_PIN) | (1 << PORTA_LED2_G_PIN) | (1 << PORTA_LED2_B_PIN);

}

void led_update(led_t led1, led_t led2)
{
    /* wait for counter to reach 0 before updating */
    while(TCA0.SPLIT.HCNT > 1);

    LED1_R_CMP = led1.r;// == 255 ? 254 : led1.r;
    LED1_G_CMP = led1.g;// == 255 ? 254 : led1.g;
    LED1_B_CMP = led1.b;// == 255 ? 254 : led1.b;
    LED2_R_CMP = led2.r;// == 255 ? 254 : led2.r;
    LED2_G_CMP = led2.g;// == 255 ? 254 : led2.g;
    LED2_B_CMP = led2.b;// == 255 ? 254 : led2.b;
}