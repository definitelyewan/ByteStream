#! bin/bash

[ ! -d "/build" ] && mkdir build

cmake -S . -B build/ -DBUILD_SHARED_LIBS=ON #-DBUILD_TESTS=ON -DBUILD_BENCHMARKS=ON

#make all targets
cd build/
make