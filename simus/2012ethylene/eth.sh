#!/bin/bash

PEXE=$HOME/penelope/penslab.sh
source $HOME/lepts/conflepts.sh

CFL="
material ethylene
xsdat    ethylene
density  1.175E-03*g/cm3

generator point
gparticle e-
genergy   10*keV

stopenergyelectron .01*eV
halflength         1*cm

verbosehistogram   1
"

time $PEXE -p 1 -K 10e3 -n 10 -m ethylene.mat11 -t 1 -o opnl1 -v
time $PEXE -p 1 -K 10e3 -n 100 -m ethylene.mat11 -t 1 -o opnl2 -v
time joblepts $CFL dirjob o1 gposition "-1*mm 0 0" beamon 10 > k1
time joblepts $CFL dirjob o2 gposition "-1*mm 0 0" beamon 100 > k1

mv opnl1/xyze.txt penelethy10.txt
mv opnl2/xyze.txt penelethy100.txt
mv  o1/detint.bcd leptsethy10.bcd
mv  o2/detint.bcd leptsethy100.bcd
