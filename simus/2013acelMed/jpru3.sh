#!/bin/bash

source $HOME/lepts/bin/conflepts.sh

MAT=" :ROTM rmz 0 90 0
      //MAT-water-gold-vacuum
      :MATE vacuum  1. 1.008*g/mole   1.e-25*g/cm3
      :MATE gold   79. 196.967*g/mole 19.3*g/cm3
      :MATE water  10. 18*g/mole 1*g/cm3
      :MATE_TEMPERATURE water 273.15*kelvin"

WRL=" //WORLD-3m3
      :VOLU   world BOX 1.5*m 1.5*m 1.5*m vacuum
      :COLOUR world 1 1 1"

WRP=" //WRAPPER-sphere-1.3m
      :VOLU wrapper SPHERE 1.3*m 1.35*m 0 360*deg 0 180*deg gold
      :PLACE  wrapper 1 world rmz 0. 0. 0.
      :COLOUR wrapper 1 1 1
      :VIS wrapper OFF"

CHB=" //CHAMBER-water-1m3
      :VOLU   chwater BOX 0.5*m 0.5*m 0.5*m water
      :PLACE  chwater 1 world rmz 0 0 0
      :COLOUR chwater 0.6 0.6 0.0"

CHBnm="//CHAMBER-water-1mm3
       :VOLU   chwater BOX 0.2*m 0.2*m 1*nm water
       :PLACE  chwater 1 world rmz 20*cm 0 0
       :COLOUR chwater 0.6 0.6 0.0"

PHY=" #PHYSICS
      /gamos/physicsList LeptsPhysicsList
      #/gamos/setParam ElectronModel lepts
      /gamos/setParam ElectronModel leptswater
      /gamos/setParam PhotonModel penelope
      /gamos/setParam XsDat water
      /gamos/setParam CutEnergyIoniz 10"

PHYPRU=" /gamos/physicsList GmEMPhysics
         /gamos/GmPhysics/addPhysics gamma-lowener
         /gamos/GmPhysics/addPhysics electron-lowener"

STPE="/gamos/setParam StopEnergyElectron 9*MeV"

UAS=" /gamos/analysis/histo1Max *Ener* 7*MeV
      /gamos/userAction GmGenerHistosUA
      #/gamos/userAction UserActionPrueba
      #/gamos/userAction UAVerbose
      /gamos/userAction UADepo
      /gamos/userAction UAInteraction"


SEED="#/gamos/random/setSeeds $sd $sd"

INI=" #RUN-INIT
      #/gamos/userAction Run
      #/gamos/userAction Event
      /run/initialize"

DET=" #DETECTOR-chwater
      #/gamos/SD/assocSD2LogVol Detector det1 chwater"

DETW=" #DETECTOR-wrapper
       #/gamos/SD/assocSD2LogVol Detector det2 wrapper"

BOX10cm=" //BOX-10cmx10cm
          :VOLU   gbox BOX 5*cm 5*cm 5*cm vacuum
          :PLACE  gbox 1 world rmz -80*cm 0 0
          :COLOUR gbox 1 0 0"

GENMONO=" #GENERATOR-1MeV
          $BOX10cm
          /gamos/generator GmGenerator
          /gamos/generator/addSingleParticleSource gene gamma 1*MeV
          /gamos/generator/positionDist  gene GmGenerDistPositionInG4Volumes gbox
          /gamos/generator/directionDist gene GmGenerDistDirectionConst 1 0 0"

GENSPEC=" #GENERATOR-spect5.9MeV
          $GENMONO
          /gamos/generator/energyDist gene GmGenerDistEnergyFromFile
                              photonSpec5p9MeV.txt interpolate 1*MeV"

GENe=" #GENERATOR-e100eV
       $BOX10cm
       /gamos/generator GmGenerator
       /gamos/generator/addSingleParticleSource gene e- 100*eV
       /gamos/generator/positionDist  gene GmGenerDistPositionInG4Volumes gbox
       /gamos/generator/directionDist gene GmGenerDistDirectionConst 1 0 0"

ge10k="#GENERATOR-e10keV
       $BOX10cm
       /gamos/generator GmGenerator
       /gamos/generator/addSingleParticleSource gene e- 10*keV
       /gamos/generator/positionDist  gene GmGenerDistPositionInG4Volumes gbox
       /gamos/generator/directionDist gene GmGenerDistDirectionConst 1 0 0"

