#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

VAC=":MATE vacuum 1 1*g/mole 1e-25*g/cm3"
WRL=$(wrl 5*mm 5*mm 5*mm)
COIN=$(coin G4_Au 1*mm 100*um 0)

GENEL=$(gen e-    1*keV 10*um -2*mm Tl204elec.txt)
GENPH=$(gen gamma 1*keV 10*um -2*mm Tl204phot.txt)
GENflat=":VOLU   gbox ORB 10*um/2 vacuum
         :PLACE  gbox 1 world rmz -2*mm 0 0  :COLOUR gbox 0 0 1
         /gamos/generator GmGenerator
         /gamos/generator/addSingleParticleSource gn e- 1*keV
         /gamos/generator/positionDist gn GmGenerDistPositionInG4Volumes gbox
         /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0
         /gamos/generator/energyDist gn GmGenerDistEnergyRandomFlat 0 100*keV"
 
INI="/run/setCut 1 um /run/initialize"

UAS="#/gamos/userAction GmCountProcessesUA
     #/gamos/analysis/histo1Max *Ener* 800*keV
     #/gamos/analysis/histo1NBins *Ener* 800
     #/gamos/userAction GmGenerHistosUA
     /gamos/setParam   UAExit:EnergyMax 780*keV
     /gamos/setParam   UAExit:EnergyBins 2000
     /gamos/userAction UAExit
     #/gamos/setParam TimeMark 10
     /gamos/userAction UAClock"

    
if [ $1 == "visgeom" ] ; then
    COIN=$(coin G4_Au 1*mm 100*um 0)
    CFL="$WRL $COIN $PHYem $GENPH"
    time jlepts $CFL $(vis)                  /run/beamOn 10   /dir oov >& kkv &
    time jlepts $CFL /gamos/userAction UAWIF /run/beamOn 1000 /dir oow >& kkw &
    wait
    #wrl oov/g4_01.wrl 
    #lcv oow/job.bin 

    
elif [ $1 == "gener" ] ; then
    PHY="/gamos/physicsList GmEMPhysics $INI"
    RUN="/run/beamOn 10000000"
    CFL="$WRL $PHY $UAS"
    time jlepts $CFL $GENEL $RUN /dir ooge >& kkge &
    time jlepts $CFL $GENPH $RUN /dir oogp >& kkgp &
    wait

