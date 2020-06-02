cmake_minimum_required(VERSION 3.5)
project( msp430elf LANGUAGES C )

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(
    ${PROJECT_NAME}
    common
    )   

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CONTROL_BIN_DIR})
