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

    printf "Expected ${expected}, but was ${wasThis}\n\n"
}

numberOfTests=0
passed=0
failed=0

# Hide the output of makefile all target
make all > /dev/null

echo -n Test 1: No arguments given to pgmEcho
exeOut="$(./pgmEcho)"
expected="Usage: ./pgmEcho inputImage.pgm outputImage.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 2: No arguments given to pgmComp
exeOut="$(./pgmComp)"
expected="Usage: ./pgmComp inputImage.pgm inputImage.pgm"
if [[ $exeOut = $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 3: No arguments given to pgma2b
exeOut="$(./pgma2b)"
expected="Usage: ./pgma2b inputImage.pgm outputImage.pgm"
if [[ $exeOut = $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 4: No arguments given to pgmb2a
exeOut="$(./pgmb2a)"
expected="Usage: ./pgmb2a inputImage.pgm outputImage.pgm"
if [[ $exeOut = $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 5: No arguments given to pgmReduce
exeOut="$(./pgmReduce)"
expected="Usage: ./pgmReduce inputImage.pgm reduction_factor outputImage.pgm"
if [[ $exeOut = $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 6: No arguments given to pgmTile
exeOut="$(./pgmTile)"
expected="Usage: ./pgmTile inputImage.pgm tiling_factor outputImage_<row>_<column>.pgm"
if [[ $exeOut = $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 7: Too few arguments given to pgmEcho
exeOut="$(./pgmEcho pgmImages/casablanca.pgm)"
expected="ERROR: Bad Argument Count"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 8: Too few arguments given to pgmComp
exeOut="$(./pgmComp pgmImages/casablanca.pgm)"
expected="ERROR: Bad Argument Count"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 9: Too few arguments given to pgma2b
exeOut="$(./pgma2b pgmImages/casablanca.pgm)"
expected="ERROR: Bad Argument Count"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 10: Too few arguments given to pgmb2a
exeOut="$(./pgmb2a pgmImages/casablanca.pgm)"
expected="ERROR: Bad Argument Count"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 11: Too few arguments given to pgmReduce
exeOut="$(./pgmReduce pgmImages/casablanca.pgm)"
expected="ERROR: Bad Argument Count"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 12: Too few arguments given to pgmTile
exeOut="$(./pgmTile pgmImages/casablanca.pgm)"
expected="ERROR: Bad Argument Count"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 13: Too many arguments given to pgmEcho
exeOut="$(./pgmEcho pgmImages/casablanca.pgm erasdasd out.pgm)"
expected="ERROR: Bad Argument Count"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 14: Too many arguments given to pgmComp
exeOut="$(./pgmComp pgmImages/casablanca.pgm ajskdasjkdkhlas out.pgm)"
expected="ERROR: Bad Argument Count"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 15: Too many arguments given to pgma2b
exeOut="$(./pgma2b pgmImages/casablanca.pgm ajskdasjkdkhlas out.pgm)"
expected="ERROR: Bad Argument Count"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 16: Too many arguments given to pgmb2a
exeOut="$(./pgmb2a pgmImages/casablanca.pgm ajskdasjkdkhlas out.pgm)"
expected="ERROR: Bad Argument Count"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))

echo -n Test 17: Too many arguments given to pgmReduce
exeOut="$(./pgmReduce pgmImages/casablanca.pgm 3 out.pgm another.txt)"
expected="ERROR: Bad Argument Count"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 18: Too many arguments given to pgmTile
exeOut="$(./pgmTile pgmImages/casablanca.pgm 4 ajskdasjkdkhlas out.pgm)"
expected="ERROR: Bad Argument Count"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 19: Reduction factor of 0 given to pgmReduce
exeOut="$(./pgmReduce pgmImages/casablanca.pgm 0 out.pgm)"
expected="ERROR: Miscellaneous: Factor was not an integer greater than 0"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 20: Tiling factor of 0 given to pgmTile
exeOut="$(./pgmTile pgmImages/casablanca.pgm 0 out\<column\>__\<row\>.pgm)"
expected="ERROR: Miscellaneous: Factor was not an integer greater than 0"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 21: pgmTile output file template does not contain \<row\> tag
exeOut="$(./pgmTile pgmImages/casablanca.pgm 5 ohNoIDont\<column\>ContainRowTag.pgm)"
expected="ERROR: Miscellaneous: <row> tag was not found in output file name template"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 22: pgmTile output file template does not contain \<column\> tag
exeOut="$(./pgmTile pgmImages/casablanca.pgm 3 ohNoIDont\<row\>ContainColumnTag.pgm)"
expected="ERROR: Miscellaneous: <column> tag was not found in output file name template"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 23: pgmTile output file template does not contain \<row\> and \<column\> tags
exeOut="$(./pgmTile pgmImages/casablanca.pgm 3 ohNoIDontContainAnyTags.pgm)"
expected="ERROR: Miscellaneous: <row> and <column> tags were not found in output file name template"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


# Test 24


# Test 25


# Test 26


# Test 27


# Test 28


# Test 29


# Test 30


# Test 31


# Test 32


# Test 33


# Test 34


# Test 35


# Test 36




# Test Summary
echo Test Summary:
echo "Tests Passed: $passed/$numberOfTests"
echo "Tests Failed: $failed/$numberOfTests"

# Hide the output of makefile clean target
make clean > /dev/null