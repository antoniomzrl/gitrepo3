#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

MT="PRU"
WRL=$(wrl 5*mm 5*mm 5*mm)
MAT=":MIXT_BY_NATOMS $MT 1*g/cm3 2  O 1 H 2"
CHB="$MAT $(chb $MT 1*mm 1*mm 1*mm 1*mm)"
COIN="$MAT $(coin $MT 1*mm 0.2*nm 0)"   
PHY="/gamos/physicsList GmLEPTSPhysics /run/initialize"
GEN=$(gen e- 5*keV 100*um -1*mm pruspectr.txt)
GENP=$(gen gamma 100*keV 100*um -1*mm)

    
if [ $1 == "visgeom" ] ; then
    UAS="/gamos/userAction UAWIF
         /gamos/userAction GmKillAtStackingActionUA GmElectronFilter"
    CFL="$WRL $CHB $PHY $UAS $(vis) $GENP /run/beamOn 10"
    jgamos --dir oov $CFL
    #wrl oov/g4_01.wrl

    
elif [ $1 == "gener" ] ; then
    UAS="/gamos/analysis/histo1Max *Ener* 40*keV
         /gamos/analysis/histo1NBins *Ener* 400
         /gamos/userAction GmGenerHistosUA
         /gamos/setParam   UAExit:EnergyMax 40*keV
         /gamos/setParam   UAExit:EnergyBins 000
         /gamos/userAction UAExit
         /gamos/setParam TimeMark 10
         /gamos/userAction UAClock"

    RUN="/run/beamOn 1000000"
    for g in fixed interpolate interpolate_log ; do
	GEN=$(gen e- 1*keV 100*um -1*mm pruspectr.txt $g)
	CFL="$WRL $PHY $UAS $GEN $RUN"
	jgamos --dir oo${g} $CFL &
    done
    wait
    #root oog/*root    

    
elif [ $1 == "eld" ] ; then
    UAS="/gamos/userAction UAClock
         /gamos/analysis/histo1Max *EnergyLost* 0.3*keV
         /gamos/analysis/histo1NBins *EnergyLost* 300
         /gamos/analysis/addFileFormat root
         /gamos/analysis/addFileFormat TXT
         /gamos/setParam GmStepDataHistosUA:DataList AccumulatedEnergyLost
         /gamos/userAction GmStepDataHistosUA
         /gamos/setParam GmStepDataHistosUA_GmClassifierByProcess:DataList AccumulatedEnergyLost
         /gamos/userAction GmStepDataHistosUA GmClassifierByProcess"
    GEN=$(gen e- 300*eV 10*nm -1*mm)
    ULI=$(uli $MT e- 299.99*eV)
    RUN="/run/beamOn 1000000"
    CFL="$WRL $MAT $COIN $PHY $UAS $ULI $GEN $RUN"
    jgamos --dir ooeld $CFL


elif [ $1 == "dxs" ] ; then
    UAS="/gamos/userAction UAClock
         #/gamos/analysis/histo1Max *Dir* 280*degree
         #/gamos/analysis/histo1NBins *Dir* 280
         #/gamos/userAction GmStepDataHistosUA
         #/gamos/setParam GmStepDataHistosUA_GmClassifierByProcess:DataList
         #                             acos(FinalMomX/FinalMomMag)*180/3.14
         #/gamos/userAction GmStepDataHistosUA GmClassifierByProcess
         /gamos/userAction UAInteraction"
    RUN="/run/beamOn 1000000"

    for E in 1 2 10 100 1000 5000 10000 ; do
	EGY="${E}*eV"
	GEN=$(gen e- $EGY 10*nm -1*mm)
	ULI=$(uli $MT e- 0.99*${EGY} )
	CFL="$WRL $MAT $COIN $PHY $UAS $ULI $GEN $RUN"
	jgamos --dir oo${E} $CFL &
    done
    wait

    
elif [ $1 == "xs" ] ; then
    MT="PRU" ; Dens="0.001*g/cm3" ; DX="0.1*nm"
    MAT=":MIXT_BY_NATOMS $MT $Dens 2  O 1 H 2"
    COIN="$MAT $(coin $MT 1*mm $DX 0)"
    
    UAS="#/gamos/userAction UAWIF
         /gamos/userAction UAClock
         /gamos/setParam STPW:Density $Dens
         /gamos/setParam STPW:Length  $DX
         /gamos/setParam STPW:MolecularMass 18*g/mole
         /gamos/userAction UAInteraction"

    RUN="/run/beamOn 10000000"
    Eis="00100 00300 01000 03000 10000"
    Eis="0300"

    for Ei in ${Eis} ; do
	E="${Ei}*eV"
	GEN=$(gen e- $E 1*nm -1*mm)
	CFL="$WRL $COIN $PHY $UAS $GEN $RUN"
	#jgamos --dir oo${Ei} $CFL &
	jgamos --host amunoz@login-hpc.ceta-ciemat.es --dir oo${Ei} $CFL 
    done
    wait
    # grep TXS 

    
elif [ $1 == "timing" ] ; then
    UAS="/gamos/userAction UAInteraction
         /gamos/userAction UAClock"
    RUN="/run/beamOn 20"
    CFL="$WRL $CHB $PHY $UAS $ULI $GEN $RUN"

    for(( i=6; i<24; i++ )) ; do
	#jgamos --host euler --jobs $i --ppn 8  --jpn $i --btime 1:00:00 --dir oo${i} $CFL
	jgamos --host dirac --jobs $i --ppn 12 --jpn $i --btime 1:00:00 --dir oodirac${i} $CFL
    done

fi
