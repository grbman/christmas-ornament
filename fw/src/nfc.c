#include <stddef.h>
#include "i2c_bitbang.h"
#include "st25dv.h"
#include "nfc.h"
#include "led.h"
headers_t headers = {0};
uint8_t *ptr = (uint8_t *)&headers;
uint8_t data[64];

extern void (*leds_next)();

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
        // printf("string: %s\r\n", data);

        strtok_ptr = data;
        // Returns first token
        char* token = strtok_r(strtok_ptr, " ", &strtok_ptr);
        //  parse_params(token);
        // Keep printing tokens while one of the
        // delimiters present in str[].
        while (token != NULL) {
            // printf(" % s\n", token);
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
    // printf("? %c%c\n", hex_str[0], hex_str[1]);
    uint8_t result = 0;

    // Parse each character (hex_str[0] and hex_str[1])
    for (int i = 2; i > 0; i--) {
        char c = hex_str[2-i];
    // printf("%c\n", c);

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
        // printf("res: %d\n", result);

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

    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL) 
    {
        // printf(" % s\n", token);
        arg = token;

        token = strtok_r(NULL, ":", &strtok_ptr);
    }
    // printf("param: %s arg: %s\n", param, arg);
    if(strncasecmp("mode", param, 32) == 0)
    {
        if(strncasecmp("rainbow", arg, 32) == 0)
        {
           leds_next = mode_rainbow;
        } 
        else if(strncasecmp("manual", arg, 32) == 0)
        {
           leds_next = mode_manual;
        }
        // printf("mode found\n");

    } 
    else if(strncasecmp("color", param, 32) == 0)
    {
        struct CRGB led;
        if(hex_to_uint8(&arg[0], &led.r) && hex_to_uint8(&arg[2], &led.g) && hex_to_uint8(&arg[4], &led.b))
        {
            update_manual_color(led);
        }

    }
    else if(strncasecmp("power", param, 32) == 0)
    {
        // printf("power found\n");

    }
     else if(strncasecmp("pattern", param, 32) == 0)
    {
        if(strncasecmp("glow", arg, 32) == 0)
        {

        }
        // printf("power found\n");

    }
}
