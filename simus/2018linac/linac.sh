#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

# physics lists
INI="/run/initialize"
PHY1="/gamos/physicsList GmEMPhysics          $INI"
PHY2="/gamos/physicsList GmEMExtendedPhysics  $INI"
PHYg="/gamos/physicsList HadrontherapyPhysics $INI"
PHYd="/gamos/physicsList GmDNAPhysics         $INI"
PHYl="/gamos/physicsList GmLeptsPhysics       $INI"

# materials
#WAT=G4_WATER
WAT=G4_WATER_VAPOR
MATS=":MATE vacuum 1 1*g/mole 1e-25*g/cm3 
      :MIXT_BY_NATOMS $WAT 1*g/cm3 2  O 1 H 2
      :MIXT_BY_NATOMS G4_AIR 1.2*mg/cm3 2  O 1 H 2"

# world
WRL="$MATS
     :VOLU world BOX 1.1*m 1.1*m 1.1*m vacuum 
     //:COLOUR world 1 1 1 
     :ROTM rm0  0  0 0 
     :ROTM rmx 90  0 0 
     :ROTM rmy  0 90 0
     :ROTM rmz  0  0 90"

# 1m3 liquid water cube
CUBE=":VOLU   cube BOX 0.5*m 0.5*m 0.5*m $WAT
      :PLACE  cube 1 world rm0 0.5*m 0 0
      :COLOUR cube 0.2 0.2 0.5
      #/gamos/geometry/createRegion cubeRegion cube"

# 2mm lead slab
SLAB=":VOLU   slab BOX 1*mm 0.5*m 0.5*m G4_Pb
      :PLACE  slab 1 world rm0 -15*cm 0 0
      :COLOUR slab 1 1 0"

# Vendor spectrum generator
GENVDR=":VOLU gbox BOX 5*cm 5*cm 5*cm vacuum 
        :PLACE gbox 1 world rmz -70*cm 0 0 
        :COLOUR gbox 1 0 0
        /gamos/generator GmGenerator 
        /gamos/generator/addSingleParticleSource gn gamma 1*MeV 
        /gamos/generator/positionDist gn GmGenerDistPositionInG4Volumes gbox 
        /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0 
        /gamos/generator/energyDist gn GmGenerDistEnergyFromFile
        photonSpec5p9MeV.txt interpolate 1*MeV"

# Varian EGS phase space generator
GENVAR="#/gamos/setParam RTGeneratorPhaseSpace_EGS:MaxNReuse 5 
        #/gamos/setParam RTGeneratorPhaseSpace_EGS:MirrorWhenReuse X 
        #/gamos/setParam RTGeneratorPhaseSpace_EGS:MaxNRecycle 1 
        /gamos/setParam  RTGeneratorPhaseSpace_EGS:InitialDisplacement -45*cm 0 0 
        #/gamos/setParam RTGeneratorPhaseSpace:Histos 1 
        /gamos/setParam  RTGeneratorPhaseSpace_EGS:InitialRotAngles 0 90*deg 0 
	#/gamos/setParam  RTGeneratorPhaseSpace_EGS:FileName //${HOME}/varian/varian10x10.egsphsp1
        /gamos/setParam  RTGeneratorPhaseSpace_EGS:FileName //${HOME}/varian/varian.egsphsp1
        /gamos/generator RTGeneratorPhaseSpace_EGS"

# vrml vis macros
VIS="/vis/scene/create 
     /vis/open VRML2FILE 
     /vis/viewer/flush 
     /tracking/storeTrajectory 1 
     /vis/scene/add/trajectories 
     /vis/scene/endOfEventAction accumulate 
     /vis/viewer/update 
     /vis/viewer/flush"

