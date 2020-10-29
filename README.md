This is a modified version of TFT_eSPI
Main changes :
* Simplified the code a  lot, removed tons of features
* Separated platform specific code as derived class and not ifdef
* Adapted to Arduino_stm32/rclarck+freeRtos, DMA spi as one of the derived class
* Ported the "better" glyph drawing algorithm
* Ported simple RLE image display
* Try to avoid blocking on dma transfer, yield CPU meanwhile (stm32duino specific)
* As it is the library *is* re-entrant , *but* if you make several simultaneous calls you may have
   corruption. This is especially true for myDrawString
