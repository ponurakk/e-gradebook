#!/bin/bash

cleanbuild() {
  mkdir -p build
  cd build
  cmake -G "Ninja" ..
  ninja
}

build() {
  cd build
  ninja
}

clean() {
  rm -r build/
}

run() {
  ./build/e-gradebook
}

if [ "$1" == "run" ]; then
  run
elif [ "$1" == "cleanbuild" ]; then
  cleanbuild
elif [ "$1" == "build" ]; then
  build
elif [ "$1" == "clean" ]; then
  clean
else
  echo "Usage: $0 {run|build|clean}"
fi
