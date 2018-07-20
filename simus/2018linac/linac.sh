#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

# physics lists
INI="/run/initialize"
PHY1="/gamos/physicsList GmEMPhysics          $INI"
PHY2="/gamos/physicsList GmEMExtendedPhysics  $INI"
PHYd="/gamos/physicsList GmDNAPhysics         $INI"
PHYl="/gamos/physicsList GmLeptsPhysics       $INI"
PHYg="/gamos/physicsList HadrontherapyPhysics $INI"

       
# materials
WAT=G4_WATER
#WAT=G4_WATER_VAPOR
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
      :COLOUR cube 0.1 0.1 0.9
      #/gamos/geometry/createRegion cubeRegion cube"

# 2mm lead slab
SLAB=":VOLU   slab BOX 1*mm 0.5*m 0.5*m G4_Pb
      :PLACE  slab 1 world rm0 -15*cm 0 0
      :COLOUR slab 1 1 0"

# Vendor spectrum generator
GENVDR=":VOLU gbox BOX 5*cm 5*cm 5*cm vacuum 
        :PLACE gbox 1 world rmz -5*cm 0 0 
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

# Rafa's generators: genrafa e- EgyFile AngFile
genrafa() {
    GBOX=":VOLU gbox BOX 5*cm 1*mm 5*cm vacuum 
          :PLACE gbox 1 world rmz -2*mm 0 0 
          :COLOUR gbox 1 0 0"
    GEN="/gamos/generator GmGenerator 
         /gamos/generator/addSingleParticleSource gn $1 1*MeV 
         /gamos/generator/positionDist gn GmGenerDistPositionInG4Volumes gbox 
         /gamos/generator/energyDist gn GmGenerDistEnergyFromFile $2 interpolate 1*MeV
         #/gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0
         /gamos/generator/directionDist gn GmGenerDistDirectionThetaFromFile $3 interpolate 1 0 0 1*degree"
    echo $GBOX $GEN
}
GENph5=$(genrafa gamma linac_ph5MeV_egy.txt linac_ph5MeV_ang.txt)
GENph6=$(genrafa gamma linac_ph6MeV_egy.txt linac_ph6MeV_ang.txt)
GENel5=$(genrafa e-    linac_e5MeV_egy.txt  linac_e5MeV_ang.txt)
GENel6=$(genrafa e-    linac_e6MeV_egy.txt  linac_e6MeV_ang.txt)
GENel4=$(genrafa e-    linac_e4MeV_egy.txt  linac_e6MeV_ang.txt)

# Multifunctional detector
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

# vrml vis macros
VIS="/vis/scene/create 
     /vis/open VRML2FILE 
     /vis/viewer/flush 
     /tracking/storeTrajectory 1 
     /vis/scene/add/trajectories 
     /vis/scene/endOfEventAction accumulate 
     /vis/viewer/update 
     /vis/viewer/flush"

# pocillos
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
if [ $1 == "vis" ] ; then
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



