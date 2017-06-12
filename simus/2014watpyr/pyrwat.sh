#!/bin/bash

source $HOME/lepts/bin/conflepts.sh 51



# mm water
mm() {
    MAT=$1
    if [ $MAT == "water" -o $MAT == "G4_WATER" -o $MAT == "LEPTS_WATER" ] ; then
	echo "18*g/mole"
    elif [ $MAT == "pyrimidine" -o $MAT == "LEPTS_PYRIMIDINE" ] ; then
	echo "80*g/mole"
    fi
}


# dens water 0.1*atmosphere
dens() {
    MAT=$1 ; PRES=$2
    Avogadro=6.02214179e+23/mole
    k_Boltzmann=8.617343e-11*MeV/kelvin
    KbNa=${k_Boltzmann}'*'${Avogadro}
    Temp=303.55*kelvin
    MM=$(mm $1)
    DENS=${PRES}*${MM}'/('${Temp}*${KbNa}')'
    echo $DENS
}


# mat water Dens
mat() {
    MAT=$1
    DENS=$2
    AA=1.0
    MM=$(mm $MAT)
    echo "//MAT_${1}
         :ROTM rmz 0 90 0
         :MATE vacuum  1. 1.008*g/mole 1.e-25*g/cm3
         :MATE ${MAT} ${AA} ${MM} ${DENS}
         //:MATE_TEMPERATURE water 273.15*kelvin
         /gamos/setParam XsDat ${MAT}"
}


# chb water 1*m 1*m 1*m
chb() {
    WRL="//WORLD
         :VOLU world BOX 1.5*${2} 1.5*${3} 1.5*${4} vacuum
         :COLOUR world 1 1 1"
    CHB="//CHAMBER_$1
         :VOLU   ch_${1} BOX $2 $3 $4 $1
         :PLACE  ch_${1} 1 world rmz $4 0 0
         :COLOUR ch_${1} 0.6 0.6 0.0"
    echo $WRL $CHB
}


# phy LeptsPhysicsList
phy() {
    echo "#PHYSICS
          /gamos/physicsList $1
          #INIT
          /run/initialize"
}


# gen e- 5*keV
gen() {
    GBOX="//GENRATOR_BOX
         :VOLU   gbox BOX 50*nm 50*nm 50*nm vacuum
         :PLACE  gbox 1 world rmz -3*cm 0 0
         :COLOUR gbox 1 0 0"

    GEN="#GENERATOR_${1}_${2}
         /gamos/generator GmGenerator
         /gamos/generator/addSingleParticleSource gene $*
         /gamos/generator/positionDist  gene GmGenerDistPositionInG4Volumes gbox
         /gamos/generator/directionDist gene GmGenerDistDirectionConst 1 0 0"
    echo $GBOX $GEN
}


uas() { # uas dep int intsp wif v pru gmcount gmhist
    gu=/gamos/userAction
    echo "#UAS"
    for u in $* ; do
	if [ $u == "dep" ] ; then
	    echo $gu UADepo
	elif [ $u == "int" ] ; then
	    echo $gu UAInteraction
	elif [ $u == "intsp" ] ; then
	    echo $gu UAInteractionSp
	elif [ $u == "wif" ] ; then
	    echo $gu UAWIF
	elif [ $u == "v" ] ; then
	    echo $gu UAVerbose
	elif [ $u == "gmcount" ] ; then
	    echo "/gamos/setParam GmCountProcessesUA:PrintPartList 1"
	    echo $gu GmCountProcessesUA
	elif [ $u == "gmhist" ] ; then
	    echo "#/gamos/analysis/histo1Max *Ener* 7*MeV"
            echo $gu GmGenerHistosUA
	elif [ $u == "pru" ] ; then
	    echo $gu UserActionPrueba
	else
	    echo $gu $u
	fi
    done
}


# uli water e- 1*keV
uli() {
    ULI="#USERLIMIT
         /gamos/physics/userLimits/setMinEKin ulimit_${1}_${2} ch_${1} $2 $3
         /gamos/physics/userLimits/print"
    echo $ULI
}


