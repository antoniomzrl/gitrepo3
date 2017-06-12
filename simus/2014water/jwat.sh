#!/bin/bash

source $HOME/lepts/bin/conflepts.sh

MAT=" //MAT-water-vacuum
      :ROTM rmz 0 90 0
      :MATE vacuum  1. 1.008*g/mole   1.e-25*g/cm3
      :MATE water  10. 18*g/mole 1*g/cm3
      :MATE_TEMPERATURE water 273.15*kelvin"

WRL=" //WORLD-3m3
      :VOLU   world BOX 5*cm 5*cm 5*cm vacuum
      :COLOUR world 1 1 1"

CHB=" //CHAMBER-water-1cm3
      :VOLU   chwater BOX 1*cm 1*cm 1*cm water
      :PLACE  chwater 1 world rmz 0 0 0
      :COLOUR chwater 0.6 0.6 0.0"

PHY=" #PHYSICS
      /gamos/physicsList LeptsPhysicsList
      #/gamos/setParam ElectronModel leptsold
      /gamos/setParam ElectronModel lepts
      /gamos/setParam XsDat water
      #/gamos/setParam CutEnergyIoniz 10"

UAS=" #/gamos/analysis/histo1Max *Ener* 7*MeV
      #/gamos/userAction GmGenerHistosUA
      #/gamos/userAction UserActionPrueba
      #/gamos/userAction UAVerbose
      #/gamos/userAction UADepo
      /gamos/userAction UAInteraction
      /gamos/userAction UAWIF"

INI=" #RUN-INIT
      #/gamos/userAction Run
      #/gamos/userAction Event
      /run/initialize"

GBOX="//BOX um
      :VOLU   gbox BOX 1*nm 1*nm 1*nm vacuum
      :PLACE  gbox 1 world rmz -3*cm 0 0
      :COLOUR gbox 1 0 0"

GEN=" #GENERATOR-e
      $GBOX
      /gamos/generator GmGenerator
      /gamos/generator/addSingleParticleSource gene e- 3*keV
      /gamos/generator/positionDist  gene GmGenerDistPositionInG4Volumes gbox
      /gamos/generator/directionDist gene GmGenerDistDirectionConst 1 0 0"

VIS=" #VISU
      /vis/scene/create
      /vis/open VRML2FILE
      /vis/viewer/flush
      /tracking/storeTrajectory 1
      /vis/scene/add/trajectories
      /vis/scene/endOfEventAction accumulate
      /vis/viewer/update
      /vis/viewer/flush"

#SEED="#/gamos/random/setSeeds $sd $sd"
#CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $GEN $VIS $RUN $SEED"


STPE="/gamos/setParam StopEnergyElectron 0.375*eV"
STPE2="/gamos/setParam StopEnergyElectron 0.2*eV"
#RUN="/run/beamOn 5"
#UAS="/gamos/userAction UAWIF"
RUN="/run/beamOn 3"
UAS="/gamos/userAction UAInteraction /gamos/userAction UAVerbose"

CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $GEN $RUN"
time jlepts $CFL /dir o > k
#time jlepts $CFL /dir o2 /batchtime 1:0:0 > k2

#CFL="$MAT $WRL $CHB $PHY $UAS $STPE2 $INI $GEN $RUN"
#time jlepts $CFL /dir o2 > k2



exit


# G4-DNA:
MAT=":ROTM rmz 0 90 0
     //MAT-water-gold-vacuum
     :MATE vacuum  1. 1.008*g/mole   1.e-25*g/cm3
     :MATE gold   79. 196.967*g/mole 19.3*g/cm3
     //:MATE G4_WATER  10. 18*g/mole 1*g/cm3
     //:MATE_TEMPERATURE G4_WATER 273.15*kelvin"

CHB="//CHAMBER-water-1m3
     :VOLU   chwater BOX 0.5*m 0.5*m 0.5*m G4_WATER
     :PLACE  chwater 1 world rmz 0 0 0
     :COLOUR chwater 0.6 0.6 0.0"

PHY="/gamos/physicsList GmDNAPhysics"
CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $ge10k $RUN"
time jlepts $CFL /dir oni2 > k2

    # g4-DNA-Sally
PHY="/gamos/physicsList SallyDNAPhysicsList"
CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $ge10k $RUN"
time jlepts $CFL /dir oni3 > k3


