#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

ENER="6MeV"
if [ $ENER == "6MeV" ] ; then
  EGS_FILENAME="//${HOME}/varian/6_MeV/AA_HWL2_a10_sp6a.egsphsp2"
  targetpoints=("-11*mm" "-25*mm" "-35*mm")
  # num. events selected for a simulation time in Xula of 4 hours approx.
  num_events=(200000 300000 4800000)
elif [ $ENER == "8MeV" ] ; then
  EGS_FILENAME="//${HOME}/varian/8_MeV/AA_HWL2_a10_sp6a.egsphsp2"
  targetpoints=("-11*mm" "-30*mm" "-40*mm")
  # num. events selected for a simulation time in Xula of 4 hours approx.
  num_events=(700000 1700000 57000000)
fi

# physics lists
INI="/run/initialize"
PHYe="/gamos/physicsList GmEMPhysics          $INI"
PHY2="/gamos/physicsList GmEMExtendedPhysics  $INI"
PHYd="/gamos/physicsList GmDNAPhysics         $INI"
PHYl="/gamos/physicsList GmLeptsPhysics       $INI"
PHYg="/gamos/physicsList HadrontherapyPhysics $INI"

#EULER="--ppn 8  --jpn 8  --host euler"
XULA="--ppn 20 --jpn 20 --host xula"
EULER="--ppn 8  --jpn 8  --host euler"
DIRAC="--ppn 12 --jpn 12 --host dirac"
CETA="--ppn 8  --jpn 8  --host amunoz@193.144.240.176"


    
# materials
WAT=G4_WATER
POLYCARBONATE=$WAT
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

# Varian EGS phase space generator
GENVAR="#/gamos/setParam RTGeneratorPhaseSpace_EGS:MaxNReuse 6 
        #/gamos/setParam RTGeneratorPhaseSpace_EGS:MirrorWhenReuse XY
        #/gamos/setParam RTGeneratorPhaseSpace_EGS:MaxNRecycle 1 
        /gamos/setParam  RTGeneratorPhaseSpace_EGS:InitialDisplacement 0 0 0*cm 
        #/gamos/setParam RTGeneratorPhaseSpace:Histos 1 
        /gamos/setParam  RTGeneratorPhaseSpace_EGS:InitialRotAngles 0 180*deg 0
        /gamos/setParam  RTGeneratorPhaseSpace_EGS:FileName $EGS_FILENAME
        /gamos/generator RTGeneratorPhaseSpace_EGS"

GENMONO=":VOLU   gbox ORB 1*mm vacuum
     	 :PLACE  gbox 1 world rmz -5*mm 0 0
     	 :COLOUR gbox 1 0 0
     	 /gamos/generator GmGenerator
     	 /gamos/generator/addSingleParticleSource gn e- 90*eV
     	 /gamos/generator/positionDist  gn GmGenerDistPositionInG4Volumes gbox
     	 /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0"

# Multifunctional detector
MFDT="/gamos/scoring/createMFDetector theDet       theTarget
      /gamos/scoring/addScorer2MFD    theEnergyDep GmG4PSEnergyDeposit theDet
      /gamos/scoring/addScorer2MFD    theDosis     GmG4PSDoseDeposit   theDet
      /gamos/scoring/addScorer2MFD    theNProc     GmG4PSNofStep       theDet
      /gamos/classifier               theCl        GmClassifierByProcess
      /gamos/scoring/assignClassifier2Scorer theCl theNProc
      /gamos/scoring/printer cout1 GmPSPrinterTextFile
      /gamos/scoring/printer cout2 GmPSPrinterTextFile
      /gamos/scoring/printer cout3 GmPSPrinterTextFile
      /gamos/scoring/addPrinter2Scorer cout1 theEnergyDep
      /gamos/scoring/addPrinter2Scorer cout2 theDosis
      /gamos/scoring/addPrinter2Scorer cout3 theNProc
      /gamos/scoring/printByEvent theEnergyDep FALSE
      /gamos/scoring/printByEvent theDosis FALSE
      /gamos/scoring/printByEvent theNProc FALSE
      /gamos/scoring/scoreErrors theEnergyDep TRUE
      /gamos/scoring/scoreErrors theDosis TRUE
      /gamos/scoring/scoreErrors theNProc TRUE"

# Volume scorer
SCOR="/gamos/setParam   UAScoreVolume:TargetName theTarget
      /gamos/setParam   UAScoreVolume:SourceActivity 1.955*Ci/1000
      /gamos/userAction UAScoreVolume"

