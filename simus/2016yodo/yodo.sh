#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

WRL=$(wrl 20*cm 20*cm 20*cm)
WL="G4_WATER"
#MAT=":MATE $WL 10 18*g/mole 1*g/cm3"CHB=$(chb $WL 15*cm 15*cm 15*cm 0)

INI="/run/initialize"
PHY="/gamos/physicsList GmLEPTSPhysics $INI"


ROTMx=":ROTM rmt 0 90 0"
ROTMz=":ROTM rmt 0 0 0"

SYO="//SEED_I125
     :VOLU   gseio TUBE 0 0.4*mm 2*mm vacuum
     :PLACE  gseio 1 world rmt 0 0 0
     :COLOUR gseio 1 0.5 0.5"

GYO="#GEN_PHOT_YODO
     /gamos/generator GmGenerator
     /gamos/generator/addSingleParticleSource gn gamma 1*keV
     /gamos/generator/positionDist  gn GmGenerDistPositionInG4Volumes gseio
     /gamos/generator/directionDist gn GmGenerDistDirectionRandom
     /gamos/generator/energyDist    gn GmGenerDistEnergyFromFile I125photHosp.txt fixed 1*keV
     #/gamos/generator/timeDist      gn GmGenerDistTimeDecay 1.955e-3*curie
     /gamos/generator/timeDist      gn GmGenerDistTimeConstant"


GSEIO="$ROTMx $SYO $GYO"
GSEIOz="$ROTMz $SYO $GYO"


pts=(" 0  0  4.8"
     " 0  0  6.8"
     " 0  0  9.8"
     " 0  0 12.8"
     " 0  5 12.8"
     " 0 10 12.8"
     "14  0 12.8"
     " 0  0 22.8"
     " 0 20 12.8"
     " 0 30 12.8"
     " 0  0 32.8"
     " 0  0 40"
     " 0  0 50"  
     " 0  0 60"
    )


sphe() {
    NAME=sph_${1}_${2}_${3}
    SPH=":VOLU   $NAME ORB 1*mm $WL
         :PLACE  $NAME 1 world rmz $1 $2 $3
         :COLOUR $NAME 1 1 1"
    echo $SPH
}


if [ $1 == "gener" ] ; then
    UAS="/gamos/analysis/histo1Max *Ener* 70*keV
         /gamos/analysis/histo1NBins *Ener* 700
         /gamos/userAction GmGenerHistosUA
         /gamos/userAction UAClock
         /gamos/setParam TimeMark 10
         /gamos/userAction UAExit
         /gamos/setParam UAExit:EnergyMax 100*keV
         /gamos/setParam UAExit:EnergyBins 1000"
    RUN="/run/beamOn 1000000"

    #GEN1=$(gen gamma 1*keV 1*mm 1*mm I125photHosp.txt fixed)
    #GEN2=$(gen gamma 1*keV 1*mm 1*mm I125photHosp.txt interpolate)
    #time jlepts $WRL $PHY $UAS $GEN1 $RUN /dir oog1 > kkg1 &
    #time jlepts $WRL $PHY $UAS $GEN2 $RUN /dir oog2 > kkg2 &
    time jlepts $WRL $PHY $UAS $GSEIO $RUN /dir oog > kkg &
    wait
    #root og*/gener.root  og*/hexit.root


elif [ $1 == "anasec" ] ; then
    HGSECO="/gamos/analysis/histo1Max   *Energy* 100*keV
        /gamos/analysis/histo1NBins *Energy* 100
        /gamos/setParam   GmSecondaryTrackDataHistosUA:FileName seco
        /gamos/setParam   GmSecondaryTrackDataHistosUA:DataList SecoKineticEnergy
        /gamos/userAction GmSecondaryTrackDataHistosUA"
    ULI="$(uli $WL e- 1*MeV) $(uli $WL e+ 1*MeV)"
    RUN="/run/beamOn 100000"

    CFL="$WRL $CHB $PHY $UAS $ULI $HGSECO $GSEIO $RUN"
    time jlepts $CFL /dir oo > kk
    #root og*/gener.root  og*/hexit.root


