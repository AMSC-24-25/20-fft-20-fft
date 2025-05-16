#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}~ Install ninja-build ~${NC}"
if ! command -v ninja &> /dev/null
then
    sudo apt install -y ninja-build || exit 1
else
    echo "Ninja is already installed."
fi
echo ""


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


echo -e "${GREEN}~ Compile CMakeLists.txt in $(pwd)/build ~${NC}"
mkdir -p "build" && cd "build" || exit 1
cmake .. --preset ninja-dev --log-level=ERROR || exit 1
cd ninja-dev
echo ""


echo -e "${GREEN}~ Compile project + examples ~${NC}"
start_time=$(date +%s)
ninja --quiet all || exit 1
end_time=$(date +%s)
elapsed_time=$((end_time - start_time))
echo "Built in ~$elapsed_time seconds"
echo "Run examples using ./build/ninja-dev/examples/example-name"
echo ""