# Target volume
TGV="/gamos/setParam   UATargetVolume:TargetName theTarget
     /gamos/setParam   UATargetVolume:NoPhotInside 1000000
     /gamos/userAction UATargetVolume"
      
# vrml vis macros
VIS="/vis/scene/create 
     /vis/open VRML2FILE 
     /vis/viewer/flush 
     /tracking/storeTrajectory 1 
     /vis/scene/add/trajectories 
     /vis/scene/endOfEventAction accumulate 
     /vis/viewer/update 
     /vis/viewer/flush"

# 1m3 liquid water cube
CUBE_X=0.5*m
CUBE_Y=0.5*m
CUBE_Z=0.5*m
# flask cover
COVER_X=26*mm
COVER_Y=26*mm
COVER_Z=12*mm
# flask 
FLASK_X=25*mm
FLASK_Y=25*mm
FLASK_Z=11*mm
# target
TARGET_X=25*mm
TARGET_Y=25*mm
TARGET_Z=0.005*mm

# liquid water cube
CUBE=":VOLU   cube BOX $CUBE_X $CUBE_Y $CUBE_Z $WAT
      :PLACE  cube 1 world rm0 0 0 -$CUBE_Z
      :COLOUR cube 0.1 0.1 0.9
      :CHECK_OVERLAPS cube TRUE"


    
# Test generators
if [ $1 == "gener" ] ; then
    UAS="/gamos/setParam UAExit:EnergyMax 9*MeV
         /gamos/setParam UAExit:EnergyBins 140
         /gamos/userAction UAExit
         /gamos/analysis/histo1Max *Ener* 9*MeV
         /gamos/userAction GmGenerHistosUA
         /gamos/userAction UAClock"
    RUN="/run/beamOn 1000000"
    jgamos --dir oog2 $WRL $PHYl $UAS $GENVAR $RUN &
    wait

    
elif [ $1 == "visgeom" ] ; then
      i=2
      FLASK=":VOLU   cover  BOX $COVER_X $COVER_Y $COVER_Z $POLYCARBONATE
        :PLACE  cover  1 cube rm0 0 0 ${targetpoints[i]}+$CUBE_Z+$FLASK_Z-2*$TARGET_Z
        :COLOUR cover  1 0 0
        :VOLU   flask  BOX $FLASK_X  $FLASK_Y $FLASK_Z $WAT
        :PLACE  flask  1 cover rm0 0 0 0
        :COLOUR flask  0 1 0
        :VOLU   theTarget  BOX $TARGET_X $TARGET_Y $TARGET_Z $WAT
        :PLACE  theTarget  1 flask rm0 0 0 -$FLASK_Z+$TARGET_Z
        :COLOUR theTarget  0 0 1
        :CHECK_OVERLAPS cover TRUE
        :CHECK_OVERLAPS flask TRUE
        :CHECK_OVERLAPS theTarget TRUE"
      
	KILL="/gamos/setParam UAClock:TimeLimit 3600*20 
              /gamos/userAction UAClock
              /gamos/userAction UAVerbose"
	UAS="/gamos/userAction GmCountProcessesUA"
	RUN="$VIS /run/beamOn 50"
	jgamos --dir oovisgeom $WRL $CUBE $PHYl $GENVAR $TGV $SCOR $UAS $KILL $RUN
	    

