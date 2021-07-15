#!/bin/bash
myDir=$PWD
clang-tidy ./src/app.cpp -p ./build -header-filter=.* -- -I${PWD}/include

