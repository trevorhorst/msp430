cmake_minimum_required(VERSION 3.5)

project(
    hwlib
    VERSION 0.0.1
    DESCRIPTION "Hardware Library"
    LANGUAGES C ASM
    )

set(
    INCLUDE ${INCLUDE}
    include/hwlib/gpio.h
    )

set(
    SOURCE ${SOURCE}
    src/gpio.c
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
