#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

WRL=$(wrl 20*cm 20*cm 20*cm)

Dens=1*g/cm3

PHY="/gamos/physicsList GmLEPTSPhysics /run/initialize"
UAS="#/gamos/userAction UAVerbose
     /gamos/userAction GmCountProcessesUA
     /gamos/userAction UAClock /gamos/setParam TimeMark 3600
     /gamos/userAction UAInteraction
     #/gamos/userAction UAInteractionSp
     #/gamos/setParam SPhg:Title kk"

#GEN=$(gen e- 1*keV 10*um -2*mm spectrTl204elec.txt
GEN=$(gen e- 10*keV 1*nm -1*cm)
#RUN="/run/beamOn 10000 /batchtime 8:00:00"
RUN="/run/beamOn 1000 /batchtime 1:00:00"


MATS="WATER      WATER_LIQUID
      PYRIMIDINE PYRIMIDINE_LIQUID
      GOLD       GOLD_SOLID"


MATS="WATER"
RUN="/run/beamOn 100 #/batchtime 1:00:00"


if [ $1 == "singleton" ] ; then
    GEN=$(gen e- 3*keV 1*nm -1*cm)
    RUN="/run/beamOn 2"
    MAT=":MATE WATER 1.0 18*g/mole ${Dens}
         :MATE PYRIMIDINE 1.0 18*g/mole ${Dens}"
    CHB="$(coin WATER 1*cm 1*cm 0) $(coin PYRIMIDINE 1*cm 1*cm 4*cm)"
    ULI="$(uli WATER e- 5*eV) $(uli PYRIMIDINE e- 5*eV)"

    CFL="$WRL $MAT $CHB $PHY $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir oob_${MT} /jn b_${MT} /version 51b >& ks
    exit
fi


if [ $1 == "timing" ] ; then
    UAS="/gamos/userAction UAClock
         /gamos/setParam TimeMark 5
         /gamos/userAction UAInteraction"
    GENe=$(gen e- 3*keV 1*nm -1*cm)
    GENp=$(gen e+ 3*keV 1*nm -1*cm)
    MAT=":MATE WATER 1.0 18*g/mole ${Dens}"
    CHB="$(coin WATER 1*cm 1*cm 0)"
    ULI="$(uli WATER e- 0.5*eV) $(uli WATER e+ 0.5*eV)"
    RUN="/run/beamOn 100"

    CFL="$WRL $MAT $CHB $PHY $UAS $GENe $ULI $RUN"
    #time jlepts $CFL /dir oo1 /version 51b >& k1
    #time jlepts $CFL /dir oo2 /version 51c >& k2

    CFL="$WRL $MAT $CHB $PHY $UAS $GENp $ULI $RUN"
    #time jlepts $CFL /dir oo3 /version 51b >& k3
    time jlepts $CFL /dir oo4 /version 51c >& k4
    
    #1m12.461s  0m58.416s 20%
    exit
fi


if [ $1 == "pru1" ] ; then
    for MT in $MATS ; do
	LMT=LEPTS_${MT}

	if echo "$MT" | grep -q "GOLD"; then
	    UAS="$UAS /gamos/setParam SPhg:x 50 0 1*um"
	    MAT=":MATE $LMT 1.0 197*g/mole 19.3*g/cm3"
	elif echo "$MT" | grep -q "PYRIMIDINE"; then
	    UAS="$UAS /gamos/setParam SPhg:x 50 0 10*um"
	    MAT=":MATE $LMT 1.0 80*g/mole 1.016*g/cm3"	
	else
	    UAS="$UAS /gamos/setParam SPhg:x 50 0 10*um"
	    MAT=":MATE $LMT 1.0 18*g/mole 1*g/cm3"
	fi

	CHB=$(coin $LMT 15*cm 14*cm 7*cm)
	ULI=$(uli $LMT e- 5*eV)

	CFL="$WRL $MAT $CHB $PHY $UAS $GEN $ULI $RUN"
	time jlepts $CFL /dir oo0_${MT} /jn 0_${MT} /version 510 >& k0 &
	time jlepts $CFL /dir oob_${MT} /jn b_${MT} /version 51c >& kc &
    done
