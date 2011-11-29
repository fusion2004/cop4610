#!/bin/bash

LISTLEN=201
TESTFILE=/test/bashpipingtest

for i in `seq 1 $LISTLEN`
do
	echo "YOUR TEST FACE x$i TIMES" >> $TESTFILE
done

TESTSIZE=$(stat -c%s "$TESTFILE")

echo "We've now added $LISTLEN lines to the test file, $TESTFILE"
echo "This test file is now $TESTSIZE bytes."
