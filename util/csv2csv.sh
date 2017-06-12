#!/bin/bash

FI=$1

#CMD='{ {$1*=1.1};print $1,$2,$3,$4,$5,$6,$7,$8,$9}'
#grep '#' $FI > k1
#grep -v '#' $FI | grep -v -e '^$' | tr ',' ' ' | awk "$CMD" | tr ' ' ',' >> k1

# hcsv awk-command fileIn fileOut
hcsv() {
    CMD=$1
    FI=$2
    FO=$3
    PRE='BEGIN{FS=OFS=",";}'
    grep '#' $FI > $FO
    grep -v '#' $FI | grep -v -e '^$' | tr -d ' ' | awk "$PRE $CMD" >> $FO
}

#hcsv '{ $1*=1.1; print $0; }' $FI out${FI}
#hcsv '{ $2*=0.28; $3*=0.28; $4*=0.28; $5*=0.28; $6*=0.28; print $0; }' $FI out${FI}

hcsv '{ $2*=$11; $3*=$11; $4*=$11; $5*=$11; $6*=$11; $7*=$11; $8*=$11; $9*=$11; $10*=$11; print $0}' \
     wge-IXS-2017mar.csv wle-IXS-2017mar.csv
