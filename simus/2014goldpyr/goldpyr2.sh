#!/bin/bash

source $HOME/lepts/bin/conflepts.sh 51



# dens water MM 0.1*atmosphere
dens() {
    MAT=$1 ; MM=$2 ; PRES=$3
    Avogadro=6.02214179e+23/mole
    k_Boltzmann=8.617343e-11*MeV/kelvin
    KbNa=${k_Boltzmann}'*'${Avogadro}
    Temp=303.55*kelvin
    DENS=${PRES}*${MM}'/('${Temp}*${KbNa}')'
    echo $DENS
}


# mat water MM Dens
mat() {
    MAT=$1
    MM=$2
    DENS=$3
    AA=1.0
    echo "//MAT_${1}
         :MATE vacuum  1. 1.008*g/mole 1.e-25*g/cm3
         :MATE ${MAT} ${AA} ${MM} ${DENS}
         //:MATE_TEMPERATURE water 273.15*kelvin
         #/gamos/setParam XsDat ${MAT}"
}


# wrl 1*m 1*m 1*m
wrl() {
    echo "//WORLD
         :ROTM rmz 0 90 0
         :VOLU world BOX ${1} ${2} ${3} vacuum
         :COLOUR world 1 1 1"
}

# coin water Diametr dx posx
coin() {
    echo "//coin-slab
         :VOLU   ch_${1} TUBE 0.0 ${2}/2 ${3}/2 $1
         :PLACE  ch_${1} 1 world rmz $4 0 0
         :COLOUR ch_${1} 0.6 0.6 0.0"
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
         :VOLU   gbox BOX 1*nm 1*nm 1*nm vacuum
         :PLACE  gbox 1 world rmz -1*um 0 0
         :COLOUR gbox 0 0 1"

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
	elif [ $u == "exit" ] ; then
	    echo $gu UAExit
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

# efi 20*eV
efi() {
    EFI="#ENERGY_FIlTER
         /gamos/filter energyF GmKineticEnergyFilter 0. ${1}
         /gamos/userAction GmKillAtStackingActionUA energyF
        "
    echo $EFI
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


#Pres=0.4*atmosphere/760000 # mTorr
DensPG=$(dens pyrimidine 0.1*atmosphere)

DensWL="1*g/cm3"
DensPL="1.016*g/cm3"
DensGS="19.3*g/cm3"
MMW="18*g/mole"
MMP="80*g/mole"
MMG="197*g/mole"

#pru
if [ $1 == "pru" ] ; then
    MAT=$(mat pyrimidine $MMP ${DensPL})
    WRL=$(wrl 10*cm 10*cm 10*cm)
    COIN=$(coin pyrimidine 1*cm 0.4*cm 0.2*cm)
    PHY=$(phy LeptsPhysicsList)
    GEN=$(gen e- 50*eV)
    ULI=$(uli pyrimidine e- 2*eV)
    UAS=$(uas int intsp dep v wif exit gmcount gmhist)

    CFL="$MAT $WRL $COIN $PHY $UAS $GEN $ULI $VIS"
    time jlepts $CFL /dir o /run/beamOn 1 > k
fi


if [ $1 == "solidgas" ] ; then
    WRL=$(wrl 5*um 5*um 5*um) 
    PHY=$(phy GmLEPTSPhysics)
    GEN=$(gen e- 2*keV)
    UAS=$(uas int exit wif)
    RUN="/run/beamOn 100"
    #UAS=$(uas int exit)
    #RUN="/run/beamOn 10000 /batchtime 3:00:00"

    # gold gas dens solid
    MAT=$(mat LEPTS_GOLD $MMG ${DensGS})
    COIN=$(coin LEPTS_GOLD 4*um 2*um 1*um)
    ULI=$(uli LEPTS_GOLD e- 5*eV)
    CFL="$MAT $WRL $COIN $PHY $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir og > k
    mv k og

    # gold solid
    MAT=$(mat LEPTS_GOLD_SOLID $MMG ${DensGS})
    COIN=$(coin LEPTS_GOLD_SOLID 4*um 2*um 1*um)
    ULI=$(uli LEPTS_GOLD_SOLID e- 5*eV)
    CFL="$MAT $WRL $COIN $PHY $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir ogs > k
    mv k ogs
fi

if [ $1 == "bigsimu" ] ; then
    WRL=$(wrl 5*um 5*um 5*um) 
    PHY=$(phy GmLEPTSPhysics)
    GEN=$(gen e- 2*keV)
    UAS=$(uas int exit)
    RUN="/run/beamOn 10000 /batchtime 3:00:00"

    MAT=$(mat LEPTS_GOLD_SOLID $MMG ${DensGS})
    COIN=$(coin LEPTS_GOLD_SOLID 4*um 2*um 1*um)
    ULI=$(uli LEPTS_GOLD_SOLID e- 5*eV)

    MAT2=$(mat LEPTS_PYRIMIDINE_LIQUID $MMP ${DensPL})
    ULI2=$(uli LEPTS_PYRIMIDINE_LIQUID e- 1.1*eV)
 
    for((i=0;i<100;i++)) ; do
	let sd=(i+1000)*100
	SEED="/gamos/random/setSeeds $sd $sd"
	CFL="$MAT $WRL $COIN $PHY $UAS $GEN $SEED $ULI $RUN"
	#time jlepts $CFL /dir ogs_${i}

	#for dx in 05 10 30 ; do
	for dx in 03 01 ; do
	    DX=${dx}'*nm'
	    SF='-0.5*'${dx}'*nm'
	    COIN2=$(coin LEPTS_PYRIMIDINE_LIQUID 4*um $DX $SF)
	    CFL2="$MAT $MAT2 $WRL $COIN $COIN2 $PHY $UAS $GEN $SEED $ULI $ULI2 $RUN"
	    time jlepts $CFL2 /dir ogs${dx}nm_${i}
	done
    done
fi

if [ $1 == "trayect" ] ; then
    WRL=$(wrl 2*um 2*um 2*um) 
    PHY=$(phy GmLEPTSPhysics)
    GEN=$(gen e- 2*keV)
    MAT=$(mat LEPTS_GOLD_SOLID $MMG ${DensGS})
    MAT2=$(mat LEPTS_PYRIMIDINE_LIQUID $MMP ${DensPL})
    ULI=$(uli LEPTS_GOLD_SOLID e- 7*eV)
    ULI2=$(uli LEPTS_PYRIMIDINE_LIQUID e- 1.1*eV)
    UAS=$(uas int exit wif)
    COIN=$(coin LEPTS_GOLD_SOLID 0.5*um 0.5*um 0.25*um)
    COIN2=$(coin LEPTS_PYRIMIDINE_LIQUID 0.3*um 2*nm -2*nm)
    RUN="/run/beamOn 500"

    CFL2="$MAT $MAT2 $WRL $COIN $COIN2 $PHY $UAS $GEN $SEED $ULI $ULI2 $RUN"
    time jlepts $CFL2 /dir ot > k
fi

############################################################
#casa
# gen e- 5*keV
gen2() {
    GBOX="//GENRATOR_BOX
         :VOLU   gbox BOX .1*nm .1*nm .1*nm vacuum
         :PLACE  gbox 1 world rmz -1*um 0 0
         :COLOUR gbox 0 0 1"

    GEN="#GENERATOR_${1}_${2}
         /gamos/generator GmGenerator
         /gamos/generator/addSingleParticleSource gene $*
         /gamos/generator/positionDist  gene GmGenerDistPositionInG4Volumes gbox
         /gamos/generator/directionDist gene GmGenerDistDirectionConst 1 0 0"
    echo $GBOX $GEN
}

if [ $1 == "casa" ] ; then
    WRL=$(wrl 2*um 2*um 2*um) 
    PHY=$(phy GmLEPTSPhysics)
    GEN=$(gen2 e- 2*keV)
    MAT=$(mat LEPTS_GOLD_SOLID $MMG ${DensGS})
    MAT2=$(mat LEPTS_PYRIMIDINE_LIQUID $MMP ${DensPL})
    UAS=$(uas int exit wif)
    COIN=$(coin LEPTS_GOLD_SOLID 0.5*um 0.5*um 0.25*um)
    #COIN2=$(coin LEPTS_PYRIMIDINE_LIQUID 0.5*um 3*nm -2*nm)
    COIN2=$(coin LEPTS_PYRIMIDINE_LIQUID 0.5*um 10*nm -6*nm)
    ULI=$(uli LEPTS_GOLD_SOLID e- 7*eV)
    ULI2=$(uli LEPTS_PYRIMIDINE_LIQUID e- 1*eV)
    EFI=$(efi 1.5*eV)
    RUN="/run/beamOn 50"
    SEED="/gamos/random/setSeeds 1003 1001"
    CFL2="$MAT $MAT2 $WRL $COIN $COIN2 $PHY $UAS $GEN $ULI $ULI2 $SEED $RUN"
    time jlepts $CFL2 /dir ot > k
fi
