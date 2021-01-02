/***************************************************
  Arduino TFT graphics library targeted at ESP8266
  and ESP32 based boards.

  This is a standalone library that contains the
  hardware driver, the graphics functions and the
  proportional fonts.

  The built-in fonts 4, 6, 7 and 8 are Run Length
  Encoded (RLE) to reduce the FLASH footprint.

  Last review/edit by Bodmer: 26/01/20
 ****************************************************/


#pragma once
#define TFT_ESPI_VERSION "2.2.20_mx"
 
/***************************************************************************************
**                         Section 1: Load required header files
***************************************************************************************/

//Standard support
#include <Arduino.h>
#include <SPI.h>

/***************************************************************************************
**                         Section 2: Load library and processor specific header files
***************************************************************************************/
// Include header file that defines the fonts loaded, the TFT drivers
// available and the pins to be used, etc, etc
#include <User_Setup_Select.h>

/***************************************************************************************
**                         Section 3: Interface setup
***************************************************************************************/
#ifndef TAB_COLOUR
  #define TAB_COLOUR 0
#endif


// Some ST7789 boards do not work with Mode 0
#if defined(ST7789_DRIVER) || defined(ST7789_2_DRIVER)
  #define TFT_SPI_MODE SPI_MODE3
#else
  #define TFT_SPI_MODE SPI_MODE0
#endif

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

/***************************************************************************************
**                         Section 8: Class member and support functions
***************************************************************************************/
// Swap any type
template <typename T> static inline void
swap_coord(T& a, T& b) { T t = a; a = b; b = t; }

// Class functions and variables
class TFT_eSPI : public Print{

 //--------------------------------------- public ------------------------------------//
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
         
    
  TFT_eSPI(int _W , int _H , int pinCS, int pinDC, int pinRst=-1);
  virtual ~TFT_eSPI()
  {
      
  }
  // init() and begin() are equivalent, begin() included for backwards compatibility
  // Sketch defined tab colour option is for ST7735 displays only
  void     init(uint8_t tc = TAB_COLOUR), begin(uint8_t tc = TAB_COLOUR);

