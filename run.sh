#!/bin/sh

./contentfilter filter/filter.txt utils/testcase.txt output.txt
./Bcontentfilter filter/filter.txt utils/testcase.txt outputB.txt
./Trie filter/filter.txt utils/testcase.txt outputT.txt