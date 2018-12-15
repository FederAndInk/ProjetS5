#!/usr/bin/env bash

mkdir -p build

cd build

cmake ..

if make -j`nproc`; then
  cd ..
  if ./launchTest.sh; then
    printf "\e[32;4mTests passed !\e[1m\n"
  else
    printf "\e[31;4mSome tests failed !\e[1m\n"
  fi
fi