VIS=" #VISU
      /vis/scene/create
      /vis/open VRML2FILE
      /vis/viewer/flush
      /tracking/storeTrajectory 1
      /vis/scene/add/trajectories
      /vis/scene/endOfEventAction accumulate
      /vis/viewer/update
      /vis/viewer/flush"


RUN=" #RUN
      /run/setCut 1e-9 um
      /run/beamOn 2"



if [ $1 == "prueba" ] ; then
    RUN="/run/beamOn 100"
   #RUN="/run/beamOn 100000000 /batchtime 24:0:0 "

    CFL="$MAT $WRL $WRP $CHB $PHY $STPE $UAS $INI $GENMONO"
    time jlepts $CFL $RUN /dir ok > k
fi


if [ $1 == "pruf" ] ; then
    RUN="/run/beamOn 10000"
    for((i=1;i<2;i++)) ; do
	let sd=(i+1000)*100
	SEED="/gamos/random/setSeeds $sd $sd"
	CFL="$MAT $WRL $CHB $PHY $STPE $UAS $INI $GENMONO $RUN $SEED"
	jlepts $CFL /dir op_$sd
    done
fi


if [ $1 == "phots" ] ; then
    RUN="/run/beamOn 10000000 /batchtime 4:0:0"
    for((i=1;i<101;i++)) ; do
	let sd=(i+1000)*100
	SEED="/gamos/random/setSeeds $sd $sd"
	CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $GENSPEC $RUN $SEED"
	jlepts $CFL /dir op_$sd
    done
fi



if [ $1 == "verbose" ] ; then
    RUN="/run/beamOn 2"
    STPE="/gamos/setParam StopEnergyElectron 100*eV"
    UAS="/gamos/userAction UAVerbose"
    CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO"
    time jlepts $CFL $RUN /dir ok > k
fi


if [ $1 == "angle" ] ; then
    RUN="/run/beamOn 1000000"
    UAS="/gamos/userAction UAInteraction"

    PHY1="/gamos/physicsList LeptsPhysicsList
         /gamos/setParam XsDat water"

    PHY="$PHY1  /gamos/setParam ElectronModel lepts"
    CFL="$MAT $WRL $CHBnm $PHY $UAS $INI $GENe $RUN"
    time jlepts $CFL  /dir ok > k1

    PHY="$PHY1  /gamos/setParam ElectronModel leptswater"
    CFL="$MAT $WRL $CHBnm $PHY $UAS $INI $GENe $RUN"
    time jlepts $CFL  /dir ok2 > k2
fi



if [ $1 == "wif" ] ; then
    RUN="/run/beamOn 10"
    STPE="/gamos/setParam StopEnergyElectron 100*eV"
    UAS="/gamos/userAction UAWIF 
         /gamos/userAction UAInteraction"
    CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO $RUN"
    time jlepts $CFL /dir ow > kw
fi


if [ $1 == "hginter" ] ; then
    STPE="/gamos/setParam StopEnergyElectron 100*eV"
    UAS="/gamos/userAction UAInteraction"
    #RUN="/run/beamOn 10000000 /batchtime 4:0:0"
    RUN="/run/beamOn 10"

    CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO $RUN"
    time jlepts $CFL /dir ok1 > k1

    #PHY="/gamos/physicsList GmEMPhysics"
    #CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO $RUN"
    #time jlepts $CFL /dir ok2 > k2

    #PHY="/gamos/physicsList GmEMPhysics
    #     /gamos/GmPhysics/replacePhysics electron-penelope"
    #CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO $RUN"
    #time jlepts $CFL /dir ok3 > k3

    PHY="/gamos/physicsList GmDNAPhysics"
    CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO $RUN"
    #time jlepts $CFL /dir ok4 > k4
fi


if [ $1 == "cpu" ] ; then
    UAS="/gamos/userAction UAInteraction"
    RUN="/run/beamOn 10"
    STPE="/gamos/setParam StopEnergyElectron 1*eV"

    for((i=0;i<${2};i++)) ; do
	let sd=(i+1000)*100
	SEED="/gamos/random/setSeeds $sd $sd"
	CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $ge10k $RUN $SEED"
	time jlepts $CFL /dir op_$sd > k_$i &
    done
fi


