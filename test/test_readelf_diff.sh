#!/usr/bin/env bash

if (($# != 2 && $# != 3)); then
	echo "Usage: $0 opt [sectionName] file"
	echo "see readelf -H for opt"
	exit 2
fi

p=$(dirname $0)

tmp1=$(mktemp)
tmp2=$(mktemp)

readelf $1 $2 $3 &>$tmp1
$p/../build/src/readelf $1 $2 $3 &>$tmp2

if diff -ZBbE --old-group-format=$'\e[0;31m< %<\e[0m' \
	--new-group-format=$'\e[0;32m> %>\e[0m' \
	--unchanged-group-format=$'' \
	$tmp1 $tmp2; then
	echo "no diff"
else
	echo "Outputs:"
	echo "-------------------------"
	cat $tmp1
	echo "-------------------------"
	cat $tmp2
	echo "-------------------------"
fi

rm $tmp1
rm $tmp2
