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

    printf "Expected ${expected}, but was ${wasThis}\n\n"
}

numberOfTests=0
passed=0
failed=0

# Hide the output of makefile all target
make all > /dev/null

echo -n Test 1: Usage message displayed when no arguments are given to pgmEcho
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


echo -n Test 2: Usage message displayed when no arguments are given to pgmComp
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


echo -n Test 3: Usage message displayed when no arguments are given to pgma2b
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


echo -n Test 4: Usage message displayed when no arguments are given to pgmb2a
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


echo -n Test 5: Usage message displayed when no arguments are given to pgmReduce
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


echo -n Test 6: Usage message displayed when no arguments are given to pgmTile
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


echo -n Test 7: Error triggered when too few arguments are given to pgmEcho
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


echo -n Test 8: Error triggered when too few arguments are given to pgmComp
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


echo -n Test 9: Error triggered when too few arguments are given to pgma2b
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


echo -n Test 10: Error triggered when too few arguments are given to pgmb2a
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


echo -n Test 11: Error triggered when too few arguments are given to pgmReduce
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


echo -n Test 12: Error triggered when too few arguments are given to pgmTile
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


echo -n Test 13: Error triggered when too many arguments are given to pgmEcho
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


echo -n Test 14: Error triggered when too many arguments are given to pgmComp
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


echo -n Test 15: Error triggered when too many arguments are given to pgma2b
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


echo -n Test 16: Error triggered when too many arguments are given to pgmb2a
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

echo -n Test 17: Error triggered when too many arguments are given to pgmReduce
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


echo -n Test 18: Error triggered when too many arguments are given to pgmTile
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


echo -n Test 19: Error triggered when a reduction factor of 0 is given to pgmReduce
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


echo -n Test 20: Error triggered when a tiling factor of 0 is given to pgmTile
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


echo -n Test 21: pgmTile output file template does not contain \<row\> tag fails
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


echo -n Test 22: pgmTile output file template does not contain \<column\> tag fails
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


echo -n Test 23: pgmTile output file template does not contain \<row\> and \<column\> tags fails
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


echo -n Test 24: pgmEcho given an input file which does not exist fails
exeOut="$(./pgmEcho notexisting.pgm output.pgm)"
expected="ERROR: Bad File Name: notexisting.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))

echo -n Test 25: pgmComp given an input file which does not exist fails
exeOut="$(./pgmComp notexisting.pgm output.pgm)"
expected="ERROR: Bad File Name: notexisting.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 26: pgma2b given an input file which does not exist fails
exeOut="$(./pgma2b notexisting.pgm output.pgm)"
expected="ERROR: Bad File Name: notexisting.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 27: pgmb2a given an input file which does not exist fails
exeOut="$(./pgmb2a notexisting.pgm output.pgm)"
expected="ERROR: Bad File Name: notexisting.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 28: pgmReduce given an input file which does not exist fails
exeOut="$(./pgmReduce notexisting.pgm 3 output.pgm)"
expected="ERROR: Bad File Name: notexisting.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 29: pgmTile given an input file which does not exist fails
exeOut="$(./pgmTile notexisting.pgm 6 \<column\>output\<row\>.pgm)"
expected="ERROR: Bad File Name: notexisting.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 30: pgma2b given a pgm file with magic number P5 fails
exeOut="$(./pgma2b pgmImages/casablanca.pgm output.pgm)"
expected="ERROR: Bad Magic Number: pgmImages/casablanca.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 31: pgmb2a given a pgm file with magic number P2 fails
exeOut="$(./pgmb2a pgmImages/x31_f18.ascii.pgm output.pgm)"
expected="ERROR: Bad Magic Number: pgmImages/x31_f18.ascii.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 32: Reading a file with a bad magic number fails
exeOut="$(./pgmEcho pgmImages/aFeepBadMagicNo.pgm output.pgm)"
expected="ERROR: Bad Magic Number: pgmImages/aFeepBadMagicNo.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 33: Reading a file with negative width fails
exeOut="$(./pgmEcho pgmImages/aFeepBadWidth1.pgm output.pgm)"
expected="ERROR: Bad Dimensions: pgmImages/aFeepBadWidth1.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 34: Reading a file with 0 width fails
exeOut="$(./pgmEcho pgmImages/aFeepBadWidth2.pgm output.pgm)"
expected="ERROR: Bad Dimensions: pgmImages/aFeepBadWidth2.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 35: Reading a file with non-integer width fails
exeOut="$(./pgmEcho pgmImages/aFeepBadWidth3.pgm output.pgm)"
expected="ERROR: Bad Dimensions: pgmImages/aFeepBadWidth3.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 36: Reading a file with width exceeding 65535 fails
exeOut="$(./pgmEcho pgmImages/aFeepBadWidth4.pgm output.pgm)"
expected="ERROR: Bad Dimensions: pgmImages/aFeepBadWidth4.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 37: Reading a file with negative height fails
exeOut="$(./pgmEcho pgmImages/aFeepBadHeight1.pgm output.pgm)"
expected="ERROR: Bad Dimensions: pgmImages/aFeepBadHeight1.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 38: Reading a file with 0 height fails
exeOut="$(./pgmEcho pgmImages/aFeepBadHeight2.pgm output.pgm)"
expected="ERROR: Bad Dimensions: pgmImages/aFeepBadHeight2.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 39: Reading a file with non-integer height fails
exeOut="$(./pgmEcho pgmImages/aFeepBadHeight3.pgm output.pgm)"
expected="ERROR: Bad Dimensions: pgmImages/aFeepBadHeight3.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 40: Reading a file with height exceeding 65535 fails
exeOut="$(./pgmEcho pgmImages/aFeepBadHeight4.pgm output.pgm)"
expected="ERROR: Bad Dimensions: pgmImages/aFeepBadHeight4.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 41: Reading a file with a negative maximum gray value fails
exeOut="$(./pgmEcho pgmImages/aFeepBadGray1.pgm output.pgm)"
expected="ERROR: Bad Max Gray Value: pgmImages/aFeepBadGray1.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 42: Reading a file with maximum gray value 0 fails
exeOut="$(./pgmEcho pgmImages/aFeepBadGray2.pgm output.pgm)"
expected="ERROR: Bad Max Gray Value: pgmImages/aFeepBadGray2.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 43: Reading a file with a non-integer maximum gray value fails
exeOut="$(./pgmEcho pgmImages/aFeepBadGray3.pgm output.pgm)"
expected="ERROR: Bad Max Gray Value: pgmImages/aFeepBadGray3.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 44: Reading a file with maximum gray value exceeding 65535 fails
exeOut="$(./pgmEcho pgmImages/aFeepBadGray4.pgm output.pgm)"
expected="ERROR: Bad Max Gray Value: pgmImages/aFeepBadGray4.pgm"
if [[ $exeOut =  $expected ]]; then
    printPassed
    passed=$((passed+1))
else
    printFailed
    failed=$((failed+1))
    assertionFailed "\${expected}" "\${exeOut}"
fi

numberOfTests=$((numberOfTests+1))


echo -n Test 45:




# Test Summary
echo Test Summary:
echo "Tests Passed: $passed/$numberOfTests"
echo "Tests Failed: $failed/$numberOfTests"

# Hide the output of makefile clean target
make clean > /dev/null
rm output.pgm
rm verylongfilenamethathopefullydoesnotcrashtheprogramwhenanerroroccursandthefilenamehastobedisplayedtotheuser.pgm
