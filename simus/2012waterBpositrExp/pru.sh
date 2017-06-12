#!/bin/bash

source $HOME/lepts/conflepts.sh

# camara 20cm
ch20cm="
   worldbox  15*cm
   chamber   ch_water,waterB,5*cm,10*cm,0    #chamber chname,material,R,L/2,PosX
   generator gdisc,vacio,0.75*mm,1*mm,-10.5*cm
   detector  detdisc,gold,1*cm,1*mm,11*cm
   detector2 detdisc2,gold,1*cm,1*mm,-11*cm
   diafragm  detdiaf,gold,4*cm,10*um,12*cm
"

Avogadro=6.02214179e+23/mole
k_Boltzmann=8.617343e-11*MeV/kelvin
KbNa=${k_Boltzmann}'*'${Avogadro}

MM=18*g/mole
Ein=10*eV
Pres=0.4*atmosphere/760000
Temp=303.55*kelvin
Dens=${Pres}*${MM}'/('${Temp}*${KbNa}')'

FL="
   density  ${Dens}
   gparticle e+
   genergy  ${Ein}
   xsdat    waterB
   material waterB
   #vis      vrml
   #verbose histograms
"

FV="
   verbose parser,gamos,lepts,headers,histograms,detectorhits
"


time joblepts $ch20cm $FL $FV dirjob o1 beamon 10000 > k1
time joblepts $ch20cm $FL     dirjob o2 beamon 10000000 > k2


#time joblepts $ch20cm $FL                     dirjob o1 > k1
#time joblepts $ch20cm $FL mag "0.2*tesla 0 0" dirjob o2 > k2
#time joblepts $ch20cm $FL mag "0.2*tesla 0 0" gdirection '1 1 0' dirjob o3 > k3
#time joblepts $ch20cm $FL locmag "0.2*tesla 0 0" gdirection '1 1 0' dirjob o4 > k4
#time joblepts $ch20cm $FL gdirection '1 0.5 0' beamon 10000 dirjob o5 > k5

