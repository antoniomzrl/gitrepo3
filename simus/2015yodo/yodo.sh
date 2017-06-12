#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

WRL=$(wrl 20*cm 20*cm 20*cm)
WL="G4_WATER"
MAT=":MATE $WL 10 18*g/mole 1*g/cm3"
CHB=$(chb $WL 15*cm 15*cm 15*cm 0)

INI="/run/initialize"
PHY="/gamos/physicsList GmLEPTSPhysics  $INI"

UAS="/gamos/analysis/histo1Max *Ener* 100*keV
     /gamos/analysis/histo1NBins *Ener* 100
     /gamos/userAction GmGenerHistosUA
     /gamos/userAction UAClock
     /gamos/setParam TimeMark 10
     /gamos/userAction UAExit
     /gamos/setParam UAExit:EnergyMax 100*keV
     /gamos/setParam UAExit:EnergyBins 1000"

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
     /gamos/generator/timeDist      gn GmGenerDistTimeDecay 1.955e-3*curie"

GSEIOx="$ROTMx $SYO $GYO"
GSEIOz="$ROTMz $SYO $GYO"


if [ $1 == "gener" ] ; then
    GEN1=$(gen gamma 1*keV 5*cm -80*cm I125photOld.txt)
    GEN2=$(gen gamma 1*keV 5*cm -80*cm I125photHosp.txt)
    RUN="/run/beamOn 100000"
    CFL="$WRL $PHY $UAS"
    time jlepts $CFL $GEN1 $RUN /dir oog1 > kkg1
    time jlepts $CFL $GEN2 $RUN /dir oog2 > kkg2
    #root og*/gener.root  og*/hexit.root
fi

if [ $1 == "vis" ] ; then
    UAS="/gamos/userAction UAWIF"
    SPHz="//Small_sphere_z10mm
         :VOLU   sph ORB 1*mm $WL
         :PLACE  sph 1 world rmt 0 0 10*mm
         :COLOUR sph 1 1 1"
    ULI="$(uli $WL e- 1*MeV) $(uli $WL e+ 1*MeV)"
    CFL="$WRL $MAT $CHB $PHY $UAS $ULI $GSEIOx $SPHz $(vis) /run/beamOn 100"
    time jlepts $CFL /dir oo > kk
    CFL="$WRL $MAT $CHB $PHY $UAS $ULI $GSEIOz $SPHz $(vis) /run/beamOn 100"
    time jlepts $CFL /dir ooz > kkz
    CFL="$WRL $MAT $CHB $PHY $UAS $ULI $GSEIOx /run/beamOn 10000"
    time jlepts $CFL /dir oo2 > kk2
fi


if [ $1 == "peq" ] ; then
    ULI="$(uli $WL e- 1*MeV) $(uli $WL e+ 1*MeV)"
    RUN="/run/beamOn 100"
    
    WRLpeq=$(wrl 6*mm 6*mm 6*mm)
    CHBpeq=$(chb $WL 3*mm 3*mm 3*mm 0)
    CFL="$WRLpeq $MAT $CHBpeq $PHY $UAS $ULI $GSEIOx $(vis) $RUN"
    time jlepts $CFL /dir oop > kkp

    RUN="/run/beamOn 100000"
    UAS="$UAS /gamos/userAction UAWIF"
    CFL="$WRLpeq $MAT $CHBpeq $PHY $UAS $ULI $GSEIOx $RUN"
    time jlepts $CFL /dir oop2 > kkp2
fi


if [ $1 == "pe" ] ; then
    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/setParam TimeMark 5
         /gamos/userAction UAClock
         /gamos/userAction UAInteraction
         /gamos/userAction UAWIF
         /gamos/setParam SPhg:x 2000 0*um 200*um
         /gamos/setParam SPhg:Title sp-edep
         /gamos/userAction UAInteractionSp
         "
    CFL="$WRL $MAT $CHB $PHY $UAS"
    RUN="/run/beamOn 100"

    for e in 70 50 40 30 20 10 5 1 0.1 ; do
	E=$e'*keV'
	GEN=$(gen e- $E 1*um 0*cm)
	time jlepts $CFL $GEN $RUN /dir oope_$e > kkpe_$e
    done
fi


if [ $1 == "anasec" ] ; then
    HGSECO="/gamos/analysis/histo1Max   *Energy* 100*keV
        /gamos/analysis/histo1NBins *Energy* 100
        /gamos/setParam   GmSecondaryTrackDataHistosUA:FileName seco
        /gamos/setParam   GmSecondaryTrackDataHistosUA:DataList SecoKineticEnergy
        /gamos/userAction GmSecondaryTrackDataHistosUA"
    ULI="$(uli $WL e- 1*MeV) $(uli $WL e+ 1*MeV)"
    RUN="/run/beamOn 100000"

    CFL="$WRL $MAT $CHB $PHY $UAS $ULI $HGSECO $GSEIOx $RUN"
    time jlepts $CFL /dir oo > kk
    #root og*/gener.root  og*/hexit.root
