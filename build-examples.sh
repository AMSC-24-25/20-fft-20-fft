#!/bin/bash

echo "~ Compile CMakeLists.txt in $(pwd)/build ~"
mkdir -p "build" && cd "build" || exit 1
# check if ninja is installed
if ! command -v ninja &> /dev/null
then
    echo "Ninja is not installed. It is highly recommended to install it for faster builds: sudo apt install ninja-build"
    cmake .. --preset make-dev --log-level=ERROR || exit 1
    cd make-dev
else
    echo "Ninja is installed. Using it for faster builds."
    cmake .. --preset ninja-dev --log-level=ERROR || exit 1
    cd ninja-dev
fi
echo ""


echo "~ Compile project + examples ~"
start_time=$(date +%s)
if command -v ninja &> /dev/null
then
    ninja --quiet all || exit 1
else
    make --silent -j $(nproc) all || exit 1
fi
end_time=$(date +%s)
elapsed_time=$((end_time - start_time))
echo "Built in ~$elapsed_time seconds"
echo ""

if command -v ninja &> /dev/null
then
    echo "Run examples using ./build/ninja-dev/examples/example-name"
else
    echo "Run examples using ./build/make-dev/examples/example-name"
fi