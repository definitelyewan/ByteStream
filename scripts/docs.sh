#! bin/bash

#! bin/bash

if [ ! -d "build/docs" ]; then
    echo "docs are not built"
    exit 1
fi

cd build/docs/
doxygen Doxyfile.docs && xdg-open html/index.html