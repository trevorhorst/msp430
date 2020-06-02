cmake_minimum_required(VERSION 3.5)

project(common LANGUAGES C ASM )

set( SOURCE ${SOURCE}
    src/font.s
    src/i2c.c
    src/ssd1306.c
    )

set( INCLUDE ${INCLUDE}
    include/common/resources.h
    include/common/i2c.h
    include/common/ssd1306.h
    )

add_library( ${PROJECT_NAME} STATIC ${SOURCE} ${INCLUDE} )

target_link_libraries(
    ${PROJECT_NAME}
    )

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/resources>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )
