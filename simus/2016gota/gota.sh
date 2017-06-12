#!/bin/bash

source $HOME/lepts/bin/gdefs.sh


WRL="//WORLD
     :MATE vacuum 1 1*g/mole 1e-25*g/cm3 
     :VOLU world BOX 10*um 10*um 10*um vacuum :COLOUR world 1 1 1
     :ROTM rm0  0  0 0"
PHY1="/gamos/physicsList GmLEPTSPhysics /run/initialize"
PHY="/gamos/physicsList GmLeptsPhysics /run/initialize"



# sph x y z R
sph() {
    SPH=":VOLU   gota ORB $4 G4_WATER
         :PLACE  gota 1 world rm0 $1 $2 $3
         :COLOUR gota 0 0 1"
    echo $SPH
}
sph1() {
    SPH=":VOLU   gota_$1 ORB $4 G4_WATER
         :PLACE  gota_$1 1 world rm0 $1 $2 $3
         :COLOUR gota_$1 1 1 0"
    echo $SPH
}

# genp e- 100*eV
genp() {
    GENP="/gamos/generator GmGenerator
          /gamos/generator/addSingleParticleSource gn $1 $2
          /gamos/generator/positionDist  gn GmGenerDistPositionPoint -3*um 0 0
          /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0"
    echo $GENP
}

svs() {
    SELV="/gamos/setParam   UASelectVolume:TargetName gota
          /gamos/setParam   UASelectVolume:TargetReach $1
          /gamos/setParam   UASelectVolume:NoPhotInside $2
          /gamos/setParam   UASelectVolume:TimeLimit $3
          /gamos/userAction UASelectVolume"
    SCOR="$(mfdet gota)
          /gamos/setParam   UAScoreVolume:TargetName gota
          /gamos/setParam   UAScoreVolume:PrintTime $4
          /gamos/setParam   UAScoreVolume:SourceActivity $5
          /gamos/userAction UAScoreVolume
          /gamos/userAction UAClock"
    echo $SELV $SCOR
}
SVS=$(svs 0.15*mm 100000  4*3600+25*60  10*60  65*1.505*0.5118*Ci/1000)
#ULI="/gamos/physics/userLimits/setMinEKin ulicuba gota e- 90*eV"

GEN=$(genp e- 10000*eV)


if [ $1 == "visu" ] ; then
    SPH=$(sph 0 0 0 1*um)
    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction UAInteraction
         /gamos/userAction UAWIF"
    CFL="$WRL $SPH $PHY $UAS $GEN $ULI $SVS"
    jgamos --dir oov $WRL $SPH $PHY  $UAS $GEN $ULI $SVS $(vis) /run/beamOn 1  &
    jgamos --dir oo1 $WRL $SPH $PHY1 $UAS $GEN $ULI $SVS        /run/beamOn 10 &
    jgamos --dir oo2 $WRL $SPH $PHY  $UAS $GEN $ULI $SVS        /run/beamOn 10 &
    wait

    
elif [ $1 == "pru" ] ; then
    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction UAInteraction
         /gamos/userAction UAWIF 
         /gamos/userAction UAClock"
    
    #jgamos --dir oo1 $WRL $(sph 0 0 0 20*nm) $PHY $UAS $GEN $ULI $SVS /run/beamOn 10 &
    #jgamos --dir oo2 $WRL $(sph 0 0 0 5*nm)  $PHY $UAS $GEN $ULI $SVS /run/beamOn 1000 &
    #jgamos --dir oo3 $WRL $(sph 0 0 0 2*nm)  $PHY $UAS $GEN $ULI $SVS /run/beamOn 10000 &
    #jgamos --dir oo4  $WRL $(sph 0 0 0 1*um) $PHY $UAS $GEN $ULI $SVS /run/beamOn 1 &
    #jgamos --dir oo5  $WRL $(sph 0 0 0 10*nm) $PHY $UAS $GEN $ULI $SVS /run/beamOn 100 &
    #jgamos --dir oo6  $WRL $(sph 0 0 0 1*nm) $PHY $UAS $GEN $ULI $SVS /run/beamOn 100 &
    jgamos --dir oo  $WRL $(sph 0 0 0 1*um) $PHY $UAS $GEN $ULI $SVS /run/beamOn 5 &
    wait
elif [ $1 == "small" ] ; then
    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction UAInteraction
         $(vis)"
    WRL="//WORLD
     :MATE vacuum 1 1*g/mole 1e-25*g/cm3 
     :VOLU world BOX .5*um .5*um  .5*um vacuum :COLOUR world 1 1 1
     :ROTM rm0  0  0 0"
    GENP="/gamos/generator GmGenerator
          /gamos/generator/addSingleParticleSource gn $1 $2
          /gamos/generator/positionDist  gn GmGenerDistPositionPoint 0 0 0
          /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0"
    SPH="$(sph1     0 0 0 10*nm)
         $(sph1 20*nm 0 0  5*nm)
         $(sph1 30*nm 0 0  1*nm)
         "
    jgamos --dir oo  $WRL $SPH $PHY $UAS $GEN $ULI /run/beamOn 1 &

    
elif [ $1 == "size" ] ; then
    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction UAInteraction
         /gamos/userAction UAWIF"
    RUN="/run/beamOn 100000"
 
    for R in 5 3 2 1; do
	SPH=$(sph 0 0 0 ${R}*Ang)
	CFL="$WRL $SPH $PHY1 $UAS $GEN $ULI $SVS $RUN"
	jgamos --dir oo${R} $CFL &
    done

    wait

elif [ $1 == "simu" ] ; then
    SPH=$(sph 0 0 0 ${R}*Ang)


fi
