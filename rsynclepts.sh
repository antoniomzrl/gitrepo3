#!/bin/bash -x

DIRS="lepts"
SUFFIX='_BCK_'`date +%Y-%m-%d-%T`

for D in $DIRS ; do
    i=$HOME/$D/
    o=euler:$D/
    rsync -avb --suffix=$SUFFIX  \
	  --exclude='*~' --exclude='*.o' --exclude='*.obj' --exclude='.git' \
	   $i $o
done
#
