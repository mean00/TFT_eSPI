include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(SRCS
        TFT_eSPI.cpp
        TFT_eSPI_draw.cpp
        TFT_eSPI_extended.cpp
        TFT_eSPI_extended_glyph.cpp
        TFT_eSPI_extended_glyph_hl.cpp
        TFT_eSPI_stm32duino.cpp
)
generate_arduino_library(${libPrefix}TFT_eSPI 
                            SRCS ${SRCS}
                            BOARD ${ARDUINO_DEFAULT_BOARD}
                        )
                            
target_compile_definitions(${libPrefix}TFT_eSPI PRIVATE  ltoa=itoa CUSTOM_DRIVER=1)
EXPORT_LIBRARY(TFT_eSPI FLAGS "" INCLUDE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}" NAME "${libPrefix}TFT_eSPI")
