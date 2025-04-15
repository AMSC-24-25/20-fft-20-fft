#!/bin/bash

if [ -e ./build/Makefile ]; then
    cd build || exit 1
    make -f ./Makefile -C . clean
    cd ..
fi

echo "All cleaned up, good!"