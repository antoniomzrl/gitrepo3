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
MM=79.10*g/mole
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
CHB=":MIXT_BY_NATOMS PIRIDINA_90eV $Dens 3  C 5 H 5 N 1
     :VOLU chb TUBE 0.0 8*cm/2 4*cm/2 PIRIDINA_90eV
     :PLACE chb 1 world rmz 4*cm/2 0 0 
     :COLOUR chb 1 1 0"
MAG="/gamos/physics/userLimits/setMaxStep ulich  chb    e- 0.1*mm
     /gamos/physics/userLimits/setMaxStep ulibck chbbck e- 0.1*mm
     /gamos/field/setLocalMagField 0.076*tesla 0 0 chbbck
     /gamos/field/setLocalMagField 0.076*tesla 0 0 chb"


GENMONO=":VOLU   gbox ORB 1*mm vacuum
     	 :PLACE  gbox 1 world rmz -5*mm 0 0
     	 :COLOUR gbox 1 0 0
     	 /gamos/generator GmGenerator
     	 /gamos/generator/addSingleParticleSource gn e- 90*eV
     	 /gamos/generator/positionDist  gn GmGenerDistPositionInG4Volumes gbox
     	 /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0"
GENDIST="$GENMONO	
	 /gamos/generator/energyDist gn GmGenerDistEnergyFromFile e90eV_june2019.txt interpolate 1*eV"

HGS="/gamos/userAction GmCountProcessesUA
     /gamos/userAction UAInteraction
     /gamos/setParam   UAAnalyser:EnergyMax 100e-6
     /gamos/setParam   UAAnalyser:EnergyBins 10000
     /gamos/setParam   UAAnalyser:VolumeOut chb
     /gamos/setParam   UAAnalyser:VolumeIn  chbana
     /gamos/setParam   UAAnalyser:AnalizerParallel no
     /gamos/userAction UAAnalyser"

if [ $1 == "gen" ] ; then
    #La distribución de pérdida de energía para 90 eV es más ancha, así que sumo 10 eV para
    # ampliar el valor del parámetro EnergyMax
    HEXIT="/gamos/setParam   UAExit:EnergyMax 100*eV
           /gamos/setParam   UAExit:EnergyBins 10000
           /gamos/userAction UAExit"
    jgamos --dir oog $WRL $PHY $GENDIST $HEXIT /run/beamOn 1000000

    
elif [ $1 == "vis" ] ; then
    RUN="/run/beamOn 50"
    MAGpeq="/gamos/physics/userLimits/setMaxStep ulich  chb    e- 0.1*mm
            /gamos/physics/userLimits/setMaxStep ulibck chbbck e- 0.1*mm
            /gamos/field/setLocalMagField 0.0076*tesla 0 0 chbbck
            /gamos/field/setLocalMagField 0.0076*tesla 0 0 chb"
    jgamos --dir oov  $WRL $REFL $BCK $CHB $ANA $PHY $MAG    $GENDIST $HGS $(vis) $RUN  &
    jgamos --dir oovb $WRL $REFL $BCK $CHB $ANA $PHY $MAGpeq $GENDIST $HGS $(vis) $RUN  &
    wait


elif [ $1 == "vis2" ] ; then
    RUN="/run/beamOn 50"
    MAG2="/gamos/physics/userLimits/setMaxStep ulich  chb    e- 0.1*mm
          /gamos/field/setLocalMagField 0.0076*tesla 0 0 chb"
    jgamos --dir oov2 $WRL $REFL $CHB $PHY $MAG2 $GENDIST /gamos/userAction UAWIF $(vis) $RUN
 

elif [ $1 == "simu" ] ; then
    pre=( 00 05 10 15 20 )
    ege=( 90 )

    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/setParam UAClock:TimeMark 1
         /gamos/userAction UAClock"
    
    for (( j=0; j<${#ege[@]}; j++ )) ; do
	fge='e'${ege[j]}'eV_june2019.txt'
	#La distribución de pérdida de energía para 90 eV es más ancha, así que sumo 10 eV para
	# ampliar el valor del parámetro EnergyMax
	((hgm=ege[j]+10))
	GEN="$GENMONO
             /gamos/generator/energyDist gn GmGenerDistEnergyFromFile $fge interpolate 1*eV"

	for (( i=0; i<${#pre[@]}; i++ )) ; do
	    Dens=${pre[i]}*${mTorr}*${MM}'/('${Temp}*${KbNa}')'
	    CHB=":MIXT_BY_NATOMS PIRIDINA_90eV $Dens 3  C 5 H 5 N 1
                 :VOLU chb TUBE 0.0 8*cm/2 4*cm/2 PIRIDINA_90eV
                 :PLACE chb 1 world rmz 4*cm/2 0 0 
                 :COLOUR chb 1 1 0"
	    HGS="/gamos/setParam   UAAnalyser:EnergyMax ${hgm}e-6
                 /gamos/setParam   UAAnalyser:EnergyBins ${hgm}00
                 /gamos/setParam   UAAnalyser:VolumeOut chb
                 /gamos/setParam   UAAnalyser:VolumeIn  chbana
                 /gamos/setParam   UAAnalyser:AnalizerParallel yes
                 /gamos/userAction UAAnalyser"
	    ULI="/gamos/physics/userLimits/setMinEKin ulie1 chb    e- 0.1*eV
 	         /gamos/physics/userLimits/setMinEKin ulie2 chbbck e- 0.1*eV"
	    #RUN="$(vis) /run/beamOn 50"
	    RUN="/run/beamOn 100000"
	    
	    #PAR="--host dirac --ppn 12 --jobs 24 --btime 2:00:00 --seed 100 --SEED 100"
	    #PAR="--seed 100 --SEED 100 --jobs 10"
	    DIR=${ege[j]}_${pre[i]}
	    jgamos $PAR --dir oosmf_${DIR} $WRL $REFL $BCK $CHB $ANA $PHYsmf   $GEN $HGS $UAS $ULI $RUN &
	    #jgamos $PAR --dir oohlx_${DIR} $WRL $REFL $BCK $CHB $ANA $PHY $MAG $GEN $HGS $UAS $ULI $RUN &
	done
    done
    wait

    #Genero ficheros .csv
    for f in oo*/*Analyser*root ; do
    	hgm.sh tabjoin $f >/dev/null 2>&1 
    done
    
fi

exit




Resultados de las simulaciones con piridina con datos para 90eV.

(1) Descripción:
   Tiene momento dipolar => Hay rotacionales
   Por debajo de 0.5 eV solo consideramos elásticos con la misma DCS que la de 0.5 eV.
   Con y sin movimiento helicoidal.
   Campo magnético de 760 G (0.076 T)
   Molecular Mass: 79.10*g/mole
   Ionis Pot:  9.27 eV
   Distribuciones de pérdida valores medios (eV): ION 13.1485, EXC 8.23373, VIB 0.20386 y ROT 0.0010901
   Cámara de 40 mm. Los e- que salen de ella hacia adelante se analizan en energía, y los que salen hacia atrás
   se devuelven con la misma energía que llevaban. He puesto una especie de reflector casero y listos.


(2) He utilizado sólo un modelo:

-Modelo SMF (strong mag field) consistente en suponer que el campo es infinito y eliminar la energía transversal tras el choque en el primario y al generar secundarios. Esto me obligó en tiempos a escribir una física modificada para generar la aproximación.


