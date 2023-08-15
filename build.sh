#! bin/bash

[ ! -d "/build" ] && mkdir build

cmake -S . -B build/

#make all targets
cd build/
make