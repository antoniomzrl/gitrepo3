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
CHB=":MIXT_BY_NATOMS BENZOQUINONE $Dens 3  C 6 H 4 O 2
     :VOLU chb TUBE 0.0 8*cm/2 4*cm/2 BENZOQUINONE
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
     	 /gamos/generator/addSingleParticleSource gn e- 15*eV
     	 /gamos/generator/positionDist  gn GmGenerDistPositionInG4Volumes gbox
     	 /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0"
GENDIST="$GENMONO	
	 /gamos/generator/energyDist gn GmGenerDistEnergyFromFile e15eVbenzoq.txt interpolate 1*eV"

HGS="/gamos/userAction GmCountProcessesUA
     /gamos/userAction UAInteraction
     /gamos/setParam   UAAnalyser:EnergyMax 17*eV
     /gamos/setParam   UAAnalyser:EnergyBins 1700
     /gamos/setParam   UAAnalyser:VolumeOut chb
     /gamos/setParam   UAAnalyser:VolumeIn  chbana
     /gamos/setParam   UAAnalyser:AnalizerParallel no
     /gamos/setParam   UAAnalyser:FileName Forward
     /gamos/userAction UAAnalyser"

if [ $1 == "gen" ] ; then
    HEXIT="/gamos/setParam   UAExit:EnergyMax 17*eV
           /gamos/setParam   UAExit:EnergyBins 1700
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
    pre=( 00 02 05 )
    pre=( 05 )
    ege=( 15 )

    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/setParam UAClock:TimeMark 1
         /gamos/userAction UAClock"
    
    for (( j=0; j<${#ege[@]}; j++ )) ; do
	fge='e'${ege[j]}'eVbenzoq.txt'
	((hgm=ege[j]+2))
	GEN="$GENMONO
             /gamos/generator/energyDist gn GmGenerDistEnergyFromFile $fge interpolate 1*eV"

	for (( i=0; i<${#pre[@]}; i++ )) ; do
	    Dens=${pre[i]}*${mTorr}*${MM}'/('${Temp}*${KbNa}')'
	    CHB=":MIXT_BY_NATOMS BENZOQUINONE $Dens 3  C 6 H 4 O 2
                 :VOLU chb TUBE 0.0 8*cm/2 4*cm/2 BENZOQUINONE
                 :PLACE chb 1 world rmz 4*cm/2 0 0 
                 :COLOUR chb 1 1 0"
	    HGS="/gamos/setParam   UAAnalyser:EnergyMax ${hgm}*eV
                 /gamos/setParam   UAAnalyser:EnergyBins ${hgm}00
                 /gamos/setParam   UAAnalyser:AnalizerParallel yes
                 /gamos/setParam   UAAnalyser:VolumeOut chb
                 /gamos/setParam   UAAnalyser:VolumeIn  chbana
                 /gamos/setParam   UAAnalyser:FileName Forward
                 /gamos/userAction UAAnalyser"
	    HGS2="/gamos/analysis/histo1NBins *Ener* ${hgm}00
	          /gamos/analysis/histo1Max   *Ener* ${hgm}*eV
		  /gamos/filter vfout GmExitLogicalVolumeFilter  chb
                  /gamos/filter vfinp GmEnterLogicalVolumeFilter chbana
                  /gamos/filter volfl GmANDFilter vfout vfinp
		  /gamos/setParam GmStepDataHistosUA_volfl:FileName xxxxx
                  /gamos/setParam GmStepDataHistosUA_volfl:DataList FinalKineticEnergy
                  /gamos/userAction GmStepDataHistosUA volfl"
	 
	    ULI="/gamos/physics/userLimits/setMinEKin ulie1 chb    e- 0.01*eV
 	         /gamos/physics/userLimits/setMinEKin ulie2 chbbck e- 0.01*eV"
	    #RUN="$(vis) /run/beamOn 50"
	    RUN="/run/beamOn 10000"
	    
	    #PAR="--host dirac --ppn 12 --jobs 24 --btime 2:00:00 --seed 100 --SEED 100"
	    #PAR="--seed 100 --SEED 100 --jobs 10"
	    DIR=${ege[j]}_${pre[i]}
	    #jgamos $PAR --dir oohlx_${DIR} $WRL $REFL $BCK $CHB $ANA $PHY $MAG $GEN $HGS $UAS $ULI $RUN &
	    jgamos $PAR --dir oosmf_${DIR} $WRL $REFL $BCK $CHB $ANA $PHYsmf $GEN $HGS $UAS $ULI $RUN &
	    jgamos $PAR --dir o3_${DIR}    $WRL $REFL $BCK $CHB $ANA $PHYsmf $GEN $HGS3 $UAS $ULI $RUN &

	done
    done
    wait

    
fi

exit

Resultados de las simulaciones con benzoquinona.

(1) Descripción:
   No tiene momento dipolar, no hay rotacionales
   Por de bajo de 1 eV solo consideramos elásticos con la misma DCS que la de 1 eV.
   Con y sin movimiento helicoidal.
   Campo magnético de 760 G (0.076 T)
   Molecular Mass: 108.1*g/mole
   Ionis Pot:  10.1 eV
   Distribuciones de pérdida valores medios (eV): ION 10.5651, EXC 6.90152 y VIB 0.157359
   Cámara de 40 mm. Los e- que salen de ella hacia adelante se analizan en energía, y los que salen hacia atrás se devuelven con la misma energía que llevaban. He puesto una especie de reflector casero y listos.

(2) He utilizado dos modelos:

-Modelo SMF (strong mag field) consistente en suponer que el campo es infinito y eliminar la energía transversal tras el choque en el primario y al generar secundarios. Esto me obligó en tiempos a escribir una física modificada para generar la aproximación.

-Modelo HLX (helix) simulando completamente el movimiento helicoidal en la cámara inmersa en un campo magnético de valor finito. Esta simu es mucho más lenta porque debe añadir los cálculos relacionados con la física del campo de geant4 y porque los electrones recorren más camino, pero se hace con la física LEPTS de siempre. He tenido que aprender a mezclar nuestra física con el campo de geant4. En este modelo supongo además que el analizador solamente ve la energía asociada a la componente longitudinal de los e- que acaban de salir de la cámara.
