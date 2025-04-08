#!/bin/bash

if [ -e ./Makefile ]; then
    make -f ./Makefile -C . clean
fi

echo "All cleaned up, good!"