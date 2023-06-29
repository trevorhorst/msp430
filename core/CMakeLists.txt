cmake_minimum_required(VERSION 3.5)

project(
    core
    VERSION 0.0.5
    LANGUAGES C ASM
    )

if(USE_GPIO)
    set(EXTRA_SOURCE ${EXTRA_SOURCE}
        src/gpio.c
    )

    set(EXTRA_INCLUDE ${EXTRA_INCLUDE}
        include/core/gpio.h
    )
endif()

set(SOURCE ${SOURCE}
    src/font.s
    src/i2c.c
    src/uart.c
    src/ssd1306.c
    src/ring_buffer.c
    src/sht3xdis.c
    ${EXTRA_SOURCE}
    )

set(INCLUDE ${INCLUDE}
    include/core/resources.h
    include/core/i2c.h
    include/core/uart.h
    include/core/ssd1306.h
    include/core/ring_buffer.h
    include/core/sht3xdis.h
    ${EXTRA_INCLUDE}
    )

add_library(
    ${PROJECT_NAME}
    STATIC
        ${SOURCE}
        ${INCLUDE}
    )

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        hwlib
    )

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/resources>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )
