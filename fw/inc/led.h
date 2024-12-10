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

// typedef struct
// {
//     uint8_t r;
//     uint8_t g;
//     uint8_t b;
// } led_t;

/// Representation of an HSV pixel (hue, saturation, value (aka brightness)).
struct CHSV {
    union {
        struct {
            union {
                /// Color hue.
                /// This is an 8-bit value representing an angle around
                /// the color wheel. Where 0 is 0°, and 255 is 358°.
                uint8_t hue;
                uint8_t h;  ///< @copydoc hue
            };
            union {
                /// Color saturation.
                /// This is an 8-bit value representing a percentage.
                uint8_t saturation;
                uint8_t sat;  ///< @copydoc saturation
                uint8_t s;    ///< @copydoc saturation
            };
            union {
                /// Color value (brightness).
                /// This is an 8-bit value representing a percentage.
                uint8_t value;
                uint8_t val;  ///< @copydoc value
                uint8_t v;    ///< @copydoc value
            };
        };
        /// Access the hue, saturation, and value data as an array.
        /// Where:
        /// * `raw[0]` is the hue
        /// * `raw[1]` is the saturation
        /// * `raw[2]` is the value
        uint8_t raw[3];
    };
};

/// Representation of an RGB pixel (Red, Green, Blue)
struct CRGB {
    union {
        struct {
            union {
                uint8_t r;    ///< Red channel value
                uint8_t red;  ///< @copydoc r
            };
            union {
                uint8_t g;      ///< Green channel value
                uint8_t green;  ///< @copydoc g
            };
            union {
                uint8_t b;     ///< Blue channel value
                uint8_t blue;  ///< @copydoc b
            };
        };
        /// Access the red, green, and blue data as an array.
        /// Where:
        /// * `raw[0]` is the red value
        /// * `raw[1]` is the green value
        /// * `raw[2]` is the blue value
        uint8_t raw[3];
    };
};
#endif //LED_H