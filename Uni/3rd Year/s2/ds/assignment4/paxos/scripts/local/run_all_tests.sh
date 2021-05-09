#!/bin/bash

# run_all_tests.sh
# Takes no argument and runs all tests in /test/, 
#  shows no output except success/fail count

trap "true" SIGABRT

function find_run_all () {
    # local test_names=`find . -depth 1 -type f -iname "test*"  -perm -111 -print0 ! -name *.dSYM | xargs -0`;
    local test_names=`find . -maxdepth 1 -type f -perm -111 -not -name '*.dSYM' -name 'test*' -print0 | xargs -0`
    local test_count=`echo $test_names | wc -w `;
    let 'test_count += 0';
    if [[ $test_count == 0 ]]; then
        echo "No tests were found, you need to build them before"
        exit
    fi

    local passed_count=0;
    local failed_tests=""
    
    echo "Running $test_count tests"
    echo
    
    local i=0;
    local short_name;
    local output_file;
    
    echo "-------------------------------------"
    for t in $test_names; do
        short_name=`basename $t`
        output_file="./output/$short_name"".txt"
        
        # "./$t &> $output_file"
        $t &>"$output_file"
        if [[ $? == 0 ]]; then
            echo -e "T-""$i"": PASS" "$short_name";
            let 'passed_count += 1';
        else
            echo -e "T-""$i"": FAIL" "$short_name";
            failed_tests="$failed_tests"" ""$short_name"
        fi
        let 'i += 1'
    done
    echo "-------------------------------------"
    
    echo
    echo "$passed_count""/""$test_count"" tests passed"
    
    if [[ $passed_count -lt $test_count ]]; then
        echo "The following tests failed: ""$failed_tests"
        echo "Output is available in test/output/"
    fi
}


if [[ ! -e ./test ]]; then
    echo "This script must be executed from the main folder"
    echo "(the one containing the subdirectories test, scripts, include, ..."
    exit 1;
fi

rm -rf test/output
mkdir -p test/output
cd test
find_run_all;


