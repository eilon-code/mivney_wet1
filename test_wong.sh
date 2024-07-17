#!/bin/bash

cd code
cd test

make -f makefile

dos2unix tester.sh
./tester.sh
