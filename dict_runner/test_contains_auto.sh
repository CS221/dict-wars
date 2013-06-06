#!/bin/bash
# 2011W2 Brendan Shillingford
# Runs and compiles the C++ file with the same name and tests all dictionaries found using the below statement

# generate dictionary initializer statementlist and iterate over them:
# (looks for the lines like:
#		dictionaries["..."] = new ...;
# and gets the part between 'new' and ';'

cat dict_runner.cpp | grep dictionaries | grep '= *new ' | grep -v '^ *//' | sed "s/^.*new \(.*\);/\1/g" | \
while read x; do
	echo ================ testing $x ==========
	if [ -e "${x}_test.cc" ]
	then
	    echo "Using custom test program ${x}_test.cc."
	    g++ -DDICT="$x"  -g -std=c++0x -o test_contents "${x}_test.cc" && ./test_contents
	else
	    g++ -DDICT="$x"  -g -std=c++0x -o test_contents test_contains.cpp && ./test_contents
	fi
	echo _-_-_-_-_-RETVAL = $?
done
echo ========== done. ==========
rm ./test_contents


