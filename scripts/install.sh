#! bin/bash

if [ "$EUID" -ne 0 ]
  then echo "Installation requires root"
  exit
fi

if [ ! -d "/build" ] 
    then mkdir build
fi

cmake -S . -B build/

#make all targets
cd build/
make
cpack
sh ByteStream-1.0.0-Linux.sh