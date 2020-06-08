cmake_minimum_required(VERSION 3.5)

project( msp430.elf LANGUAGES C ASM )

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(
    ${PROJECT_NAME}
    ${TARGET_PROJECT}
    common
    )   
