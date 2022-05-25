#!/bin/bash

fusermount -u mountdir
rm -r mountdir
rm -r rootdir
rm -r .realStorage
mkdir rootdir
mkdir mountdir
../src/bbfs rootdir/ mountdir/