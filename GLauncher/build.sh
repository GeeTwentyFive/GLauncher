#!/bin/bash

if ! g++ --version; then
        echo "ERROR: g++ not found; install GCC, then restart shell"
        exit 1
fi

if ! xmake --version; then
        echo "ERROR: xmake not found; install via 'curl -fsSL https://xmake.io/shget.text | bash', then restart shell"
        exit 1
fi

xmake config -p windows --toolchain=mingw
xmake

xmake config -p linux
xmake
