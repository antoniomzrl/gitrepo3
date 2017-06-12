#!/bin/bash -x

i=$HOME/lepts/
o=euler:cmt2540/lepts/

SUFFIX='-BCK-'`date +%Y-%m-%d-%T`

time rsync -av -b --suffix=${SUFFIX} --exclude='*~' $i $o
