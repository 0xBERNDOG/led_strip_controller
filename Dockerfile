FROM ubuntu:latest

WORKDIR /usr/src/app

RUN apt update && apt install cmake avrdude gcc-avr avr-libc -y

COPY . .

SHELL ["/bin/bash", "-c"]
RUN (source ./env_setup.sh && ./build.sh) || exit 1