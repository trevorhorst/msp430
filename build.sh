#!/bin/bash
rm -rf build
mkdir -p build
# (cd build && CC=msp430-gcc ASM=msp430-as cmake .. && make)
(cd build && CC=msp430-gcc CXX=msp430-g++ ASM=msp430-as cmake .. && make)