VIS=" #VISU
      /vis/scene/create
      /vis/open VRML2FILE
      /vis/viewer/flush
      /tracking/storeTrajectory 1
      /vis/scene/add/trajectories
      /vis/scene/endOfEventAction accumulate
      /vis/viewer/update
      /vis/viewer/flush"

HGL="/gamos/setParam SPhg:Units 'um'
     /gamos/setParam SPhg:x   0 2 100
     /gamos/setParam SPhg:y  -1 1 100
     /gamos/setParam SPhg:z  -1 1 100"

HGG="/gamos/setParam SPhg:Units 'mm'
     /gamos/setParam SPhg:x    0 22 110
     /gamos/setParam SPhg:y  -11 11 110
     /gamos/setParam SPhg:z  -11 11 110"


#Pres=0.4*atmosphere/760000 # mTorr
DensWG=$(dens water 0.1*atmosphere)
DensPG=$(dens pyrimidine 0.1*atmosphere)
DensWL=1*g/cm3
DensPL=1.016*g/cm3


#pru
if [ $1 == "examp" ] ; then
    UAS=$(uas int intsp dep v)
    MAT=$(mat pyrimidine ${DensPL})
    CHB=$(chb pyrimidine 10*cm 10*cm 10*cm)
    PHY=$(phy LeptsPhysics)
    GEN=$(gen e- 5*keV)
    ULI=$(uli pyrimidine e- 1*eV)
    CFL="$MAT $CHB $PHY $UAS $HGL $GEN $ULI"
    time jlepts $CFL /dir oex /run/beamOn 1 > k
    mv k oex
fi


if [ $1 == "examp2" ] ; then
    UAS=$(uas dep int intsp gmcount gmhist)
    MAT=$(mat pyrimidine ${DensPL})
    PHY=$(phy LeptsPhysics)
    GEN=$(gen e- 5*keV)
    ULP=$(uli pyrimidine e- 0.5*eV)
    CHB=$(chb pyrimidine 10*cm 10*cm 10*cm)

    CFL="$MAT $CHB $PHY $UAS $HGL $GEN $ULP"
    time jlepts $CFL /dir examp1 /run/beamOn 50 > k1
    mv k1 examp1
fi

if [ $1 == "examp3" ] ; then
    UAS=$(uas dep int)
    PHY=$(phy LeptsPhysicsList)
    GEN=$(gen e- 10*keV)
    MATW=$(mat water      ${DensWL})
    MATP=$(mat pyrimidine ${DensPL})
    CHBW=$(chb water      10*cm 10*cm 10*cm)
    CHBP=$(chb pyrimidine 10*cm 10*cm 10*cm)
    ULW=$(uli water      e- 0.5*eV)
    ULP=$(uli pyrimidine e- 0.5*eV)
    RUN="/run/beamOn 100"

    FLW="$MATW $CHBW $PHY $UAS $GEN $ULW $RUN"
    FLP="$MATP $CHBP $PHY $UAS $GEN $ULP $RUN"
    time jlepts $FLW /dir ow > kw
    mv kw ow
    time jlepts $FLP /dir op > kp
    mv kp op
fi


#lepts/dna phlists
if [ $1 == "lepdna" ] ; then
    #UAS=$(uas int intsp dep v wif)
    UAS=$(uas int dep )
    MAT=$(mat water ${DensWL})
    CHB=$(chb water 10*cm 10*cm 10*cm)
    PHY=$(phy LeptsPhysicsList)
    GEN=$(gen e- 5*keV)
    ULI=$(uli water e- 0.5*eV)
    #RUN="/run/beamOn 100"
    RUN="/run/beamOn 10000 /batchtime 9:0:0"

    CFL="$MAT $CHB $PHY $UAS $HGL $GEN $ULI $RUN"
    time jlepts $CFL /dir olep > k
    mv k olep

    MAT=$(mat G4_WATER ${DensWL})
    CHB=$(chb G4_WATER 10*cm 10*cm 10*cm)
    PHY=$(phy GmDNAPhysicsList)
    ULI=$(uli G4_WATER e- 0.5*eV)
    CFL="$MAT $CHB $PHY $UAS $HGL $GEN $ULI $RUN"
    time jlepts $CFL /dir odna > k
    mv k odna