# Multifuncional detector
MFDT="/gamos/scoring/createMFDetector theDet       theTarget
      /gamos/scoring/addScorer2MFD    theEnergyDep GmG4PSEnergyDeposit theDet 1
      /gamos/scoring/addScorer2MFD    theDosis     GmG4PSDoseDeposit   theDet 2
      /gamos/scoring/addScorer2MFD    theNProc     GmG4PSNofStep       theDet 3
      /gamos/classifier               theCl        GmClassifierByProcess
      /gamos/scoring/assignClassifier2Scorer theCl theNProc
      /gamos/scoring/printer cout1 GmPSPrinterCout
      /gamos/scoring/printer cout2 GmPSPrinterCout
      /gamos/scoring/printer cout3 GmPSPrinterCout
      /gamos/scoring/addPrinter2Scorer cout1 theEnergyDep
      /gamos/scoring/addPrinter2Scorer cout2 theDosis
      /gamos/scoring/addPrinter2Scorer cout3 theNProc
      /gamos/scoring/printByEvent cout1 FALSE
      /gamos/scoring/printByEvent cout2 FALSE
      /gamos/scoring/printByEvent cout3 FALSE"

# Volume scorer
SCOR="/gamos/setParam   UAScoreVolume:TargetName theTarget
      /gamos/setParam   UAScoreVolume:SourceActivity 1.955*Ci/1000
      /gamos/userAction UAScoreVolume"

# Target volume
TGV="/gamos/setParam   UATargetVolume:TargetName theTarget
     /gamos/setParam   UATargetVolume:NoPhotInside 1000000
     /gamos/userAction UATargetVolume"
 
UAS="#/gamos/userAction UAVerbose
     #/gamos/userAction UAWIF
     /gamos/userAction GmCountProcessesUA
     /gamos/userAction UAClock /gamos/setParam TimeMark 3600
     /gamos/userAction UAInteraction
     #/gamos/userAction UAInteractionSp
     /gamos/setParam SPhg:Title NoInts
     /gamos/userAction UADepo"

# Kill e- & e+ in cube
KEP="/gamos/physics/userLimits/setMinEKin ulie cube e- 9*MeV
     /gamos/physics/userLimits/setMinEKin ulip cube e+ 9*MeV 
     /gamos/physics/userLimits/print"


targetpoints=("-394.7   0    4.3"
	      "-394.7   8.6  4.3"
	      "-394.7  17.2  4.3"
	      "-394.7  25.8  4.3"
	      "-394.7  34.4  4.3"
	      "-394.7  43    4.3"
	      "-394.7  51.6  4.3"
	      "-394.7  60.2  4.3"
	      "-394.7  68.8  4.3"
	      "-394.7  77.4  4.3"
	      "-394.7  86    4.3"
	      "-394.7  94.6  4.3"
	      "-394.7 103.2  4.3"
	      "-394.7 111.8  4.3"
	      "-394.7 120.4  4.3"
	     )


