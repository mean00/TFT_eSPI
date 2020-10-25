
#pragma once
#include "MapleFreeRTOS1000_pp.h"
#include "TFT_eSPI.h"
extern int tftLocked;

class  TFT_eSPI_stm32duino : public TFT_eSPI
{
public:    
    TFT_eSPI_stm32duino(SPIClass &spi, xMutex *tex,int _W , int _H , int pinCS, int pinDC, int pinRst=-1);
    void txDone();
public:    
    void spiLock() {_tex->lock();tftLocked++;}
    void spiUnlock() {tftLocked--;if(tftLocked<0) xAssert(0);_tex->unlock();}
    
    void              myDataSend(uint16_t *data, int len, int minc);
    void              pushBlock(uint16_t color, uint32_t len);
    void              pushPixels(const void* data_in, uint32_t len);
    
    void              spiwrite(uint8_t c);
    void              writecommand(uint8_t c);
    void              writedata(uint8_t d);
    
    void              rawWrite8(uint8_t c);
    void              rawWrite16(uint16_t c);

    xMutex            *_tex;


    /**
     * 
     */
    void begin_tft_write()
    {
        spiLock();
        _spi.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, TFT_SPI_MODE));
        CS_L; 
    }  
    /**
     * 
     */
    void end_tft_write()  
    {       
        CS_H;
        _spi.endTransaction();
        spiUnlock();
    }
    
    SPIClass            &_spi;
    xBinarySemaphore    *_sem;
};

// eof
