cmake_minimum_required(VERSION 3.5)

project( msp430.elf LANGUAGES C ASM )

add_executable(${PROJECT_NAME} main.c)

# target_include_directories(
#     ${PROJECT_NAME}
#     PRIVATE
#         $ENV{HOME}/ti/msp430-gcc/include
# )
#
#
# target_link_options(msp430.elf PRIVATE
#     "-L/home/thorst/ti/msp430-gcc/include"
#     # "-Tmsp430fr2433.ld"
# )

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        ${TARGET_PROJECT}
        # core
    )
