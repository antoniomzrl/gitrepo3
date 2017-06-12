#!/bin/bash -x

DIRS="lepts"
SUFFIX='_BCK_'`date +%Y-%m-%d-%T`
MAIL=roldan@ciemat.es


rm -f /tmp/bck.txt

for D in $DIRS ; do
    i=$HOME/$D/
    o=euler:cmt2540/$D/
    rsync -av --suffix=SUFFIX  \
	  --exclude='*~' --exclude='*.o' --exclude='*.obj' \
	   $i $o >> /tmp/bck.txt
done

cat  /tmp/bck.txt | ssh euler mail -s "backup$SUFFIX" $MAIL
