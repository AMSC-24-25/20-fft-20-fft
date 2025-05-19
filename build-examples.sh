#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

./build-essential.sh || exit 1

echo -e "${GREEN}~ Install OpenCV ~${NC}"
if ! dpkg -l | grep 'libopencv-dev' &> /dev/null
then
    echo -e "${YELLOW}OpenCV is not installed. Installing...${NC}"
    sudo apt update || exit 1
    sudo apt install -y libopencv-dev || exit 1
else
    echo "OpenCV is already installed."
fi
echo ""


echo -e "${GREEN}~ Checking for Matplot++ ~${NC}"
if [ ! -d "examples/external/matplotplusplus" ]; then
    echo -e "${YELLOW}Matplot++ was not found. Cloning...${NC}"
    git submodule update --init --recursive
    if [ $? -ne 0 ]; then
        echo -e "${RED}Failed to clone Matplot++. Please check your internet connection.${NC}"
        exit 1
    fi
    echo -e "${GREEN}Matplot++ cloned successfully!${NC}"
else
    echo "Matplot++ is already cloned."
fi
echo ""


echo -e "${GREEN}~ Compile CMakeLists.txt in $(pwd)/build ~${NC}"
mkdir -p "build" && cd "build" || exit 1
cmake .. --preset ninja-dev --log-level=WARNING || exit 1
cd ninja-dev || exit 1
echo ""


echo -e "${GREEN}~ Compile project + examples ~${NC}"
start_time=$(date +%s)
ninja --quiet all || exit 1
end_time=$(date +%s)
elapsed_time=$((end_time - start_time))
echo "Built in ~$elapsed_time seconds"
echo "Run examples using ./build/ninja-dev/examples/example-name"
echo ""
