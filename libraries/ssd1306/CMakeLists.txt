cmake_minimum_required(VERSION 3.5)

project(
    ssd1306
    DESCRIPTION "SSD1306 Library"
    LANGUAGES C ASM
    )

set( HEADERS
    ${HEADERS}
    include/ssd1306/ssd1306.h
    )

set( SOURCE
    ${SOURCE}
    src/ssd1306.c
    )

add_library(
    ${PROJECT_NAME} STATIC
    ${HEADERS}
    ${SOURCE}
    )

target_link_libraries(
    ${PROJECT_NAME}
    i2c
    )

# Specifies include directories to use when compiling a given target
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )
