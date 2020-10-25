
#pragma once
#include "MapleFreeRTOS1000_pp.h"
#include "TFT_eSPI.h"

extern int tftLocked;

class  TFT_eSPI_extended : public TFT_eSPI
{
public:    
   typedef enum FontSize
   {
    SmallFont,MediumFont,BigFont
   };
   class FontInfo
    {
    public:
      int               maxHeight;          
      int               maxWidth;
      uint16_t         *filler;
      const GFXfont    *font;        
    };
      
    TFT_eSPI_extended(SPIClass &spi,xMutex *tex,int _W  , int _H, int pinCS, int pinDC, int pinRst) : TFT_eSPI(_W,_H, pinCS,pinDC,pinRst),_spi(spi)
    {
        this->tex=tex;
        currentFont=NULL;
        textcolor=ILI9341_WHITE;
        textbgcolor=ILI9341_BLACK;        
    }
    void drawBitmap(int width, int height, int wx, int wy, int fgcolor, int bgcolor, const uint8_t *data);
    void drawRLEBitmap(int width, int height, int wx, int wy, int fgcolor, int bgcolor, const uint8_t *data);
    void  setFontFamily(const GFXfont *small, const GFXfont *medium, const GFXfont *big);        
    void  setFontSize(FontSize size);
    void    setTextColor(int color, int bgColor)
    {
        textcolor=color;
        textbgcolor=bgColor;
    }
    void  myDrawString(const char *st, int padd_up_to_n_pixels=0);
    
protected:    
    xMutex *tex;
    void spiLock() {tex->lock();tftLocked++;}
    void spiUnlock() {tftLocked--;if(tftLocked<0) xAssert(0);tex->unlock();}
    int myDrawChar(int x, int y, unsigned char c,  int color, int bg,FontInfo &infos);
    
    int mySquare(int x, int y, int w, int xheight, uint16_t *filler);
    
    FontInfo          fontInfo[3];
    FontInfo          *currentFont;    
    uint32_t          textcolor, textbgcolor;         // Text foreground and background colours
    void              myDataSend(SPIClass &mySpi, uint16_t *data, int len, int minc);
    void              pushBlock(uint16_t color, uint32_t len);
    void              pushPixels(const void* data_in, uint32_t len);
    
    void                spiwrite(uint8_t c);
    void                writecommand(uint8_t c);
    void                writedata(uint8_t d);
    SPIClass            &_spi;
};

// eof