fi

#lepts/gamos5lepts
if [ $1 == "lepgm5" ] ; then
    #UAS=$(uas int intsp dep v wif)
    UAS=$(uas int dep )
    MAT=$(mat water ${DensWL})
    CHB=$(chb water 10*cm 10*cm 10*cm)
    PHY=$(phy LeptsPhysicsList)
    GEN=$(gen e- 5*keV)
    ULI=$(uli water e- 0.5*eV)
    #RUN="/run/beamOn 10000 /batchtime 9:0:0"
    RUN="/run/beamOn 100"

    CFL="$MAT $CHB $PHY $UAS $HGL $GEN $ULI $RUN"
    time jlepts $CFL /dir olep > k
    mv k olep

    MAT=$(mat LEPTS_WATER ${DensWL})
    CHB=$(chb LEPTS_WATER 10*cm 10*cm 10*cm)
    PHY=$(phy GmLEPTSPhysics)
    ULI=$(uli LEPTS_WATER e- 0.5*eV)
    CFL="$MAT $CHB $PHY $UAS $HGL $GEN $ULI $RUN"
    time jlepts $CFL /dir oglep > k
    mv k oglep
fi



if [ $1 == "jets" ] ; then
    RUN="/run/beamOn 50"
    UAS=$(uas wif dep int intsp)
    PHY=$(phy LeptsPhysicsList)
    GEN=$(gen e- 5*keV)

    MATWL=$(mat water ${DensWL})
    MATWG=$(mat water ${DensWG})
    MATPL=$(mat pyrimidine ${DensPL})
    MATPG=$(mat pyrimidine ${DensPG})
    CHBW=$(chb water 10*cm 10*cm 10*cm)
    CHBP=$(chb pyrimidine 10*cm 10*cm 10*cm)
    ULW=$(uli water e- 0.5*eV)
    ULP=$(uli pyrimidine e- 0.4*eV)

    CFLWL="$MATWL $CHBW $PHY $UAS $HGL $GEN $ULW $RUN"
    CFLPL="$MATPL $CHBP $PHY $UAS $HGL $GEN $ULP $RUN"
    CFLWG="$MATWG $CHBW $PHY $UAS $HGG $GEN $ULW $RUN"
    CFLPG="$MATPG $CHBP $PHY $UAS $HGG $GEN $ULP $RUN"
    time jlepts $CFLWL /dir owl > kwl
    time jlepts $CFLPL /dir opl > kpl
    #time jlepts $CFLWG /dir owg > kwg
    #time jlepts $CFLPG /dir opg > kpg
    exit
fi



if [ $1 == "bigsimu" ] ; then
    RUN="/run/beamOn 1000 /batchtime 2:0:0"
    PHY=$(phy LeptsPhysicsList)
    GEN=$(gen e- 5*keV)
    UAS=$(uas gmcount dep int intsp)

    MATWL=$(mat water ${DensWL})
    MATWG=$(mat water ${DensWG})
    MATPL=$(mat pyrimidine ${DensPL})
    MATPG=$(mat pyrimidine ${DensPG})
    CHBW=$(chb water 10*cm 10*cm 10*cm)
    CHBP=$(chb pyrimidine 10*cm 10*cm 10*cm)
    ULW=$(uli water e- 0.5*eV)
    ULP=$(uli pyrimidine e- 0.5*eV)

    for((i=0;i<100;i++)) ; do
	let sd=(i+1000)*100
	SEED="/gamos/random/setSeeds $sd $sd"

	CFLWL="$MATWL $CHBW $PHY $UAS $HGL $GEN $SEED $ULW $RUN"
	CFLPL="$MATPL $CHBP $PHY $UAS $HGL $GEN $SEED $ULP $RUN"
	CFLWG="$MATWG $CHBW $PHY $UAS $HGG $GEN $SEED $ULW $RUN"
	CFLPG="$MATPG $CHBP $PHY $UAS $HGG $GEN $SEED $ULP $RUN"

	jlepts $CFLWL /dir owl_${i}
	jlepts $CFLPL /dir opl_${i}
	#jlepts $CFLPG /dir opg_${i} 
	#jlepts $CFLWG /dir owg_${i}
    done
    exit
