#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}~ Build Essential for Signal Processing Library ~${NC}"
if ! dpkg -l | grep 'build-essential' &> /dev/null
then
    echo -e "${YELLOW}Build Essential is not installed. Installing...${NC}"
    sudo apt-get update
    sudo apt-get install -y build-essential
    echo -e "${GREEN}Build Essential installed successfully!${NC}"
else
    echo "Build Essential is already installed."
fi
echo ""

echo -e "${GREEN}~ Install CMake ~${NC}"
if ! dpkg -l | grep 'cmake' &> /dev/null
then
    echo -e "${YELLOW}CMake is not installed. Installing...${NC}"
    sudo apt-get update
    sudo apt-get install -y cmake
    echo -e "${GREEN}CMake installed successfully!${NC}"
else
    echo "CMake is already installed."
fi
echo ""

echo -e "${GREEN}~ Install Ninja ~${NC}"
if ! dpkg -l | grep 'ninja-build' &> /dev/null
then
    echo -e "${YELLOW}Ninja is not installed. Installing...${NC}"
    sudo apt-get update
    sudo apt-get install -y ninja-build
    echo -e "${GREEN}Ninja installed successfully!${NC}"
else
    echo "Ninja is already installed."
fi
echo ""

echo -e "${GREEN} ~ Install gnuplot ~${NC}"
if ! dpkg -l | grep 'gnuplot' &> /dev/null
then
    echo -e "${YELLOW}Gnuplot is not installed. Installing...${NC}"
    sudo apt-get update
    sudo apt-get install -y gnuplot
    echo -e "${GREEN}Gnuplot installed successfully!${NC}"
else
    echo "Gnuplot is already installed."
fi
echo ""
