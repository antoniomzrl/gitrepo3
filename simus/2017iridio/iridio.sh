#!/bin/bash

#source $HOME/lepts/bin/gdefs.sh

INI="/run/initialize"
#PHY="/gamos/physicsList GmEMPhysics $INI"
PHY="/gamos/physicsList HadrontherapyPhysics $INI"
PHYl="/gamos/physicsList GmLeptsPhysics      $INI"

MATS=" :MATE vacuum 1 1*g/mole 1e-25*g/cm3 
       :MIXT_BY_NATOMS G4_WATER_VAPOR 1*g/cm3 2  O 1 H 2
       :MATE iridium 77 192.217 22.42*g/cm3
       :MIXT steel316L 8.03*g/cm3 9
             G4_Fe  0.68745
             G4_Cr  0.19
             G4_Ni  0.0925
             G4_Mn  0.02
             G4_Si  0.0075
             G4_N   0.001
             G4_C   0.0008
             G4_P   0.00045
             G4_S   0.0003"

RMT=":ROTM rm0  0  0 0 
     :ROTM rmx 90  0 0 
     :ROTM rmy  0 90 0
     :ROTM rmz  0  0 90"

WRL="$MATS $RMT
     :VOLU world BOX 1.1*cm 1.1*cm 1.1*cm vacuum"


GIR=":VOLU   gir TUBE 0 0.6*mm/2  3.5*mm/2 iridium
     :PLACE  gir 1 world rmy 0 0 0
     :COLOUR gir 1 1 0
     /gamos/generator GmGenerator
     /gamos/generator/addSingleParticleSource gn gamma 1*keV
     /gamos/generator/positionDist  gn GmGenerDistPositionInG4Volumes gir
     /gamos/generator/energyDist    gn GmGenerDistEnergyFromFile Ir192.txt fixed 1*keV
     /gamos/generator/directionDist gn GmGenerDistDirectionRandom
     /gamos/generator/timeDist      gn GmGenerDistTimeConstant"

CAPS=":VOLU   caps TUBE 0.7*mm/2 0.9*mm/2 3.5*mm/2+0.065 steel316L
      :PLACE  caps 1 world rmy 0 0 0
      :COLOUR caps 0 1 1
      :VOLU   tapa TUBE 0 0.9*mm/2 0.2*mm/2 steel316L
      :PLACE  tapa 1 world rmy  3.5*mm/2+0.065*mm+0.2/2*mm 0 0
      :PLACE  tapa 2 world rmy -3.5*mm/2-0.065*mm-0.2/2*mm 0 0	
      :COLOUR tapa 0 1 1"


VIS="/vis/scene/create 
     /vis/open VRML2FILE 
     /vis/viewer/flush 
     /tracking/storeTrajectory 1 
     /vis/scene/add/trajectories 
     /vis/scene/endOfEventAction accumulate 
     /vis/viewer/update 
     /vis/viewer/flush"


UAS="#/gamos/userAction UAVerbose
     #/gamos/userAction UAWIF
     /gamos/userAction GmCountProcessesUA
     /gamos/userAction UAClock /gamos/setParam TimeMark 3600
     /gamos/userAction UAInteraction
     #/gamos/userAction UAInteractionSp
     /gamos/setParam SPhg:Title NoInts
     /gamos/userAction UADepo"

KEE="/gamos/physics/userLimits/setMinEKin ulie gir e- 2*MeV
     /gamos/physics/userLimits/setMinEKin ulip gir e+ 2*MeV"

#tp=(5 10 20 30 40 45 50 55 60 70 80 90 100 120 140 150 200 250 300 350 400)
tp=( 005 010 020 030 050 070 100 150 200 250 300 400 )


if [ $1 == "vis" ] ; then
    #RUN="/gamos/userAction UAWIF /run/beamOn 1000000"
    RUN="$VIS /run/beamOn 50"
    jgamos --dir oov $WRL $PHY $GIR $CAPS $KEE $RUN



elif [ $1 == "gener" ] ; then
    UAS="/gamos/setParam UAExit:EnergyMax 1400*keV
         /gamos/setParam UAExit:EnergyBins 1400
         /gamos/userAction UAExit
         /gamos/analysis/histo1Max *Ener* 1400*keV
         /gamos/analysis/histo1NBins *Ener* 1400
         /gamos/userAction GmGenerHistosUA
         /gamos/userAction UAClock"
    RUN="/run/beamOn 1000000"
    jgamos --dir oog $WRL $PHY $GIR $UAS $KEE $UAS $RUN

    

