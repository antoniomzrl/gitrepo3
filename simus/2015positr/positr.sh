#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

WRL=$(wrl 5*cm 5*cm 5*cm)
WL="WATER_LIQ"
MAT=":MATE $WL 1.0 18*g/mole 1*g/cm3"
#CHB=":VOLU cuba BOX 1*cm 1*cm 1*cm $WL :PLACE cuba 1 world rm0 1*cm 0 0"
CHB=$(chb $WL 1*cm 1*cm 1*cm 1*cm)
 
PHY="/gamos/physicsList GmLEPTSPhysics /run/initialize"

gNa=$(gen e- 1*keV 1*um -1*cm Na22positr.txt)
ge=$(gen e- 10*keV 1*um -1*cm)
gp=$(gen e+ 10*keV 1*um -1*cm)


if [ $1 == "gener" ] ; then
    UAS="/gamos/analysis/histo1Max *Ener* 2*MeV
         /gamos/analysis/histo1NBins *Ener* 2000
         /gamos/userAction GmGenerHistosUA
         /gamos/setParam UAExit:EnergyMax 2*MeV
         /gamos/setParam UAExit:EnergyBins 2000
         /gamos/userAction UAExit
         /gamos/setParam TimeMark 5
         /gamos/userAction UAClock"
    RUN="/run/beamOn 1000000"
    time jlepts $WRL $PHY $UAS $gNa $RUN /dir oog > kkg
    #root oog*/*.root

    
elif [ $1 == "vis" ] ; then
    UAS="/gamos/userAction UAWIF $(vis)"
    GEN=$(gen e- 100*eV 1*nm -1*cm)
    RUN="/run/beamOn 1"
    time jlepts $WRL $MAT $CHB $PHY $UAS $GEN $RUN /dir oov > kkv

    
elif [ $1 == "ovn" ] ; then
    RUN="/run/beamOn 1000"
    UAS="/gamos/userAction UAInteraction
         /gamos/userAction UAClock
         #/gamos/userAction UAWIF
         #/gamos/userAction UAVerbose"
    GEN=$(gen e+ 10*keV 1*nm -1*cm)
    
    WAT="WATER_GAS_OLD2"
    MAT=":MATE $WAT 1.0 18*g/mole 1*g/cm3"
    CHB=$(chb $WAT 1*cm 1*cm 1*cm 1*cm)
    #ULI="$(uli $WAT e- 0.9*keV)"
    CFL="$WRL $MAT $CHB $PHY $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir oowo > kkwo &

    WAT="WATER_GAS"
    MAT=":MATE $WAT 1.0 18*g/mole 1*g/cm3"
    CHB=$(chb $WAT 1*cm 1*cm 1*cm 1*cm)
    #ULI="$(uli $WAT e- 0.9*keV)"
    CFL="$WRL $MAT $CHB $PHY $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir oown > kkwn &
    wait

    
elif [ $1 == "pru" ] ; then
    RUN="/run/beamOn 1000"
    UAS="/gamos/userAction UAInteraction
         /gamos/setParam TimeMark 5
         /gamos/userAction UAClock
         #/gamos/userAction UAWIF
         #/gamos/userAction UAVerbose"
    GEN=$(gen e+ 10*keV 1*nm -1*cm)

    WAT="WATER"
    MAT=":MATE $WAT 1.0 18*g/mole 1*g/cm3"
    CHB=$(chb $WAT 1*cm 1*cm 1*cm 1*cm)
    CFL="$WRL $MAT $CHB $PHY $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir oop > kkp

    
elif [ $1 == "dxs" ] ; then
    RUN="/run/beamOn 10000000"
    UAS="/gamos/userAction UAInteraction
         /gamos/userAction UAClock
        #/gamos/userAction UAVerbose"
    GEN=$(genline e+ 7*keV 50*nm -5*mm)
    CHB=$(chb $WL 0.1*nm 1*cm 1*cm 0)
    ULI="$(uli $WL e- 6.9*keV)"
    CFL="$WRL $MAT $CHB $PHY $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir oodxs > kkdxs 
    exit
    #root -b -p -q .x ${LEPTSDIR}/macros/tabhgs.cc\(\"oodxs/hijob.root\"\) > p1.txt
    #plot [:20]'dxs7k' w linesp lw 2 ps 2, 'dxs.txt' u 1:($2/500) w histe lt 3

    
elif [ $1 == "jete" ] ; then
    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         #/gamos/userAction UAWIF
         #/gamos/userAction UAVerbose"
    CHB=$(chb $WL 1*cm 1*cm 1*cm 0)
    ge=$(gen e- 10*keV 1*nm 0)
    RUN="/run/beamOn 10"
    time jlepts $WRL $MAT $CHB $PHY $UAS $ge $RUN /dir ooe > kke

    
