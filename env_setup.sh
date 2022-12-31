export CC=avr-gcc

export TARGET=rev30 # todo: make something to select this
export PORT=/dev/ttyACM0 # todo: make something to select this

export PROJECT_NAME=led_controller
export BINARY_NAME=${PROJECT_NAME}
export BINARY_PATH=./build/${BINARY_NAME}