elif [ $1 == "spectrcaps" ] ; then
    CFT="/gamos/setParam GmAnalysisMgr:FileNameSuffix .phsp
         /gamos/filter exitF GmExitLogicalVolumeFilter world
         /gamos/setParam GmTrackDataTextFileUA_GmEMParticleFilter_exitF:FileName Ir192caps
         /gamos/setParam GmTrackDataTextFileUA_GmEMParticleFilter_exitF:DataList \
	    		 EventID Particle InitialPosX InitialPosY InitialPosZ \
			 InitialMomX InitialMomY InitialMomZ InitialTime InitialWeight
	 /gamos/userAction GmTrackDataTextFileUA GmEMParticleFilter exitF"

    CFB="/gamos/setParam GmAnalysisMgr:FileNameSuffix .phsp
         /gamos/filter exitF GmExitLogicalVolumeFilter world
         /gamos/setParam GmTrackDataBinFileUA_GmEMParticleFilter_exitF:FileName Ir192capsBin
         /gamos/setParam GmTrackDataBinFileUA_GmEMParticleFilter_exitF:DataList \
	 		 EventID Particle InitialPosX InitialPosY InitialPosZ \
			 InitialMomX InitialMomY InitialMomZ InitialTime InitialWeight
	 /gamos/userAction GmTrackDataBinFileUA GmEMParticleFilter exitF"
    
    LIMANG=":MATE mlimang 77 192.217 1e8*g/cm3
	    :VOLU limang  SPHERE 5*mm 6*mm 0 360*degree 45*degree 180*degree mlimang
       	    :PLACE limang 1 world rmx 0 -4*mm 0
	    :COLOUR gir 1 0 0
      	    /gamos/physics/userLimits/setMinEKin ulie limang e- 9*MeV
       	    /gamos/physics/userLimits/setMinEKin ulip limang e+ 9*MeV
       	    /gamos/physics/userLimits/setMinEKin ulig limang gamma 9*MeV"

    UAS="/gamos/userAction UAClock"
    RUN="/run/beamOn 30000000"
    #RUN="$VIS /run/beamOn 100"
    #jgamos --dir oosc1 $WRL $PHY $GIR $CAPS         $UAS $CFT $RUN
    #jgamos --dir oosc2 $WRL $PHY $GIR $CAPS $LIMANG $UAS $CFB $RUN
    jgamos --dir oosc  $WRL $PHY $GIR $CAPS $LIMANG $UAS $CFT $RUN
    
    mv oosc/Ir192caps.phsp ~/varian



elif [ $1 == "simu" ] ; then
    WRLS="$MATS $RMT
     	 :VOLU world BOX 70*cm 70*cm 70*cm vacuum
         :VOLU sphewat ORB 55*cm G4_WATER_VAPOR
         :PLACE sphewat 1 world rm0 0 0 0
	 :COLOUR sphewat 0 1 0"
    CILV=":VOLU  cilv TUBE 0 0.9*mm/2 3.5*mm/2+0.065 vacuum
          :PLACE cilv 1 sphewat rmy 0 0 0
          :COLOUR cilv 1 0 0"
    GENF="/gamos/setParam GmGeneratorFromTextFile:FileName //${HOME}/varian/Ir192caps.phsp
          /gamos/generator GmGeneratorFromTextFile"

    TGTV="/gamos/setParam   UATargetVolume:TargetName theTarget
          /gamos/setParam   UATargetVolume:NoPhotInside 1000000
          /gamos/userAction UATargetVolume"
    SCOR="/gamos/setParam   UAScoreVolume:TargetName theTarget
          /gamos/setParam   UAScoreVolume:SourceActivity 1.955*Ci/1000
   	  /gamos/setParam   UAScoreVolume:TimeMark 3600
          /gamos/userAction UAScoreVolume"

    UAS="/gamos/userAction UAInteraction
	 /gamos/userAction GmCountProcessesUA"

 
    # Vis target spheres
    # RUN="$VIS /run/beamOn 1"
    # for (( i=0; i<${#tp[@]}; i++ )) ; do
    # 	rad='sqrt('${tp[i]}')'
    # 	TGT="$TGT
    # 	     :VOLU   theTarget${i} ORB ${rad}*mm G4_WATER_VAPOR
    # 	     :PLACE  theTarget${i} 1 sphewat rm0 0 ${tp[i]}*mm 0
    # 	     :COLOUR theTarget${i} 1 0 0"
    # done
    # jgamos --dir oo${tp[i]} $WRLS $CILV $PHYl $GENF $TGTV $SCOR $TGT $UAS $RUN

    #RUN="/gamos/setParam WriteTransport 1 /gamos/userAction UAWIF /run/beamOn 1000"

    CLKEND="/gamos/setParam UAClock:TimeLimit 3600*20
    	    /gamos/setParam UAClock:TimeMark 600
	    /gamos/userAction UAClock"

    RUN="/run/beamOn 10000000"

    for (( i=0; i<${#tp[@]}; i++ )) ; do
     	rad='sqrt('${tp[i]}')/2'
     	TGT=":VOLU   theTarget ORB ${rad}*mm G4_WATER_VAPOR
     	     :PLACE  theTarget 1 sphewat rm0 0 ${tp[i]}*mm 0
     	     :COLOUR theTarget 1 0 0"

	s=3000 #500 #600 #2000 #1000
	JOB="--host euler --ppn 8 --jpn 10 --jobs 20 --btime 24:05:00"
	SEED="--dir ooiri${tp[i]}_${s} --seed $s --SEED $s"
	jgamos $JOB $SEED $WRLS $CILV $PHYl $GENF $TGTV $SCOR $TGT $CLKEND $RUN
    done
    wait

    
fi

exit


for (( i=0; i<${#tp[@]}; i++ )) ; do
    myhadd.sh hgscore${tp[i]}.root ooiri${tp[i]}*/UAScoreVol*root
done

for f in hgscore*root ; do
    hgm.sh tabh $f
done

cat hgscore*Pri*  > tab.csv; echo ',' >> tab.csv
cat hgscore*Err* >> tab.csv; echo ',' >> tab.csv
head -1 tab.csv      > tab2.csv
grep -v '#' tab.csv >> tab2.csv


Para la fuente he simulado 30 millones de fotones en la física de Geant4 y con distribución energética de acuerdo con el espectro que se me dio.
Tras atravesar la cápsula desecho los que salen hacia abajo y me quedo con la mitad que salen hacia arriba, almacenándolos en un fichero "phasespace" para utilizarlos como fuente en la siguiente simulación.
    11473692 total
    11458654 gamma
       15038 electron

El factor entre fotones de la fuente desnuda y la encapsulada tomando solo la mitad es:
      11458654 / 30000000 = 0.382
      y su inverso: 2.6181
      
