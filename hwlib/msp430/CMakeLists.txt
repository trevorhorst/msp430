cmake_minimum_required(VERSION 3.5)

project(
    hwlib
    VERSION 0.0.4
    DESCRIPTION "Hardware Library"
    LANGUAGES C ASM
    )

set(
    INCLUDE ${INCLUDE}
    include/hwlib/gpio.h
    include/hwlib/i2c.h
    include/hwlib/uart.h
    )

set(
    SOURCE ${SOURCE}
    src/gpio.c
    src/i2c.c
    src/uart.c
    )

add_library(
    ${PROJECT_NAME}
    STATIC
        ${INCLUDE}
        ${SOURCE}
    )

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )
