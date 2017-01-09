#!/bin/bash
#
# Copyright (C) 2016 Betti Oesterholz
#
# Author: Betti Oesterholz
#
#######################################################################

echo -n > out_test.txt

date;
for ITERATIONS in 32 64 128 256 512 1024 2048 4096 8192 16384
do
	echo "testcase/tMessageGloveStates ${ITERATIONS}"
	testcase/tMessageGloveStates ${ITERATIONS} >> out_test.txt 2>&1
	RETURNVALUE=$?
	if [ ${RETURNVALUE} -ne 0 ]; then
		tail -n32 out_test.txt
	fi
	date
done
