#!/bin/bash

green="\e[0;92m"
red="\e[0;91m"
reset="\e[0m"

printPassed() {
    printf " ${green}\xE2\x9C\x94\n\n${reset}"
}

printFailed() {
    printf " ${red}\xE2\x9D\x8C\n\n${reset}"
}

assertionFailed() {
    eval expected=$1
    eval wasThis=$2
    printf "Expected: ${expected}, was ${wasThis}\n\n"
}

numberOfTests=0
passed=0
failed=0

# Hide the output of makefile all target
make all > /dev/null

echo -n Test 1: Usage message displayed when no arguments are given to gtopoEcho
exeOut="$(./gtopoEcho)"
expected="Usage: ./gtopoEcho inputFile width height outputFile"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi
numberOfTests=$((numberOfTests+1))


echo -n Test 2: Usage message displayed when no arguments are given to gtopoComp
exeOut="$(./gtopoComp)"
expected="Usage: ./gtopoComp firstFile width height secondFile"
if [[ $exeOut = $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi
numberOfTests=$((numberOfTests+1))


echo -n Test 3: Usage message displayed when no arguments are given to gtopoReduce
exeOut="$(./gtopoReduce)"
expected="Usage: ./gtopoReduce input width height reduction_factor output"
if [[ $exeOut = $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi
numberOfTests=$((numberOfTests+1))


echo -n Test 4: Usage message displayed when no arguments are given to gtopoTile
exeOut="$(./gtopoTile)"
expected="Usage: ./gtopoTile inputFile width height tiling_factor outputFile_<row>_<column>"
if [[ $exeOut = $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi
numberOfTests=$((numberOfTests+1))


echo -n Test 5: Usage message displayed when no arguments are given to gtopoAssemble
exeOut="$(./gtopoAssemble)"
expected="Usage: ./gtopoAssemble outputFile width height (row column inputFile width height)+"
if [[ $exeOut = $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi
numberOfTests=$((numberOfTests+1))


echo -n Test 6: Usage message displayed when no arguments are given to gtopoPrintLand
exeOut="$(./gtopoPrintLand)"
expected="Usage: ./gtopoPrintLand inputFile width height outputFile sea hill mountain"
if [[ $exeOut = $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi
numberOfTests=$((numberOfTests+1))


echo -n Test 7: Usage message displayed when no arguments are given to gtopoAssembleReduce
exeOut="$(./gtopoAssembleReduce)"
expected="Usage: ./gtopoAssembleReduce outputArray.gtopo width height reduction_factor (row column inputArray.gtopo width height)+"
if [[ $exeOut = $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi
numberOfTests=$((numberOfTests+1))


echo -n Test 8: gtopoEcho successfully echoes a DEM file
exeOut="$(./gtopoEcho /vol/scratch/SoC/COMP1921/GTOPO30/gt30w020n90_dem/gt30w020n90.dem 4800 6000 output.dem)"
expected="ECHOED"
comparison="$(diff /vol/scratch/SoC/COMP1921/GTOPO30/gt30w020n90_dem/gt30w020n90.dem output.dem)"
if [[ $exeOut = $expected ]]; then
    if [[ $comparison = "" ]]; then
        printPassed
        passed=$((passed+1))
    else
        printFailed
        failed=$((failed+1))
        echo Output file was different
    fi
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi
numberOfTests=$((numberOfTests+1))
rm output.dem


echo -n Test 9: gtopoComp successfully compares the same DEM file
exeOut="$(./gtopoComp /vol/scratch/SoC/COMP1921/GTOPO30/gt30w020n90_dem/gt30w020n90.dem 4800 6000 /vol/scratch/SoC/COMP1921/GTOPO30/gt30w020n90_dem/gt30w020n90.dem)"
expected="IDENTICAL"
comparison="$(diff /vol/scratch/SoC/COMP1921/GTOPO30/gt30w020n90_dem/gt30w020n90.dem /vol/scratch/SoC/COMP1921/GTOPO30/gt30w020n90_dem/gt30w020n90.dem)"
if [[ $exeOut = $expected ]]; then
    if [[ $comparison = "" ]]; then
        printPassed
        passed=$((passed+1))
    else
        printFailed
        failed=$((failed+1))
        echo The files were different
    fi
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi
numberOfTests=$((numberOfTests+1))


echo -n Test 10: gtopoComp successfully compares different DEM files
exeOut="$(./gtopoComp /vol/scratch/SoC/COMP1921/GTOPO30/gt30w020n90_dem/gt30w020n90.dem 4800 6000 /vol/scratch/SoC/COMP1921/GTOPO30/gt30w020n40_dem/gt30w020n40.dem)"
expected="DIFFERENT"
comparison="$(diff /vol/scratch/SoC/COMP1921/GTOPO30/gt30w020n90_dem/gt30w020n90.dem /vol/scratch/SoC/COMP1921/GTOPO30/gt30w020n40_dem/gt30w020n40.dem)"
if [[ $exeOut = $expected ]]; then
    if [[ $comparison != "" ]]; then
        printPassed
        passed=$((passed+1))
    else
        printFailed
        failed=$((failed+1))
        echo The files were identical
    fi
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi
numberOfTests=$((numberOfTests+1))


# Test Summary
echo Test Summary:
echo "Tests Passed: $passed/$numberOfTests"
echo "Tests Failed: $failed/$numberOfTests"

# Hide the output of makefile clean target
make clean > /dev/null
