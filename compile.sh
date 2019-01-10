#!/usr/bin/env bash

mkdir -p build

cd build

cmake ..

if make -j$(nproc); then
	cd ..
	./launchTest.sh
else
	exit 1
fi
