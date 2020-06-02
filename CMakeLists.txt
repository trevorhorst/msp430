cmake_minimum_required(VERSION 3.5)

# Add compiler options
set( CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -mmcu=msp430g2553 -0g -Wall -g -std=c99"
    )

project(msp430 LANGUAGES C ASM)

add_subdirectory(app)
add_subdirectory(common)
# add_subdirectory(hardware/${CONTROL_PROJECT})