#!/bin/bash

source $HOME/lepts/bin/conflepts.sh 510
source $HOME/lepts/bin/gdefs.sh

WRL=$(wrl 5*mm 5*mm 5*mm)
GEN=$(gen e- 5*keV 10*um -2*mm)
#VIS=$(vis)
UAS="/gamos/analysis/histo1Max *Ener* 1*MeV
     /gamos/analysis/histo1NBins *Ener* 1000
     /gamos/userAction GmGenerHistosUA
     /gamos/userAction UAExit
     /gamos/userAction UAInteraction
     /gamos/userAction GmCountProcessesUA"

#RUN="/run/beamOn 1000000 /batchtime 0:05:00"
RUN="/run/beamOn 100000"

# GmLEPTSPhysics e-
WL="LEPTS_WATER_LIQUID"
MAT=":MATE vacuum 1 1*g/mole 1e-25*g/cm3
     :MATE $WL 1.0 18*g/mole 1*g/cm3"
ULI=$(uli $WL e- 10*eV)
PHY="/gamos/physicsList GmLEPTSPhysics
     /run/initialize"
COIN=$(coin $WL 1*mm 0.1*nm 0)
CFL="$WRL $MAT $COIN $PHY $UAS $GEN $SEED $ULI $VIS $RUN"
time jlepts $CFL /dir o1 >& k1


# GmEMPhysics e-
WL="G4_WATER"
MAT=":MATE vacuum 1 1*g/mole 1e-25*g/cm3
     :MATE $WL 1.0 18*g/mole 1*g/cm3"
ULI=$(uli $WL e- 10*eV)
PHY="/gamos/physicsList GmEMPhysics
     /gamos/GmPhysics/replacePhysics electron-lowener
     /gamos/GmPhysics/replacePhysics gamma-lowener
     /run/initialize"
COIN=$(coin $WL 1*mm 0.1*nm 0)
CFL="$WRL $MAT $COIN $PHY $UAS $GEN $SEED $ULI $VIS $RUN"
time jlepts $CFL /dir o2 >& k2


#GmDNAPhysics
WL="G4_WATER"
MAT=":MATE vacuum 1 1*g/mole 1e-25*g/cm3
     :MATE $WL 1.0 18*g/mole 1*g/cm3"
ULI=$(uli $WL e- 10*eV)
PHY="/gamos/physicsList GmDNAPhysics
     /run/initialize"
COIN=$(coin $WL 1*mm 0.1*nm 0)
CFL="$WRL $MAT $COIN $PHY $UAS $GEN $SEED $ULI $VIS $RUN"
time jlepts $CFL /dir o3 >& k3
