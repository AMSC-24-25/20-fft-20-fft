#!/bin/bash

echo "~ Export configuration ./resources/sample-config.json ~"
export CONFIG_FILE_PATH_FFT=$(pwd)/resources/sample-config.json
echo ""

echo "~ Compile CMakeLists.txt ~"
cmake . --log-level=WARNING || exit 1
echo ""

echo "~ Compile main ~"
make -f ./Makefile -C . parallel_build || exit 1
echo ""

echo "~ Run Main ~"
./main