#!/bin/bash

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# This script finds what day is next 5 days from Tuesday	#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

CMD=../build/rot

C=`echo MON,TUE,WED,THU,FRI,SAT,SUN | ./$CMD -n 5 -s 1 -r 0 -d ,`
if [ "$C" == "SUN" ]; then
	echo "Test passed"
else
	echo "Test failed"
fi