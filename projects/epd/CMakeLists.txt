cmake_minimum_required(VERSION 3.5)
project(epd
    LANGUAGES C ASM
    )

# option( USE_I2C "Use optional I2C library" ON )
# option( USE_UART "Use optional UART library" ON )
option( USE_GPIO " Use optional GPIO library" ON )
option( USE_SPI "Use optional SPI library" ON )

# List of source files
set( SOURCE
    ${SOURCE}
    src/project.c
    )

# List of header files
set( HEADERS
    ${HEADERS}
    include/project/project.h
    )

add_library(
    ${PROJECT_NAME} STATIC
    ${SOURCE}
    ${HEADERS}
    )

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    core
    )

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        $ENV{HOME}/ti/msp430-gcc/include
)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CONTROL_LIB_DIR}
    ARCHIVE DESTINATION ${CONTROL_LIB_DIR}
    )
