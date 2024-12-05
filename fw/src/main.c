#define F_CPU 32768
// #include <avr/iotn404.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "pindefs.h"
#include "updi_stdio.h"
#include "led.h"
// #define __AVR_ATtiny404__

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
    CLKCTRL.MCLKCTRLB = 0;//CLKCTRL_PDIV_64X_gc | CLKCTRL_PEN_bm;

    // //PORT A Direction for NFC interrupt
    // //TODO: add interrupt settings
    // PORTA.DIR |= (0 << PORTA_NFC_INT_PIN);

    /* set default GPIO direction to input */
    PORTA.DIR = 0;
    PORTB.DIR = 0;
  
    led_init();
    i2c_bitbang_init();
	updi_stdio_init();	// redirect stdin/stdout

    uint8_t addr = 0xA6;
    uint8_t result = 0;
    uint8_t reg = 0x1D;
    uint8_t data = 0;
    // Set built-in LED pin as output
    // PORTB.DIR |= (1 << 2);
    led_t led1, led2 = {0};
    // led2.g = 0;
    // led2.r = 0;
    uint8_t count = 0;

    // turn on i2c pullups
    PORTA.DIRSET = (1 << PORTA_I2C_PULLUP_PIN);
    PORTA.OUTSET = (1 << PORTA_I2C_PULLUP_PIN);

    // PORTA.OUTCLR = (1 << PORTA_I2C_SCL_PIN);

    // PORTA.DIRSET = (1 << PORTA_I2C_SCL_PIN);

    // while(1)
    // {
    //     PORTA.DIRSET = (1 << PORTA_I2C_SCL_PIN);
    //     PORTA.DIRSET = (1 << PORTA_I2C_SDA_PIN);
    //     PORTA.DIRCLR = (1 << PORTA_I2C_SCL_PIN);
    //     PORTA.DIRCLR = (1 << PORTA_I2C_SDA_PIN);
    //         // PORTA.OUTCLR = (1 << PORTA_I2C_SCL_PIN);
    //         // PORTA.OUTSET = (1 << PORTA_I2C_SCL_PIN);

    // }
    while (1) {

        if(count == 0)
        {
            led1.r++;
            led2.r++;
            if(led1.r == 255)
            {
                led1.r = 0;
                led2.r = 0;
                count = 1;
            }
        } else if(count == 1)
        {
            led1.g++;
            led2.g++;
            if(led1.g == 255)
            {
                led1.g = 0;
                led2.g = 0;
                count = 2;
            }
        }
        else if(count == 2)
        {
            led1.b++;
            led2.b++;
            if(led1.b == 255)
            {
                led1.b = 0;
                led2.b = 0;
                count = 0;
            }
        }
        led_update(led1, led2);

// i2c_bitbang_start();
// i2c_bitbang_start();
// i2c_bitbang_start();
          i2c_bitbang_start();
         result = i2c_bitbang_tx((addr & 0xFE)| 0x00);
        i2c_bitbang_tx(0x00); // Register address where we want to write
        i2c_bitbang_tx(reg); // Data to be written 
        i2c_bitbang_stop();

        i2c_bitbang_start();
        result = 0;
         result = i2c_bitbang_tx((addr & 0xFE)| 0x01);
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(1); // Register address where we want to write
        data = i2c_bitbang_rx(0); // Register address where we want to write
        i2c_bitbang_stop();
        // printf("result: 0x%02x\n", result);
        // addr++;
        _delay_ms(1);

    }
    // while(1);
    return 0;
}