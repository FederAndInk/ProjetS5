
cd `dirname $0`

nbTests=0
nbTestsPassed=0

# -Sshr

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
done