#define F_CPU 32768
// #define _AVR_ATTINY404
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "pindefs.h"
#include "updi_stdio.h"

void updi_uart_tx_str(char *str)
{
    char *tmp = str;
    while(tmp)
    {
        putchar(*tmp);
        tmp++;
    }

}

int main()
{
    //clk setup
    //use 20MHz oscillator, no clk output
    CPU_CCP = CCP_IOREG_gc;
    CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL_OSCULP32K_gc;
    //prescale by 10, enable prescalar
    CPU_CCP = CCP_IOREG_gc;
    CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_64X_gc | CLKCTRL_PEN_bm;

    // PORTA.DIR = 0;
    // //PORT A Direction for bitbang i2c (leave as input for init)
    // PORTA.DIR |= (0 << PORTA_I2C_SCL_PIN) | (0 << PORTA_I2C_SDA_PIN) | (0 << PORTA_I2C_PULLUP_PIN);
    // //PORT A Direction for D2 (LED2)
    // PORTA.DIR |= (1 << PORTA_LED2_R_PIN) | (1 << PORTA_LED2_G_PIN) | (1 << PORTA_LED2_B_PIN);
    // //PORT A Direction for NFC interrupt
    // //TODO: add interrupt settings
    // PORTA.DIR |= (0 << PORTA_NFC_INT_PIN);

    // PORTB.DIR = 0;
    // //PORT B Direction for D1 (LED1)
    // PORTA.DIR |= (1 << PORTB_LED2_R_PIN) | (1 << PORTB_LED2_G_PIN) | (1 << PORTB_LED2_B_PIN);
    
    i2c_bitbang_init();
	updi_stdio_init();	// redirect stdin/stdout

    uint8_t addr = 0xAE;
    uint8_t result = 0;
    uint8_t reg = 0x1D;
    uint8_t data = 0;
    // Set built-in LED pin as output
    PORTB.DIR |= (1 << 2);
    // while (1) {
    //     PORTB.OUTSET |=  (1 << 2);   // LED on
        // _delay_ms(500);
        
        // puts("hello");
        // printf( "\nRESET\n" );
        // putchar('h');

            result = 0;
         i2c_bitbang_start();
         result = i2c_bitbang_tx((addr & 0xFE)| 0x00);
        i2c_bitbang_tx(0x00); // Register address where we want to write
        i2c_bitbang_tx(reg); // Data to be written 
        // i2c_bitbang_stop();

        i2c_bitbang_start();
         result = i2c_bitbang_tx((addr & 0xFE)| 0x01);
        data = i2c_bitbang_rx(0); // Register address where we want to write
        i2c_bitbang_stop();
        // //          if(addr > 127)
        // //  {
        // //     addr = 1;
        // //  }
        // printf("addr 0x%02x: %d\n", addr, data);
        // addr++;
        // _delay_ms(1);

        // PORTB.OUTCLR |=  (1 << 2);   // LED off
        // _delay_ms(1);

    // }
    while(1);
    return 0;
}