# Vis geom esferitas
elif [ $1 == "visorb" ] ; then
    tps=( 2 4 8 10 15 20 30 40 50 60)
    for (( i=0; i<${#tps[@]}; i++ )) ; do
	(( x = ${tps[i]} -500 ))
	SPHS="$SPHS
	      :VOLU   target${i} ORB 5*mm $WAT
              :PLACE  target${i} 1 cube rm0 $x 0 0
              :COLOUR target${i} 1 0 0"
    done

    ULI="/gamos/physics/userLimits/setMinEKin ulicube cube e- 10*MeV"
    #ULI="/gamos/userAction GmKillAtStackingActionUA GmElectronFilter"
    RUN="/run/beamOn 10"
    jgamos --dir oovisorb $WRL $CUBE $SPHS $PHYl $GENVDR $VIS $ULI $RUN


# Vis generator   
elif [ $1 == "visgen" ] ; then
    jgamos --dir oovigen  $WRL $CUBE $PHYg $GENph5 $VIS $RUN /run/beamOn 10
    #jgamos --dir oovigen2 $WRL $CUBE $PHYl $GENel5 $VIS $RUN /run/beamOn 10

    
#Vis trajectories complete / eco 
elif [ $1 == "vistraj" ] ; then
    GEN=":VOLU gbox BOX 1*um 1*um 1*um vacuum
         :PLACE gbox 1 world rmz -1*um 0 0 
         :COLOUR gbox 0 0 1 
         /gamos/generator GmGenerator /gamos/generator/addSingleParticleSource gn e- 1*MeV
         /gamos/generator/positionDist gn GmGenerDistPositionInG4Volumes gbox
         /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0"
    SPH=":VOLU   theTarget ORB 2*mm $WAT
         :PLACE  theTarget 1 cube rm0 -494 0 0
         :COLOUR theTarget 1 0 0"
    UAS="/gamos/userAction UAWIF
         #/gamos/userAction UAVerbose
         #/gamos/userAction UAInteraction"
    RUN="/run/beamOn 1"
    #jgamos --dir oovtj  $WRL $PHYl $GEN $CUBE                       $UAS $RUN &
    jgamos --dir oovtj2 $WRL $PHYl $GEN $CUBE $SPH $TGV $MFDT $SCOR $UAS $RUN &
    wait


 
# Vis 
elif [ $1 == "visw" ] ; then
    ULI="/gamos/physics/userLimits/setMinEKin ulicube cube e- 10*MeV"
    UAS="/gamos/userAction UAWIF"
    RUN="/run/beamOn 1000"
    jgamos --dir oovisw $WRL $CUBE $PHYl $GENph5 $ULI $UAS $RUN


    
# Test generators
elif [ $1 == "gener" ] ; then
    UAS="/gamos/setParam UAExit:EnergyMax 7*MeV
         /gamos/setParam UAExit:EnergyBins 700
         /gamos/userAction UAExit
         /gamos/analysis/histo1Max *Ener* 7*MeV
         /gamos/userAction GmGenerHistosUA
         /gamos/userAction UAClock"
    RUN="/run/beamOn 1000000"
    #jgamos --dir oog1 $WRL $PHYl $UAS $GENVDR $RUN &
    #jgamos --dir oog2 $WRL $PHYl $UAS $GENVAR /run/beamOn 4000000 &
    jgamos --dir ooph5 $WRL $PHYl $UAS $GENph5 $RUN &
    jgamos --dir ooel5 $WRL $PHYl $UAS $GENel5 $RUN &
    jgamos --dir ooph6 $WRL $PHYl $UAS $GENph6 $RUN &
    jgamos --dir ooel6 $WRL $PHYl $UAS $GENel6 $RUN &
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
    RUN="/run/beamOn 100000000"
    #PAR="--jobs 10 --ppn 10"
    #PAR="--host euler --ppn 2 --jobs 2 --btime 24:00:00"
    for E in 1 3 10 30 100 300 1000 5000 ; do
	GEN=$(gen e- ${E}*keV 1*um -20*cm)
	jgamos --dir ood${E} $PAR $WRL $CHBL $PHYd $UAS $GEN $ULI $RUN &
	jgamos --dir oog${E} $PAR $WRL $CHBV $PHYg $UAS $GEN $ULI $RUN &
	jgamos --dir ool${E} $PAR $WRL $CHBV $PHYl $UAS $GEN $ULI $RUN &
    done
    wait



#Edep versus depth
elif [ $1 == "dedx" ] ; then
    UAS="#/gamos/userAction UAVerbose
         #/gamos/userAction UAWIF
         #/gamos/userAction UAInteraction
         #/gamos/userAction UAInteractionSp
         /gamos/userAction UAClock"

    E=(10  100  1000  2000  4000) #Incident keV
    D=(10  300 10000 20000 40000) #depth microns
    E=( 999)
    D=(10000)
    E=(5000)
    D=(50000)
    ULI="/gamos/userAction GmKillAtStackingActionUA GmSecondaryFilter
         /gamos/physics/userLimits/setMinEKin ulie cube e- 10*keV
         /gamos/physics/userLimits/setMinEKin ulip cube e+ 10*keV"

    RUN="/run/beamOn 100"

    for (( i=0; i<${#E[@]}; i++ )) ; do
	GEN=$(gen e- ${E[i]}*keV 1*nm -20*cm)
	ISP="/gamos/setParam UAInteractionSp:Title Edep-Depth
             /gamos/setParam UAInteractionSp:x 100 0 ${D[i]}*um
             #/gamos/setParam UAInteractionSp:Width 5*cm
             /gamos/userAction UAInteractionSp"

	jgamos --dir oog${E[i]} $PAR $WRL $CUBE $PHYg $UAS $ISP $GEN $ULI $RUN'000' &
	jgamos --dir ool${E[i]} $PAR $WRL $CUBE $PHYl $UAS $ISP $GEN $ULI $RUN &
	#jgamos --dir ood${E[i]} $PAR $WRL $CUBE $PHYd $UAS $ISP $GEN $ULI $RUN &

	#PAR="--host dirac --ppn 12 --jobs 100 --btime 4:00:00"
	#RUN="/run/beamOn 10"
	#jgamos --dir bbood${E[i]} $PAR $WRL $CHB $PHYd $UAS $ISP $GEN $ULI $RUN &
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
    

    
elif [ $1 == "simupocillos" ] ; then
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



elif [ $1 == "simu1" ] ; then
    # centros esferitas #tps=( 02 04 08 10 15 20 30 40 50 60 )
    tps=( 02.25 05.25 08.25 11.30 14.30 20.30 29.30 41.30 50.30 59.30 )
    rds=( 1     1     1     2     2     2     4     4     4     4 )

    for (( i=0; i<${#tps[@]}; i++ )) ; do
	#ii=$(printf "%02d" $i)
	#(( x = ${tps[i]} -500 ))
	x=$(bc -l <<< "${tps[i]} -500")
	SPH=":VOLU   theTarget ORB ${rds[i]}*mm $WAT
             :PLACE  theTarget 1 cube rm0 ${x}*mm 0 0
             :COLOUR theTarget 1 0 0"

	UAS="/gamos/userAction GmCountProcessesUA"

	#RUN="$VIS /run/beamOn 100"
	RUN="/run/beamOn 1000000000"
	#EULER="--ppn 8  --jpn 8  --host euler"
	EULER="--ppn 8  --jpn 10  --host euler"
	DIRAC="--ppn 12 --jpn 12 --host dirac"
	CETA="--ppn 8  --jpn 8  --host amunoz@193.144.240.176"
	PRU="--jobs 1 --jpn 1"

	s=1000
	KILL="/gamos/setParam UAClock:TimeLimit 3600*20 /gamos/userAction UAClock"
	JOB="$EULER --jobs 10 --btime 20:09:00 --seed $s --SEED $s"
	JOB=
	DIRe="--dir oo_el5_p${tps[i]}_s${s}"
	DIRp="--dir oo_ph5_p${tps[i]}_s${s}"
	#DIReg4="--dir oo_g4el5_p${tps[i]}_s${s}"
	#jgamos $JOB $DIReg4 $WRL $CUBE $SPH $PHYg $GENel5 $TGV $SCOR $UAS $KILL $RUN
	jgamos $JOB $DIRe $WRL $CUBE $SPH $PHYl $GENel5 $TGV $SCOR $UAS $KILL $RUN
	jgamos $JOB $DIRp $WRL $CUBE $SPH $PHYl $GENph5 $TGV $SCOR $UAS $KILL $RUN
    done
    wait


    
elif [ $1 == "simu" ] ; then
    UAS="#/gamos/userAction UAVerbose
         #/gamos/userAction UAWIF
         /gamos/userAction UAClock
         /gamos/userAction UAInteraction"


    ISP="/gamos/setParam UAInteractionSp:Title Edep-Depth
         /gamos/setParam UAInteractionSp:x 50 0 50*mm
         /gamos/setParam UAInteractionSp:Width 1*cm
         /gamos/userAction UAInteractionSp"

    EULER="--ppn 8 --jpn 10 --host euler"
    DIRAC="--ppn 12 --jpn 15 --host dirac"
    #JOB="$EULER --jobs 30 --btime 4:00:00"
    JOB="$DIRAC --jobs 30 --btime 4:00:00"
    KILL="/gamos/setParam UAClock:TimeLimit 3600*71 /gamos/userAction UAClock"
    RUNe="/run/beamOn 400"
    RUNp="/run/beamOn 2000"
    

    ULI="#/gamos/userAction GmKillAtStackingActionUA GmSecondaryFilter
         /gamos/physics/userLimits/setMinEKin ulie cube e- 10*keV
         /gamos/physics/userLimits/setMinEKin ulip cube e+ 10*keV"
    RUNe="/run/beamOn 5000"
    GENMONO=$(gen e- 5*MeV 1*nm -20*cm)

    jgamos --dir oolr $WRL $CUBE $PHYl $GENel5  $UAS $ISP $ULI $RUNe
exit
    jgamos --dir oogr $JOB $WRL $CUBE $PHYg $GENel5  $UAS $ISP $ULI $RUNe'000' &
    jgamos --dir oogm $JOB $WRL $CUBE $PHYg $GENMONO $UAS $ISP $ULI $RUNe'000' &
    jgamos --dir oolr $JOB $WRL $CUBE $PHYl $GENel5  $UAS $ISP $ULI $RUNe &
    jgamos --dir oolm $JOB $WRL $CUBE $PHYl $GENMONO $UAS $ISP $ULI $RUNe &
    wait
    exit



    #seeds="1000 ... 1490"
    for (( i=1500; i<2000; i+=10 )) ; do
	seeds="$seeds $i"
    done
    
    for s in $seeds ; do
	SEED="--seed $s --SEED $s"
	DIRe="--dir ooel_s${s}"
	DIRp="--dir ooph_s${s}"
	jgamos $JOB $DIRe  $WRL $CUBE $PHYl $GENel5 $UAS $ULI $KILL $RUNe
	jgamos $JOB $DIRp  $WRL $CUBE $PHYl $GENph5 $UAS $ULI $KILL $RUNp
    done

    
fi
exit




######################################
gns=( el5 g4el5 ph5 )
tps=( 02.25 05.25 08.25 11.30 14.30 20.30 29.30 41.30 50.30 59.30 )
for (( j=0; j<${#gns[@]}; j++ )) ; do
    for (( i=0; i<${#tps[@]}; i++ )) ; do
	GP=${gns[j]}_p${tps[i]}
	FR=${GP}.root
	myhadd.sh $FR oo_${GP}_s*/UAScoreVol*root
	hgm.sh tab2 $FR
    done
done

paste -d "," g4el5*Pri*csv > tab.csv
paste -d "," el5*Pri*csv  >> tab.csv
paste -d "," ph5*Pri*csv  >> tab.csv
sed 's/Nproc_//g' tab.csv | sed 's/_Lepts//g' | sed 's/\/pri//g' | \
    sed 's/\.root_PerPrimary\.csv//g' | sed 's/_p/ /g' > tab2.csv
grep -v _Mass tab2.csv | grep -v Charge |grep -v Dose |grep -v Transport > tab3.csv
######################################





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
