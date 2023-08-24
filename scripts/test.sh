#! bin/bash

if [ ! -d "build/test" ]; then
    echo "tests are not built"
    exit 1
fi

cd build/test/
./stream_test
./unicode_test
./endian_test
./int_test