fi


# cofivoxel r x y z
cofivoxel1() {
    HD="/gamos/setParam GmTrackDataTextFileUA_GmElectronOrPositronFilter:"
    DL="EventID Particle InitialPosX InitialPosY InitialPosZ \
        InitialMomX InitialMomY InitialMomZ InitialTime InitialWeight"
    CF="${HD}FileName contfil.txt ${HD}WriteHeaderData 0  ${HD}DataList $DL
        /gamos/userAction GmTrackDataTextFileUA GmElectronOrPositronFilter
        /gamos/userAction GmKillAtSteppingActionUA GmSecondaryFilter"
    echo "$CF"
}

# cofivoxel r x y z
cofivoxel() {
    VOX="//Smallsphere
         :VOLU   theVoxel ORB $1 $WL
         :PLACE  theVoxel 1 world rmz $2 $3 $4
         :COLOUR theVoxel 1 1 0"
    
    DL="EventID Particle InitialPosX InitialPosY InitialPosZ \
        InitialMomX InitialMomY InitialMomZ InitialTime InitialWeight"
    HD="/gamos/setParam GmTrackDataTextFileUA_GmElectronOrPositronFilter"
    FLT="/gamos/filter startVoxelF GmStartLogicalVolumeFilter theVoxel
        ${HD}_startVoxelF:FileName ContFile
        ${HD}_startVoxelF:WriteHeaderData 0
        ${HD}_startVoxelF:UseAtInitial 1
        ${HD}_startVoxelF:DataList $DL
        /gamos/userAction GmTrackDataTextFileUA GmElectronOrPositronFilter startVoxelF"
    LIM="#/gamos/userAction GmKillAtSteppingActionUA GmSecondaryFilter         
         /gamos/physics/userLimits/setMinEKin ulie theVoxel e- 99*MeV
         /gamos/physics/userLimits/setMinEKin ulip theVoxel e+ 99*MeV"
    echo "$VOX $FLT $LIM"
}

if [ $1 == "voxelcofi" ] ; then
    UAS="#/gamos/userAction UAVerbose
         #/gamos/userAction UAWIF
         /gamos/userAction UAInteraction
         /gamos/userAction GmCountProcessesUA"
    ULI="$(uli $WL e- 99*MeV) $(uli $WL e+ 99*MeV)"
    COFI="$(cofivoxel 1000*um 0 1*cm 0)"
    RUN="/run/beamOn 1000000"
    COFI="$(cofivoxel 0.9*cm 0 1*cm 0)"
    RUN="$(vis) /run/beamOn 1000"
    CFL="$WRL $MAT $CHB $PHY $COFI $UAS $ULI $GSEIOx $VIS $RUN"
    time jlepts $CFL /dir oopix /jn gamma > kkpix


    N=$(cat oopix/ContFile | wc -l)
    echo "cofi:" $N
exit 
    RUN="/run/beamOn $N"
    GENe="/gamos/setParam GmGeneratorFromTextFile:FileName ContFile
          /gamos/generator GmGeneratorFromTextFile"
    ULI="$(uli $WL e- 99*MeV) $(uli $WL e+ 99*MeV)
         #/gamos/userAction GmKillAtSteppingActionUA GmSecondaryFilter"
    CFL="$WRL $MATl $CHB $PHY $UAS $GENe $ULI $RUN"
    time jlepts $CFL /dir oopix /jn elec >& kkpixe
fi




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