elif [ $1 == "vispoints" ] ; then
    for (( i=0; i<${#pts[@]}; i++ )) ; do
	SPHERES="$SPHERES $(sphe ${pts[i]})"
    done
    
    #ULI="$(uli $WL e- 1*MeV) $(uli $WL e+ 1*MeV)"
    CFL="$WRL $CHB $PHY $UAS $ULI $GSEIO $SPHERES $(vis) /run/beamOn 10"
    time jlepts $CFL /dir oo > kk &
    CFL="$WRL $CHB $PHY $UAS $ULI $GSEIOz $SPHERES $(vis) /run/beamOn 10"
    time jlepts $CFL /dir ooz > kkz &
    wait


elif [ $1 == "voxelrad" ] ; then
    ULI="$(uli $WL e- 1*eV) $(uli $WL e+ 1*eV)"
    RUN="/run/beamOn 1000000000"

    UAS="/gamos/userAction UAInteraction
         /gamos/setParam UAVoxel:Center 1*cm 1*cm 0
         #/gamos/setParam UAVoxel:Radius 1*mm
         #/gamos/setParam UAVoxel:ExternalRadius 1*mm+150*um
         /gamos/setParam UAVoxel:SourceActivity 1.955*Ci/1000
         /gamos/setParam UAVoxel:NoPhotInside 1000
         /gamos/userAction UAVoxel"
    
    for R in 1 0.5 0.1 ; do
	RAD="/gamos/setParam UAVoxel:Radius ${R}*mm
             /gamos/setParam UAVoxel:ExternalRadius ${R}*mm+150*um"
	DIROUT="or_${R}"
	CFL="$WRL $CHB $PHY $UAS $RAD $ULI $GSEIO $RUN"
	plepts -t 40:00:00 -N 100 -d $DIROUT $CFL
    done


elif [ $1 == "voxels" ] ; then
    ULI="$(uli $WL e- 1*eV) $(uli $WL e+ 1*eV)"
    #RUN="$(vis) /run/beamOn 100"
    RUN="/run/beamOn 1000000000"

    UAS="/gamos/userAction UAInteraction
         /gamos/setParam UAVoxel:Radius 1*mm
         /gamos/setParam UAVoxel:ExternalRadius 1*mm+150*um
         /gamos/setParam UAVoxel:SourceActivity 1.955*Ci/1000
         /gamos/setParam UAVoxel:NoPhotInside 1000
         /gamos/userAction UAVoxel"
    
    for (( i=0; i<${#pts[@]}; i++ )) ; do
	CENTER="/gamos/setParam UAVoxel:Center ${pts[i]}"
	DIROUT=$(echo ${pts[i]} | tr ' ' '_')
	CFL="$WRL $CHB $PHY $UAS $CENTER $ULI $GSEIO $RUN"
	plepts -t 24:00:00 -N 100 -d $DIROUT $CFL
    done

    exit

    for dir in * ; do
	echo $dir >> dose.txt
	echo $dir >> dose2.txt
	echo $dir >> proc.txt
	grep Dose/primary ${dir}/*log | awk '{$1=""; print $0}' | medvar >> dose.txt
	grep Dose:        ${dir}/*log | awk '{$1=""; print $0}' | medvar >> dose2.txt
	grep vvxx2        ${dir}/*log | awk '{$1=""; print $0}' | medvar >> proc.txt
    done


elif [ $1 == "voxel-volu" ] ; then
    UAS="/gamos/setParam UAVoxel:Center 10*mm 0 0
         /gamos/setParam UAVoxel:Radius 2*mm
         /gamos/setParam UAVoxel:ExternalRadius 2.2*mm
         /gamos/setParam UAVoxel:NoPhotInside 1000
         /gamos/userAction UAVoxel"
    ULI="$(uli $WL e- 1*keV)"
    RUN="/run/beamOn 1000000000"
    #RUN="$(vis) /run/beamOn 1"

    CFL="$WRL $CHB $PHY $UAS $ULI $GSEIO $RUN"
    time jlepts $CFL /dir oovx /jn vx > kkx
    
    UAS="/gamos/setParam UAVolume:TargetName theSphere
         /gamos/setParam UAVolume:TargetReach 0.2*mm
         /gamos/setParam UAVolume:NoPhotInside 2000
         /gamos/userAction UAVolume"

    CHB=":VOLU cuba BOX 15*cm 15*cm 15*cm $WL
         :PLACE cuba 1 world rm0 0 0 0 
         :COLOUR cuba 0.4 0.4 0.8"
    SPH=":VOLU   theSphere ORB 2*mm $WL
         :PLACE  theSphere 1 cuba rm0 10*mm 0 0
         :COLOUR theSphere 1 1 1"
    ULI="/gamos/physics/userLimits/setMinEKin ulicuba cuba e- 1*keV
         /gamos/physics/userLimits/setMinEKin ulisph theSphere e- 1*keV"

    CFL="$WRL $CHB $SPH $PHY $UAS $ULI $GSEIO $RUN"
    time jlepts $CFL /dir oovl /jn vl > kkl

    
elif [ $1 == "volucomp" ] ; then
    RUN="/run/beamOn 1000000000"

    CHB=":VOLU cuba BOX 15*cm 15*cm 15*cm $WL
         :PLACE cuba 1 world rm0 0 0 0 
         :COLOUR cuba 0.4 0.4 0.8"
    SPH=":VOLU   theSphere ORB 2*mm $WL
         :PLACE  theSphere 1 cuba rm0 10*mm 0 0
         :COLOUR theSphere 1 1 1"
    ULI="/gamos/physics/userLimits/setMinEKin ulicuba cuba e- 1*keV
         /gamos/physics/userLimits/setMinEKin ulisph theSphere e- 1*keV"

    SCOR=$(mfdet theSphere)

    UAS="/gamos/setParam UAVolume:TargetName theSphere
         /gamos/setParam UAVolume:TargetReach 0.2*mm
         /gamos/setParam UAVolume:NoPhotInside 1000
         /gamos/userAction UAVolume"

    CFL="$WRL $CHB $SPH $PHY $UAS $ULI $GSEIO $SCOR $RUN"
    time jlepts $CFL /dir oov1 > kk1

    UAS="/gamos/setParam   UASelectVolume:TargetName theSphere
         /gamos/setParam   UASelectVolume:TargetReach 0.2*mm
         /gamos/setParam   UASelectVolume:NoPhotInside 1000
         /gamos/userAction UASelectVolume
         /gamos/setParam   UAScoreVolume:TargetName theSphere
         /gamos/userAction UAScoreVolume"
    CFL="$WRL $CHB $SPH $PHY $UAS $ULI $GSEIO $SCOR $RUN"
    time jlepts $CFL /dir oov2 > kk2


elif [ $1 == "scorescomp" ] ; then
    RUN="/run/beamOn 1000000000"

    CHB=":VOLU cuba BOX 15*cm 15*cm 15*cm $WL
         :PLACE cuba 1 world rm0 0 0 0 
         :COLOUR cuba 0.4 0.4 0.8"
    SPH=":VOLU   theSphere ORB 2*mm $WL
         :PLACE  theSphere 1 cuba rm0 10*mm 0 0
         :COLOUR theSphere 1 1 1"
    ULI="/gamos/physics/userLimits/setMinEKin ulicuba cuba e- 1*eV
         /gamos/physics/userLimits/setMinEKin ulisph theSphere e- 1*eV"

    SELV="/gamos/setParam   UASelectVolume:TargetName theSphere
          /gamos/setParam   UASelectVolume:TargetReach 0.2*mm
          /gamos/setParam   UASelectVolume:NoPhotInside 1000
          /gamos/userAction UASelectVolume"

    SCOR=$(mfdet theSphere)

    SCOR2="/gamos/setParam   UAScoreVolume:TargetName theSphere
           /gamos/userAction UAScoreVolume"

    CFL="$WRL $CHB $SPH $PHY $ULI $GSEIO $SELV $SCOR $RUN"
    plepts -t 40:00:00 -N 50 -d ooscoregm $CFL

    CFL="$WRL $CHB $SPH $PHY $ULI $GSEIO $SELV $SCOR2 $RUN"
    plepts -t 40:00:00 -N 50 -d ooscore2 $CFL

    exit
    
    for dir in ooscore2 ; do
	grep Dose/primary ${dir}/*log | awk '{$1=""; print $0}' | medvar >> ${dir}_dosep.txt
	grep Dose:        ${dir}/*log | awk '{$1=""; print $0}' | medvar >> ${dir}_dose.txt
	grep vvxx2        ${dir}/*log | awk '{$1=""; print $0}' | medvar >> ${dir}_proc.txt
    done
    echo ooscoregm/*ooscore*log |tr ' ' '\n' > fl.txt
    sumScores -fl fl.txt

elif [ $1 == "edr" ] ; then
    WRL=$(wrl 20*cm 20*cm 20*cm)
    WL="G4_WATER"
    CHB=":VOLU   sphe ORB 100*mm $WL
         :PLACE  sphe 1 world rm0 0 0 0"
    ULI="/gamos/physics/userLimits/setMinEKin l1 sphe e- 1*MeV
         /gamos/physics/userLimits/setMinEKin l2 sphe e- 1*MeV"
    UAS="/gamos/analysis/histo1Min  *Pos* 0
         /gamos/analysis/histo1Max  *Pos* 120*mm
         /gamos/analysis/histo1NBins *Pos* 120
         /gamos/analysis/addFileFormat root
         /gamos/analysis/addFileFormat TXT
         /gamos/setParam GmStepDataHistosUA:DataList
             FinalPosMag.prof.AccumulatedEnergyDeposited
             FinalPosMag.prof.AccumulatedEnergyLost
             #FinalPosMag.prof.(AccumulatedEnergyDeposited/(1e-20+sqr(FinalPosMag)))
             #FinalPosMag.prof.(AccumulatedEnergyLost/(1e-20+sqr(FinalPosMag)))
             /gamos/userAction GmStepDataHistosUA
             /gamos/userAction UADepoSpheric"
    #RUN="/gamos/userAction UAWIF /run/beamOn 10000"
    RUN="/run/beamOn 1000000"

    #CFL="$WRL $MAT $CHB $PHY $GSEIO $ULI $UAS $RUN"
    #time jlepts $CFL /dir ooer > kker &
    
    #CFL="$WRL $MAT $CHB $PHY $(gen gamma 27.4*keV 1*nm 0) $(vis) $ULI $UAS /run/beamOn 10"
    #time jlepts $CFL /dir oo > kk
    #exit
    
    #CFL="$WRL $MAT $CHB $PHY $(gen gamma 30*keV 1*nm 0) $ULI $UAS $RUN"
    #time jlepts $CFL /dir ooer3 > kker3 &
    #CFL="$WRL $MAT $CHB $PHY $(gen gamma 40*keV 1*nm 0) $ULI $UAS $RUN"
    #time jlepts $CFL /dir ooer4 > kker4 &
    #CFL="$WRL $MAT $CHB $PHY $(gen gamma 50*keV 1*nm 0) $ULI $UAS $RUN"
    #time jlepts $CFL /dir ooer5 > kker5 &

    
    #CFL="$WRL $MAT $CHB $PHY $(gen gamma 27.4*keV 1*nm 0) $ULI $UAS $RUN"
    #time jlepts $CFL /dir ooer1 > kker1 &
    #CFL="$WRL $MAT $CHB $PHY $(gen gamma 31.4*keV 1*nm 0) $ULI $UAS $RUN"
    #time jlepts $CFL /dir ooer2 > kker2 &
    #CFL="$WRL $MAT $CHB $PHY $(gen gamma 35.5*keV 1*nm 0) $ULI $UAS $RUN"
    #time jlepts $CFL /dir ooer3 > kker3 &
    #CFL="$WRL      $CHB $PHY $(gen gamma 35.5*keV 1*nm 0) $ULI $UAS $RUN"
    #time jlepts $CFL /dir ooer3b > kker3b &

    
    #PHY="/gamos/physicsList GmDNAPhysics $INI"
    #PHY="/gamos/physicsList GmEMPhysics $INI"
    MAT=":MATE $WL 8 18*g/mole 1*g/cm3
         :MATE_MEE G4_WATER 78.*eV"
    CFL="$WRL $MAT $CHB $PHY $GSEIO $ULI $UAS $RUN"
    time jlepts $CFL /dir ooer1 > kker1 &
    CFL="$WRL      $CHB $PHY $GSEIO $ULI $UAS $RUN"
    time jlepts $CFL /dir ooer2 > kker2 &
    wait

fi




CHB=":VOLU cuba BOX 15*cm 15*cm 15*cm $WL
     :PLACE cuba 1 world rm0 0 0 0 
     :COLOUR cuba 0.4 0.4 0.8"

ULI="/gamos/physics/userLimits/setMinEKin ulicuba cuba e- 1*eV
     /gamos/physics/userLimits/setMinEKin ulisph theSphere e- 1*eV"

SELV="/gamos/setParam   UASelectVolume:TargetName theSphere
      /gamos/setParam   UASelectVolume:TargetReach 0.15*mm
      /gamos/setParam   UASelectVolume:NoPhotInside 500
      /gamos/userAction UASelectVolume"

SCOR="$(mfdet theSphere)
      /gamos/setParam   UAScoreVolume:TargetName theSphere
      /gamos/setParam   UAScoreVolume:SourceActivity 1.955*Ci/1000
      /gamos/userAction UAScoreVolume
      /gamos/userAction UAClock"


if [ $1 == "simupru" ] ; then
    SPH=":VOLU  theSphere ORB 1*mm $WL  :PLACE theSphere 1 cuba rm0 ${pts[0]}"
    ULI="/gamos/userAction GmKillAtStackingActionUA GmElectronFilter"
    #RUN="$(vis) /run/beamOn 50"
    RUN="/gamos/userAction UAWIF /run/beamOn 10000"
    CFL="$WRL $CHB $SPH $PHY $ULI $UAS $GSEIO $SELV $SCOR $RUN"
    time jlepts $CFL /dir oopru > kkpru


elif [ $1 == "simusmall" ] ; then
    #pts=("0 0 4.8" "0 0 6.8" "0 0 9.8" "0 0 12.8" "0 0 22.8" "0 0 32.8" "0 0 50" )
    pts=("0 0 4.8")
    SELV="/gamos/setParam   UASelectVolume:TargetName theSphere
          /gamos/setParam   UASelectVolume:TargetReach 0.15*mm
          /gamos/setParam   UASelectVolume:NoPhotInside 10000
          /gamos/userAction UASelectVolume"
    for (( i=0; i<${#pts[@]}; i++ )) ; do
     	SPH=":VOLU theSphere ORB 1*mm $WL
             :PLACE theSphere 1 cuba rm0 ${pts[i]}"
     	DIROUT="ooP_${i}_$(echo ${pts[i]} | tr ' ' '_')"
	#ULI="/gamos/userAction GmKillAtStackingActionUA GmElectronFilter"
	#ULI="/gamos/physics/userLimits/setMinEKin l1 theSphere e- 1*MeV"

     	RUN="/run/beamOn 1000000"
     	CFL="$WRL $CHB $SPH $PHY $GSEIO $SELV $SCOR $ULI $RUN"
     	time jlepts $CFL /dir ${DIROUT} > kk${DIROUT} &
    done
    wait


elif [ $1 == "simu" ] ; then
    for (( i=0; i<${#pts[@]}; i++ )) ; do
     	SPH=":VOLU  theSphere ORB 1*mm $WL
             :PLACE theSphere 1 cuba rm0 ${pts[i]}"
     	DIROUT="ooP_${i}_$(echo ${pts[i]} | tr ' ' '_')"
     	RUN="/run/beamOn 1000000000"
     	CFL="$WRL $CHB $SPH $PHY $GSEIO $SELV $SCOR $ULI $RUN"
	#plepts -h dirac -t 96:00:00 -N 200 -I 1000 -i 1000 -d ${DIROUT} $CFL
	jgamos $CFL &
    done
    
    exit


    #source /home/arce/gamos/GAMOS.5.0.0/config/confgamos.sh
    for dir in ooP* ; do
	#echo $dir
	echo $dir/*inp.log | tr ' ' '\n' > fl_${dir}.txt
	sumScores -fl fl_${dir}.txt -fOut scor_${dir}.txt
    done

    chs="NoPrimaries cGyHour pCmm3s pCugHour \
         compt phot Rayl Elastic ExcitRotat ExcitVibrat \
         Excitation Ionisation DissocNeutr Attachment"

    rm -f SCOR.txt
    for ch in $chs ; do
	( echo $ch $ch ; grep $ch scor_ooP* |awk '{print $1,$5}' ) | column -t >> SCOR.txt
    done
    
fi
