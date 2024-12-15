#include "led.h"

struct CRGB manual_color_left;
struct CRGB manual_color_right;

void (*brightness_next)(uint8_t *, uint8_t *);
uint8_t max_power = 32;

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


    // enable interrupt
    TCA0.SPLIT.INTCTRL |= TCA_SPLIT_LUNF_bm;


    // enable TCA 
    TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV2_gc | TCA_SPLIT_ENABLE_bm;

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

void led_update(struct CRGB led_left, struct CRGB led_right)
{
    /* wait for counter to reach 0 before updating */
    while(TCA0.SPLIT.HCNT > LED_UPDATE_TIMER_VAL);
    LED1_R_CMP = led_left.r;// == 255 ? 254 : led1.r;

    LED1_G_CMP = led_left.g;// == 255 ? 254 : led1.g;

    LED1_B_CMP = led_left.b;// == 255 ? 254 : led1.b;
       
    while(TCA0.SPLIT.HCNT > LED_UPDATE_TIMER_VAL);
    LED2_R_CMP = led_right.r;// == 255 ? 254 : led2.r;
      
    LED2_G_CMP = led_right.g;// == 255 ? 254 : led2.g;
    
    LED2_B_CMP = led_right.b;// == 255 ? 254 : led2.b;
}

void mode_rainbow()
{
    static struct CHSV hsv = {0, 255, 255};
    static uint8_t prescale = 0;

    if(prescale++ > 3)
    {
        prescale = 0;
        hsv.hue++;
        if(hsv.hue > HSV_MAX_HUE)
        {
            hsv.hue = 0;
        }
        struct CRGB color_left, color_right, color_rainbow;
        uint8_t brightness_left, brightness_right = 0;
        if(brightness_next)
        {
            brightness_next(&brightness_left, &brightness_right);
        }
        hsv2rgb_raw(&hsv, &color_rainbow);
        color_left.r = scale8(scale8(color_rainbow.r, brightness_left), max_power);
        color_left.g = scale8(scale8(color_rainbow.g, brightness_left), max_power);
        color_left.b = scale8(scale8(color_rainbow.b, brightness_left), max_power);
        color_right.r = scale8(scale8(color_rainbow.r, brightness_right), max_power);
        color_right.g = scale8(scale8(color_rainbow.g, brightness_right), max_power);
        color_right.b = scale8(scale8(color_rainbow.b, brightness_right), max_power);
        led_update(color_left, color_right);
    }
   

}
void update_manual_color_left(struct CRGB color)
{
    manual_color_left.r = color.r;
    manual_color_left.g = color.g;
    manual_color_left.b = color.b;
}
void update_manual_color_right(struct CRGB color)
{
    manual_color_right.r = color.r;
    manual_color_right.g = color.g;
    manual_color_right.b = color.b;
}
void mode_manual()
{
    struct CRGB color_left, color_right;
    uint8_t brightness_left, brightness_right = 0;
    if(brightness_next)
    {
        brightness_next(&brightness_left, &brightness_right);
    }
    color_left.r = scale8(manual_color_left.r, brightness_left);
    color_left.r = scale8(color_left.r, max_power);
    color_left.g = scale8(manual_color_left.g, brightness_left);
    color_left.g = scale8(color_left.g, max_power);
    color_left.b = scale8(manual_color_left.b, brightness_left);
    color_left.b = scale8(color_left.b, max_power);

    color_right.r = scale8(manual_color_right.r, brightness_right);
    color_right.r = scale8(color_right.r, max_power);
    color_right.g = scale8(manual_color_right.g, brightness_right);
    color_right.g = scale8(color_right.g, max_power);
    color_right.b = scale8(manual_color_right.b, brightness_right);
    color_right.b = scale8(color_right.b, max_power);
    led_update(color_left, color_right);
}
void power_set(uint8_t power)
{
    max_power = power;
}
void brightness_glow(uint8_t *brightness_left, uint8_t *brightness_right)
{
    static uint8_t ctr = 0;
    static uint8_t dir = 1;

    if(dir)
    {
        ctr+=2;
    }
    else
    {
        ctr-=2;
    }

    if(ctr == 254)
    {

        dir = 0;
    }
    if(ctr == 0)
    {
        dir = 1;
    }
    *brightness_left = ctr;
    *brightness_right = ctr;
}