elif [ $1 == "phys" ] ; then
    PHYlep="/gamos/physicsList GmLEPTSPhysics $INI"
    PHYemg="/gamos/physicsList GmEMPhysics $INI"
    PHYlow="$PHYemg /gamos/GmPhysics/replacePhysics electron-lowener
           /gamos/GmPhysics/replacePhysics gamma-lowener"
    PHYpen="$PHYemg /gamos/GmPhysics/replacePhysics electron-penelope
           /gamos/GmPhysics/replacePhysics gamma-penelope"       
    PHYcou="/gamos/physicsList GmEMExtendedPhysics $INI
            /gamos/physics/removeProcessesByName msc 
            /gamos/physics/addPhysics coulombScattering"
    PHYdna="/gamos/physicsList GmDNAPhysics $INI"
    PHYsss="/gamos/physicsList PhysListEmStandardSS $INI"
    
    PHYop4="/gamos/physicsList GmEMStandardPhysics
            /gamos/EMStandardPhysics/replacePhysics emstandard_opt4 $INI
            #_opt0,_opt1,_opt2,_opt3(default),_opt4"
    PHYop3="/gamos/physicsList GmEMStandardPhysics
            /gamos/EMStandardPhysics/replacePhysics emstandard_opt3 $INI"
    PHYop2="/gamos/physicsList GmEMStandardPhysics
            /gamos/EMStandardPhysics/replacePhysics emstandard_opt2 $INI"
    PHYop1="/gamos/physicsList GmEMStandardPhysics
            /gamos/EMStandardPhysics/replacePhysics emstandard_opt1 $INI"
    PHYop0="/gamos/physicsList GmEMStandardPhysics
            /gamos/EMStandardPhysics/replacePhysics emstandard_opt0 $INI"

    GEN=$GENEL
    #ULI="$(uli G4_Au e- 10*keV)"
    UAS="/gamos/setParam   UAExit:EnergyMax 780*keV
         #/gamos/setParam   UAExit:EnergyBins 2000
         /gamos/setParam   UAExit:EnergyBins 500
         /gamos/userAction UAExit
         /gamos/userAction UAClock"
    COIN=$(coin G4_Au 1*mm 60*um 0)
    RUN="/run/beamOn 1000000"
    
    #time jlepts $WRL $COIN $PHYlep $UAS $ULI $GEN $RUN /dir oo1 >& kk1 &
    time jlepts $WRL $COIN $PHYemg $UAS $ULI $GEN $RUN /dir oo2 >& kk2 &
    time jlepts $WRL $COIN $PHYlow $UAS $ULI $GEN $RUN /dir oo3 >& kk3 &
    time jlepts $WRL $COIN $PHYpen $UAS $ULI $GEN $RUN /dir oo4 >& kk4 &
    #time jlepts $WRL $COIN $PHYdna $UAS $ULI $GEN $RUN /dir oo5 >& kk5 &
    time jlepts $WRL $COIN $PHYsss $UAS $ULI $GEN $RUN /dir oo6 >& kk6 &
    time jlepts $WRL $COIN $PHYop0 $UAS $ULI $GEN $RUN /dir oo7>& kk7 &
    time jlepts $WRL $COIN $PHYop1 $UAS $ULI $GEN $RUN /dir oo8>& kk8 &
    time jlepts $WRL $COIN $PHYop2 $UAS $ULI $GEN $RUN /dir oo9>& kk9 &
    time jlepts $WRL $COIN $PHYop3 $UAS $ULI $GEN $RUN /dir ooa>& kka &
    time jlepts $WRL $COIN $PHYop4 $UAS $ULI $GEN $RUN /dir oob>& kkb &

    wait

    
 elif [ $1 == "debug" ] ; then
    PHY="/gamos/physicsList GmEMPhysics $INI"
    UAS="/gamos/setParam   UAExit:EnergyMax 780*keV
         #/gamos/setParam   UAExit:EnergyBins 2000
         /gamos/setParam   UAExit:EnergyBins 200
         /gamos/userAction UAExit
         /gamos/userAction UAClock
         /gamos/userAction GmKillAtStackingActionUA GmGammaFilter"
    RUN="/run/beamOn 10000000"
    
    COIN=$(coin G4_Au 1*mm 60*um 0)
    CFL="$WRL $COIN $PHY $UAS $GENEL $RUN"
    jlepts $CFL /dir oo_60um > kk_60um 

    
elif [ $1 == "simu" ] ; then
    PHY="/gamos/physicsList GmEMPhysics $INI"
    DXS="000 020 040 060 100"
    #DXS="000 020"
    RUN="/run/beamOn 50000000"
    
    for dx in $DXS ; do
	if [ $dx == "000" ] ; then
	    COIN="//"
	else
	    COIN=$(coin G4_Au 1*mm ${dx}'*um' 0)
	fi
	CFL="$WRL $COIN $PHY $UAS $ULI $GENEL $RUN"
	plepts -h euler -n 8 -N 100 -I 1001 -t 4:29:00 -d ooe_${dx}um $CFL
	CFL="$WRL $COIN $PHY $UAS $ULI $GENPH $RUN"
	plepts -h euler -n 8 -N 100 -I 1001 -t 4:29:00 -d oop_${dx}um $CFL
    done

  
elif [ $1 == "simunoph" ] ; then
    PHY="/gamos/physicsList GmEMPhysics $INI"
    DXS="020 040 060 100"
    #DXS="000 020"
    ULI="/gamos/userAction GmKillAtStackingActionUA GmGammaFilter"
    RUN="/run/beamOn 50000000"
    RUN="/run/beamOn 50"
    
    for dx in $DXS ; do
	COIN=$(coin G4_Au 1*mm ${dx}'*um' 0)
	CFL="$WRL $COIN $PHY $UAS $ULI $GENEL $RUN"
	plepts -h euler -n 8 -N 100 -I 1001 -t 4:29:00 -d oonoph_${dx}um $CFL
    done

    
