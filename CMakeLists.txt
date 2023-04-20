cmake_minimum_required(VERSION 3.5)

# Add compiler options, this needs to be setup before the project call
set( CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -mmcu=msp430g2553 -std=c99"
    )

project(
    msp430
    DESCRIPTION "MSP430 Project"
    LANGUAGES C ASM
    )

if(DEFINED ENV{TARGET_PROJECT})
    set( TARGET_PROJECT "$ENV{TARGET_PROJECT}" )
else()
    message("No project set, building test project")
    set( TARGET_PROJECT "test" )
endif()
message("Building ${TARGET_PROJECT}")

if(DEFINED ENV{TARGET_HWLIB})
    set( TARGET_HWLIB "$ENV{TARGET_HWLIB}" )
else()
    message("No hwlib set.")
endif()

add_subdirectory(app)
add_subdirectory(projects/${TARGET_PROJECT})
add_subdirectory(core)
add_subdirectory(hwlib/${TARGET_HWLIB})

if( USE_GPIO )
    message( "Using GPIO Library" )
endif()

if( USE_I2C )
    message( "Using I2C Library" )
    add_subdirectory( libraries/i2c )
endif()

if( USE_UART )
    message( "Using UART Library" )
    add_subdirectory( libraries/uart )
endif()

if( USE_SSD1306 )
    message( "Using SSD1306 Library" )
    add_subdirectory( libraries/ssd1306 )
endif()

# install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CONTROL_BIN_DIR})

