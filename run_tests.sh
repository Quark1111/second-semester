#!/bin/bash

cd build || exit 1
make || exit 1

total=0
passed=0
failed=0

for test_file in ../Tests/*.csv; do
    name=$(basename "$test_file" .csv)
    out_file="../Tests/${name}.out"
    expected_file="../Tests/${name}.txt"
    
    echo -n "Running test $name ... "
    
    ./csv_printer "$test_file" "$out_file"
    
    if [ -f "$expected_file" ]; then
        if diff -q "$expected_file" "$out_file" > /dev/null; then
            echo "PASSED"
            ((passed++))
        else
            echo "FAILED"
            ((failed++))
            echo "Differences:"
            diff "$expected_file" "$out_file" | head -10
        fi
    else
        echo "NO EXPECTED FILE"
    fi
    
    ((total++))
done

echo "Tests: $total, Passed: $passed, Failed: $failed"
