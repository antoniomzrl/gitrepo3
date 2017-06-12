#!/bin/bash

source head.sh

DBG="
   verbose 5
   #vis ogl
   vis vrml
"

#pes*1.1  & temp*1.1
Pres=1*atmosphere/760000
Temp=273.15*kelvin
Dens=${Pres}*${MM}'/('${Temp}*${KbNa}')'
time joblepts $ch2cm11 $CFL $DBG genergy 10*eV density ${Dens} dirjob o1 > k1

Pres=1*atmosphere/760000*1.1
Temp=273.15*kelvin*1.1
Dens=${Pres}*${MM}'/('${Temp}*${KbNa}')'
time joblepts $ch2cm11 $CFL $DBG genergy 10*eV density ${Dens} dirjob o2 > k2

diff -r o1 o2
