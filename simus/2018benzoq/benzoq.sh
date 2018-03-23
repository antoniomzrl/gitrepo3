#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

WRL=":MATE vacuum 1 1*g/mole 0*g/cm3 
     :ROTM rmz 0 90 0 
     :ROTM rm0 0 0 0 
     :VOLU world BOX 10*cm 10*cm 10*cm vacuum 
     :COLOUR world 0.5 0.5 0.5"

PHY="/gamos/physicsList GmLeptsPhysics /run/initialize"
PHYsmf="/gamos/physicsList GmLeptsSMFPhysics /run/initialize"

mTorr=atmosphere/760000
Avogadro=6.02214179e+23/mole
k_Boltzmann=8.617343e-11*MeV/kelvin
KbNa=${k_Boltzmann}'*'${Avogadro}
MM=108.096*g/mole
Temp=301*kelvin
pre=2*${mTorr}
Dens=${pre[i]}*${MM}'/('${Temp}*${KbNa}')'


ANA=":VOLU chbana TUBE 0.0 9*cm/2 1*cm/2 vacuum 
     :PLACE chbana 1 world rmz 4.5*cm 0 0 
     :COLOUR chbana  0 1 1"
BCK=":VOLU chbbck TUBE 0.0 9*cm/2 2*cm/2 vacuum 
     :PLACE chbbck 1 world rmz -1*cm 0 0 
     :COLOUR chbbck  0 1 1"
REFL=":MATE USERMAT 1 1*g/mole 1e6*g/cm3
      :VOLU chu TUBE 0.0 9*cm/2 1*mm/2 USERMAT 
      :PLACE chu 1 world rmz -2.1*cm 0 0"
CHB=":MIXT_BY_NATOMS PYRIDINE $Dens 3  C 5  H 5  N 1
     :VOLU chb TUBE 0.0 8*cm/2 4*cm/2 PYRIDINE 
     :PLACE chb 1 world rmz 4*cm/2 0 0 
     :COLOUR chb 1 1 0"
MAG="/gamos/physics/userLimits/setMaxStep ulich  chb    e- 0.1*mm
     /gamos/physics/userLimits/setMaxStep ulibck chbbck e- 0.1*mm
     /gamos/field/setLocalMagField 0.01*tesla 0 0 chbbck
     /gamos/field/setLocalMagField 0.01*tesla 0 0 chb"


GENMONO=":VOLU   gbox ORB 1*mm vacuum
     	 :PLACE  gbox 1 world rmz -5*mm 0 0
     	 :COLOUR gbox 1 0 0
     	 /gamos/generator GmGenerator
     	 /gamos/generator/addSingleParticleSource gn e- 15*eV
     	 /gamos/generator/positionDist  gn GmGenerDistPositionInG4Volumes gbox
     	 /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0"
GENDIST="$GENMONO	
	 /gamos/generator/energyDist gn GmGenerDistEnergyFromFile e15eVbenzoq.txt interpolate 1*eV"

HGS="/gamos/userAction GmCountProcessesUA
     /gamos/userAction UAInteraction
     /gamos/setParam   UAExit:EnergyMax 17*eV
     /gamos/setParam   UAExit:EnergyBins 1700
     /gamos/userAction UAExit
     /gamos/setParam   UAAnalyser:EnergyMax 17*eV
     /gamos/setParam   UAAnalyser:EnergyBins 1700
     /gamos/setParam   UAAnalyser:VolumeOut chb
     /gamos/setParam   UAAnalyser:VolumeIn  chbana
     /gamos/setParam   UAAnalyser:AnalizerParallel no
     /gamos/userAction UAAnalyser"


if [ $1 == "helix" ] ; then
    UAS="$(vis)"
    RUN="/run/beamOn 50"
    jgamos --dir ooh1 $WRL $REFL $BCK $CHB $ANA $PHY $MAG $GEN $HGS $UAS $RUN  &
    wait
  

elif [ $1 == "simu" ] ; then
    pre=( 00 02 9.5 10 )
    ege=( 03 10 15 20 )

    #ege=( 10 ) ; pre=( 00 02 10 )
    ege=( 15 ) ; pre=( 00 02 9.5 )
    ege=( 15 ) ; pre=( 02 )
    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/setParam UAClock:TimeMark 1
         /gamos/userAction UAClock"
    
    for (( j=0; j<${#ege[@]}; j++ )) ; do
	fge='el'${ege[j]}'eV.txt'
	((hgm=ege[j]+2))
	GEN=$(gen e- 1*eV 1*mm -5*mm $fge)

	for (( i=0; i<${#pre[@]}; i++ )) ; do
	    Dens=${pre[i]}*${mTorr}*${MM}'/('${Temp}*${KbNa}')'
	    CHB=":MIXT_BY_NATOMS PYRIDINE $Dens 3  C 5  H 5  N 1
                 :VOLU chb TUBE 0.0 8*cm/2 4*cm/2 PYRIDINE 
            	 :PLACE chb 1 world rmz 4*cm/2 0 0 
		 :COLOUR chb 0.6 0.6 0.0"
	    HGS="/gamos/setParam   UAAnalyser:EnergyMax ${hgm}*eV
                 /gamos/setParam   UAAnalyser:EnergyBins ${hgm}00
                 /gamos/setParam   UAAnalyser:VolumeOut chb
                 /gamos/setParam   UAAnalyser:VolumeIn  chbana
                 /gamos/setParam   UAAnalyser:AnalizerParallel yes
                 /gamos/userAction UAAnalyser"
	    ULI="/gamos/physics/userLimits/setMinEKin ulie chb e- 0.1*eV"
	    #RUN="$(vis) /run/beamOn 50"
	    RUN="/run/beamOn 10000"
	    
	    #PAR="--host dirac --ppn 12 --jobs 24 --btime 2:00:00 --seed 100 --SEED 100"
	    PAR="--seed 100 --SEED 100"
	    DIR=${ege[j]}_${pre[i]}
	    #jgamos $PAR --dir oor_${DIR} $WRL $REFL $BCK $CHB $ANA $PHY $MAG $GEN $HGS $UAS $RUN  &
	    jgamos $PAR --dir oon_${DIR} $WRL       $BCK $CHB $ANA $PHY $MAG $GEN $HGS $UAS $RUN  &
	done
    done
    wait

fi

exit


Por de bajo de 1 eV, sólo consideramos elásticos con la misma DCS que la de 1 eV.
No tiene momento dipolar, no hay rotacionales
Cámara ahora son 40 mm
0.1 Tesla pero primero quiero ver la simulación sin movimiento helicoidal, hay que pensar un poco en las implicacione sde ese movimiento.
Los espectros enviados corresponden a un campo magnético de 760 G (0.076 T)

Molecular Mass: 108.1*g/mole
Eloss Rotational: 9.95e-4 eV furfural
Eloss Ionisation: nueva distribución, promedio: 14.9716 eV
Eloss Excitation: nueva distribución, promedio:  7.5169 eV
Eloss Vibrational:  furfural, promedio: 0.1866 eV
IXS Vibrational: furfural
Ionis Pot:  10.1 eV (p9.27 furfural 9.22 y pirimidina 9.23)
Cámara 4cm