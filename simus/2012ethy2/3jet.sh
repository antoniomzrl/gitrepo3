#!/bin/bash

source $HOME/lepts/conflepts.sh

chbr="
   worldbox  7*cm
   #chamber chname,material,R,L/2,PosX
   chamber  ch_ethylene,ethylene,1*cm,0.5*cm,0.5*cm
   generator gdisc,vacio,0.5*mm,1*mm,-6*cm
   detector  detdisc,gold,1*mm,1*mm,59*mm
   diafragm  detdiaf,gold,0.75*mm,10*um,5.01*cm
"

FL="
   xsdat    ethylene
   density  0.5e-03*g/cm3

   gparticle e-
   genergy   10*keV

   stopenergyelectron 1*eV
   #verbose parser,gamos,lepts,headers,detectorhits,histograms
   verbose histograms
   #vis vrml
   #beamon 100000
   #batch 48:00:00
"

FL2="
   genergy 1*keV
   density 0.1e-04*g/cm3
   stopenergyelectron 1*eV
"

# detint.bcd:
time joblepts $FL $FL2 $chbr dirjob out  beamon 50000
exit


#time joblepts $FL $chbr dirjob o1 genergy 10*keV beamon   1 batch 48:00:00 > k1
#time joblepts $FL $chbr dirjob o2 genergy 10*keV beamon  10 batch 48:00:00 > k1
#time joblepts $FL $chbr dirjob o3 genergy 10*keV beamon  50 batch 48:00:00 > k1
#time joblepts $FL $chbr dirjob o4 genergy 10*keV beamon 100 batch 48:00:00 > k1

#for((i=1;i<101;i++)) ; do
#    let sd=(i+1000)*100
#    joblepts $FL $FL2 $chbr seed $sd dirjob o_${sd} beamon 100 batch 24:00:00
#done
