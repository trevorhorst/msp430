cmake_minimum_required(VERSION 3.5)
project(common LANGUAGES C ASM)

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

add_library( ${PROJECT_NAME} ${SOURCE} ${INCLUDE} )

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/resources>
    $<INSTALL_INTERFACE:include>
    )
