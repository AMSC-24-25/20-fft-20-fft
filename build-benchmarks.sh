#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

./build-essential.sh || exit 1

echo -e "${GREEN}~ Install Google Benchmark ~${NC}"
if ls /usr/local/lib/libbenchmark* &> /dev/null && ls /usr/local/include/benchmark/ &> /dev/null
then
    echo "Google Benchmark is already installed. Skipping installation..."
else
    echo -e "${YELLOW}Google Benchmark is not installed. Installing...${NC}"
    mkdir -p "tmp-google-benchmark-build" && cd "tmp-google-benchmark-build" || exit 1
    git clone https://github.com/google/benchmark.git
    cd benchmark || exit 1
    cmake -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_DOWNLOAD_DEPENDENCIES=ON -DBENCHMARK_ENABLE_TESTING=OFF -S . -B build
    cmake --build build --config Release -j "$(nproc)"
    sudo cmake --install build
    cd ../.. || exit 1
    rm -rf tmp-google-benchmark-build
    echo -e "${GREEN}Google Benchmark installed successfully!${NC}"
fi
echo ""


echo -e "${GREEN}~ Compile CMakeLists.txt in $(pwd)/build ~${NC}"
mkdir -p "build" && cd "build" || exit 1
cmake .. --preset ninja-dev-benchmark --log-level=WARNING || exit 1
cd ninja-dev-benchmark || exit 1
echo ""


echo -e "${GREEN}~ Compile project + benchmarks ~${NC}"
start_time=$(date +%s)
ninja all || exit 1
end_time=$(date +%s)
elapsed_time=$((end_time - start_time))
echo "Built in ~$elapsed_time seconds"
cd ../.. || exit 1
echo "Run examples using ./build/ninja-dev-benchmark/benchmarks/benchmark-name"
echo ""