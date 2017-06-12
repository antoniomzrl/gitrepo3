#!/bin/bash

source $HOME/lepts/conflepts.sh

CH1mm="
   worldbox  7*cm
   #chamber chname,material,R,L/2,PosX
   chamber  ch_ethylene,ethylene,1*cm,0.5*mm,0
   generator gdisc,vacio,0.75*mm,1*mm,-6*cm
   detector  detdisc,gold,1*mm,1*mm,59*mm
   diafragm  detdiaf,gold,0.75*mm,10*um,5.01*cm
"

FL="
   xsdat    ethylene
   density  0.1E-06*g/cm3

   gparticle e-
   genergy   5*eV

   stopenergyelectron .01*eV
   #verbose parser,gamos,lepts,headers,detectorhits,histograms
   #vis vrml
   #beamon 100000
   #batch 48:00:00
"

# angle.hg & angli.hg:

#time joblepts $FL $CH1mm dirjob od1 genergy 5*eV   > k1
#time joblepts $FL $CH1mm dirjob od2 genergy 10*eV  > k2
#time joblepts $FL $CH1mm dirjob od3 genergy 100*eV > k3
#time joblepts $FL $CH1mm dirjob od4 genergy 1*keV  > k4
#time joblepts $FL $CH1mm dirjob od5 genergy 10*keV > k5

#time joblepts $FL $CH1mm dirjob od20 genergy 20*eV  > k
#time joblepts $FL $CH1mm dirjob od30 genergy 30*eV  > k
#time joblepts $FL $CH1mm dirjob od40 genergy 40*eV  > k
#time joblepts $FL $CH1mm dirjob od50 genergy 50*eV  > k
#time joblepts $FL $CH1mm dirjob od60 genergy 60*eV  > k
#time joblepts $FL $CH1mm dirjob od80 genergy 80*eV  > k
#time joblepts $FL $CH1mm dirjob od90 genergy 90*eV  > k

