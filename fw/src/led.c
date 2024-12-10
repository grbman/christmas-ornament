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

void led_update(struct CRGB led1, struct CRGB led2)
{
    /* wait for counter to reach 0 before updating */
    while(TCA0.SPLIT.HCNT > 2);

    LED1_R_CMP = led1.r;// == 255 ? 254 : led1.r;
    LED1_G_CMP = led1.g;// == 255 ? 254 : led1.g;
    LED1_B_CMP = led1.b;// == 255 ? 254 : led1.b;
    LED2_R_CMP = led2.r;// == 255 ? 254 : led2.r;
    LED2_G_CMP = led2.g;// == 255 ? 254 : led2.g;
    LED2_B_CMP = led2.b;// == 255 ? 254 : led2.b;
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