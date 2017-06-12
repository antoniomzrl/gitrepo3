#!/bin/bash

source $HOME/lepts/conflepts.sh

MAT="
  // Matrs
  :ROTM rm0 0 0. 0.
  :ROTM rmz 0 90 0
  :ELEM Hydrogen H 1. 1.
  :ELEM Oxygen   O 8 16.
  :ELEM Carbon   C 6 12.
  :MIXT Water    1*g/cm3 2   Hydrogen 2./18.   Oxygen  16./18.
  :MIXT Ethylene 1*g/cm3 2   Hydrogen 4./28.   Carbon  24./28.
  :MIXT TetraHF  1*g/cm3 3   Hydrogen 8./72.   Carbon  48./72.   Oxygen  16./72.
  :MATE vacio 1.0   1.008*g/mole   1.e-25*g/cm3
  :MATE gold        79. 196.967*g/mole 19.3*g/cm3
  :MATE goldwrapper 80 160*g/mole  1e9*g/cm3
  :MATE water       10. 18*g/mole 1*g/cm3
  :MATE waterB      10. 18*g/mole 1*g/cm3
  :MATE ethylene    16. 28*g/mole 1*g/cm3
  :MATE argon       18. 39.948*g/mole 1*g/cm3
  :MATE THF         40. 72.11*g/mole 1*g/cm3
  :MATE_TEMPERATURE water 273.15*kelvin
"
WRL="
  :VOLU   world BOX 1.5*m 1.5*m 1.5*m vacio
  :COLOUR world 1 1 1
"
WRP="
  :VOLU wrapper SPHERE 0.89*1.5*m 0.9*1.5*m 0 360*deg 0 180*deg gold
  :PLACE  wrapper 1 world rmz 0. 0. 0.
  :COLOUR wrapper 1 1 1
  :VIS wrapper OFF
"

CHB="
  :VOLU ch_box BOX 0.5*m 0.5*m 0.5*m water
  :PLACE  ch_box 1 world rmz 0 0 0
  :COLOUR ch_box 0.6 0.6 0.0
"


PHY="
# PHYSICS
/gamos/physicsList LeptsPhysicsList
# OUTPUT ANALYSIS, USERACTIONS
/gamos/setParam XsectionDir xx
/gamos/setParam ElectronModel lepts
/gamos/setParam PhotonModel penelope
/gamos/setParam XsDat water
/gamos/setParam CutEnergyIoniz   10
/gamos/setParam StopEnergyElectron 9*MeV
/gamos/setParam probOfDeathElastic 0.9
/gamos/setParam VerboseLepts 0
/gamos/setParam VerboseHeaders 0
/gamos/setParam VerboseDetector 0
/gamos/setParam VerboseHistograms 1
/gamos/setParam ChamberHalfLength 0.5*m
/gamos/setParam SpatialHistogram 101,-505,505,101,-505,505,101,-505,505
/gamos/setParam ContinuationFile none

/gamos/userAction Run
/gamos/userAction Event

#/gamos/setParam GmCountTracksUA:EachNEvent 1000
#/gamos/userAction GmCountTracksUA
#/gamos/userAction GmCountProcessesUA
#/gamos/setParam track:hEMax 10*keV
#/gamos/setParam track:hENbins 1000
#/gamos/userAction GmTrackHistosUA

/gamos/analysis/histo1Max *Ener* 7*MeV
/gamos/userAction GmGenerHistosUA
/gamos/userAction UserActionPrueba
# INIT
/run/initialize

#filtro generador track
#/gamos/setParam track.text_GmPrimaryFilter:PosInitialY 1
#/gamos/setParam track.text_GmPrimaryFilter:PosInitialX 1
#/gamos/setParam track.text_GmPrimaryFilter:PosInitialZ 1
#/gamos/setParam track.text_GmPrimaryFilter:EnergyInitial 1
#/gamos/setParam track.text_GmPrimaryFilter:EDepo 1
#/gamos/setParam track.text_GmPrimaryFilter:ELost 1
#/gamos/setParam track.text_GmPrimaryFilter:NSeco 1
#/gamos/setParam track.text_GmPrimaryFilter:NSteps 1
#/gamos/setParam track.text_GmPrimaryFilter:CreatorProcess 1
#/gamos/userAction GmTrackTextFileUA GmPrimaryFilter
"

DETW="
  #DETECTOR2
  /gamos/SD/assocSD2LogVol Detector detector2 wrapper
"

DET="
  #DETECTOR
  /gamos/SD/assocSD2LogVol Detector det  ch_box
"

GEN="
  :VOLU   gbox BOX 5*cm 5*cm 5*cm vacio
  :PLACE  gbox 1 world rmz -80*cm 0 0
  :COLOUR gbox 1 0 0

  #GENERATOR
  /gamos/generator GmGenerator
  /gamos/generator/addSingleParticleSource gene gamma 1*MeV
  /gamos/generator/positionDist  gene GmGenerDistPositionInG4Volumes gbox
  /gamos/generator/directionDist gene GmGenerDistDirectionConst 1 0 0
  /gamos/generator/energyDist    gene GmGenerDistEnergyFromFile
                                 photonSpec5p9MeV.txt interpolate 1*MeV
  #/gamos/generator/listSources
"

RUN="
  # RUN
  /run/setCut 1e-9 um
  /run/beamOn 10000
"


if [ $1 == "useraction" ] ; then
    set -x
    RUN="/run/beamOn 20000"
    CFL="$MAT $WRL $WRP $CHB $PHY $DET $DETW $GEN"
    CFL="$MAT $WRL      $CHB $PHY $DET       $GEN"
    time jlepts $CFL $RUN /dir ok > k
fi
