#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

WRL=":MATE vacuum 1 1*g/mole 0*g/cm3 
     :ROTM rmz 0 90 0 
     :ROTM rm0 0 0 0 
     :VOLU world BOX 10*cm 10*cm 10*cm vacuum 
     :COLOUR world 1 1 1"

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
CHB=":MIXT_BY_NATOMS PYRIDINE $Dens 3  C 5  H 5  N 1
     :VOLU chb TUBE 0.0 8*cm/2 4*cm/2 PYRIDINE 
     :PLACE chb 1 world rmz 4*cm/2 0 0 
     :COLOUR chb 0.6 0.6 0.0"
MAG="/gamos/physics/userLimits/setMaxStep ulich  chb    e- 0.1*mm
     /gamos/physics/userLimits/setMaxStep ulibck chbbck e- 0.1*mm
     /gamos/field/setLocalMagField 0.01*tesla 0 0 chbbck
     /gamos/field/setLocalMagField 0.01*tesla 0 0 chb"

GEN=":VOLU   gbox ORB 2*mm vacuum
     :PLACE  gbox 1 world rmz -5*mm 0 0
     :COLOUR gbox 1 1 1
     /gamos/generator GmGenerator
     /gamos/generator/addSingleParticleSource gn e- 10*eV
     /gamos/generator/positionDist  gn GmGenerDistPositionInG4Volumes gbox
     /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0"

HGS="/gamos/userAction GmCountProcessesUA
     /gamos/userAction UAInteraction
     /gamos/setParam   UAExit:EnergyMax 12*eV
     /gamos/setParam   UAExit:EnergyBins 1200
     /gamos/userAction UAExit
     /gamos/setParam   UAAnalyser:EnergyMax 12*eV
     /gamos/setParam   UAAnalyser:EnergyBins 1200
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
    ege=( 10 15 )

    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         #/gamos/setParam TimeMark 1
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

	    #RUN="$(vis) /run/beamOn 50"
	    RUN="/run/beamOn 100000"
	    
	    PAR="--host dirac --ppn 1 --jobs 1 --btime 4:29:00"
	    #PAR="--host euler --ppn 10 --jobs 250 --btime 4:29:00 --seed 1400 --SEED 1400"
	    #PAR="--jobs 10 --ppn 10"
	    DIR=${ege[j]}_${pre[i]}
	    jgamos $PAR --dir oor_${DIR} $WRL $REFL $BCK $CHB $ANA $PHY $MAG $GEN $HGS $UAS $RUN  &
	    jgamos $PAR --dir oon_${DIR} $WRL       $BCK $CHB $ANA $PHY $MAG $GEN $HGS $UAS $RUN  &
	    wait
	done
    done
    wait

fi

exit




Básicamente es lo mismo que hicimos en el último artículo de furfural. La distribución inicial de elctrones viene integrada porque simplemente la intensidad transmitida cuando aplicamos un potencial retardador. En el excel viene representado por el espectro a presión cero.

Después hacemos lo mismo introduciendo un par de valores de la presión, uno bajo en el que esperamos que no haya colisiones múltiples y otro más alto en el que éstas serán importantes.

Esto lo repetimos para tres energías, una muy baja (3 3V) en la que sólo habrá elásticos y rotacionales, otra media (10 eV) con excitaciones electrónicas (y también vibracionales pero por desgracia no tenemos datos para éstos, usaremos los de pirimidina) y la última por encima de la ionización (20 eV) por lo que también ionizarán.

Los datos que te envío son las secciones eficaces diferenciales e integrales calculadas y los espectros de pérdida de energía para excitación electrónica y para ionización.

Faltan las secciones eficaces vibracionales pero puedes utilizar las de pirimidina (lo mismo para la pérdida de energía vibracional y rotacional, usa los  valores de pirimidina).


Pyridine:
Molecular Mass: 79.10*g/mole
Eloss Rotational: 9.95e-4 eV furfural
Eloss Ionisation: nueva distribución, promedio: 14.9716 eV
Eloss Excitation: nueva distribución, promedio:  7.5169 eV
Eloss Vibrational:  furfural, promedio: 0.1866 eV
IXS Vibrational: furfural
Ionis Pot:  9.27 eV  (furfural 9.22 y pirimidina 9.23)
Cámara 4 cm


Como te dije le mejor forma de 
enetender el problema es separar el vector velocidad en dos componentes, 
una paralela y otra perpendicular , lo que nos permite hablar de una 
energía cinética paralela y otra perpendicular. Como bien dices, el 
campo sólo actúa sobre la velocidad perpendicular, induciendo un 
movimiento circular (que al componerlo con el paralelo nos da una 
trayectoria helicoidal) al que está asociada la energía cinética 
perpendicular. Nosotros no suponemos que el campo se come la energía 
transversal. De hecho, como esta energía depende del ángulo de 
scattering lo que decimos es que la dispersión se convierte en una 
pérdida de energía en la dirección longitudinal (fíjate que para un 
ángulo exactamente igual a 90º el electrón se quedaría parado y si es un 
poco mayor se da la vuelta). El radio de la órbita circular es 
inversamente proporcional a la intensidad del campo. Lo que decimos es 
que si el campo es suficientemente intenso el radio es tan pequeño, 
comparado con las dimensiones de los orificios de la cámara, que podemos 
suponer que es cero y que después del choque el electrón sigue hacia 
adelante pero con una pérdida de energía igual a (1/2)*m*[v*cos(ang)]**2 
. Eso es lo que estás haciendo en la simulación, no supones que la 
energía perpendicular sea cero, pues en realidad es igual a 
(1/2)*m*[v*sen(ang)]**2, sino que el radio es cero. Ahora lo que 
queremos hacer es introducir el efecto del campo, es decir simular ese 
radio (R), por lo que los electrones avanzarían a una distancia R del 
eje que dependerá del ángulo de colisión. Cuando hay colisiones 
inelásticas, entra una pérdida adicional. Además de la pérdida de 
energía paralela (que es sobre la que actúa el analizador de energía) 
hay una pérdida dada por espectro de pérdidas. Como el cociente energía 
perpendicular/B es un invariante ésta pérdida también afectará al radio.
