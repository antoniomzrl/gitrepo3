#!/bin/bash

source $HOME/lepts/bin/gdefs.sh -V 51c


sp=("-0.5  -0.2  0"
    " 1.5   1    0"
    "-1     2    0"
    " 1.5   0    0.5"
    " 2     0.5  0.5"
    "-1     1.05 0.5"
    " 1.5   1.5  0.5"
    " 0     2    0.5"
    "-1.05  2.4  0.5"
    " 1.05  2.4  0.5"
    "-0.5  -1    1"
    " 1.5  -0.45 1"
    "-0.5  -0.2  1"
    "-1.45  0.05 1"
    " 1.5   1    1"
    "-1     2    1"
    " 2.15  1.8  1"
    "-1.45 -0.4  1.5"
    " 1.5  -0.05 1.5"
    " 2     0.5  1.5"
    "-1     1    1.5"
    "-2     1.5  1.5"
    " 1.5   1.5  1.5"
    " 0     1.95 1.5"
    "-1.05  2.4  1.5"
    " 1.05  2.4  1.5"
    "-0.5  -1    2"
    " 1.5  -0.45 2"
    "-0.5  -0.2  2"
    "-1.5   0    2"
    " 1.5   1    2"
    "-1     1.95 2"
    " 2.15  1.8  2"
    "-1.45 -0.4  2.5"
    " 1.5  -0.05 2.5"
    " 2.05  0.45 2.5"
    "-1     1    2.5"
    "-2     1.5  2.5"
    " 1.5   1.5  2.5"
    " 0     2.05 2.5"
    "-1.05  2.4  2.5"
    " 1.05  2.4  2.5"
    "-0.5  -1    3"
    " 1.5  -0.5  3"
    "-0.5  -0.2  3"
    "-1.5   0    3"
    " 1.5   1    3"
    "-1.05  1.9  3"
    " 2.15  1.8  3.08"
    "-1.45 -0.4  3.5"
    " 1.55 -0.1  3.5"
    " 1     0.4  3.5"
    " 2     0.5  3.5"
    "-1.05  0.85 3.5"
    " 1.5   1.45 3.5"
    "-2     1.5  3.5"
    " 0     2    3.5"
    " 1.05  2.4  3.5"
    "-0.5  -0.2  4"
    "-1.5   0    4"
    " 1.5   0.95 4"
    "-1.05  1.85 4"
    " 1.05  0.35 4.5"
    "-1.05  0.8  4.5"
    " 0     1.95 4.5"
   )

tp=(" 0    3.2  1.3"
    "-1.4  4.4  4.5"
    " 0.3  0    5.3"
    " 0.2  0.8  2"
    "-0.7  0.3  2"
    "-4.9  2.9  5.3"
    "-8.2 -3.5  0"
    " 4.4  0.6  2"
    " 0.0 -10.0	8.0"
   )

#pru vis
#sp=("0 0 0")
#tp=("0 0 0.4"  "0 0 0.6")
#tp=("0.4 0 0"  "0.6 0 0")


WRL="//WORLD
     :MATE vacuum 1 1*g/mole 1e-25*g/cm3 
     :VOLU world BOX 15*cm 15*cm 15*cm vacuum :COLOUR world 1 1 1
     :ROTM rm0  0  0 0 
     :ROTM rmx 90  0 0 
     :ROTM rmy  0 90 0
     :ROTM rmz  0  0 90"

CHB=":VOLU cuba BOX 10*cm 10*cm 10*cm G4_WATER
     :PLACE cuba 1 world rm0 0 0 0 
     :COLOUR cuba 0.4 0.4 0.8"
PHY="/gamos/physicsList GmLEPTSPhysics /run/initialize"


