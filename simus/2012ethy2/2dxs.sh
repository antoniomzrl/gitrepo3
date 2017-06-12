#!/bin/bash

source $HOME/lepts/conflepts.sh


CHsmall="
   worldbox  7*cm
   #chamber chname,material,R,L/2,PosX
   chamber  ch_ethylene,ethylene,1*cm,0.5*um,0
   generator gdisc,vacio,1*um,1*mm,-5*cm
   detector  detdisc,gold,1*mm,1*mm,69*mm
   diafragm  detdiaf,gold,0.75*mm,10*um,66*mm

   wrapper radius 6*cm
"

FL2="
   xsdat    ethylene
   density  0.1e-03*g/cm3

   gparticle e-

   #vis vrml
   #verbose parser,gamos,lepts,headers,detectorhits,histograms
   verbose histograms
   #beamon 100000
   #batch 48:00:00
"

# te20.hg & te80.hg:

time joblepts $CHsmall $FL2 dirjob o50 genergy 50*eV beamon 1000000 batch 48:00:00  > k 
time joblepts $CHsmall $FL2 dirjob o20 genergy 20*eV beamon 1000000 batch 48:00:00  > k 