void brightness_pingpong(uint8_t *brightness_left, uint8_t *brightness_right)
{
    static uint8_t ctr = 0;
    static uint8_t dir = 1;
    if(dir)
    {
        ctr+=4;
    }
    else
    {
        ctr-=4;
    }

    if(ctr == 252)
    {
        dir = 0;
    }
    if(ctr == 0)
    {
        dir = 1;
    }
    *brightness_left = ctr;
    *brightness_right = 252 - ctr;
}





#define LIB8STATIC_ALWAYS_INLINE __attribute__ ((always_inline)) static inline
/// Clean up the r1 register after a series of *LEAVING_R1_DIRTY calls
/// @ingroup ScalingDirty
LIB8STATIC_ALWAYS_INLINE void cleanup_R1() {
#if CLEANUP_R1_AVRASM == 1
    // Restore r1 to "0"; it's expected to always be that
    asm volatile("clr __zero_reg__  \n\t" : : : "r1");
#endif
}
/// HSV to RGB implementation in raw C, platform independent
void hsv2rgb_raw_C (struct CHSV *hsv, struct CRGB *rgb);
/// HSV to RGB implementation in raw C, for the AVR platform only
void hsv2rgb_raw_avr(struct CHSV *hsv, struct CRGB *rgb);

#if defined(__AVR__) && !defined( LIB8_ATTINY )
void hsv2rgb_raw(struct CHSV *hsv, struct CRGB *rgb)
{
    hsv2rgb_raw_avr( hsv, rgb);
}
#else
void hsv2rgb_raw(struct CHSV *hsv, struct CRGB *rgb)
{
    hsv2rgb_raw_C( hsv, rgb);
}
#endif


/// Apply dimming compensation to values
#define APPLY_DIMMING(X) (X)

/// Divide the color wheel into eight sections, 32 elements each
/// @todo Unused. Remove?
#define HSV_SECTION_6 (0x20)

/// Divide the color wheel into four sections, 64 elements each
/// @todo I believe this is mis-named, and should be HSV_SECTION_4
#define HSV_SECTION_3 (0x40)

void hsv2rgb_raw_C (struct CHSV *hsv, struct CRGB *rgb)
{
    // Convert hue, saturation and brightness ( HSV/HSB ) to RGB
    // "Dimming" is used on saturation and brightness to make
    // the output more visually linear.

    // Apply dimming curves
    uint8_t value = APPLY_DIMMING( hsv->val);  // cppcheck-suppress selfAssignment
    uint8_t saturation = hsv->sat;

    // The brightness floor is minimum number that all of
    // R, G, and B will be set to.
    uint8_t invsat = APPLY_DIMMING( 255 - saturation);  // cppcheck-suppress selfAssignment
    uint8_t brightness_floor = (value * invsat) / 256;

    // The color amplitude is the maximum amount of R, G, and B
    // that will be added on top of the brightness_floor to
    // create the specific hue desired.
    uint8_t color_amplitude = value - brightness_floor;

    // Figure out which section of the hue wheel we're in,
    // and how far offset we are withing that section
    uint8_t section = hsv->hue / HSV_SECTION_3; // 0..2
    uint8_t offset = hsv->hue % HSV_SECTION_3;  // 0..63

    uint8_t rampup = offset; // 0..63
    uint8_t rampdown = (HSV_SECTION_3 - 1) - offset; // 63..0

    // We now scale rampup and rampdown to a 0-255 range -- at least
    // in theory, but here's where architecture-specific decsions
    // come in to play:
    // To scale them up to 0-255, we'd want to multiply by 4.
    // But in the very next step, we multiply the ramps by other
    // values and then divide the resulting product by 256.
    // So which is faster?
    //   ((ramp * 4) * othervalue) / 256
    // or
    //   ((ramp    ) * othervalue) /  64
    // It depends on your processor architecture.
    // On 8-bit AVR, the "/ 256" is just a one-cycle register move,
    // but the "/ 64" might be a multicycle shift process. So on AVR
    // it's faster do multiply the ramp values by four, and then
    // divide by 256.
    // On ARM, the "/ 256" and "/ 64" are one cycle each, so it's
    // faster to NOT multiply the ramp values by four, and just to
    // divide the resulting product by 64 (instead of 256).
    // Moral of the story: trust your profiler, not your insticts.

    // Since there's an AVR assembly version elsewhere, we'll
    // assume what we're on an architecture where any number of
    // bit shifts has roughly the same cost, and we'll remove the
    // redundant math at the source level:

    //  // scale up to 255 range
    //  //rampup *= 4; // 0..252
    //  //rampdown *= 4; // 0..252

    // compute color-amplitude-scaled-down versions of rampup and rampdown
    uint8_t rampup_amp_adj   = (rampup   * color_amplitude) / (256 / 4);
    uint8_t rampdown_amp_adj = (rampdown * color_amplitude) / (256 / 4);

    // add brightness_floor offset to everything
    uint8_t rampup_adj_with_floor   = rampup_amp_adj   + brightness_floor;
    uint8_t rampdown_adj_with_floor = rampdown_amp_adj + brightness_floor;


    if( section ) {
        if( section == 1) {
            // section 1: 0x40..0x7F
            rgb->r = brightness_floor;
            rgb->g = rampdown_adj_with_floor;
            rgb->b = rampup_adj_with_floor;
        } else {
            // section 2; 0x80..0xBF
            rgb->r = rampup_adj_with_floor;
            rgb->g = brightness_floor;
            rgb->b = rampdown_adj_with_floor;
        }
    } else {
        // section 0: 0x00..0x3F
        rgb->r = rampdown_adj_with_floor;
        rgb->g = rampup_adj_with_floor;
        rgb->b = brightness_floor;
    }
}



