#include <stddef.h>
#include "i2c_bitbang.h"
#include "st25dv.h"
#include "nfc.h"
#include "led.h"
headers_t headers = {0};
uint8_t *ptr = (uint8_t *)&headers;
uint8_t data[96];

extern void (*leds_next)();
extern void (*brightness_next)(uint8_t *);

void read_params()
{
    char *strtok_ptr;
    if(st25_read_user_reg(0x00, sizeof(headers), &headers) == false)
    {
        return;
    }

    if(headers.nfc_tag != NDEF_TAG_MAGIC_NUM)
    {
        return;
    }
    uint8_t text_addr, text_len;
    if(headers.text_magic_num == TEXT_MAGIC_NUM)
    {
        text_addr = offsetof(headers_t, text_lang_type_len) + 1 + headers.text_lang_type_len;
        text_len = headers.ndef_payload_len - (1 + headers.text_lang_type_len);
        st25_read_user_reg(text_addr, text_len, data);
        data[text_len] = 0;

        strtok_ptr = data;
        // Returns first token
        char* token = strtok_r(strtok_ptr, " ", &strtok_ptr);
        // Keep printing tokens while one of the
        // delimiters present in str[].
        while (token != NULL) {
            brightness_next = NULL;
            parse_params(token);

            token = strtok_r(NULL, " ", &strtok_ptr);

            }
    }
    else
    {
        return;
    }
}

bool hex_to_uint8(const char *hex_str, uint8_t *val) {
    if (hex_str == NULL) {
        // Handle error if the string length is not exactly 2 characters
        return false;
    }
    uint8_t result = 0;

    // Parse each character (hex_str[0] and hex_str[1])
    for (int i = 2; i > 0; i--) {
        char c = hex_str[2-i];

        if (c >= '0' && c <= '9') {
            result += ((c - '0') * ((i-1)> 0 ? 16 : 1));  // '0' -> 0, '1' -> 1, ..., '9' -> 9
        } else if (c >= 'A' && c <= 'F') {
            result += ((c - 'A' + 10) * ((i-1)> 0 ? 16 : 1));  // 'A' -> 10, ..., 'F' -> 15
        } else if (c >= 'a' && c <= 'f') {
            result += ((c - 'a' + 10) * ((i-1)> 0 ? 16 : 1));  // 'a' -> 10, ..., 'f' -> 15
        } else {
            // Invalid character in hex string
            return false;
        }

    }
    *val = result;
    return true;
}


void parse_params(char *str)
{
    char *strtok_ptr = str;

    // Returns first token
    char* token = strtok_r(strtok_ptr, ":", &strtok_ptr);
    char* param = token;
    char* arg;
    bool pattern_found = false;
    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL) 
    {
        arg = token;

        token = strtok_r(NULL, ":", &strtok_ptr);
    }
    if(strncasecmp("mode", param, STRNCMP_LEN) == 0)
    {
        if(strncasecmp("rainbow", arg, STRNCMP_LEN) == 0)
        {
           leds_next = mode_rainbow;
        } 
        else if(strncasecmp("custom", arg, STRNCMP_LEN) == 0)
        {
           leds_next = mode_manual;
        }

    } 
    else if(strncasecmp("left", param, STRNCMP_LEN) == 0)
    {
        struct CRGB led = {0};
        if(strncasecmp("red", arg, STRNCMP_LEN) == 0)
        {
            led.r = 255;
            led.g = 0;
            led.b = 0;
            update_manual_color_left(led);
        }
        else if(strncasecmp("orange", arg, STRNCMP_LEN) == 0)
        {
            led.r = 255;
            led.g = 50;
            led.b = 0;
            update_manual_color_left(led);
        }
        else if(strncasecmp("yellow", arg, STRNCMP_LEN) == 0)
        {
            led.r = 255;
            led.g = 100;
            led.b = 0;
            update_manual_color_left(led);
        }
        else if(strncasecmp("green", arg, STRNCMP_LEN) == 0)
        {
            led.r = 0;
            led.g = 255;
            led.b = 0;

            update_manual_color_left(led);
        }
        else if(strncasecmp("blue", arg, STRNCMP_LEN) == 0)
        {
            led.r = 0;
            led.g = 0;
            led.b = 255;
            update_manual_color_left(led);
        }
        else if(strncasecmp("cyan", arg, STRNCMP_LEN) == 0)
        {
            led.r = 0;
            led.g = 255;
            led.b = 255;
            update_manual_color_left(led);
        }
        else if(strncasecmp("purple", arg, STRNCMP_LEN) == 0)
        {
            led.r = 127;
            led.g = 0;
            led.b = 127;
            update_manual_color_left(led);

        }
        else if(hex_to_uint8(&arg[0], &led.r) && hex_to_uint8(&arg[2], &led.g) && hex_to_uint8(&arg[4], &led.b))
        {
            update_manual_color_left(led);
        }

    }
    else if(strncasecmp("right", param, STRNCMP_LEN) == 0)
    {
        struct CRGB led;
        if(strncasecmp("red", arg, STRNCMP_LEN) == 0)
        {
            led.r = 255;
            led.g = 0;
            led.b = 0;
            update_manual_color_right(led);
        }
        else if(strncasecmp("orange", arg, STRNCMP_LEN) == 0)
        {
            led.r = 255;
            led.g = 50;
            led.b = 0;
            update_manual_color_right(led);
        }
        else if(strncasecmp("yellow", arg, STRNCMP_LEN) == 0)
        {
            led.r = 255;
            led.g = 100;
            led.b = 0;
            update_manual_color_right(led);
        }
        else if(strncasecmp("green", arg, STRNCMP_LEN) == 0)
        {
            led.r = 0;
            led.g = 255;
            led.b = 0;

            update_manual_color_right(led);
        }
        else if(strncasecmp("blue", arg, STRNCMP_LEN) == 0)
        {
            led.r = 0;
            led.g = 0;
            led.b = 255;
            update_manual_color_right(led);
        }
        else if(strncasecmp("cyan", arg, STRNCMP_LEN) == 0)
        {
            led.r = 0;
            led.g = 255;
            led.b = 255;
            update_manual_color_right(led);
        }
        else if(strncasecmp("purple", arg, STRNCMP_LEN) == 0)
        {
            led.r = 127;
            led.g = 0;
            led.b = 127;
            update_manual_color_right(led);

        }
        else if(hex_to_uint8(&arg[0], &led.r) && hex_to_uint8(&arg[2], &led.g) && hex_to_uint8(&arg[4], &led.b))
        {
            update_manual_color_right(led);
        }

    }
    else if(strncasecmp("power", param, STRNCMP_LEN) == 0)
    {
        
         if(strncasecmp("low", arg, STRNCMP_LEN) == 0)
        {
            power_set(32);
        }
        else if(strncasecmp("med", arg, STRNCMP_LEN) == 0)
        {
            power_set(64);
        }
        else if(strncasecmp("high", arg, STRNCMP_LEN) == 0)
        {
            power_set(255);
        }

    }
     else if(strncasecmp("pattern", param, STRNCMP_LEN) == 0)
    {
        if(strncasecmp("glow", arg, STRNCMP_LEN) == 0)
        {
            brightness_next = brightness_glow;
        }
        else if(strncasecmp("pingpong", arg, STRNCMP_LEN) == 0)
        {
            brightness_next = brightness_pingpong;
        }
    }
}
