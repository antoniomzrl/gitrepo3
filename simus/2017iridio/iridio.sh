#!/bin/bash

#source $HOME/lepts/bin/gdefs.sh

INI="/run/initialize"
#PHY="/gamos/physicsList GmEMPhysics $INI"
PHY="/gamos/physicsList HadrontherapyPhysics $INI"

MATS=":MATE vacuum  1  1       1e-25*g/cm3
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

WRL="$MATS
     :VOLU world BOX 1.1*cm 1.1*cm 1.1*cm vacuum 
     //:COLOUR world 1 1 1 
     :ROTM rm0  0  0 0 
     :ROTM rmx 90  0 0 
     :ROTM rmy  0 90 0
     :ROTM rmz  0  0 90"


GIR=":VOLU   gir TUBE 0 0.6*mm/2  3.5*mm/2 iridium
     :PLACE  gir 1 world rmy 0 0 0
     :COLOUR gir 1 1 0
     /gamos/generator GmGenerator
     /gamos/generator/addSingleParticleSource gn gamma 1*keV
     /gamos/generator/positionDist  gn GmGenerDistPositionInG4Volumes gir
     /gamos/generator/directionDist gn GmGenerDistDirectionRandom
     /gamos/generator/energyDist    gn GmGenerDistEnergyFromFile Ir192.txt fixed 1*keV
     /gamos/generator/timeDist      gn GmGenerDistTimeConstant"

CAPS=":VOLU   caps TUBE 0.7*mm/2 0.9*mm/2 3.5*mm/2+0.065 steel316L
      :PLACE  caps 1 world rmy 0 0 0
      :COLOUR caps 0 1 1
      :VOLU   tapa TUBE 0 0.9*mm/2 0.2*mm/2 steel316L
      :PLACE  tapa 1 world rmy  3.5*mm/2+0.065*mm+0.2/2*mm 0 0
      :PLACE  tapa 2 world rmy -3.5*mm/2-0.065*mm-0.2/2*mm 0 0	
      :COLOUR tapa 0 1 1"


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
    


if [ $1 == "vis" ] ; then
    RUN="/gamos/userAction UAWIF /run/beamOn 1000000"
    #RUN="$VIS /run/beamOn 50"
    jgamos --dir oov $MATS $WRL $PHY $GIR $CAPS $KEE $RUN



elif [ $1 == "gener" ] ; then
    UAS="/gamos/setParam UAExit:EnergyMax 1400*keV
         /gamos/setParam UAExit:EnergyBins 1400
         /gamos/userAction UAExit
         /gamos/analysis/histo1Max *Ener* 1400*keV
	 /gamos/analysis/histo1NBins *Ener* 1400
         /gamos/userAction GmGenerHistosUA
         /gamos/userAction UAClock"
    RUN="/run/beamOn 1000000"
    jgamos --dir oog $MATS $WRL $PHY $GIR $UAS $KEE $UAS $RUN

    

elif [ $1 == "simu" ] ; then
    UAS="/gamos/userAction UAInteraction
         /gamos/setParam UAExit:EnergyMax 1400*keV
         /gamos/setParam UAExit:EnergyBins 1400
         /gamos/userAction UAExit
         /gamos/analysis/histo1Max *Ener* 1400*keV
	 /gamos/analysis/histo1NBins *Ener* 1400
         /gamos/userAction GmGenerHistosUA
         /gamos/userAction UAClock"
    RUN="/run/beamOn 100000000"
    PAR="--jobs 10 --jpn 10 --seed 0 --SEED 0"
    jgamos --dir oo $PAR $MATS $WRL $PHY $GIR $CAPS $UAS $UAS $RUN



fi
exit
