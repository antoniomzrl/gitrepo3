#!/bin/bash

#source $HOME/lepts/bin/gdefs.sh

INI="/run/initialize"
#PHY="/gamos/physicsList GmEMPhysics $INI"
PHY="/gamos/physicsList HadrontherapyPhysics $INI"
PHYl="/gamos/physicsList GmLeptsPhysics      $INI"

MATS=":MATE vacuum  1  1       1e-25*g/cm3
MATS=":MATE vacuum 1 1*g/mole 1e-25*g/cm3 
      :MIXT_BY_NATOMS G4_WATER_VAPOR 1*g/cm3 2  O 1 H 2
      :MATE iridium 77 192.217 22.42*g/cm3
      :MIXT steel316L 8.03*g/cm3 9
            G4_Fe  0.68745
            G4_Cr  0.19
            G4_Ni  0.0925
            G4_Mn  0.02
            G4_Si  0.0075
            G4_N   0.001
            G4_C   0.0008
            G4_P   0.00045
            G4_S   0.0003"

RMT=":ROTM rm0  0  0 0 
     :ROTM rmx 90  0 0 
     :ROTM rmy  0 90 0
     :ROTM rmz  0  0 90"

WRL="$MATS $RMT
     :VOLU world BOX 1.1*cm 1.1*cm 1.1*cm vacuum"


GIR=":VOLU   gir TUBE 0 0.6*mm/2  3.5*mm/2 iridium
     :PLACE  gir 1 world rmy 0 0 0
     :COLOUR gir 1 1 0
     /gamos/generator GmGenerator
     /gamos/generator/addSingleParticleSource gn gamma 1*keV
     /gamos/generator/positionDist  gn GmGenerDistPositionInG4Volumes gir
     /gamos/generator/energyDist    gn GmGenerDistEnergyFromFile Ir192.txt fixed 1*keV
     /gamos/generator/directionDist gn GmGenerDistDirectionRandom
     /gamos/generator/timeDist      gn GmGenerDistTimeConstant"

CAPS=":VOLU   caps TUBE 0.7*mm/2 0.9*mm/2 3.5*mm/2+0.065 steel316L
:PLACE  caps 1 world rmy 0 0 0
:COLOUR caps 0 1 1
:VOLU   tapa TUBE 0 0.9*mm/2 0.2*mm/2 steel316L
:PLACE  tapa 1 world rmy  3.5*mm/2+0.065*mm+0.2/2*mm 0 0
:PLACE  tapa 2 world rmy -3.5*mm/2-0.065*mm-0.2/2*mm 0 0	
:COLOUR tapa 0 1 1"

LCAPS=":VOLU   caps TUBE 0 0.9*mm/2 3.5*mm/2+0.065 vacuum"

VIS="/vis/scene/create 
/vis/open VRML2FILE 
/vis/viewer/flush 
/tracking/storeTrajectory 1 
/vis/scene/add/trajectories 
/vis/scene/endOfEventAction accumulate 
/vis/viewer/update 
/vis/viewer/flush"

 
UAS="#/gamos/userAction UAVerbose
#/gamos/userAction UAWIF
/gamos/userAction GmCountProcessesUA
/gamos/userAction UAClock /gamos/setParam TimeMark 3600
/gamos/userAction UAInteraction
#/gamos/userAction UAInteractionSp
/gamos/setParam SPhg:Title NoInts
/gamos/userAction UADepo"

KEE="/gamos/physics/userLimits/setMinEKin ulie gir e- 2*MeV
/gamos/physics/userLimits/setMinEKin ulip gir e+ 2*MeV"
    
#tp=(5 10 20 30 40 45 50 55 60 70 80 90 100 120 140 150 200 250 300 350 400)
tp=(005 010 020 030 050 070 100 150 200 250 300 400)


if [ $1 == "vis" ] ; then
    #RUN="/gamos/userAction UAWIF /run/beamOn 1000000"
    RUN="$VIS /run/beamOn 50"
    jgamos --dir oov $WRL $PHY $GIR $CAPS $KEE $RUN



elif [ $1 == "gener" ] ; then
    UAS="/gamos/setParam UAExit:EnergyMax 1400*keV
/gamos/setParam UAExit:EnergyBins 1400
/gamos/userAction UAExit
/gamos/analysis/histo1Max *Ener* 1400*keV
/gamos/analysis/histo1NBins *Ener* 1400
/gamos/userAction GmGenerHistosUA
/gamos/userAction UAClock"
    RUN="/run/beamOn 1000000"
    jgamos --dir oog $WRL $PHY $GIR $UAS $KEE $UAS $RUN

    

elif [ $1 == "spectrcaps" ] ; then
    WRLPEQ="$MATS $RMT :VOLU  world ORB 3*mm vacuum"

    COFI="/gamos/setParam GmAnalysisMgr:FileNameSuffix .phsp
/gamos/filter exitF GmExitLogicalVolumeFilter world
/gamos/setParam GmTrackDataTextFileUA_GmEMParticleFilter_exitF:FileName Ir192caps
/gamos/setParam GmTrackDataTextFileUA_GmEMParticleFilter_exitF:DataList \
                EventID Particle InitialPosX InitialPosY InitialPosZ \
	        InitialMomX InitialMomY InitialMomZ InitialTime InitialWeight
/gamos/userAction GmTrackDataTextFileUA GmEMParticleFilter exitF"         
    
    UAS="/gamos/userAction UAClock"

    RUN="/run/beamOn 1000000"
	
    jgamos --dir oosc $WRLPEQ $PHY $GIR $CAPS $UAS $COFI $RUN
    mv oosc/Ir192caps.phsp ~/varian



elif [ $1 == "simu" ] ; then
    WRLWAT="$MATS $RMT :VOLU  world ORB 500*mm G4_WATER_VAPOR"
    GEN="/gamos/setParam GmGeneratorFromTextFile:FileName //${HOME}/varian/Ir192caps.phsp
/gamos/generator GmGeneratorFromTextFile"
    TGV="/gamos/setParam   UATargetVolume:TargetName theTarget
/gamos/setParam   UATargetVolume:NoPhotInside 1000000
/gamos/userAction UATargetVolume"
    SCOR="/gamos/setParam   UAScoreVolume:TargetName theTarget
/gamos/setParam   UAScoreVolume:SourceActivity 1.955*Ci/1000
/gamos/userAction UAScoreVolume"

    UAS="/gamos/userAction UAClock
/gamos/userAction UAInteraction"
    RUN="/run/beamOn 1000000"
    RUN="$VIS /run/beamOn 10"

    for (( i=0; i<${#tp[@]}; i++ )) ; do
    	SPH=":VOLU   theTarget ORB 1*mm G4_WATER
:PLACE  theTarget 1 world rm0 ${tp[i]}*cm 0 0
:COLOUR theTarget 1 1 1"
    
       jgamos --dir oo $WRL $PHYl $GEN $LCAPS $UAS $RUN



fi
exit


    RUN="/run/beamOn 500000000"
    s=2000
    DIRAC="--ppn 12 --jpn 12 --host dirac"
    PAR="$DIRAC --jobs 50 --btime 15:00:00 --seed $s --SEED $s --dir oo_sourceiri"
