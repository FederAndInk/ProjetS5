#!/usr/bin/env bash

cd ./build/test
ctest -V
if [[ $? != 0 ]]; then
	printf "\e[31;4mSome tests failed ! (ctest)\e[1m\n"
	exit 1
fi

cd ../../test

./testReadElf.sh
if [[ $? != 0 ]]; then
	printf "\e[31;4mSome tests failed ! (testReadElf.sh)\e[1m\n"
	exit 1
fi

cd ../build

rm -r coverage
mkdir -p coverage

covFile=$(find . -path ./external -prune -o -name "*.gcda" -print)
covFile="$covFile $(find . -path ./external -prune -o -name "*.gcno" -print)"

cp $covFile coverage

if command -v gcovr; then
	cd ..
	cp build/coverage/* .
	gcovr -r . --html-details -o build/coverage/index.html 2>/dev/null

	rm *.gcda
	rm *.gcno
fi

printf "\e[32;4mTests passed !\e[1m\n"
