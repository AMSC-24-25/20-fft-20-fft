#!/bin/bash

echo "~ Export configuration ./resources/sample-config.json ~"
CONFIG_FILE_PATH_FFT=$(pwd)/resources/sample-config.json
export CONFIG_FILE_PATH_FFT
echo ""

echo "~ Compile CMakeLists.txt in $(pwd)/build ~"
mkdir -p "build" && cd "build" || exit 1
cmake .. --log-level=ERROR || exit 1
echo ""


echo "~ Compile main ~"
start_time=$(date +%s)
make --silent -f ./Makefile -C . parallel_build || exit 1
end_time=$(date +%s)
elapsed_time=$((end_time - start_time))
echo "Built in ~$elapsed_time seconds"
echo ""

echo "~ Run Main ~"
./main
cd ..
