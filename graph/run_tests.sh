#!/bin/bash
total=0
passed=0
failed=0
for test_file in ../Tests/test*_res.txt; do
    name=$(basename "$test_file" _res.txt)
    input_file="../Tests/${name}.txt"
    expected_file="$test_file"
    output_file="../Tests/${name}.out"
    ./states "$input_file" "$output_file"
    if diff -w "$expected_file" "$output_file" > /dev/null 2>&1; then
        echo "PASSED"
        ((passed++))
    else
        echo "FAILED"
        ((failed++))
        echo "Differences:"
        diff -w "$expected_file" "$output_file" | head -10
    fi
    rm -f "$output_file"
    ((total++))
done
echo "Tests: $total, Passed: $passed, Failed: $failed"