#if defined(__AVR__) && !defined( LIB8_ATTINY )
void hsv2rgb_raw_avr(struct CHSV *hsv, struct CRGB *rgb)
{
    uint8_t hue, saturation, value;

    hue =        hsv->hue;
    saturation = hsv->sat;
    value =      hsv->val;

    // Saturation more useful the other way around
    saturation = 255 - saturation;
    uint8_t invsat = APPLY_DIMMING( saturation );  // cppcheck-suppress selfAssignment

    // Apply dimming curves
    value = APPLY_DIMMING( value );  // cppcheck-suppress selfAssignment

    // The brightness floor is minimum number that all of
    // R, G, and B will be set to, which is value * invsat
    uint8_t brightness_floor;

    asm volatile(
                 "mul %[value], %[invsat]            \n"
                 "mov %[brightness_floor], r1        \n"
                 : [brightness_floor] "=r" (brightness_floor)
                 : [value] "r" (value),
                 [invsat] "r" (invsat)
                 : "r0", "r1"
                 );

    // The color amplitude is the maximum amount of R, G, and B
    // that will be added on top of the brightness_floor to
    // create the specific hue desired.
    uint8_t color_amplitude = value - brightness_floor;

    // Figure how far we are offset into the section of the
    // color wheel that we're in
    uint8_t offset = hsv->hue & (HSV_SECTION_3 - 1);  // 0..63
    uint8_t rampup = offset * 4; // 0..252


    // compute color-amplitude-scaled-down versions of rampup and rampdown
    uint8_t rampup_amp_adj;
    uint8_t rampdown_amp_adj;

    asm volatile(
                 "mul %[rampup], %[color_amplitude]       \n"
                 "mov %[rampup_amp_adj], r1               \n"
                 "com %[rampup]                           \n"
                 "mul %[rampup], %[color_amplitude]       \n"
                 "mov %[rampdown_amp_adj], r1             \n"
                 : [rampup_amp_adj] "=&r" (rampup_amp_adj),
                 [rampdown_amp_adj] "=&r" (rampdown_amp_adj),
                 [rampup] "+r" (rampup)
                 : [color_amplitude] "r" (color_amplitude)
                 : "r0", "r1"
                 );


    // add brightness_floor offset to everything
    uint8_t rampup_adj_with_floor   = rampup_amp_adj   + brightness_floor;
    uint8_t rampdown_adj_with_floor = rampdown_amp_adj + brightness_floor;


    // keep gcc from using "X" as the index register for storing
    // results back in the return structure.  AVR's X register can't
    // do "std X+q, rnn", but the Y and Z registers can.
    // if the pointer to 'rgb' is in X, gcc will add all kinds of crazy
    // extra instructions.  Simply killing X here seems to help it
    // try Y or Z first.
    asm volatile(  ""  :  :  : "r26", "r27" );


    if( hue & 0x80 ) {
        // section 2: 0x80..0xBF
        rgb->r = rampup_adj_with_floor;
        rgb->g = brightness_floor;
        rgb->b = rampdown_adj_with_floor;
    } else {
        if( hue & 0x40) {
            // section 1: 0x40..0x7F
            rgb->r = brightness_floor;
            rgb->g = rampdown_adj_with_floor;
            rgb->b = rampup_adj_with_floor;
        } else {
            // section 0: 0x00..0x3F
            rgb->r = rampdown_adj_with_floor;
            rgb->g = rampup_adj_with_floor;
            rgb->b = brightness_floor;
        }
    }

    cleanup_R1();
}
// End of AVR asm implementation

