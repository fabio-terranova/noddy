#!/bin/bash

cmake -S . -B build

if cmake --build build/
then
  echo "Build successful. Running..."
  ./build/bin/noddy_gui
else
  echo "Failed build"
  exit 1
fi
