cmake_minimum_required(VERSION 2.8)

# Add compiler options
set( CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -mmcu=msp430g2553 -0g -Wall -g -std=c99"
    )

project(memtest LANGUAGES C ASM)

set( SOURCE ${SOURCE}
    src/font.s
    src/i2c.c
    src/ssd1306.c
    )

set( INCLUDE ${INCLUDE}
    include/msp430/resources.h
    include/msp430/i2c.h
    include/msp430/ssd1306.h
    )

add_executable( ${PROJECT_NAME} ${SOURCE} ${INCLUDE} main.c )

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/resources>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )
