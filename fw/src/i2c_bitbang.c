#include "i2c_bitbang.h"

// void dly()
// {
//     volatile uint8_t delay = 16;
//     while(delay--);
// }

inline void dly(){asm("nop");};

void i2c_bitbang_init()
{
    //set output reg for scl and sda as 0.
    // this will pull the lines low when set as output.
    PORTA.OUTCLR = (1 << PORTA_I2C_SDA_PIN);
    PORTA.OUTCLR = (1 << PORTA_I2C_SCL_PIN);

}

/*  i2c start sequence */
void i2c_bitbang_start()
{
    SDA_HIGH;
    dly();
    SCL_HIGH;
    dly();
    SDA_LOW;
    dly();
    SCL_LOW;
    dly();
}

/*  i2c stop sequence */
void i2c_bitbang_stop()
{
    SDA_LOW;
    dly();
    SCL_HIGH;
    dly();
    SDA_HIGH;
    dly();
}

/* Transmit 8 bit data to slave */
bool i2c_bitbang_tx(uint8_t dat)
{

    for(uint8_t i = 8; i; i--)
    {
        (dat & 0x80) ? SDA_HIGH : SDA_LOW; //Mask for the eigth bit
        dat<<=1;  //Move 
        dly();
        SCL_HIGH;
        dly();
        SCL_LOW;
        dly();
    }
    SDA_HIGH;
    SCL_HIGH;
    dly();
    bool ack = !SDA_READ;    // Acknowledge bit
    SCL_LOW;
    return ack;
}

bool i2c_bitbang_rx(bool ack, uint8_t *data, uint8_t timeout)
{
    uint8_t dat = 0;
    uint8_t timeout_cnt = 0;
    SDA_HIGH;
    for( uint8_t i =0; i<8; i++)
    {
        dat <<= 1;
        do{
            SCL_HIGH;
        }while(SCL_READ == 0 && timeout_cnt++ < timeout);  //clock stretching
        if(timeout_cnt == timeout)
        {
            return false;
        }
        
        dly();
        if(SDA_READ) dat |=1;
        dly();
        SCL_LOW;
    }
    ack ? SDA_LOW : SDA_HIGH;
    SCL_HIGH;
    dly();
    SCL_LOW;
    SDA_HIGH;
    *data = dat;
    return(true);
}


