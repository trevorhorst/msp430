cmake_minimum_required(VERSION 3.5)

project(
    i2c
    DESCRIPTION "I2C Serial Communication Library"
    LANGUAGES C ASM
    )

set( HEADERS
    ${HEADERS}
    include/i2c/i2c.h
    )

set( SOURCE
    ${SOURCE}
    src/i2c.c
    )

add_library(
    ${PROJECT_NAME} STATIC
    ${HEADERS}
    ${SOURCE}
    )

target_link_libraries(
    ${PROJECT_NAME}
    )

# Specifies include directories to use when compiling a given target
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )
