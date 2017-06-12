#!/bin/bash

source $HOME/lepts/conflepts.sh

# camara
chamber="
   worldbox  15*cm
   #chamber  chname,material,R,L/2,PosX
   chamber   ch_water,water,5*cm,5*cm,0
   generator gdisc,vacio,0.5*mm,1*mm,-10.5*cm
   detector  detdisc,gold,1*cm,1*mm,11*cm
   detector2 detdisc2,gold,1*cm,1*mm,-11*cm
   diafragm  detdiaf,gold,4*cm,10*um,12*cm
"

Avogadro=6.02214179e+23/mole
k_Boltzmann=8.617343e-11*MeV/kelvin
KbNa=${k_Boltzmann}'*'${Avogadro}
MM=18*g/mole
Temp=303.55*kelvin
#Pres=0.4*atmosphere/760000 # mTorr
Pres=0.2*atmosphere
Dens=${Pres}*${MM}'/('${Temp}*${KbNa}')'
Ein=10*keV

#Dens=1*g/cm3


FL="
   xsdat     water  material water   density  ${Dens}
   gparticle e+     genergy  ${Ein}
   stopenergyelectron 100*eV
   #vis      vrml
   verbose parser,gamos,lepts,headers,histograms,detectorhits
   #verbose histograms

   #beamon   1000000000#batch    26:00:00
   beamon   100
"


#time joblepts $chamber $FL genergy 10*keV beamon 1000 dirjob o1 > k1
#mv o1/detint.bcd jet.bcd

#time joblepts $chamber $FL genergy 5*keV beamon 1 stpe 0.4*eV dirjob o2 > k2
#mv o2/detint.bcd traj.bcd





time joblepts $chamber $FL stpe 0.4*eV beamon 1 genergy 10*keV > k


exit


for((i=1;i<102;i++))
do
    joblepts $chamber $FL genergy 5*keV beamon 1 stpe 0.4*eV seed $i > k
    drawpw jobdir/detint.bcd
done





