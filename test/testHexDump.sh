#!/usr/bin/env bash

p=$(dirname $0)

tmp1=$(mktemp)
tmp2=$(mktemp)

readelf -x $1 $2 &>$tmp1
$p/../build/src/readelf -x $1 $2 &>$tmp2

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
