#! bin/bash

if [ ! -d "build/benchmark" ]; then
    echo "benchmarks are not built"
    exit 1
fi

cd build/benchmark/
./stream_bench
./unicode_bench