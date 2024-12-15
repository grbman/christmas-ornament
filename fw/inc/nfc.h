#ifndef NFC_H
#define NFC_H

#include <avr/io.h>
#include <stdbool.h>
#include "pindefs.h"

#define NDEF_TAG_MAGIC_NUM 0x03
#define TEXT_MAGIC_NUM 'T'

#define NFC_CCFILE_OFFSET 0x00
#define NFC_TLV_TAG_OFFSET 0x04
#define NFC_TLV_LEN_OFFSET 0x05
#define NFC_TLV_VAL_OFFSET 0x06

#define STRNCMP_LEN 32U

typedef struct nfc_ndef_text_headers
{
    uint8_t ccfile[4];
    uint8_t nfc_tag;
    uint8_t nfc_len;
    uint8_t ndef_record_1st;
    uint8_t ndef_type_len;
    uint8_t ndef_payload_len;
    uint8_t text_magic_num;
    uint8_t text_lang_type_len; //assuming english
} headers_t;


#endif //NFC_H