# svs  0.15*mm     1000         4*3600+25*60 10         65*1.505*0.5118*Ci/1000
# svs  TargetReach NoPhotInside TimeLimit    PrintTime  SourceActivity
svs() {
    SELV="/gamos/setParam   UASelectVolume:TargetName theSphere
          /gamos/setParam   UASelectVolume:TargetReach $1
          /gamos/setParam   UASelectVolume:NoPhotInside $2
          /gamos/setParam   UASelectVolume:TimeLimit $3
          /gamos/userAction UASelectVolume"
    SCOR="$(mfdet theSphere)
          /gamos/setParam   UAScoreVolume:TargetName theSphere
          /gamos/setParam   UAScoreVolume:PrintTime $4
          /gamos/setParam   UAScoreVolume:SourceActivity $5
          /gamos/userAction UAScoreVolume
          /gamos/userAction UAClock"
    echo $SELV $SCOR
}

SVS=$(svs 0.15*mm 100000  4*3600+25*60  10  65*1.505*0.5118*Ci/1000)


sphcm() {
    NAME=sph_${1}_${2}_${3}
    SPH=":VOLU   $NAME ORB 2*mm G4_WATER
         :PLACE  $NAME 1 cuba rm0 $1*cm $2*cm $3*cm
         :COLOUR $NAME 1 1 1"
    echo $SPH
}

# tsphcm x y z rad
tsphcm() {
    NAME="theSphere"
    SPH=":VOLU   $NAME ORB $4*cm G4_WATER
         :PLACE  $NAME 1 cuba rm0 $1*cm $2*cm $3*cm
         :COLOUR $NAME 1 1 1"
    echo $SPH
}

#iplcm Ncopy rotMat x y z
iplcm() {
    IPLCM=":PLACE gseio $1 cuba $2 $3*cm $4*cm $5*cm"
    echo $IPLCM
}