fi


if [ $1 == "errorDepo" ] ; then
    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction UAInteraction
#imprime la E perdida por los primarios
/gamos/setParam GmTrackDataCoutUA_GmPrimaryFilter:DataList EventID TrackID Particle Particle AccumulatedEnergyLost AccumulatedEnergyDeposited
/gamos/userAction GmTrackDataCoutUA  GmPrimaryFilter
#imprime la suma de las E perdida y la suma de las E depositadas
/gamos/setParam GmEventDataCoutUA:DataList EventID AccumulatedEnergyLost AccumulatedEnergyDeposited
/gamos/userAction GmEventDataCoutUA 
         #$(vis)"

UAS="/gamos/userAction UAInteraction"

    GEN=$(gen e- 1*keV 1*nm 0)

    WL="G4_WATER"
    MAT=":MATE $WL 1.0 18*g/mole ${Dens}"
    CHB="$(coin $WL 5*cm 5*cm 0)"
    ULI="$(uli $WL e- 0.001*eV)"

    RUN="/run/beamOn 1000"
    
    for (( i=27; i<28; i++ )) ; do
	sd='1'${i}
	SEED="/gamos/random/setSeeds $sd $sd"
	CFL="$WRL $MAT $CHB $PHY $UAS $GEN $ULI $SEED $RUN"
	time jlepts $CFL /dir ooe >& kke_$i
    done
fi

if [ $1 == "prot" ] ; then
    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/userAction UAClock
         /gamos/setParam TimeMark 5
         #/gamos/userAction UAVerbose
         /gamos/userAction UAWIF"

    FLTR="/gamos/analysis/histo1NBins *Angle* 1800
          /gamos/analysis/histo1NBins *Energy* 5000
          /gamos/analysis/histo1Max   *Energy* 5*keV
          $(procfilt proton_G4LEPTSElastic)
          $(procfilt proton_G4LEPTSExcitation)
          $(procfilt proton_G4LEPTSIonisation)
          $(procfilt proton_G4LEPTSChargeDecrease)
          $(procfilt hydrogen_G4LEPTSElastic)
          $(procfilt hydrogen_G4LEPTSExcitation)
          $(procfilt hydrogen_G4LEPTSIonisation)
          $(procfilt hydrogen_G4LEPTSChargeIncrease)"
          
    GEN=$(gen proton 1*MeV 1*nm -1*cm)

    WL="G4_WATER"
    MAT=":MATE $WL 1.0 18*g/mole ${Dens}"
    CHB="$(coin $WL 1*cm 1*cm 0)"
    ULI="$(uli $WL e- 1*GeV)"

    RUN="/run/beamOn 1"
    RUN="/run/beamOn 10"

    CFL="$WRL $MAT $CHB $PHY $FLTR $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir oop >& kkp
fi


if [ $1 == "pru" ] ; then
    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction UAInteraction
         /gamos/userAction UAWIF"

    GEN=$(gen e- 10*keV 1*nm 0)
    GENp=$(gen e+ 10*keV 1*nm 0)
    GENpr=$(gen proton 5*MeV 1*nm 0)
    RUN="/run/beamOn 1"

    #for MT in WATER_LIQ WATER_LIQ_OLD WATER_GAS WATER_GAS_OLD ; do
    for MT in WATER_LIQ WATER_LIQ_OLD ; do
	MAT=":MATE $MT 1.0 18*g/mole 1*g/cm3"
	CHB="$(coin $MT 5*cm 5*cm 0)"

        CFL="$WRL $MAT $CHB $PHY $UAS $GEN $ULI $SEED $RUN"
	time jlepts $CFL /dir oo_$MT >& kke_$MT

        CFL="$WRL $MAT $CHB $PHY $UAS $GENp $ULI $SEED $RUN"
	time jlepts $CFL /dir oop_$MT >& kkp_$MT

        CFL="$WRL $MAT $CHB $PHY $UAS $GENpr $(uli $MT e- 1*GeV) $SEED $RUN"
	time jlepts $CFL /dir oopr_$MT >& kkpr_$MT
    done
fi
