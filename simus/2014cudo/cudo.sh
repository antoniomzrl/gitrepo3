#!/bin/bash

source $HOME/lepts/bin/conflepts.sh 510
source $HOME/lepts/bin/gdefs.sh

INI="/run/initialize"
PHYg="/gamos/physicsList GmEMPhysics     $INI"
PHYd="/gamos/physicsList GmDNAPhysics    $INI"
PHYl="/gamos/physicsList GmLEPTSPhysics  $INI"

WRL=$(wrl 1.5*m 1.5*m 1.5*m)
WL="G4_WATER"

#mTorr=atmosphere/760000
#Pres=1.5*${mTorr}
#Dens=$(dens 18*g/mole ${Pres})

Dens="1*g/cm3"
MAT=":MATE $WL 1.0 18*g/mole ${Dens}"
CHB=$(chb $WL 0.5*m 0.5*m 0.5*m 0.25*m)

UAS="#USERACTIONS
     /gamos/userAction GmCountProcessesUA
     /gamos/userAction GmTrackingVerboseUA"

ULI=$(uli $WL e- 1*eV)
GENMONO=$(genbox gamma 1*MeV 5*cm -70*cm)
GENSPEC=$(genbox gamma 1*MeV 5*cm -70*cm photonSpec5p9MeV.txt)

RUN="#RUN /run/beamOn 20"
#MAG="/gamos/field/setMagField 0.1*tesla 0 0"

CFL="$WRL $MAT $CHB $PHYg $UAS $GENMONO $ULI $(vis) $RUN"
#time jlepts $CFL /dir examp /jn examp1

UAS="#USERACTIONS
     /gamos/userAction GmCountProcessesUA
     /gamos/analysis/histo1Max *Ener* 2*MeV
     /gamos/analysis/histo1NBins *Ener* 200
     /gamos/userAction GmGenerHistosUA"
RUN="#RUN /run/beamOn 1000"
CFL="$WRL $MAT $CHB $PHYg $UAS $GENSPEC $ULI $RUN"
time jlepts $CFL /dir examp4 /jn examp4
