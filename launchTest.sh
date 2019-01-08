#!/usr/bin/env bash

cd ./build/test
ctest -V
if [[ $? != 0 ]]; then
  exit 1
fi

cd ..

rm -r coverage
mkdir -p coverage

covFile=`find . -path ./external -prune -o -name "*.gcda" -print`
covFile="$covFile `find . -path ./external -prune -o -name "*.gcno" -print`"

cp $covFile coverage

if command -v gcovr; then
  cd ..
  cp build/coverage/* .
  gcovr -r . --html-details -o build/coverage/index.html

  rm *.gcda
  rm *.gcno
fi
