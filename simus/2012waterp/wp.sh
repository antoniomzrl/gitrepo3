#!/bin/bash

PEXE=$HOME/penelope/penslab.sh

source $HOME/lepts/conflepts.sh

CFL="
material water xsdat water density 1*g/cm3
#generator fudisco05mm
generator point gparticle e+ genergy 10*keV 
stopenergyelectron 0.4*eV
halflength 1*cm
#verbose 1
#verbosehistogram 1
#verbosedetector 1
"
echo $CFL

time $PEXE -p 3 -K 10e3 -n 10 -m waterl.mat11 -t 1 -o opnl1 -v
time $PEXE -p 3 -K 10e3 -n 100 -m waterl.mat11 -t 1 -o opnl2 -v
time joblepts $CFL dirjob o1 gposition "-1*mm 0 0" beamon 10 > k1
time joblepts $CFL dirjob o2 gposition "-1*mm 0 0" beamon 100 > k2

mv o1/detint.bcd leptswp10.bcd
mv o2/detint.bcd leptswp100.bcd
mv opnl1/xyze.txt penelwp10.txt
mv opnl2/xyze.txt penelwp100.txt
