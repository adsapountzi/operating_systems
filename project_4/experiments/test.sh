#!/bin/bash

cd mountdir
printf '=%.0s' {1..8192} > test1_extended.output
printf '+%.0s' {1..8192} > test2_extended.output
cp test1_extended.output test1.copy
cp test2_extended.output test2.copy