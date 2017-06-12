#!/bin/bash

source head.sh


CFL=$CFL beamon 10000000 batch 1:00:00

# variando radio haz & detector
for RD in 1 2 3 4 5 6 7 8 9 10 30 50 70 100 300 500 700 1000 ; do
    ch="
    worldbox  2.5*cm
    chamber   ch_thf,THF,10*mm,11.05*mm,0
    generator gdisc,vacio,${RD}*um,1*mm,-15*mm
    detector  detdisc,gold,${RD}*um,1*mm,20.05*mm
    diafragm  detdiaf,gold,${RD}*um,10*um,11.2*mm
    "
    Eo=5*eV
    Pres=1*atmosphere/760000
    Dens=${Pres}*${MM}'/('${Temp}*${KbNa}')'
    of=o_${RD}
    joblepts $ch $CFL genergy ${Eo} density ${Dens} dirjob $of
done
