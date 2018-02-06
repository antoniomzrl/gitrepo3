#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

# physics lists
INI="/run/initialize"
PHY="/gamos/physicsList GmEMPhysics          $INI"
PHY="/gamos/physicsList GmEMExtendedPhysics  $INI"
PHY="/gamos/physicsList HadrontherapyPhysics $INI"

MATS=":MATE vacuum 1 1*g/mole 0*g/cm3 
      :MIXT_BY_NATOMS G4_WATER 1*g/cm3 2  O 1 H 2"
WRL="$MATS
     :VOLU world BOX 31*m 31*m 31*m vacuum 
     //:COLOUR world 1 1 1 
     :ROTM rm0  0  0 0 
     :ROTM rmx 90  0 0 
     :ROTM rmy  0 90 0
     :ROTM rmz  0  0 90"
CHB=":MIXT_BY_NATOMS G4_WATER 1*g/cm3 2  O 1 H 2
     :VOLU  chb BOX 15*m 15*m 15*m G4_WATER
     :PLACE chb 1 world rm0 15*m 0 0"

UAS="#/gamos/userAction UAVerbose
     #/gamos/userAction UAWIF
     #/gamos/userAction UAInteraction
     /gamos/userAction UAClock"
ULI="/gamos/userAction GmKillAtStackingActionUA GmElectronFilter"
#RUN="$(vis) /run/beamOn 10"
#PAR="--jobs 10 --ppn 10"



#Edep versus depth

GEN=$(gen gamma 1*MeV 1*nm -20*cm)
ISP="/gamos/setParam UAInteractionSp:x 600 0 3000*mm
     /gamos/setParam UAInteractionSp:Title Edep-Depth
     /gamos/userAction UAInteractionSp"
RUN="/run/beamOn 100000"
#jgamos --dir ooph $PAR $WRL $CHB $PHY $UAS $ISP $GEN $ULI $RUN &

#250MeV->400mm 180MeV->225mm
GEN=$(gen proton 180*MeV 1*nm -20*cm)
ISP="/gamos/setParam UAInteractionSp:x 500 0 250*mm 
     /gamos/setParam UAInteractionSp:Title Edep-Depth
     /gamos/userAction UAInteractionSp"
RUN="/run/beamOn 1000000"
jgamos --dir oopr $PAR $WRL $CHB $PHY $UAS $ISP $GEN $ULI $RUN &

wait


exit

Estoy escribiendo un proyecto europeo sobre el efecto radiobiológico de electrones secundarios y radicales en hadronterapia. Necesitaría una figura que no quiero tomar de la web sino que preferiría que fuese original nuestra. Es la típoca de la

energía depositada en función de la profundidad en agua para haces incidentes de fotones de 1 MeV y de protones de 250 MeV.

Utilizando Geant-Gamos saldrá bastante rápido aunque necesitaría un dato adicional para cada profundidad añadir la energía media de los fotones/protones del haz. Sería una tabla de tres columnas: profundidad, energía depositada, energía media a esa profundidad. Si ves algo raro pregúntame.
