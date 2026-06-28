cmake_minimum_required(VERSION 3.15)

# 1. Identify the Target System
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR msp430)

# 2. Define the MCU variable if not already provided
if(NOT DEFINED MCU)
    set(MCU "msp430fr2433" CACHE STRING "Target MSP430 MCU type")
    message(STATUS "No MCU specified. Defaulting to ${MCU}")
endif()

# 3. Define Toolchain Paths (Adjust this path to match your installation)
set(TOOLCHAIN_PREFIX "$ENV{HOME}/ti/msp430-gcc")

set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}/bin/msp430-elf-gcc")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}/bin/msp430-elf-gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}/bin/msp430-elf-g++")
set(CMAKE_OBJCOPY "${TOOLCHAIN_PREFIX}/bin/msp430-elf-objcopy" CACHE INTERNAL "")
set(CMAKE_SIZE_UTIL "${TOOLCHAIN_PREFIX}/bin/msp430-elf-size" CACHE INTERNAL "")

# Prevent CMake from executing test compilation checks (breaks on bare-metal easily)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# 4. Target Specific Flags
# Crucial: Specify the compiler include path where msp430.h and msp430fr2433.ld reside
set(MCU_SUPPORT_DIR "${TOOLCHAIN_PREFIX}/include")

include_directories(${MCU_SUPPORT_DIR})

add_compile_options(-mmcu=${MCU} -I${MCU_SUPPORT_DIR} -O2 -Wall -Wextra -pedantic -g)
add_link_options(-mmcu=${MCU} -L${MCU_SUPPORT_DIR} -Wl,-Map=${MCU}.map)
