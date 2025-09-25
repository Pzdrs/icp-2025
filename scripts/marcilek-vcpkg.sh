#!/bin/bash

if [ ! -d "vcpkg" ]; then
  git clone https://github.com/microsoft/vcpkg.git --depth 1
fi

cd vcpkg
./bootstrap-vcpkg.sh

export VCPKG_ROOT=$(pwd)
export PATH="$VCPKG_ROOT:$PATH"

cd ..