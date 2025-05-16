#!/bin/bash

if [ -e ./build/make-dev/Makefile ]; then
    cd build/make-dev || exit 1
    make -f ./Makefile -C . clean
    cd ../..
fi

if [ -e ./build/make-lib/Makefile ]; then
    cd build/make-lib || exit 1
    make -f ./Makefile -C . clean
    cd ../..
fi

if [ -e ./build/ninja-dev/build.ninja ]; then
    cd build/ninja-dev || exit 1
    ninja clean
    cd ../..
fi

if [ -e ./build/ninja-lib/build.ninja ]; then
    cd build/ninja-lib || exit 1
    ninja clean
    cd ../..
fi

echo "All cleaned up, good!"