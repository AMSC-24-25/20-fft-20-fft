#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

if [ -e ./build/make-dev/Makefile ]; then
    cd build/make-dev || exit 1
    make -f ./Makefile -C . clean
    cd ../..
fi

if [ -e ./build/make-dev-cuda/Makefile ]; then
    cd build/make-dev-cuda || exit 1
    make -f ./Makefile -C . clean
    cd ../..
fi

if [ -e ./build/make-lib/Makefile ]; then
    cd build/make-lib || exit 1
    make -f ./Makefile -C . clean
    cd ../..
fi

if [ -e ./build/make-lib-cuda/Makefile ]; then
    cd build/make-lib-cuda || exit 1
    make -f ./Makefile -C . clean
    cd ../..
fi

if [ -e ./build/ninja-dev/build.ninja ]; then
    cd build/ninja-dev || exit 1
    ninja clean
    cd ../..
fi

if [ -e ./build/ninja-dev-cuda/build.ninja ]; then
    cd build/ninja-dev-cuda || exit 1
    ninja clean
    cd ../..
fi

if [ -e ./build/ninja-lib/build.ninja ]; then
    cd build/ninja-lib || exit 1
    ninja clean
    cd ../..
fi

if [ -e ./build/ninja-lib-cuda/build.ninja ]; then
    cd build/ninja-lib-cuda || exit 1
    ninja clean
    cd ../..
fi

if [ -e ./build/ninja-dev-benchmark/build.ninja ]; then
    cd build/ninja-dev-benchmark || exit 1
    ninja clean
    cd ../..
fi

echo -e "${GREEN}All cleaned up, good!${NC}"