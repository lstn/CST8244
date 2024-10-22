#!/bin/sh

###
### Unit tests for calc_client
###
### Usage
###   ./calc_unit_tests.sh [calc_serverPID]
###   where [calc_serverPID] is the processID of calc_server (./calc_server &)
###
### Installation
###   1. Copy this shell script file into the same directory on Neutrino as the calc_client and calc_server exe binaries.
###   2. Use sed (stream editor) to remove the carriage-return (^M) end-of-line symbol: sed 's/\r$//g' ./calc_unit_tests.sh.txt > calc_unit_tests.sh
###   3. Change file permissions of the script file to be runnable: chmod +x calc_unit_tests.sh
###
### Author
###   Gerald.Hurdle@AlgonquinCollege.com
###

## validate correct number of command-line arguments; exit if incorrect number
if [[ $# -ne 1 ]]; then
  echo "Usage: ./calc_unit_tests.sh [calc_serverPID]"
  exit 1
fi

echo "\n"
echo "Unit Test ID 1: ./calc_client"
echo "Expected result: usage message of calc_client"
./calc_client

echo "\n"
echo "Unit Test ID 2: ./calc_client 2 2 + 3"
echo "Expected result: error, as calc_client can't connect attached to processID 2 (slogger:O)"
./calc_client 2 2 + 3

echo "\n"
echo "Unit Test ID 3: ./calc_client $1 2 + 3"
echo "Expected result: 5.00 (normal case +)"
./calc_client $1 2 + 3

echo "\n"
echo "Unit Test ID 4: ./calc_client $1 2 - 3"
echo "Expected result: -1.00 (normal case -)"
./calc_client $1 2 - 3

echo "\n"
echo "Unit Test ID 5: ./calc_client $1 2 x 3"
echo "Expected result: 6.00 (normal case x)"
./calc_client $1 2 x 3

echo "\n"
echo "Unit Test ID 6: ./calc_client $1 22 / 7"
echo "Expected result: 3.14 (normal case /; approx. of PI)"
./calc_client $1 22 / 7

echo "\n"
echo "Unit Test ID 7: ./calc_client $1 2 / 0"
echo "Expected result: SRVR_UNDEFINED (handle divide by 0)"
./calc_client $1 2 / 0

echo "\n"
echo "Unit Test ID 8: ./calc_client $1 2 ? 3"
echo "Expected result: SRVR_INVALID_OPERATOR (handle unsupported operator)"
./calc_client $1 2 ? 3

echo "\n"
echo "Unit Test ID 9: ./calc_client $1 100000000000 + 1234567890"
echo "Expected result: SRVR_OVERFLOW (handle overflow)"
./calc_client $1 10000000000 + 1234567890

## end of unit tests
exit