elif [ $1 == "simuflask" ] ; then

    for (( i=0; i<${#targetpoints[@]}; i++ )) ; do
	FLASK=":VOLU   cover  BOX $COVER_X $COVER_Y $COVER_Z $POLYCARBONATE
		    :PLACE  cover  1 cube rm0 0 0 ${targetpoints[i]}+$CUBE_Z+$FLASK_Z-2*$TARGET_Z
        :COLOUR cover  1 0 0
        :VOLU   flask  BOX $FLASK_X  $FLASK_Y $FLASK_Z $WAT
        :PLACE  flask  1 cover rm0 0 0 0
        :COLOUR flask  0 1 0
        :VOLU   theTarget  BOX $TARGET_X $TARGET_Y $TARGET_Z $WAT
        :PLACE  theTarget  1 flask rm0 0 0 -$FLASK_Z+$TARGET_Z
        :COLOUR theTarget  0 0 1
        :CHECK_OVERLAPS cover TRUE
        :CHECK_OVERLAPS flask TRUE
        :CHECK_OVERLAPS theTarget TRUE"
  # position 1 (i=0): the flask is not full
	if [ "$i" -eq "0" ] ; then
		EMPTY_Z=0.5*${targetpoints[i]}+$FLASK_Z-$TARGET_Z
		FLASK="$FLASK
          :VOLU   empty  BOX $FLASK_X  $FLASK_Y $EMPTY_Z vacuum
          :PLACE  empty  1 flask rm0 0 0 $FLASK_Z-($EMPTY_Z)
          :COLOUR empty  0 1 0
          :CHECK_OVERLAPS empty TRUE"
	fi

	KILL="/gamos/setParam UAClock:TimeLimit 3600*20 /gamos/userAction UAClock"
	UAS="/gamos/userAction GmCountProcessesUA"


	#RUN="$VIS /run/beamOn 5"
	RUN="/run/beamOn ${num_events[i]}"
	PRU="--jobs 1 --jpn 1"

	# 1000 (10), 1010 (10), 1020 (10), 2000 (40), 2040 (40)
	s=2040
  JOB="$XULA --jobs 40 --btime 20:05:00 --seed $s --SEED $s --dir ${DIR_PREF}"
  
  jgamos  $JOB $WRL $CUBE $FLASK $PHYl $GENVAR $TGV $SCOR $UAS $KILL $RUN
  #jgamos  $JOB $WRL $CUBE $FLASK $PHYl $GENVAR   $MFDT   $TGV $SCOR $UAS $KILL $RUN
 


 
 #Generate sum root script
cat <<- EOF > ${DIR_PREF}.sh
#!/bin/bash
myhadd.sh ${DIR_PREF}.root ${DIR_PREF}/UAScoreVol*root
hgm.sh tab2 ${DIR_PREF}.root
sed 's/Nproc_//g' ${DIR_PREF}.root_PerPrimary.csv | sed 's/_Lepts//g' | sed 's/\/pri//g' | \
sed 's/\.root_PerPrimary\.csv//g' | sed 's/_p/ /g' > tab2.csv
grep -v _Mass tab2.csv | grep -v Charge |grep -v Dose |grep -v Transport | grep -v -e '^ , '> tab3.csv
EOF
chmod +x ${DIR_PREF}.sh
 
 
  done
  
  wait
  
fi

exit


######################################

Vamos  a empezar con la simulación de los electrones (6 y 8 MeV) del nuevo acelerador LINAC en agua. Esta simulación servirá de base para el experimentos con células, epiteliales en este caso.
Los espacios de fases para 6 y 8 MeV los envió el fabricante a través de google:
https://drive.google.com/open?id=1_gPhMivyNyqxOrwevuzMAT49TxvVFGkG
Espero que los bajéis sin problemas, yo tengo copia en cualquier caso. Están dados para un aplicador de 10 cm de diámetro. Es decir tienen la energía y el momento de los electrones justo en la superficie del agua. En este experimento, el contenedor de las células no serán pocillos sino un flask de 5x5 cm en el que las células van pegadas sobre el fondo formando una única capa celular con un espesor de 10 micras.
El flask estará centrado en el campo de radiación de 10 cm de diámetro y vamos a simular tres posiciones en profundidad para cada uno de los haces primarios (6 y 8 MeV)
Posición 1: La capa de células de 5x5 cmx10 micras se encuentra, en posición paralela a la superficie, a una profundidad de 11 mm. Esta posición es común a los haces de 6 y 8 MeV
Posición 2: Aumentamos la profundidad en posición paralela a la anterior hasta 25 mm para 6 MeV y 30 mm para 8 MeV
Posición 3: Procedemos como en el caso anterior hasta una profundidad de 35 mm para 6 MeV y 40 mm para 8 MeV.
Como véis el detector es la capa de células que tiene una superficie de 2500 mm2 y un espesor de 0.01 mm,. Dentro del detector tenemos que simular todo lo que seamos capaces de determinar: energía depositada, tipo y número de procesos que tienen lugar, dosis absorbida (suponemos la densidad de las células es igual a la densidad del agua).
En la posición 1, para hacer las cosas con mayor precisión habría que tener en cuenta que el flask no estaría lleno, de los 23 mm que tiene de espesor el flask, sólo habria 11 mm de espesor de agua.
Es decir, enrasando el nivel del agua con la superficie del líquido, el flask sobresaldría 12 mm sobre la superficie del agua estos (11 mm de aire y 1 mm de policarbonato que tiene la densidad del agua).