#endif


/// Because of the AVR(Arduino) and ARM assembly language
/// implementations provided, using these functions often
/// results in smaller and faster code than the equivalent
/// program using plain "C" arithmetic and logic.
/// @{

/// Scale one byte by a second one, which is treated as
/// the numerator of a fraction whose denominator is 256.
///
/// In other words, it computes i * (scale / 256)
/// @param i input value to scale
/// @param scale scale factor, in n/256 units
/// @returns scaled value
/// @note Takes 4 clocks on AVR with MUL, 2 clocks on ARM
__attribute__ ((always_inline)) static inline uint8_t scale8(uint8_t i, uint8_t scale) {
#if SCALE8_C == 1
#if (FASTLED_SCALE8_FIXED == 1)
    return (((uint16_t)i) * (1 + (uint16_t)(scale))) >> 8;
#else
    return ((uint16_t)i * (uint16_t)(scale)) >> 8;
#endif
#elif SCALE8_AVRASM == 1
#if defined(LIB8_ATTINY)
#if (FASTLED_SCALE8_FIXED == 1)
    uint8_t work = i;
#else
    uint8_t work = 0;
#endif
    uint8_t cnt = 0x80;
    asm volatile(
#if (FASTLED_SCALE8_FIXED == 1)
        "  inc %[scale]                 \n\t"
        "  breq DONE_%=                 \n\t"
        "  clr %[work]                  \n\t"
#endif
        "LOOP_%=:                       \n\t"
        /*"  sbrc %[scale], 0             \n\t"
        "  add %[work], %[i]            \n\t"
        "  ror %[work]                  \n\t"
        "  lsr %[scale]                 \n\t"
        "  clc                          \n\t"*/
        "  sbrc %[scale], 0             \n\t"
        "  add %[work], %[i]            \n\t"
        "  ror %[work]                  \n\t"
        "  lsr %[scale]                 \n\t"
        "  lsr %[cnt]                   \n\t"
        "brcc LOOP_%=                   \n\t"
        "DONE_%=:                       \n\t"
        : [work] "+r"(work), [cnt] "+r"(cnt)
        : [scale] "r"(scale), [i] "r"(i)
        :);
    return work;
#else
    asm volatile(
#if (FASTLED_SCALE8_FIXED == 1)
        // Multiply 8-bit i * 8-bit scale, giving 16-bit r1,r0
        "mul %0, %1          \n\t"
        // Add i to r0, possibly setting the carry flag
        "add r0, %0         \n\t"
        // load the immediate 0 into i (note, this does _not_ touch any flags)
        "ldi %0, 0x00       \n\t"
        // walk and chew gum at the same time
        "adc %0, r1          \n\t"
#else
        /* Multiply 8-bit i * 8-bit scale, giving 16-bit r1,r0 */
        "mul %0, %1          \n\t"
        /* Move the high 8-bits of the product (r1) back to i */
        "mov %0, r1          \n\t"
    /* Restore r1 to "0"; it's expected to always be that */
#endif
        "clr __zero_reg__    \n\t"

        : "+d"(i)    /* writes to i; r16-r31, restricted by ldi */
        : "r"(scale) /* uses scale */
        : "r0", "r1" /* clobbers r0, r1 */
    );
    /* Return the result */
    return i;
#endif
#else
#error "No implementation for scale8 available."
#endif
}