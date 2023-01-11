#!/bin/bash
source ./env_setup.sh
rm -rf ./out
rm -rf ./build
mkdir -p ./out
cd ./out
cmake -S ../ -B .
make "${PROJECT_NAME}"
make hex
make memory_usage