#mgy rad halflen rmx rmy rmz
mgy() {
    for (( i=0; i<${#sp[@]}; i++ )) ; do
	IPLACES="$IPLACES $(iplcm $i rmt ${sp[i]})"
    done

    GG="//SEED_I125
        :ROTM rmt $3 $4 $5
        :VOLU   gseio TUBE 0 $1 $2 vacuum
        :COLOUR gseio 1 0 0 
        $IPLACES
        #GEN_PHOT_YODO
        /gamos/generator GmGenerator
        /gamos/generator/addSingleParticleSource gn gamma 1*keV
        /gamos/generator/positionDist  gn GmGenerDistPositionInG4Volumes gseio
        /gamos/generator/directionDist gn GmGenerDistDirectionRandom
        /gamos/generator/energyDist    gn GmGenerDistEnergyFromFile I125photHosp.txt fixed 1*keV
        /gamos/generator/timeDist      gn GmGenerDistTimeConstant"
    echo $GG
}

#Diamet 0.6 long 3.5 direc z
#MGY=$(mgy 0.3*mm 1.75*mm 0 0 0)
MGY=$(mgy 0.4*mm 2*mm 0 0 0)


if [ $1 == "gener" ] ; then
    UAS="/gamos/analysis/histo1Max *Ener* 70*keV
         /gamos/analysis/histo1NBins *Ener* 700
         /gamos/analysis/histo1Max *Pos* 5*cm
         /gamos/analysis/histo1Min *Pos* -5*cm
         /gamos/analysis/histo2MaxX *Pos* 5*cm
         /gamos/analysis/histo2MaxY *Pos* 5*cm
         /gamos/analysis/histo2MinX *Pos* -5*cm
         /gamos/analysis/histo2MinY *Pos* -5*cm
         /gamos/analysis/histo1NBins *Pos* 1000
         /gamos/userAction GmGenerHistosUA
         /gamos/userAction UAClock
         /gamos/setParam TimeMark 10
         /gamos/userAction UAInteraction"
    ULI="/gamos/physics/userLimits/setMinEKin ulicuba cuba e- 1*MeV"

    RUN="/run/beamOn 1000000"
    jgamos $WRL $CHB $PHY $UAS $ULI $MGY $RUN /dir oog > kkg

elif [ $1 == "visu" ] ; then
    for (( i=0; i<${#tp[@]}; i++ )) ; do
	SPHERES="$SPHERES $(sphcm ${tp[i]})"
    done

    ULI="/gamos/physics/userLimits/setMinEKin ulicuba cuba e- 1*MeV"

    CFL="$WRL $CHB $PHY $UAS $MGY $SPHERES $ULI $(vis) /run/beamOn 10"
    jgamos $CFL 
    
    
elif [ $1 == "simu" ] ; then
    ULI="/gamos/physics/userLimits/setMinEKin ulicuba cuba     e- 1*eV
         /gamos/physics/userLimits/setMinEKin ulisph theSphere e- 1*eV"
    RUN="/run/beamOn 1000000000"
    MGYpoint=$(mgy 1*nm   1*nm 0 0 0)

    SVS=$(svs 0.15*mm 100000  4*3600+25*60  10*60  65*1.505*0.5118*Ci/1000)

    for (( i=0; i<${#tp[@]}; i++ )) ; do
	SPH=$(tsphcm ${tp[i]} 0.2)
     	DIROUT="ooP${i}_$(echo ${tp[i]} | tr ' ' '_')"
     	CFL="$WRL $CHB $SPH $PHY $MGY $SVS $ULI $RUN"

	#vis:
	#ULI="/gamos/physics/userLimits/setMinEKin ulicuba cuba     e- 1*MeV
        #     /gamos/physics/userLimits/setMinEKin ulisph theSphere e- 1*MeV"
	#RUN="$(vis) /run/beamOn 5"
	#SPH=$(tsphcm ${tp[i]} 0.2)
	#CFL="$WRL $CHB $SPH $PHY $MGY $SVS $ULI $RUN"
	#time jlepts $CFL /dir oo${i} /version 51c > kk${i} &
	

	ULI="/gamos/physics/userLimits/setMinEKin ulicuba cuba     e- 1*MeV
             /gamos/physics/userLimits/setMinEKin ulisph theSphere e- 1*MeV"
	UAS="/gamos/setParam UAClock:TimeMark 5
             /gamos/setParam UAClock:TimeLimit 8*5
             /gamos/userAction UAClock"
	CFL="$WRL $CHB $SPH $PHY $MGYpoint $SVS $UAS $ULI $RUN"
	#time jlepts $CFL /dir oo${i} /version 51c > kk${i} &

	CFL1="$WRL $CHB $SPH $PHY $MGYpoint $SVS $ULI $RUN"
	CFL2="$WRL $CHB $SPH $PHY $MGY      $SVS $ULI $RUN"
	CFL3="$WRL $CHB $SPH $PHY $MGY      $SVS      $RUN"
	#plepts -h dirac -n 12 -t 4:29:00 -N 10 -I 1000 -i 1000 -d ${DIROUT}_1 $CFL1
	#plepts -h dirac -n 12 -t 4:29:00 -N 100 -I 1500 -i 2000 -d ${DIROUT}_2 $CFL2
	#plepts -h dirac -n 12 -t 4:29:00 -N 100 -I 1500 -i 2000 -d ${DIROUT}_3 $CFL3
	#plepts -h euler -n 8 -t 4:29:00 -N 100 -I 2000 -i 3000 -d ${DIROUT}_2b $CFL2
	#plepts -h euler -n 8 -t 4:29:00 -N 100 -I 2500 -i 4000 -d ${DIROUT}_3b $CFL3
    done

    wait
fi



exit
#source /home/arce/gamos/GAMOS.5.0.0/config/confgamos.sh
for n in 0 1 2 3 4 5 6 7 ; do
    FL=fl_${n}.txt
    FO=scor_${n}.txt
    echo ooP${n}*/*inp.log | tr ' ' '\n' > $FL
    sumScores -fl $FL -fOut $FO
done

chs="NoPrimaries cGyHour pCmm3s pCugHour compt phot Rayl \
     Elastic ExcitRotat ExcitVibrat Excitation Ionisation DissocNeutr Attachment"

rm -f SCOR.txt
for ch in $chs ; do
    ( echo $ch $ch ; grep $ch scor_* |awk '{print $1,$5}' ) | column -t >> SCOR.txt
done



