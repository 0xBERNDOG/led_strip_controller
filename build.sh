rm -rf ./out
mkdir -p ./out
cd ./out
cmake -S ../ -B .
make "${PROJECT_NAME}"
make hex
make memory_usage