elif [ $1 == "sphg" ] ; then
    PHY="/gamos/physicsList GmEMPhysics $INI"
    UAS="#/gamos/userAction UAVerbose
         #/gamos/userAction UAWIF
         /gamos/userAction UAClock
         /gamos/setParam UAInteractionSp:Title xxx
         /gamos/setParam UAInteractionSp:x 100 0 200*um
         /gamos/userAction UAInteractionSp"
    COIN=$(chb G4_Au 1*mm 1*mm 1*mm 1*mm)
    #RUN="$(vis) /run/beamOn 10"
    RUN="/run/beamOn 1000000"

    CFL="$WRL $COIN $PHY $UAS $ULI $GENEL $RUN"
    #jlepts $CFL /dir oosp > kksp &
    plepts -h euler -n 8 -N 48 -I 1001 -d oosp -t 4:29:00 $CFL

    ULI="/gamos/userAction GmKillAtStackingActionUA GmGammaFilter"
    CFL="$WRL $COIN $PHY $UAS $ULI $GENEL $RUN"
    #jlepts $CFL /dir oosp2 > kksp2 &
    plepts -h euler -n 8 -N 48 -I 1001 -d oosp2 -t 4:29:00 $CFL

    wait


elif [ $1 == "pen" ] ; then
    EXE="/home/roldan/lepts/penelope/penslab.sh -m gold.mat"
    SPF="/home/roldan/lepts/spectr/Tl204elec.txt"

    #monoenerg
    time $EXE -K 3e5         -t 0.1e-4 -o oo1  -n 100000  >& kk1 &

    #lamina fina = fuente
    time $EXE -K 1e3 -S $SPF -t 0.00000001e-4 -o oo2 -n 10000000 >& kk2 &

    #60 micras
    time $EXE -K 1e3 -S $SPF -t 60e-4 -o oo3 -n 10000 >& kk3 &
    wait
    

elif [ $1 == "bpen" ] ; then
    EXE="$HOME/lepts/penelope/penslab.sh $MAT $SPF $FLG
         -m $HOME/lepts/penelope/gold.mat
         -K 1e3 -S $HOME/lepts/spectr/Tl204elec.txt
         -t 60e-4 -n 10000 --btime 1:00:00"  #100K: 14min, 1M: 140min=2h20

    #parjob  -h    local  -p   12  -j    100  -d   oo1  -S    1  -s    1 $EXE
    parjob --host dirac --ppn 12 --jobs 100 --dir oo2 --SEED 1 --seed 1 $EXE
    

elif [ $1 == "pru" ] ; then
    PHY="/gamos/physicsList GmEMPhysics $INI"
    RUN="/run/beamOn 1000"
    COIN=$(coin G4_Au 1*mm 60*um 0)
    UAS="/gamos/userAction UAInteraction"
    CFL="$WRL $COIN $PHY $UAS $ULI $GENEL $RUN"

    #compar
    #jgamos --host dirac --ppn 8 --jobs 20 --btime 0:0:10 --seed 400 --SEED 300 --dir oo1 $CFL
    #plepts     -h dirac    -n 8     -N 20      -t 0:0:10     -I 300     -i 400  -d   oo2 $CFL

    #jgamos --dir oopru2 --SEED 500 $CFL
    jgamos --dir oopru --jn pru --seed 400 --SEED 300 \
	   --norun --host dirac --ppn 8 --jobs 21 --btime 00:01:00 $CFL


elif [ $1 == "timing" ] ; then
    PHY="/gamos/physicsList GmEMPhysics $INI"
    COIN=$(coin G4_Au 1*mm 60*um 0)
    UAS="/gamos/userAction UAInteraction
         /gamos/userAction UAClock"
    RUN="/run/beamOn 1000000"
    CFL="$WRL $COIN $PHY $UAS $ULI $GENEL $RUN"
    for(( i=1; i<30; i++ )) ; do
	time jgamos --norun --jobs $i --dir oo${i} $CFL
	echo "iiii " $i
    done

fi
