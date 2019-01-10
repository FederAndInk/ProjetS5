cd $(dirname $0)

nbTests=0
nbTestsPassed=0

# -Sshr
printf "\n\e[4;35mLaunch testReadElf\e[0m\n"

for file in bin/*.o; do
	for opt in s S h r; do
		./test_readelf_diff.sh -"$opt" "$file"
		ret=$?
		((nbTests++))
		if [[ $ret != 0 ]]; then
			echo "./test_readelf_diff.sh -$opt $file"
			echo "returned $ret"
		else
			((nbTestsPassed++))
		fi
	done
	for sec in ".data" ".text"; do
		./test_readelf_diff.sh -x $sec "$file"
		ret=$?
		((nbTests++))
		if [[ $ret != 0 ]]; then
			echo "./test_readelf_diff.sh -x $sec $file"
			echo "returned $ret"
		else
			((nbTestsPassed++))
		fi
	done

done

if (($nbTestsPassed != $nbTests)); then
	printf "\e[31mTests $nbTestsPassed/$nbTests passed\e[0m\n"
	exit 1
else
	printf "\e[32mTests $nbTestsPassed/$nbTests passed\e[0m\n"
fi
