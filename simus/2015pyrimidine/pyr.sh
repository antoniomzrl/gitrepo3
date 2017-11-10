#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

WRL=$(wrl 3*cm 3*cm 3*cm)
PYRIM="PYRIMIDINE"
MAT=":MATE $PYRIM 1.0 80*g/mole 1.016*g/cm3"
CHB=$(chb $PYRIM 1*cm 1*cm 1*cm 1*cm)

PHY="/gamos/physicsList GmLEPTSPhysics
     /run/initialize"
ULI="$(uli $PYRIM e- 1.2*eV)"

GEN=$(genline e- 5*keV 50*nm -5*mm)

if [ $1 == "vis" ] ; then
    GEN=$(gen e- 2*eV 1*um -5*mm)
    RUN="/run/beamOn 1"
    UAS="$(vis) /gamos/userAction UAInteraction /gamos/userAction UAVerbose"
    CFL="$WRL $MAT $CHB $PHY $UAS $ULI $GEN $RUN"
    time jlepts $CFL /dir oo > kk
fi


if [ $1 == "prudxs" ] ; then
    RUN="/run/beamOn 5000000"
    UAS="/gamos/userAction UAInteraction
         /gamos/userAction UAClock
        #/gamos/userAction UAVerbose"
    GEN=$(genline e- 1*keV 50*nm -5*mm)
    
    WAT="PRUDXS"
    MAT=":MATE $WAT 1.0 80*g/mole 1.016*g/cm3"
    CHB=$(chb $WAT 0.1*nm 1*cm 1*cm 0)
    ULI="$(uli $WAT e- 0.95*keV)"
    CFL="$WRL $MAT $CHB $PHY $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir oo > kk
fi


if [ $1 == "dxs" ] ; then
    RUN="/run/beamOn 10000000"
    UAS="/gamos/userAction UAInteraction
         /gamos/userAction UAClock
        #/gamos/userAction UAVerbose"
    GEN=$(genline e- 1*keV 50*nm -5*mm)
    
    CHB=$(chb $PYRIM 0.1*nm 1*mm 1*mm 0)
    ULI="$(uli $PYRIM e- 0.9*keV)"
    CFL="$WRL $MAT $CHB $PHY $UAS $GEN $ULI $RUN"
    #time jlepts $CFL /dir oop > kkp &

    WAT="WATER"
    MAT=":MATE $WAT 1.0 80*g/mole 1.016*g/cm3"
    CHB=$(chb $WAT 0.1*nm 1*cm 1*cm 0)
    ULI="$(uli $WAT e- 0.9*keV)"
    CFL="$WRL $MAT $CHB $PHY $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir oow > kkw &

    WAT="iWATER"
    MAT=":MATE $WAT 1.0 80*g/mole 1.016*g/cm3"
    CHB=$(chb $WAT 0.1*nm 1*cm 1*cm 0)
    ULI="$(uli $WAT e- 0.9*keV)"
    CFL="$WRL $MAT $CHB $PHY $UAS $GEN $ULI $RUN"
    #time jlepts $CFL /dir oowi > kkwi &

    wait
    exit
    root -b -p -q .x ${LEPTSDIR}/macros/tabhgs.cc\(\"oop1/hijob.root\"\) > p1.txt
    root -b -p -q .x ${LEPTSDIR}/macros/tabhgs.cc\(\"oop/hijob.root\"\)  > p.txt
    root -b -p -q .x ${LEPTSDIR}/macros/tabhgs.cc\(\"oow1/hijob.root\"\) > w1.txt
    root -b -p -q .x ${LEPTSDIR}/macros/tabhgs.cc\(\"oow/hijob.root\"\)  > w.txt
fi



if [ $1 == "jets" ] ; then
    SEED="/gamos/random/setSeeds 3 3"
    RUN="/run/beamOn 20"

    #GEN=$(genline e- 5*keV 10*um -5*mm)
    CFL="$WRL $MAT $CHB $PHY /gamos/userAction UAWIF $ULI $GEN $SEED /run/beamOn 20"
    time jlepts $CFL /dir ooa1 >& kka1 &
    CFL="$WRL $MAT $CHB $PHY /gamos/userAction UAInteraction $ULI $GEN $SEED /run/beamOn 10000"
    #time jlepts $CFL /dir ooa2 >& kka2 &

    PYRIM="PYRIMIDINE_OLD"
    MAT=":MATE $PYRIM 1.0 80*g/mole 1.016*g/cm3"
    CHB=$(chb $PYRIM 1*cm 1*cm 1*cm 1*cm)
    ULI="$(uli $PYRIM e- 1.2*eV)"

    CFL="$WRL $MAT $CHB $PHY /gamos/userAction UAWIF $ULI $GEN $SEED /run/beamOn 20"
    #time jlepts $CFL /dir oob1 >& kkb1 &
    for s in 4 26  ; do
	SEED="/gamos/random/setSeeds $s $s"
	CFL="$WRL $MAT $CHB $PHY /gamos/userAction UAWIF $ULI $GEN $SEED /run/beamOn 20"
	#time jlepts $CFL /dir oo_${s} >& kk$s &
    done
    CFL="$WRL $MAT $CHB $PHY /gamos/userAction UAInteraction $ULI $GEN $SEED /run/beamOn 10000"
    #time jlepts $CFL /dir oob2 >& kkb2 &

    wait
fi



if [ $1 == "sphg" ] ; then
    UAS="/gamos/userAction UAInteraction
         /gamos/setParam TimeMark 10
         /gamos/userAction UAClock
         /gamos/setParam UAInteractionSp:x 600 0 3000*nm
         /gamos/setParam UAInteractionSp:Title EdIntDepth 
         /gamos/userAction UAInteractionSp"
    RUN="/run/beamOn 10000"
    CFL="$WRL $MAT $CHB $PHY $UAS $ULI $GEN $RUN"
    time jlepts $CFL /dir oo > kk ; exit
    #plepts       -t 3:00:00 -N 1000         -d oopyreuler $CFL
    #plepts -n 12 -t 3:00:00 -N 1000 -I 1001 -d oopyrdirac $CFL

    PYRIM="PYRIMIDINE_OLD"
    MAT=":MATE $PYRIM 1.0 80*g/mole 1.016*g/cm3"
    CHB=$(chb $PYRIM 1*cm 1*cm 1*cm 1*cm)
    ULI="$(uli $PYRIM e- 1.2*eV)"
    CFL="$WRL $MAT $CHB $PHY $UAS $ULI $GEN $RUN"
    plepts       -t 3:00:00 -N 1000         -d pyoldeuler $CFL
    plepts -n 12 -t 3:00:00 -N 1000 -I 1001 -d pyolddirac $CFL

    exit
    hadd pyr.root    oopyr*/hisp*root
    hadd pyrold.root py*/hisp*root
    root -b -p -q .x ${LEPTSDIR}/macros/tabhgsjoin.cc\(\"pyr.root\"\) > pyr.txt
    root -b -p -q .x ${LEPTSDIR}/macros/tabhgsjoin.cc\(\"pyrold.root\"\) > pyrold.txt
fi



exit


# gas2liq gasIXS.txt liqIXS.txt
gas2liq() {
    grep '#' $1 > ftmp
    grep -v '#' $1 | awk '{print $1,$2*$9,$3*$9,$4*$9,$5*$9,$6*$9,$7*$9,$8*$9}' >> ftmp
    column -t ftmp > $2
    rm ftmp
}

gas2liq pyrimidinepru_IXS.txt pyrimidinepru_liquid_IXS.txt
gas2liq pyrimg_IXS.txt  pyriml_IXS.txt
