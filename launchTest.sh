#!/usr/bin/env bash

cd ./build/test
ctest -V
if [[ $? != 0 ]]; then
  exit 1
fi

cd ..

rm -r coverage
mkdir -p coverage

covFile=`find . -name "*.gcda"`
covFile="$covFile `find . -name "*.gcno"`"

cp $covFile coverage

if command -v gcovr; then
  cd ..
  cp build/coverage/* .
  gcovr -r . --html-details -o build/coverage/index.html

  rm *.gcda
  rm *.gcno
fi