if [ $1 == "vispoints" ] ; then
    for (( i=0; i<${#pts[@]}; i++ )) ; do
	SPHERES="$SPHERES $(sphe ${pts[i]})"
    done
    
    ULI="$(uli $WL e- 1*MeV) $(uli $WL e+ 1*MeV)"
    CFL="$WRL $MAT $CHB $PHY $UAS $ULI $GSEIOx $SPHERES $(vis) /run/beamOn 10"
    time jlepts $CFL /dir oo > kk
    CFL="$WRL $MAT $CHB $PHY $UAS $ULI $GSEIOz $SPHERES $(vis) /run/beamOn 10"
    time jlepts $CFL /dir ooz > kkz
fi



if [ $1 == "voxelrad" ] ; then
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
	CFL="$WRL $MAT $CHB $PHY $UAS $RAD $ULI $GSEIOx $RUN"
	plepts -t 40:00:00 -N 100 -d $DIROUT $CFL
    done
fi


if [ $1 == "voxels" ] ; then
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
	CFL="$WRL $MAT $CHB $PHY $UAS $CENTER $ULI $GSEIOx $RUN"
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
fi


if [ $1 == "voxel-volu" ] ; then
    UAS="/gamos/setParam UAVoxel:Center 10*mm 0 0
         /gamos/setParam UAVoxel:Radius 2*mm
         /gamos/setParam UAVoxel:ExternalRadius 2.2*mm
         /gamos/setParam UAVoxel:NoPhotInside 1000
         /gamos/userAction UAVoxel"
    ULI="$(uli $WL e- 1*keV)"
    RUN="/run/beamOn 1000000000"
    #RUN="$(vis) /run/beamOn 1"

    CFL="$WRL $MAT $CHB $PHY $UAS $ULI $GSEIOx $RUN"
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

    CFL="$WRL $MAT $CHB $SPH $PHY $UAS $ULI $GSEIOx $RUN"
    time jlepts $CFL /dir oovl /jn vl > kkl
fi



if [ $1 == "volucomp" ] ; then
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

    CFL="$WRL $MAT $CHB $SPH $PHY $UAS $ULI $GSEIOx $SCOR $RUN"
    time jlepts $CFL /dir oov1 > kk1

    UAS="/gamos/setParam   UASelectVolume:TargetName theSphere
         /gamos/setParam   UASelectVolume:TargetReach 0.2*mm
         /gamos/setParam   UASelectVolume:NoPhotInside 1000
         /gamos/userAction UASelectVolume
         /gamos/setParam   UAScoreVolume:TargetName theSphere
         /gamos/userAction UAScoreVolume"
    CFL="$WRL $MAT $CHB $SPH $PHY $UAS $ULI $GSEIOx $SCOR $RUN"
    time jlepts $CFL /dir oov2 > kk2

fi



if [ $1 == "scorescomp" ] ; then
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

    CFL="$WRL $MAT $CHB $SPH $PHY $ULI $GSEIOx $SELV $SCOR $RUN"
    plepts -t 40:00:00 -N 50 -d ooscoregm $CFL

    CFL="$WRL $MAT $CHB $SPH $PHY $ULI $GSEIOx $SELV $SCOR2 $RUN"
    plepts -t 40:00:00 -N 50 -d ooscore2 $CFL

    exit
    
    for dir in ooscore2 ; do
	grep Dose/primary ${dir}/*log | awk '{$1=""; print $0}' | medvar >> ${dir}_dosep.txt
	grep Dose:        ${dir}/*log | awk '{$1=""; print $0}' | medvar >> ${dir}_dose.txt
	grep vvxx2        ${dir}/*log | awk '{$1=""; print $0}' | medvar >> ${dir}_proc.txt
    done
    echo ooscoregm/*ooscore*log |tr ' ' '\n' > fl.txt
    sumScores -fl fl.txt
fi


if [ $1 == "simu" ] ; then
    CHB=":VOLU cuba BOX 15*cm 15*cm 15*cm $WL
         :PLACE cuba 1 world rm0 0 0 0 
         :COLOUR cuba 0.4 0.4 0.8"
    
    ULI="/gamos/physics/userLimits/setMinEKin ulicuba cuba e- 1*eV
         /gamos/physics/userLimits/setMinEKin ulisph theSphere e- 1*eV"

    SELV="/gamos/setParam   UASelectVolume:TargetName theSphere
          /gamos/setParam   UASelectVolume:TargetReach 0.15*mm
          /gamos/setParam   UASelectVolume:NoPhotInside 100
          /gamos/userAction UASelectVolume"

    SCOR="$(mfdet theSphere)
          /gamos/setParam   UAScoreVolume:TargetName theSphere
          /gamos/setParam   UAScoreVolume:SourceActivity 1.955*Ci/1000
          /gamos/userAction UAScoreVolume
          /gamos/userAction UAClock"

    if [ $2 == "pru" ] ; then
	SPH=":VOLU theSphere ORB 1*mm $WL :PLACE theSphere 1 cuba rm0 ${pts[1]}"
	CFL="$WRL $MAT $CHB $SPH $PHY $ULI $GSEIOx $SELV $SCOR"
	jlepts  /dir oo $CFL /run/beamOn 100000
    fi
    
    if [ $2 == "points" ] ; then
	for (( i=0; i<${#pts[@]}; i++ )) ; do
     	    SPH=":VOLU  theSphere ORB 1*mm $WL
                 :PLACE theSphere 1 cuba rm0 ${pts[i]}"
     	    DIROUT="P_${i}_$(echo ${pts[i]} | tr ' ' '_')"
     	    RUN="/run/beamOn 1000000000"
     	    CFL="$WRL $MAT $CHB $SPH $PHY $ULI $GSEIOx $SELV $SCOR $RUN"
     	    plepts -t 24:00:00 -N 100 -d ${DIROUT} $CFL
	done
    fi

    
    exit
    #source /home/arce/gamos/GAMOS.5.0.0/config/confgamos.sh
    for dir in P_1_* ; do
	echo $dir/*inp.log | tr ' ' '\n' > fl_${dir}.txt
	echo sumScores -fl fl_${dir}.txt -fOut scor_${dir}.txt
    done
    chs="compt phot Rayl ExcitRotat Ionisation Elastic DissocNeutr ExcitVibrat Excitation Attachment"
    for ch in $chs ; do
	echo $ch
	grep $ch scor_P_* |awk '{print $5}'
    done
fi
