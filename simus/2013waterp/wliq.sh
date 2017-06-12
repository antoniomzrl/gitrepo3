#!/bin/bash

source $HOME/lepts/conflepts.sh

# camara
chamber="
   worldbox  15*cm
   #chamber  chname,material,R,L/2,PosX
   chamber   ch_water,water,5*cm,5*cm,0
   #generator gdisc,vacio,0.25*um,1*um,-10.5*cm
   generator gdisc,vacio,0.25*um,1*um,0
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
#Pres=0.2*atmosphere
#Dens=${Pres}*${MM}'/('${Temp}*${KbNa}')'
Dens=1*g/cm3
Ein=10*keV


FL="
   xsdat  water  material water   density  ${Dens}
   gparticle e-  genergy  ${Ein}
   stopenergyelectron 100*eV
   vis      vrml
   #verbose parser,gamos,lepts,headers,histograms,detectorhits
   verbose histograms

   #beamon   1000000000#batch    26:00:00
   beamon   100
"



time joblepts $chamber $FL genergy 10*keV stpe 0.4*eV beamon 1 dirjob o1 > k1
mv o1/detint.bcd una.bcd
exit
time joblepts $chamber $FL genergy 10*keV stpe 0.4*eV beamon 10 dirjob o2 > k2
mv o2/detint.bcd diez.bcd
time joblepts $chamber $FL genergy 10*keV stpe 100*eV beamon 1000 dirjob o3 > k3
mv o3/detint.bcd mil.bcd
time joblepts $chamber $FL genergy 10*keV stpe 10*eV beamon 1000 dirjob o4 > k4
mv o4/detint.bcd mil2.bcd




time joblepts $chamber $FL genergy 1*keV stpe 50*eV beamon 10 dirjob o1 > k1
mv o1/detint.bcd k1.bcd

time joblepts $chamber $FL genergy 1*keV stpe 10*eV beamon 10 dirjob o2 > k2
mv o2/detint.bcd k2.bcd

time joblepts $chamber $FL genergy 1*keV stpe 5*eV beamon 10 dirjob o3 > k3
mv o3/detint.bcd k3.bcd




time joblepts $chamber $FL stpe 100*eV beamon 1000 dirjob o1 > k1
mv o1/detint.bcd 1-mil-electr.bcd
#time joblepts $chamber $FL stpe 0.4*eV beamon 10 dirjob o2 > k2
#mv o2/detint.bcd 2-diez-electr.bcd
#time joblepts $chamber $FL stpe 0.4*eV beamon 1 dirjob o3 > k3
#mv o3/detint.bcd 3-un-electr.bcd
#time joblepts $chamber $FL stpe 0.4*eV beamon 1 dirjob o3b seed 111 > k3b
#mv o3b/detint.bcd 3b-un-electr.bcd
time joblepts $chamber $FL stpe 0.35*eV beamon 1 gparticle e+ dirjob o4 > k4
mv o4/detint.bcd 4-un-positr.bcd
time joblepts $chamber $FL stpe 0.35*eV beamon 1 gparticle e+ dirjob o4b seed 111 > k4b
mv o4b/detint.bcd 4b-un-positr.bcd

exit

for((i=1;i<101;i++)) ; do
    let sd=(i+1000)*100
    time joblepts $chamber $FL stpe 0.4*eV beamon 1 seed $sd > k
    drawpw jobdir/detint.bcd
done