fi


# Stopping Power
slab() {
    MATE=$1 ; Dens=$2 ; DX=$3 ; Ei=$4 ; Phy=$5
    #arg=( $* )
    #JARGS=${arg[@]:4}

    MAT=$(mat $MATE $Dens)
    CHB=$(chb $MATE 10*cm 10*cm ${DX}/2)
    PHY=$(phy $Phy)
    UAS=$(uas int dep)
    PAR="/gamos/setParam STPW:D  ${Dens}
         /gamos/setParam STPW:L  ${DX}
         /gamos/setParam STPW:MM $(mm $MATE)"
    GEN=$(gen e- $Ei)
    ULI=$(uli $MATE e- 0.5*eV)
    echo $MAT $CHB $PHY $UAS $PAR $GEN $ULI
}


if [ $1 == "stpru" ] ; then
    Dens="1e-6*g/cm3"
    DX="50*um"
    Ei="2*keV"
    Np=1000000
    dir=osp
    time jlepts $(slab water $Dens $DX $Ei LeptsPhysicsList) /dir $dir /run/beamOn $Np > k
    mv k $dir
fi


if [ $1 == "stpwdx" ] ; then
    Ei="10*keV"
    Dens="1e-6*g/cm3"
    RUN="/run/beamOn 10000000 /batchtime 2:0:0"
    Ls="0.01 0.02 0.05 0.1 0.2 0.5 1 2 3 4 5 6 7 8 9 10 15 20 30 50 60 70 80 90 100 200 500 1000"
    for dx in $Ls ; do
	DX="${dx}*um"
	dir=osp_${dx}
	jlepts $(slab water $Dens $DX $Ei LeptsPhysicsList) /dir $dir $RUN
    done
fi


if [ $1 == "stpw" ] ; then
    Dens="1e-6*g/cm3"
    DX="50*um"
    #RUN="/run/beamOn 100000000 /batchtime 2:0:0"
    RUN="/run/beamOn 10000000 /batchtime 0:20:0"
    ES="00001 00002 00003 00004 00005 00007 00010 00015 00020 00030 \
        00040 00050 00070 00100 00150 00200 00300 00400 00500 00700 \
        01000 01500 02000 03000 04000 05000 07000 10000"
    for E in $ES ; do
	jlepts $(slab pyrimidine $Dens $DX "${E}*eV" LeptsPhysicsList) /dir op_${E} $RUN
	jlepts $(slab water      $Dens $DX "${E}*eV" LeptsPhysicsList) /dir ow_${E} $RUN
	jlepts $(slab G4_WATER   $Dens $DX "${E}*eV" GmDNAPhysicsList) /dir ow2_${E} $RUN
	#egrep "SP\(|L\(" $kout
    done
fi


if [ $1 == "spectra" ] ; then
    Dens="1e-6*g/cm3"
    DX="50*um"
    RUN="/run/beamOn 10000000 /batchtime 0:20:0"
    RUN="/run/beamOn 100000000 /batchtime 2:0:0"
    for((i=0;i<10;i++)) ; do
	let sd=(i+1000)*100
	SD="/gamos/random/setSeeds $sd $sd"
	jlepts $(slab water    $Dens $DX "1*keV" LeptsPhysicsList) $SD $RUN /dir olep_${i}
	jlepts $(slab G4_WATER $Dens $DX "1*keV" GmDNAPhysicsList) $SD $RUN /dir odna_${i}
	#jlepts $(slab pyrimidine $Dens $DX "${E}*eV" LeptsPhysicsList) /dir op_${E} $RUN
    done
fi


Pru() { # prueba
    XX=1
    echo a b $*
    echo "XX1:" $XX
}
if [ $1 == "pru" ] ; then
    dx="3*um"
    b=$(Pru $dx)
    echo "b:" $b
    echo "XX2:" $XX
fi

