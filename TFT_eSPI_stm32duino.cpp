        ////////////////////////////////////////////////////
        //       TFT_eSPI generic driver functions        //
        ////////////////////////////////////////////////////

// This is the part where you want the platform specific / optimized code to be

////////////////////////////////////////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////////////////////////////////////////
#include "TFT_eSPI_stm32duino.h"

/***************************************************************************************
** Function name:           pushBlock - for STM32
** Description:             Write a block of pixels of the same colour
***************************************************************************************/
void TFT_eSPI_stm32duino::myDataSend( uint16_t *data, int len, int minc)
{
    while(len)
    {
        int v;
        if(len>65535) v=65535;
            else v=len;
        len-=v;     
        _spi.dmaSend(data,v,minc);
        if(minc)
            data+=v;
    }   
}
void TFT_eSPI_stm32duino::pushBlock(uint16_t color, uint32_t len)
{
    //color=(color>>8)+((color&0xff)<<8);
    uint16_t colors[1]={color};
    _spi.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, TFT_SPI_MODE,DATA_SIZE_16BIT));
    myDataSend(&color,len,0);
    _spi.endTransaction();
}

/***************************************************************************************
** Function name:           pushPixels - for STM32
** Description:             Write a sequence of pixels
***************************************************************************************/
void TFT_eSPI_stm32duino::pushPixels(const void* data_in, uint32_t len)
{
  _spi.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, TFT_SPI_MODE,DATA_SIZE_16BIT));
  uint16_t *data = (uint16_t*)data_in;
  myDataSend(data,len,2);
  _spi.endTransaction();
}


/***************************************************************************************
** Function name:           spiwrite
** Description:             Write 8 bits to SPI port (legacy support only)
***************************************************************************************/
void TFT_eSPI_stm32duino::spiwrite(uint8_t c)
{
  begin_tft_write();
  tft_Write_8(c);
  end_tft_write();
}


/***************************************************************************************
** Function name:           writecommand
** Description:             Send an 8 bit command to the TFT
***************************************************************************************/
void TFT_eSPI_stm32duino::writecommand(uint8_t c)
{
  begin_tft_write();

  DC_C;

  tft_Write_8(c);

  DC_D;

  end_tft_write();

}


/***************************************************************************************
** Function name:           writedata
** Description:             Send a 8 bit data value to the TFT
***************************************************************************************/
void TFT_eSPI_stm32duino::writedata(uint8_t d)
{
  begin_tft_write();

  DC_D;        // Play safe, but should already be in data mode

  tft_Write_8(d);

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

