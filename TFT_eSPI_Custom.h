        ////////////////////////////////////////////////////
        //       TFT_eSPI generic driver functions        //
        ////////////////////////////////////////////////////

// This is a generic driver for Arduino boards, it supports SPI interface displays
// 8 bit parallel interface to TFT is not supported for generic processors

#pragma once

// Initialise processor specific SPI functions, used by init()
#define INIT_TFT_DATA_BUS


#define DC_C digitalWrite(_dcPin, LOW)
#define DC_D digitalWrite(_dcPin, HIGH)
#define CS_L digitalWrite(_csPin, LOW)
#define CS_H digitalWrite(_csPin, HIGH)

#define tft_Write_8(C)   rawWrite8(C)
#define tft_Write_16(C)  rawWrite16(C)
#define tft_Write_16S(C) rawWrite8(((C)>>8) | ((C)<<8))

#define tft_Write_32(C) \
tft_Write_16((uint16_t) ((C)>>16)); \
tft_Write_16((uint16_t) ((C)>>0))

#define tft_Write_32C(C,D) \
tft_Write_16((uint16_t) (C)); \
tft_Write_16((uint16_t) (D))

#define tft_Write_32D(C) \
tft_Write_16((uint16_t) (C)); \
tft_Write_16((uint16_t) (C))


// EOF