elif [ $1 == "jetp" ] ; then
    V1=":VOLU gbox BOX 0.1*nm   9*um 0.1*nm vacuum"
    V2=":VOLU gbox BOX 0.1*nm   2*um 0.1*nm vacuum"
    V3=":VOLU gbox BOX 0.1*nm 0.12*um 0.1*nm vacuum"
    GP=":PLACE gbox 1 world rm0 0 0 0 
        /gamos/generator GmGenerator 
        /gamos/generator/addSingleParticleSource gn e+ 10*keV
        /gamos/generator/positionDist gn GmGenerDistPositionInG4Volumes gbox
        /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0"
    CHB=$(chb $WL 1*cm 1*cm 1*cm 0)
    
    UAS="/gamos/userAction UAWIF
         /gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/userAction UAClock 
         #/gamos/userAction UAVerbose
         #$(vis)"

    RUN="/run/beamOn 50"
    ULI="$(uli $WL e- 0.1*eV)"

    # for s in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20  ; do
    # 	RUN="/run/beamOn 20"
    # 	SEED="/gamos/random/setSeeds $s $s"
    # 	#time jlepts $WRL $MAT $CHB $PHY $UAS $V3 $GP $SEED $RUN /dir oo > kk
    # 	#drawpw oo/job.bin
    # done
    # RUN="/run/beamOn 1"
    # SEED="/gamos/random/setSeeds 17 17"
    # UAS="$UAS /gamos/userAction UAVerbose"
    # time jlepts $WRL $MAT $CHB $PHY $UAS $V3 $GP $SEED $RUN /dir oo > kk
    # exit
    
    #time jlepts $WRL $MAT $CHB $PHY $UAS $V1 $GP $RUN /dir oop1 > kkp1 &
    #time jlepts $WRL $MAT $CHB $PHY $UAS $V2 $GP $RUN /dir oop2 > kkp2 &
    time jlepts $WRL $MAT $CHB $PHY $UAS $V3 $GP $ULI $RUN /dir oop3 > kkp3 &
    wait


elif [ $1 == "depx" ] ; then
    UAS="/gamos/setParam TimeMark 10
         /gamos/userAction UAClock         
         /gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/setParam UAInteractionSp:Title Edep-Depth
         /gamos/setParam UAInteractionSp:x 300 0 6*um
         /gamos/userAction UAInteractionSp
         #/gamos/userAction UAVerbose"
    #ULI="$(uli $WL e- 0.1*eV)"
    RUN="/run/beamOn 10000"
    CFL="$WRL $MAT $CHB $PHY $UAS"
    #time jlepts $CFL $gp /run/beamOn 10000 /dir oop > kkp ; exit
    plepts -v 51d          -n 8  -t 5:00:00 -N 5        -d oeu $CFL $gp $RUN
    plepts -v 51d -h dirac -n 12 -t 5:00:00 -N 5 -I 501 -d odi $CFL $gp $RUN

    exit
    
    hadd hisp.root ops*/hisp*root
    root -b -p -q .x ${LEPTSDIR}/macros/tabhgs.cc\(\"hi.root\"\) > k
    root -b -p -q .x ${LEPTSDIR}/macros/tabhgsjoin.cc\(\"hisp.root\"\) > k
    for f in *root ; do
	root -b -p -q .x ${LEPTSDIR}/macros/tabhgsjoin.cc\(\"${f}\"\) > ${f}_tabhgs.txt
    done

elif [ $1 == "timing" ] ; then
    UAS="/gamos/setParam TimeMark 10
         /gamos/userAction UAClock         
         /gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/setParam UAInteractionSp:Title Edep-Depth
         /gamos/setParam UAInteractionSp:x 300 0 6*um
         /gamos/userAction UAInteractionSp"
    ULI="$(uli $WL e- 0.1*eV)"
    RUN="/run/beamOn 1000"
    CFL="$WRL $MAT $CHB $PHY $UAS $ULI $gp $RUN"
    plepts -v 51d          -n 8  -t 5:00:00 -N 1 -d oeul $CFL
    plepts -v 51d -h dirac -n 12 -t 5:00:00 -N 1 -d odir $CFL
    jlepts $CFL /dir oop > kkp
fi


exit

# gas2liq gasIXS.txt liqIXS.txt
gas2liq() {
    grep '#' $1 > ftmp
    grep -v '#' $1 | awk '{print $1,$2*$10,$3*$10,$4*$10,$5*$10,$6*$10,$7*$10,$8*$10,$9*$10}' >> ftmp
    column -t ftmp > $2
    rm ftmp
}