  // These are virtual so the TFT_eSprite class can override them with sprite specific functions
  virtual void     drawPixel(int32_t x, int32_t y, uint32_t color),
                   drawLine(int32_t xs, int32_t ys, int32_t xe, int32_t ye, uint32_t color),
                   drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color),
                   drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color),
                   fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);

  virtual int  height(void);
  virtual int  width(void);

  void     setRotation(uint8_t r); // Set the display image orientation to 0, 1, 2 or 3
  uint8_t  getRotation(void);      // Read the current rotation

  void     invertDisplay(bool i);  // Tell TFT to invert all displayed colours


  // The TFT_eSprite class inherits the following functions (not all are useful to Sprite class
  void     setAddrWindow(int32_t xs, int32_t ys, int32_t w, int32_t h), // Note: start coordinates + width and height
           setWindow(int32_t xs, int32_t ys, int32_t xe, int32_t ye);   // Note: start + end coordinates
  void     setWindowLocked(int32_t x0, int32_t y0, int32_t x1, int32_t y1); // same as above but with begin/end spi 
  // Push (aka write pixel) colours to the TFT (use setAddrWindow() first)
  void     pushColor(uint16_t color),
           pushColor(uint16_t color, uint32_t len),  // Deprecated, use pushBlock()
           pushColors(uint16_t  *data, uint32_t len, bool swap = true), // With byte swap option
           pushColors(uint8_t  *data, uint32_t len); // Deprecated, use pushPixels()


          
  // Graphics drawing
  void     fillScreen(uint32_t color),
           drawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color),
           drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color),
           fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color);


  void     drawCircle(int32_t x, int32_t y, int32_t r, uint32_t color),
           drawCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, uint32_t color),
           fillCircle(int32_t x, int32_t y, int32_t r, uint32_t color),
           fillCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, int32_t delta, uint32_t color),

           drawEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color),
           fillEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, uint16_t color),

           //                 Corner 1               Corner 2               Corner 3
           drawTriangle(int32_t x1,int32_t y1, int32_t x2,int32_t y2, int32_t x3,int32_t y3, uint32_t color),
           fillTriangle(int32_t x1,int32_t y1, int32_t x2,int32_t y2, int32_t x3,int32_t y3, uint32_t color);

  // Image rendering
           // Swap the byte order for pushImage() and pushPixels() - corrects endianness
  void     setSwapBytes(bool swap);
  bool     getSwapBytes(void);


           // Write a block of pixels to the screen which have been read by readRect()
  void     pushRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);

  // Text rendering and font handling support funtions
  void     setCursor(int16_t x, int16_t y),                 // Set cursor for tft.print()
           setCursor(int16_t x, int16_t y, uint8_t font);   // Set cursor and font number for tft.print()

  int16_t  getCursorX(void),                                // Read current cursor x position (moves with tft.print())
           getCursorY(void);                                // Read current cursor y position
  
  void     commandList(const uint8_t *addr); // Send a initialisation sequence to TFT stored in FLASH


  // Colour conversion
           // Convert 8 bit red, green and blue to 16 bits
  uint16_t color565(uint8_t red, uint8_t green, uint8_t blue);

           // Convert 8 bit colour to 16 bits
  uint16_t color8to16(uint8_t color332);
           // Convert 16 bit colour to 8 bits
  uint8_t  color16to8(uint16_t color565);

           // Convert 16 bit colour to/from 24 bit, R+G+B concatenated into LS 24 bits
  uint32_t color16to24(uint16_t color565);
  uint32_t color24to16(uint32_t color888);

           // Alpha blend 2 colours, see generic "alphaBlend_Test" example
           // alpha =   0 = 100% background colour
           // alpha = 255 = 100% foreground colour
  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc);
           // 16 bit colour alphaBlend with alpha dither (dither reduces colour banding)
  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc, uint8_t dither);
           // 24 bit colour alphaBlend with optional alpha dither
  uint32_t alphaBlend24(uint8_t alpha, uint32_t fgc, uint32_t bgc, uint8_t dither = 0);


  void     writeColor(uint16_t color, uint32_t len); // Deprecated, use pushBlock()



  int32_t  cursor_x, cursor_y, padX;       // Text cursor x,y and padding setting

    int     rotation;  // Display rotation (0-3)
    void    drawBitmap(int width, int height, int wx, int wy, int fgcolor, int bgcolor, const uint8_t *data);
    void    drawRLEBitmap(int width, int height, int wx, int wy, int fgcolor, int bgcolor, const uint8_t *data);
    void    setFontFamily(const GFXfont *small, const GFXfont *medium, const GFXfont *big);        
    void    setFontSize(FontSize size);
    int     getStringSize(const char *s);
    void    setTextColor(int color, int bgColor)
    {
        textcolor=color;
        textbgcolor=bgColor;
    }
    void    myDrawString(const char *st, int padd_up_to_n_pixels=0);
    int     myDrawChar(int x, int y, unsigned char c,  int color, int bg,FontInfo &infos);
    
    int     mySquare(int x, int y, int w, int xheight, uint16_t *filler);
    
    FontInfo          fontInfo[3];
    FontInfo          *currentFont;    
    uint32_t          textcolor, textbgcolor;         // Text foreground and background colours

protected:
  
    // Put your highly optimized§platform stuff here
    virtual void              begin_tft_write()=0;
    virtual void              end_tft_write()=0;  
    virtual void              rawWrite8(uint8_t c)=0;
    virtual void              rawWrite16(uint16_t c)=0;
    virtual void              pushBlock(uint16_t color, uint32_t len)=0;
    virtual void              pushPixels(const void* data_in, uint32_t len)=0;
    virtual void              spiwrite(uint8_t c)=0;
    virtual void              writecommand(uint8_t c)=0;
    virtual void              writedata(uint8_t d)=0;
  
  
 //--------------------------------------- private ------------------------------------//
 private:
 
 //-------------------------------------- protected ----------------------------------//
 protected:

  //int32_t  win_xe, win_ye;          // Window end coords - not needed

  int  _init_width, _init_height; // Display w/h as input, used by setRotation()
  int  _width, _height;           // Display w/h as modified by current rotation
 

  bool     _swapBytes; // Swap the byte order for TFT pushImage()
  

  bool     _booted;    // init() or begin() has already run once
  
    // pinout
  int _csPin,_dcPin,_rstPin;
  int  addr_row, addr_col;    

  
  
  
  virtual size_t write(uint8) {return 1;} // stub for print  
}; // End of class TFT_eSPI

// EOF

      
  