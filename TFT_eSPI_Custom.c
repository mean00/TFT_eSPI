        ////////////////////////////////////////////////////
        //       TFT_eSPI generic driver functions        //
        ////////////////////////////////////////////////////

// This is the part where you want the platform specific / optimized code to be

////////////////////////////////////////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////////////////////////////////////////

// Select the SPI port to use
SPIClass& spi = SPI;


////////////////////////////////////////////////////////////////////////////////////////
//                   Standard SPI 16 bit colour TFT                               
////////////////////////////////////////////////////////////////////////////////////////

/***************************************************************************************
** Function name:           pushBlock - for STM32
** Description:             Write a block of pixels of the same colour
***************************************************************************************/
void TFT_eSPI::pushBlock(uint16_t color, uint32_t len)
{
    //color=(color>>8)+((color&0xff)<<8);
    uint16_t colors[1]={color};
    spi.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, TFT_SPI_MODE,DATA_SIZE_16BIT));

    while(len)
    {
        int v;
        if(len>65535) v=65535;
            else v=len;
        len-=v;
        spi.dmaSend(colors,v,0);
    }   
    spi.endTransaction();
}

/***************************************************************************************
** Function name:           pushPixels - for STM32
** Description:             Write a sequence of pixels
***************************************************************************************/
void TFT_eSPI::pushPixels(const void* data_in, uint32_t len)
{
  spi.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, TFT_SPI_MODE,DATA_SIZE_16BIT));
  uint16_t *data = (uint16_t*)data_in;

   while(len)
   {
        int v;
        if(len>65535) v=65535;
            else v=len;
        len-=v;
  
        spi.dmaSend(data_in,v,2);
        data_in+=v;
   }
  spi.endTransaction();
}

