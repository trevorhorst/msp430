cmake_minimum_required(VERSION 2.8)
project(flash_mem
    LANGUAGES C ASM
    )

# option( USE_I2C "Use optional I2C library" ON )
option( USE_UART "Use optional UART library" ON )
# option( USE_SSD1306 "Use optional SSD1306 library" ON )

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
    uart
    # ssd1306
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