if [ $1 == "phl" ] ; then
    RUN="/run/beamOn 10"
    STPE="/gamos/setParam StopEnergyElectron 100*eV"
    UAS="/gamos/userAction UAInteraction"

    PHY1="/gamos/physicsList LeptsPhysicsList
          /gamos/setParam XsDat water
          /gamos/setParam PhotonModel penelope"

  
    PHY="$PHY1  /gamos/setParam ElectronModel lepts"
    CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO"
    time jlepts $CFL $RUN /dir oph1 > kph1

    PHY="$PHY1  /gamos/setParam ElectronModel leptswater"
    CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO"
    time jlepts $CFL $RUN /dir oph2 > kph2
fi



if [ $1 == "genfile" ] ; then
    GFF="/gamos/setParam GmGeneratorFromTextFile:FileName ../genfile.txt
	 /gamos/generator GmGeneratorFromTextFile"
    RUN="/run/beamOn 11"
    STPE="/gamos/setParam StopEnergyElectron 1.27*MeV"
    UAS="/gamos/userAction UAWIF"
    CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $GFF $RUN"
    time jlepts $CFL /dir ogf > kgf
    #drawpw ogf/int.bcd
fi


if [ $1 == "contfile" ] ; then
# (1) 2 fotones & e- hasta:
    RUN="/run/beamOn 1"
    STPE="/gamos/setParam StopEnergyElectron 0.5*MeV"
    UAS="/gamos/userAction UAWIF"
    CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $GENMONO $RUN"
    time jlepts $CFL /dir ocf1 > kcf1

# (2a) fotones & stop e- en fichero:
    STPE="/gamos/setParam StopEnergyElectron 10*MeV"
    UAS="/gamos/setParam ContinuationFileName elgen.txt
         /gamos/userAction UAContinuationFile
         /gamos/userAction UAWIF"
    CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $GENMONO $RUN"
    time jlepts $CFL /dir ocf2a > kcf2a

# (2b) cont a partir del fichero:
    Nl=`cat ocf2a/elgen.txt | wc -l`
    RUN="/run/beamOn $Nl"
    GFF="/gamos/setParam GmGeneratorFromTextFile:FileName ../ocf2a/elgen.txt
	 /gamos/generator GmGeneratorFromTextFile"
    STPE="/gamos/setParam StopEnergyElectron 0.5*MeV"
    UAS="/gamos/userAction UAWIF"
    CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $GFF $RUN"
    time jlepts $CFL /dir ocf2b > kcf2b

    cat ocf1/int.bcd                > 1.bcd
    cat ocf2a/int.bcd ocf2b/int.bcd > 2.bcd
fi



if [ $1 == "nintel" ] ; then
    UAS="/gamos/userAction UAInteraction"
    RUN="/run/beamOn 10"
    STPE="/gamos/setParam StopEnergyElectron 1*eV"

    CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $ge10k $RUN"
    time jlepts $CFL /dir oni > k
exit
    # G4-DNA:
    MAT=":ROTM rmz 0 90 0
         //MAT-water-gold-vacuum
         :MATE vacuum  1. 1.008*g/mole   1.e-25*g/cm3
         :MATE gold   79. 196.967*g/mole 19.3*g/cm3
         //:MATE G4_WATER  10. 18*g/mole 1*g/cm3
         //:MATE_TEMPERATURE G4_WATER 273.15*kelvin"
    CHB="//CHAMBER-water-1m3
         :VOLU   chwater BOX 0.5*m 0.5*m 0.5*m G4_WATER
         :PLACE  chwater 1 world rmz 0 0 0
         :COLOUR chwater 0.6 0.6 0.0"
    PHY="/gamos/physicsList GmDNAPhysics"
    CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $ge10k $RUN"
    time jlepts $CFL /dir oni2 > k2

    # g4-DNA-Sally
    PHY="/gamos/physicsList SallyDNAPhysicsList"
    CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $ge10k $RUN"
    time jlepts $CFL /dir oni3 > k3
fi


if [ $1 == "ecut" ] ; then
    UAS="/gamos/userAction UAInteraction"
    RUN="/run/beamOn 1000 /batchtime 23:0:0"

    for Ec in 1000 100 10 1 0.5 0.1 ; do
	for sd in 100001 100002 ; do
	    STPE="/gamos/setParam StopEnergyElectron ${Ec}*eV"
	    SEED="/gamos/random/setSeeds $sd $sd"
	    DIR="/dir o_${Ec}_${sd}"
	    CFL="$MAT $WRL $CHB $PHY $UAS $SEED $STPE $INI $ge10k $RUN $DIR"
	    #echo $STPE $DIR
	    jlepts $CFL
	done
    done
fi
