#!/usr/bin/env bash

if (($# != 2 && $# != 3)); then
	echo "Usage: $0 opt [sectionName] file"
	echo "see readelf -H for opt"
	exit 2
fi

exclude="NOTE: This section has relocations against it, but these have NOT been applied to this dump."
# exclude="$exclude|somemore"

p=$(dirname $0)

tmp1=$(mktemp)
tmp2=$(mktemp)

if [[ $1 == -*h* ]]; then
	readelf $1 $2 $3 | grep -Ev "$exclude" | sed -r 's/(Flags:\s+0x[0-9a-fA-F]+)(,.*)/\1/g' &>$tmp1
else 
	readelf $1 $2 $3 | grep -Ev "$exclude" &>$tmp1
fi

$p/../build/src/readelf $1 $2 $3 &>$tmp2

if diff -ZBbE --old-group-format=$'\e[0;31m< %<\e[0m' \
	--new-group-format=$'\e[0;32m> %>\e[0m' \
	--unchanged-group-format=$'' \
	$tmp1 $tmp2; then
	echo "no diff"
else
	echo "Outputs:"
	echo "-------------------------"
	echo "System readelf:"
	echo ""
	cat $tmp1
	echo "-------------------------"
	echo "Project readelf:"
	echo ""
	cat $tmp2
	echo "-------------------------"
	exit 1
fi

rm $tmp1
rm $tmp2