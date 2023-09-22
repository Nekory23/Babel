#!/usr/bin/bash

mkdir build && cd build && conan install .. --build=asio && cmake .. -G "Unix Makefiles" && cmake –build . && make && mv bin/* ../