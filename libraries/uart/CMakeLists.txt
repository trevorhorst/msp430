cmake_minimum_required(VERSION 3.5)

project(
    uart
    DESCRIPTION "UART Serial Communication Library"
    LANGUAGES C ASM
    )

set( HEADERS
    ${HEADERS}
    include/uart/uart.h
    )

set( SOURCE
    ${SOURCE}
    src/uart.c
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
