#define F_CPU 32768
// #include <avr/iotn404.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include <stdbool.h>
#include "pindefs.h"
#include "updi_stdio.h"
#include "led.h"
#include "i2c_bitbang.h"
#include "nfc.h"

// #define __AVR_ATtiny404__

void updi_uart_tx_str(char *str)
{
    char *tmp = str;
    while (tmp)
    {
        putchar(*tmp);
        tmp++;
    }
}
// volatile uint8_t num_ints = 0;
 uint8_t data[60];
const uint8_t addr = 0xA6;
 uint8_t result = 0;
uint8_t reg = 0x00;

  struct CHSV testcolor;
    struct CRGB testcolor_out;

int main()
{
    // clk setup
    // use 20MHz oscillator, no clk output
    CPU_CCP = CCP_IOREG_gc;
    CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL_OSCULP32K_gc;
    // prescale by 10, enable prescalar
    CPU_CCP = CCP_IOREG_gc;
    CLKCTRL.MCLKCTRLB = 0; // CLKCTRL_PDIV_64X_gc | CLKCTRL_PEN_bm;

    // //PORT A Direction for NFC interrupt
    // //TODO: add interrupt settings
    // PORTA.DIR |= (0 << PORTA_NFC_INT_PIN);

    /* set default GPIO direction to input */
    PORTA.DIR = 0;
    PORTB.DIR = 0;

    /* enable falling edge interrupt */
    PORTA.PIN7CTRL |= PORT_ISC_FALLING_gc;
    PORTA.INTFLAGS |= PORT_INT_7_bm;

    // sei(); // Enable global interrupts


    led_init();
    i2c_bitbang_init();
    updi_stdio_init(); // redirect stdin/stdout



    led_t led1, led2 = {0};
    // led_update(led1, led2);

    uint8_t count = 0;

     struct CHSV testcolor1;
    struct CRGB testcolor_out1;

    testcolor1.hue = 0;
    testcolor1.saturation = 255;
    testcolor1.value = 255;

    // turn on i2c pullups
    PORTA.DIRSET = (1 << PORTA_I2C_PULLUP_PIN);
    PORTA.OUTSET = (1 << PORTA_I2C_PULLUP_PIN);

    // put CPU into idle mode
    // SLPCTRL.CTRLA |= SLPCTRL_SMODE_IDLE_gc;

        uint8_t i;
        struct nfc_ndef_text_headers headers = {0};
        uint8_t *ptr = (uint8_t *)&headers;
    while (1)
    {


 _delay_ms(1000);
        //  printf("Data: \r\n");

        // printf("\n");
        // testcolor1.hue++;
        // hsv2rgb_raw_avr(&testcolor1, &testcolor_out1);
        // led1.r = testcolor_out1.r;
        // led1.g = testcolor_out1.g;
        // led1.b = testcolor_out1.b;
        // led2.r = testcolor_out1.r;
        // led2.g = testcolor_out1.g;
        // led2.b = testcolor_out1.b;
        // led_update(led1, led2);
        // printf("hue: %d rgb: %d %d %d\n", testcolor.hue, testcolor_out.r, testcolor_out.g, testcolor_out.b);

       
            st25_read_user_reg(0x00, sizeof(headers), &headers);
        //  updi_uart_tx_str("idk\r\n");
        // i = 0;
        //  for(i=0; i < sizeof(headers); i++)
        // {
        //     printf("0x%02x ", ptr[i]);
        //     // printf("%c ", data[i]);
        // }
        //          printf("\r\n");

        if(headers.nfc_tag == NDEF_TAG_MAGIC_NUM)
        {
            printf("payload_len: %d\r\n", headers.ndef_payload_len);
        }
        uint8_t text_addr, text_len;
        if(headers.text_magic_num == TEXT_MAGIC_NUM)
        {
            text_addr = offsetof(headers_t, text_lang_type_len) + 1 + headers.text_lang_type_len;
            text_len = headers.ndef_payload_len - 3;
            st25_read_user_reg(text_addr, text_len, data);
            printf("string: %s\r\n", data);

        }


    }
    // while(1);
    return 0;
}

volatile bool nfc_write_occurred = false;
volatile bool nfc_write_flag = false;
volatile uint16_t nfc_write_debounce = 0;
volatile    uint8_t i = 0;

// NFC write interrupt
ISR(PORTA_PORT_vect)
{
    // cli();

    PORTA_INTFLAGS |= PORT_INT_7_bm; // Clear the PA0 interrupt flag

    // nfc_write_flag = true;

    // sei();

    // put CPU into idle mode
    // SLPCTRL.CTRLA |= SLPCTRL_SMODE_IDLE_gc;
}

// PWM underflow interrupt
ISR(TCA0_LUNF_vect)
{
    
    // cli();

    TCA0.SPLIT.INTFLAGS |= TCA_SPLIT_LUNF_bm;
    // static led_t led1, led2 = {0};
    // if (nfc_write_flag)
    // {
    //     // nfc_write_flag = false;
    //     PORTA.OUTSET = (1 << PORTA_I2C_PULLUP_PIN);
    //     nfc_write_debounce++;
    // }
    // // tBoot is 0.6ms
    // if (nfc_write_debounce > 32)
    // {
    //     nfc_write_flag = false;
    //         nfc_write_debounce = 0;
    //     nfc_write_occurred = true;
    // }
    // if (nfc_write_occurred)
    // {
    //     nfc_write_occurred = false;


    

    //     // testcolor.hue++;
    //     // hsv2rgb_raw(&testcolor, &testcolor_out);
    //     // led1.r = testcolor_out.r;
    //     // led1.g = testcolor_out.g;
    //     // led1.b = testcolor_out.b;
    //     // led2.r = testcolor_out.r;
    //     // led2.g = testcolor_out.g;
    //     // led2.b = testcolor_out.b;
    //     // led_update(led1, led2);
    //     // PORTA.OUTCLR = (1 << PORTA_I2C_PULLUP_PIN);
    // }

    // sei();
    // // put CPU into idle mode
    // // SLPCTRL.CTRLA |= SLPCTRL_SMODE_IDLE_gc;
}