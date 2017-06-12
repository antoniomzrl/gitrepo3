#!/bin/bash

source $HOME/lepts/conflepts.sh

MAT="
  :ROTM rmz 0 90 0
  //MAT-water-gold-vacuum
  :MATE vacuum  1. 1.008*g/mole   1.e-25*g/cm3
  :MATE gold   79. 196.967*g/mole 19.3*g/cm3
  :MATE water  10. 18*g/mole 1*g/cm3
  :MATE_TEMPERATURE water 273.15*kelvin
"
WRL="
  //WORLD-3m3
  :VOLU   world BOX 1.5*m 1.5*m 1.5*m vacuum
  :COLOUR world 1 1 1
"
WRP="
  //WRAPPER-sphere-1.3m
  :VOLU wrapper SPHERE 1.3*m 1.35*m 0 360*deg 0 180*deg gold
  :PLACE  wrapper 1 world rmz 0. 0. 0.
  :COLOUR wrapper 1 1 1
  :VIS wrapper OFF
"

CHB="
  //CHAMBER-water-1m3
  :VOLU   chwater BOX 0.5*m 0.5*m 0.5*m water
  :PLACE  chwater 1 world rmz 0 0 0
  :COLOUR chwater 0.6 0.6 0.0
"


PHY="
#PHYSICS
/gamos/physicsList LeptsPhysicsList
#OUTPUT-ANALYSIS-USERACTIONS
/gamos/setParam ElectronModel lepts
/gamos/setParam PhotonModel penelope
/gamos/setParam XsDat water
/gamos/setParam CutEnergyIoniz   10
/gamos/setParam ContinuationFile none
"


PHYPRU="
  /gamos/physicsList GmEMPhysics
  /gamos/GmPhysics/addPhysics gamma-lowener
  /gamos/GmPhysics/addPhysics electron-lowener
"


STPE="/gamos/setParam StopEnergyElectron 9*MeV"

UAS="
   /gamos/analysis/histo1Max *Ener* 7*MeV
   /gamos/userAction GmGenerHistosUA
   #/gamos/userAction UserActionPrueba
   #/gamos/userAction UAVerbose
   /gamos/userAction UADepo
   /gamos/userAction UAInteraction
"


SEED="#/gamos/random/setSeeds $sd $sd"


INI="
   #RUN-INIT
   #/gamos/userAction Run
   #/gamos/userAction Event
   /run/initialize
"

DET="
  #DETECTOR-chwater
  #/gamos/SD/assocSD2LogVol Detector det1 chwater
"

DETW="
  #DETECTOR-wrapper
  #/gamos/SD/assocSD2LogVol Detector det2 wrapper
"


BOX10cm="
  //BOX-10cmx10cm
  :VOLU   gbox BOX 5*cm 5*cm 5*cm vacuum
  :PLACE  gbox 1 world rmz -80*cm 0 0
  :COLOUR gbox 1 0 0
"

GENMONO="
  $BOX10cm
  #GENERATOR-1MeV
  /gamos/generator GmGenerator
  /gamos/generator/addSingleParticleSource gene gamma 1*MeV
  /gamos/generator/positionDist  gene GmGenerDistPositionInG4Volumes gbox
  /gamos/generator/directionDist gene GmGenerDistDirectionConst 1 0 0
"


GENSPEC="
  $GENMONO
  #GENERATOR-spect5.9MeV
  /gamos/generator/energyDist gene GmGenerDistEnergyFromFile
                              photonSpec5p9MeV.txt interpolate 1*MeV
"


VIS="
   #VISU
   /vis/scene/create
   /vis/open VRML2FILE
   /vis/viewer/flush
   /tracking/storeTrajectory 1
   /vis/scene/add/trajectories
   /vis/scene/endOfEventAction accumulate
   /vis/viewer/update
   /vis/viewer/flush
"


RUN="
  #RUN
  /run/setCut 1e-9 um
  /run/beamOn 10000
"


if [ $1 == "prueba" ] ; then
    RUN="/run/beamOn 100"
   #RUN="/run/beamOn 100000000 /batchtime 24:0:0 "

    CFL="$MAT $WRL $WRP $CHB $PHY $STPE $UAS $INI $GENMONO"
    time jlepts $CFL $RUN /dir ok > k
fi


if [ $1 == "pru" ] ; then
    RUN="/run/beamOn 10000"
    for((i=1;i<2;i++)) ; do
	let sd=(i+1000)*100
	SEED="/gamos/random/setSeeds $sd $sd"
	CFL="$MAT $WRL $CHB $PHY $STPE $UAS $INI $GENMONO $RUN $SEED"
	jlepts $CFL /dir op_$sd
    done
fi


if [ $1 == "phots" ] ; then
    RUN="/run/beamOn 10000000 /batchtime 4:0:0"
    for((i=1;i<101;i++)) ; do
	let sd=(i+1000)*100
	SEED="/gamos/random/setSeeds $sd $sd"
	CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $GENSPEC $RUN $SEED"
	jlepts $CFL /dir op_$sd
    done
fi



if [ $1 == "verbose" ] ; then
    RUN="/run/beamOn 2"
    STPE="/gamos/setParam StopEnergyElectron 100*eV"
    UAS="/gamos/userAction UAVerbose"
    CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO"
    time jlepts $CFL $RUN /dir ok > k
fi

if [ $1 == "wif" ] ; then
    RUN="/run/beamOn 5"
    STPE="/gamos/setParam StopEnergyElectron 100*eV"
    UAS="/gamos/userAction UAWIF 
         /gamos/userAction UAInteraction"
    CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO"
    time jlepts $CFL $RUN /dir ow > kw
fi


if [ $1 == "hginter" ] ; then
    RUN="/run/beamOn 10"
    STPE="/gamos/setParam StopEnergyElectron 100*eV"
    UAS="/gamos/userAction UAInteraction"

    CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO $RUN"
    time jlepts $CFL /dir ok1 > k1

    PHY="/gamos/physicsList GmEMPhysics"
    CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO $RUN"
    time jlepts $CFL /dir ok2 > k2

    PHY="/gamos/physicsList GmEMPhysics
         /gamos/GmPhysics/replacePhysics electron-penelope"
    CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO $RUN"
    time jlepts $CFL /dir ok3 > k3

    PHY="/gamos/physicsList GmDNAPhysics"
    CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO $RUN"
    time jlepts $CFL /dir ok4 > k4
fi
