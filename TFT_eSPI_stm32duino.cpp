/**
 Low level platform specific stuff
 */
#include "TFT_eSPI_stm32duino.h"

TFT_eSPI_stm32duino *instance=NULL;
/**
 * 
 */
void txCallback()
{
    if(instance)
        instance->txDone();
}

/**
 */
void TFT_eSPI_stm32duino::txDone()
{
   _sem->giveFromInterrupt();
}


/**
 * 
 * @param spi
 * @param tex
 * @param _W
 * @param _H
 * @param pinCS
 * @param pinDC
 * @param pinRst
 */

TFT_eSPI_stm32duino::TFT_eSPI_stm32duino(SPIClass &spi, xMutex *tex,int _W , int _H , int pinCS, int pinDC, int pinRst)  :  TFT_eSPI(_W ,   _H ,   pinCS,   pinDC,   pinRst),_spi(spi),_tex(tex)
{
    instance=this;
    _sem=new xBinarySemaphore;
}
/**
 * 
 * @param data
 * @param len
 * @param minc
 */
void TFT_eSPI_stm32duino::myDataSend( uint16_t *data, int len, int minc)
{
    while(len)
    {
        int v;
        if(len>65535) v=65535;
            else v=len;
        len-=v;     
        if(v>1000) // more than ~ 500 us
        {
            _spi.onTransmit(txCallback);
            _spi.dmaSendAsync(data,v,minc); 
            _sem->take();
            _spi.onTransmit(NULL);
            
        }else
        {
            _spi.dmaSend(data,v,minc); // just send and wait
        }        
        if(minc)
            data+=v;
    }   
}
/**
 * 
 * @param color
 * @param len
 */
void TFT_eSPI_stm32duino::pushBlock(uint16_t color, uint32_t len)
{
    //color=(color>>8)+((color&0xff)<<8);
    uint16_t colors[1]={color};
    _spi.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, TFT_SPI_MODE,DATA_SIZE_16BIT));
    myDataSend(&color,len,0);
    _spi.endTransaction();
}

/**
 * 
 * @param data_in
 * @param len
 */
void TFT_eSPI_stm32duino::pushPixels(const void* data_in, uint32_t len)
{
  _spi.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, TFT_SPI_MODE,DATA_SIZE_16BIT));
  uint16_t *data = (uint16_t*)data_in;
  myDataSend(data,len,2);
  _spi.endTransaction();
}

/**
 * 
 * @param c
 */
void TFT_eSPI_stm32duino::spiwrite(uint8_t c)
{
  begin_tft_write();
  tft_Write_8(c);
  end_tft_write();
}

/**
 * 
 * @param c
 */
void TFT_eSPI_stm32duino::writecommand(uint8_t c)
{
  begin_tft_write();
  DC_C;
  _spi.transfer(c);
  DC_D;
  end_tft_write();
}


/**
 * 
 * @param d
 */
void TFT_eSPI_stm32duino::writedata(uint8_t d)
{
  begin_tft_write();
  DC_D;        // Play safe, but should already be in data mode
  _spi.transfer(d);
  CS_L;        // Allow more hold time for low VDI rail
  end_tft_write();
}
/**
 * 
 * @param c
 */
void              TFT_eSPI_stm32duino::rawWrite8(uint8_t c)
{
    _spi.transfer(c);
}
/**
 * 
 * @param c
 */
void              TFT_eSPI_stm32duino::rawWrite16(uint16_t c)
{
    _spi.transfer16(c);
}
// EOF

