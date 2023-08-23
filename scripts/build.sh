#! bin/bash

if [ ! -d "build/" ]; then
    mkdir build
fi

#change this line for options
cmake -S . -B build/ -DBUILD_SHARED_LIBS=ON -DBUILD_TESTS=ON -DBUILD_BENCHMARKS=ON

#make all targets
cd build/
make