# Visualize geometry
if [ $1 == "vis1" ] ; then
    targetpoints=("0 0 0" "-450 200 0" "-450 0 0" )

    for (( i=0; i<${#targetpoints[@]}; i++ )) ; do
	CYLS="$CYLS
	      :VOLU   cover${i}  TUBE 0 4*cm 6*cm G4_POLYSTYRENE
              :PLACE  cover${i}  1 cube rmy ${targetpoints[i]}
              :COLOUR cover${i}  1 0 0
              :VOLU   pad${i}    TUBE 0 3*cm 1.7*cm G4_AIR
              :PLACE  pad${i}    1 cover${i} rm0 0 0 3.3*cm
              :COLOUR pad${i}    0 1 0
              :VOLU   target${i} TUBE 0 3*cm 3.3*cm $WAT
              :PLACE  target${i} 1 cover${i} rm0 0 0 -1.7*cm
              :COLOUR target${i} 0 0 1"
    done
    jgamos --dir oovis1 $WRL $CUBE $SLAB $CYLS $PHYl $VIS

    

elif [ $1 == "vis" ] ; then
    for (( i=0; i<${#targetpoints[@]}; i++ )) ; do
	CYLS="$CYLS
	      :VOLU   cover${i}  TUBE 0 4*mm 6*mm G4_POLYSTYRENE
              :PLACE  cover${i}  1 cube rmy ${targetpoints[i]}
              :COLOUR cover${i}  1 0 0
              :VOLU   pad${i}    TUBE 0 3*mm 1.7*mm G4_AIR
              :PLACE  pad${i}    1 cover${i} rm0 0 0 3.3*mm
              :COLOUR pad${i}    0 1 0
              :VOLU   target${i} TUBE 0 3*mm 3.3*mm $WAT
              :PLACE  target${i} 1 cover${i} rm0 0 0 -1.7*mm
              :COLOUR target${i} 0 0 1"	
    done

    ULI="/gamos/physics/userLimits/setMinEKin ulicube cube e- 10*MeV"
    RUN="/run/beamOn 50"
    jgamos --dir oovis $WRL $CUBE $CYLS $PHYl $GENVAR $VIS $ULI $RUN



elif [ $1 == "visw" ] ; then
    ULI="/gamos/physics/userLimits/setMinEKin ulicube cube e- 10*MeV"
    UAS="/gamos/userAction UAWIF"
    RUN="/run/beamOn 100000"
    jgamos --dir oovisw $WRL $CUBE $PHYl $GENVAR $ULI $UAS $RUN


    
elif [ $1 == "viszone" ] ; then
    WRL="$MATS
         :VOLU world BOX 1*cm 1*cm 1*cm vacuum 
         :ROTM rm0  0  0 0 
         :ROTM rmx 90  0 0 
         :ROTM rmy  0 90 0
         :ROTM rmz  0  0 90"
    MINICUBE=":VOLU   cube BOX 50*um 50*um 50*um $WAT
      	     :PLACE  cube 1 world rm0 1*mm 0.1*mm 0
             :COLOUR cube 0.2 0.2 0.5"
    MINIGENVDR=":VOLU gbox BOX 0.1*mm 0.1*mm 0.1*mm vacuum 
                :PLACE gbox 1 world rmz -1*mm 0 0 
                :COLOUR gbox 1 0 0
                /gamos/generator GmGenerator 
                /gamos/generator/addSingleParticleSource gn gamma 1*MeV 
                /gamos/generator/positionDist gn GmGenerDistPositionInG4Volumes gbox 
                /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0 
                /gamos/generator/energyDist gn GmGenerDistEnergyFromFile photonSpec5p9MeV.txt interpolate 1*MeV"
    #ULI="/gamos/physics/userLimits/setMinEKin ulicube cube e- 1*keV"
    UAS="/gamos/userAction UAInteraction
         /gamos/userAction UAWIF
         /gamos/userAction UAClock"
    RUN="$VIS /run/beamOn 100"
    #RUN="/run/beamOn 10000000"
    jgamos --dir oovisz $WRL $MINICUBE $PHYl $MINIGENVDR $ULI $UAS $RUN


    
# Test generators
elif [ $1 == "gener" ] ; then
    UAS="/gamos/setParam UAExit:EnergyMax 7*MeV
         /gamos/setParam UAExit:EnergyBins 700
         /gamos/userAction UAExit
         /gamos/analysis/histo1Max *Ener* 7*MeV
         /gamos/userAction GmGenerHistosUA
         /gamos/userAction UAClock"
    RUN="/run/beamOn 4000000"
    jgamos --dir oog1 $WRL $PHYl $UAS $GENVDR $RUN &
    jgamos --dir oog2 $WRL $PHYl $UAS $GENVAR $RUN &
    wait


    
# Analyse DXS / MomTransfer
elif [ $1 == "dxs" ] ; then
    UAS="/gamos/userAction UAClock
         /gamos/userAction UAInteraction"
    NEWM=G4_WATER
    NEWL=":MIXT_BY_NATOMS $NEWM 0.1*g/cm3 2  O 1 H 2
          :VOLU slab BOX 0.01*nm 0.5*m 0.5*m $NEWM
          :PLACE  slab 1 world rm0 0.5*m 0 0"
    GEN1=$(gen e-  9999*eV 5*cm -80*cm)
    GEN2=$(gen e- 10100*eV 5*cm -80*cm)
    #ULI1="/gamos/physics/userLimits/setMinEKin ulie slab e-  9998.9*eV"
    #ULI2="/gamos/physics/userLimits/setMinEKin ulie slab e- 10099.9*eV"

    RUN="/run/beamOn 10000000"
    PAR="--jobs 10 --ppn 10"
    #jgamos --dir oodxs1 $PAR $WRL $NEWL $PHYl $UAS $GEN1 $ULI1 $RUN &
    jgamos --dir oodxs2 $PAR $WRL $NEWL $PHYl $UAS $GEN2 $ULI2 $RUN &
    wait



# Jets lepts / lepts old / G4-DNA
elif [ $1 == "jets" ] ; then
    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction UAWIF
         /gamos/userAction UAClock
         /gamos/userAction UAInteraction"

    WL=G4_WATER
    WLO=G4_WATER_OLD
    
    CHB=":MIXT_BY_NATOMS $WL 1*g/cm3 2  O 1 H 2
         :VOLU  chb BOX 0.5*m 0.5*m 0.5*m $WL
         :PLACE chb 1 world rm0 0.5*m 0 0"
    CHBO=":MIXT_BY_NATOMS $WLO 1*g/cm3 2  O 1 H 2
         :VOLU  chb BOX 0.5*m 0.5*m 0.5*m $WLO
         :PLACE chb 1 world rm0 0.5*m 0 0"

    GEN=$(gen e- 10*keV 1*nm -20*cm)
    RUN="/run/beamOn 50"

    jgamos --dir oolo --seed 3 $PAR $WRL $CHBO $PHYl $UAS $ISP $GEN $ULI $RUN &
    jgamos --dir ooln --seed 3 $PAR $WRL $CHB  $PHYl $UAS $ISP $GEN $ULI $RUN &
    jgamos --dir ood  --seed 3 $PAR $WRL $CHB  $PHYd $UAS $ISP $GEN $ULI $RUN &
    wait    



# Stoping power
elif [ $1 == "stp" ] ; then
    UAS="#/gamos/userAction UAVerbose
         #/gamos/userAction UAWIF
         /gamos/userAction UAClock
         /gamos/userAction UAInteraction"

    WL=G4_WATER
    WV=G4_WATER_VAPOR
    DX=0.01/2.0*nm
    CHBL=":MIXT_BY_NATOMS $WL 1*g/cm3 2  O 1 H 2
          :VOLU  chb BOX $DX 0.5*m 0.5*m $WL
          :PLACE chb 1 world rm0 0.5*m 0 0"
    CHBV=":MIXT_BY_NATOMS $WV 1*g/cm3 2  O 1 H 2
          :VOLU  chb BOX $DX 0.5*m 0.5*m $WV
          :PLACE chb 1 world rm0 0.5*m 0 0"
    RUN="/run/beamOn 1000000000"
    #PAR="--jobs 10 --ppn 10"
    PAR="--host euler --ppn 2 --jobs 2 --btime 24:00:00"
    for E in 1 3 5 10 30 100 300 1000 5000 6000 ; do
	GEN=$(gen e- ${E}*keV 1*um -20*cm)
	jgamos --dir oodl${E} $PAR $WRL $CHBL $PHYd $UAS $GEN $ULI $RUN 
	jgamos --dir oogv${E} $PAR $WRL $CHBV $PHYg $UAS $GEN $ULI $RUN 
	jgamos --dir oolv${E} $PAR $WRL $CHBV $PHYl $UAS $GEN $ULI $RUN 
    done
    wait



#Edep versus depth
elif [ $1 == "dedx" ] ; then
    UAS="#/gamos/userAction UAVerbose
         #/gamos/userAction UAWIF
         #/gamos/userAction UAInteraction
         /gamos/userAction UAInteractionSp
         /gamos/userAction UAClock"

    WV=G4_WATER_VAPOR
    WL=G4_WATER
    
    CHBV=":MIXT_BY_NATOMS $WV 1*g/cm3 2  O 1 H 2
          :VOLU  chb BOX 0.5*m 0.5*m 0.5*m $WV
          :PLACE chb 1 world rm0 0.5*m 0 0"
    CHBL=":MIXT_BY_NATOMS $WL 1*g/cm3 2  O 1 H 2
          :VOLU  chb BOX 0.5*m 0.5*m 0.5*m $WL
          :PLACE chb 1 world rm0 0.5*m 0 0"

    #RUN="/run/beamOn 1000"
    #E=( 1    3  10 30 100  300  1000  5000  6000 ) #Incident keV
    #D=( 0.15 1  10 30 500 3000 10000 60000 80000 ) #depth microns

    RUN="/run/beamOn 10000"
    E=(0.01 0.1  1    3  10 ) #Incident keV
    D=(0.01 0.1  0.15 1  10 ) #depth microns

    #for (( i=1; i<2; i++ )) ; do
    for (( i=0; i<${#E[@]}; i++ )) ; do
	GEN=$(gen e- ${E[i]}*keV 1*nm -20*cm)
	ULI="/gamos/physics/userLimits/setMinEKin ulie chb e- ${E[i]}*keV*0.01"
	ISP="/gamos/setParam UAInteractionSp:Title Edep-Depth
             /gamos/setParam UAInteractionSp:x 100 0 ${D[i]}*um
             /gamos/userAction UAInteractionSp"

	jgamos --dir oog${E[i]} $PAR $WRL $CHBV $PHYg $UAS $ISP $GEN $ULI $RUN'000' &
	jgamos --dir ool${E[i]} $PAR $WRL $CHBV $PHYl $UAS $ISP $GEN $ULI $RUN &
	jgamos --dir ood${E[i]} $PAR $WRL $CHBL $PHYd $UAS $ISP $GEN $ULI $RUN &
    done
    wait

    

# Lead Slab
elif [ $1 == "atenu_Pb" ] ; then
    UAS="/gamos/userAction UAExit
         /gamos/setParam UAExit:EnergyMax 10*MeV
         /gamos/setParam UAExit:EnergyBins 1000
         /gamos/userAction UAClock
         /gamos/setParam TimeMark 3600"
    #RUN="$VIS /run/beamOn 100"
    RUN="/run/beamOn 10000000"
    PLA=$(chb G4_Pb 0.5*m 0.5*m 1*cm 0.5*m)
    ULI="$(uli G4_Pb e- 9*MeV) $(uli G4_Pb e+ 9*MeV)"

    for dx in 000 001 002 004 006 010 020 040 060 100 200 400 600 ; do
	PLA=$(chb G4_Pb 0.5*m 0.5*m ${dx}'*mm' 0.5*m)
	if [ $dx == 000 ] ; then
	    CFL="$WRL       $PHY1 $UAS      $GENVDR $RUN"
	else
	    CFL="$WRL  $PLA $PHY1 $UAS $ULI $GENVDR $RUN"
	fi

	jgamos $CFL /dir oat /jn oat${dx} /batch 08:00:00
	#scp -r oat euler:batch
	#ssh euler 'cd batch/oat; for f in *sh ; do qsub $f; done'
    done
    exit
    for f in *root ; do
	root -b -p -q .x ${LEPTSDIR}/macros/tabhgs.cc\(\"${f}\"\) >> tabhgs.txt
    done


elif [ $1 == "seco" ] ; then
    SECO="/gamos/analysis/histo1Max *Ener* 6*MeV
          /gamos/analysis/histo1NBins *Ener* 100
          #seco
          /gamos/setParam   GmSecondaryTrackDataHistosUA:FileName seco
          /gamos/setParam   GmSecondaryTrackDataHistosUA:DataList SecoKineticEnergy
          /gamos/userAction GmSecondaryTrackDataHistosUA
          #only_seco_e-
          /gamos/filter secoF GmOnSecondaryFilter GmElectronFilter
          /gamos/setParam GmSecondaryTrackDataHistosUA_secoF:FileName secoel
          /gamos/setParam GmSecondaryTrackDataHistosUA_secoF:DataList SecoKineticEnergy
          /gamos/userAction GmSecondaryTrackDataHistosUA secoF"
    UAS="/gamos/userAction UAInteraction
         /gamos/userAction UAClock"
    ULI="/gamos/physics/userLimits/setMinEKin ulicube cube e- 10*MeV"
    
    RUN="/run/beamOn 100000"
    #PAR="--jobs 10 --ppn 10"
    jgamos --dir oosec${i} $PAR $WRL $CUBE $PHYl $GENVDR $UAS $SECO $ULI $RUN


# e- approaching small target
elif [ $1 == "appsmall" ] ; then
    UAS="/gamos/userAction UAClock
         /gamos/userAction UAWIF"
    TRG=":VOLU   theTarget TUBE 0 10*um 30*um $WAT 
         :PLACE  theTarget 1 cube rm0 -499.988 0 0
         :COLOUR theTarget 0 1 0"
    GENe=$(gen e- 10*keV 5*um 0)
    RUN="/run/beamOn 10"
    jgamos --dir ooap  $WRL $CUBE $TRG $PHYl $GENe $TGV $MFDT $SCOR $UAS $RUN


	
# e- approaching 200um target
elif [ $1 == "app200um" ] ; then
    UAS="/gamos/userAction UAClock
         /gamos/userAction UAWIF
         /gamos/userAction UAInteraction"
    GENe=$(gen e- 200*keV 0.2*mm 0)
    RUN="/run/beamOn 10"
    TRG=":VOLU   theTarget TUBE 0 0.1*mm 0.1*mm $WAT
    	 :PLACE  theTarget 1 cube rm0 -499.6 0 0
         :COLOUR theTarget 0 1 0"
    jgamos --dir ooap  $WRL $CUBE $TRG $PHYl $GENe $TGV $MFDT $SCOR $UAS $RUN &
    jgamos --dir ooap2 $WRL $CUBE $TRG $PHYl $GENe      $MFDT $SCOR $UAS $RUN &
    wait


    
# Adding polistyrene cover
elif [ $1 == "addcover" ] ; then
    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction UAClock
         /gamos/userAction UAWIF
         /gamos/userAction UAInteraction"
    TRG1=":VOLU   theTarget TUBE 0 3*mm 4*mm $WAT
          :PLACE  theTarget 1 cube rmy -494 0 0
          :COLOUR theTarget 0 1 0"
    TRG2=":VOLU   theCover  TUBE 0 4*mm 5*mm G4_POLYSTYRENE
          :PLACE  theCover  1 cube rmy -494 0 0
          :COLOUR theCover  1 0 0
          :VOLU   theTarget TUBE 0 3*mm 4*mm $WAT 
          :PLACE  theTarget 1 theCover rm0 0 0 0
          :COLOUR theTarget 0 1 0"
    RUN="/run/beamOn 1"
    GENe=$(gen e- 1*MeV 1*nm 0)
    #jgamos --dir ooc1 $WRL $CUBE $TRG1 $PHYl $GENe $TGV $MFDT $SCOR $UAS $RUN &
    #jgamos --dir ooc2 $WRL $CUBE $TRG2 $PHYl $GENe $TGV $MFDT $SCOR $UAS $RUN &

    RUN="/run/beamOn 500000"
    jgamos --dir ooc3 $WRL $CUBE $TRG1 $PHYl $GENVDR $TGV $MFDT $SCOR $UAS $RUN &
    jgamos --dir ooc4 $WRL $CUBE $TRG2 $PHYl $GENVDR $TGV $MFDT $SCOR $UAS $RUN &
    wait
    

    
elif [ $1 == "simu" ] ; then
    for (( i=0; i<${#targetpoints[@]}; i++ )) ; do
    #for (( i=0; i<1; i++ )) ; do
	ii=$(printf "%02d" $i)

	CYL=":VOLU   cover  TUBE 0 4*mm 6.3*mm G4_POLYSTYRENE
             :PLACE  cover  1 cube rmy ${targetpoints[i]}
             :COLOUR cover  1 0 0
             :VOLU   pad        TUBE 0 3*mm 3.53/2.0*mm G4_AIR
             :PLACE  pad        1 cover rm0 0 0 3.535*mm
             :COLOUR pad        0 1 0
             :VOLU   theTarget  TUBE 0 3*mm 7.07/2.0*mm $WAT
             :PLACE  theTarget  1 cover rm0 0 0 -1.765*mm
             :COLOUR theTarget  0 0 1"

	#KILL="/gamos/setParam UAClock:TimeLimit 3600*4 /gamos/userAction UAClock"
	KILL="/gamos/setParam UAClock:TimeLimit 3600*20 /gamos/userAction UAClock"
	UAS="/gamos/userAction GmCountProcessesUA"

	#RUN="$VIS /run/beamOn 100"
	RUN="/run/beamOn 1000000000"
	EULER="--ppn 8  --jpn 8  --host euler"
	DIRAC="--ppn 12 --jpn 12 --host dirac"
	CETA="--ppn 8  --jpn 8  --host amunoz@193.144.240.176"
	PRU="--jobs 1 --jpn 1"

	# 1000 (10), 1010 (10), 1020 (10), 2000 (40), 2040 (40)
	s=2040
	JOB="$EULER --jobs 40 --btime 20:05:00 --seed $s --SEED $s --dir oo_p${ii}_s${s}"
	
	jgamos  $JOB $WRL $CUBE $CYL $PHYl $GENVAR $TGV $SCOR $UAS $KILL $RUN
	#jgamos  $JOB $WRL $CUBE $CYL $PHYl $GENVDR $TGV $SCOR $UAS $KILL $RUN
    done
    wait

    
fi

exit



for i in 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 ; do
    myhadd.sh all${i}.root oo_p${i}_s*/UAScoreVol*root
done

for f in *root ; do
    hgm.sh tabh $f
done

cat all*Pri*  > tab.csv; echo ',' >> tab.csv
cat all*Err* >> tab.csv; echo ',' >> tab.csv
head -1 tab.csv      > tab2.csv
grep -v '#' tab.csv >> tab2.csv




electrones de alta energía en agua líquida
excel con los datos de entrada tal como los utiliza Rafa en el planificador de dosis para el LINAC
trabajando directamente con electrones

Las columnas más importantes son la de electrones totales (incluye los dispersados en los colimadores) y la de fotones que se generan por colisión (frenado) con los colimadores.

La columna de ángulos está un poco confusa tengo todavía que pensar cómo incluirla pero de momento con los espectros de electrones y fotones vamos bien.

empezamos con la física Livermore hasta llegar a los 10 keV que metemos la nuestra.

Hay que incluir la ionización múltiple de Champion  que nos permitió resolver la discrepancia del poder de frenado con el NIST.

La superficie de 10x10 cm está en la superficie del agua.



Vamos a hacer la simulación para dos de los espectros y ángulos de entrada  correspondientes  a 5 MV y 6MV. Es un fichero excel con varias hojas, sólo nos fijamos en los espectros de 5 y 6 MeV junto con sus datos de dosis en profundidad.

Primero simulamos la dosis en función de la profundidad para los  puntos dados, en los que suponemos un volumen (esfera) pequeño a lo largo del eje de penetración (lo más pequeño que te permita la estadística) y después el número y tipo de procesos en cada lámina. Como siempre, nos interesa una buena estadística para ver detalles al final del alcance.

También nos interesa obtener un buen dibujo de trayectorias.
