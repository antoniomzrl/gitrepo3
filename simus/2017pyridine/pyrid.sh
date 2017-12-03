#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

WRL=":MATE vacuum 1 1*g/mole 0*g/cm3 
     :ROTM rmz 0 90 0 
     :ROTM rm0 0 0 0 
     :VOLU world BOX 20*cm 20*cm 20*cm vacuum 
     :COLOUR world 1 1 1"

PHY="/gamos/physicsList GmLeptsPhysics /run/initialize"
PHYsmf="/gamos/physicsList GmLeptsSMFPhysics /run/initialize"


mTorr=atmosphere/760000
Avogadro=6.02214179e+23/mole
k_Boltzmann=8.617343e-11*MeV/kelvin
KbNa=${k_Boltzmann}'*'${Avogadro}
MM=79.10*g/mole
Temp=301*kelvin

Pres=1${mTorr}
Dens=${Pres}*${MM}'/('${Temp}*${KbNa}')'
# MAT=":MIXT_BY_NATOMS FURFURAL $Dens 3  C 5  H 4  O 2"
# MAT=":MATE FURFURAL 1.0 ${MM} ${Dens}"

UAS="#/gamos/userAction UAWIF
     #/gamos/userAction UAVerbose
     /gamos/userAction UAInteraction
     /gamos/userAction UAClock
     /gamos/setParam   UAExit:EnergyMax 22*eV
     /gamos/setParam   UAExit:EnergyBins 220
     /gamos/userAction UAExit"

    
if [ $1 == "helix" ] ; then
    pre=2*${mTorr}
    Dens=${pre[i]}*${MM}'/('${Temp}*${KbNa}')'
    CHB=":MIXT_BY_NATOMS PYRIDINE $Dens 3  C 5  H 5  N 1
         :VOLU chb TUBE 0.0 9*cm/2 4*cm/2 PYRIDINE 
         :PLACE chb 1 world rmz 0 0 0 
         :COLOUR chb 0.6 0.6 0.0"
    GEN="//G_BALL
         :VOLU   gbox ORB 3*mm vacuum
         :PLACE  gbox 1 world rmz -15*cm 0 0
         :COLOUR gbox 1 0 0
         /gamos/generator GmGenerator
         /gamos/generator/addSingleParticleSource gn e- 10*eV
         /gamos/generator/positionDist  gn GmGenerDistPositionInG4Volumes gbox
         /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0"
    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/userAction UAExit
         /gamos/userAction UAWIF
         $(vis)"
    RUN="/run/beamOn 10"
    STP="/gamos/physics/userLimits/setMaxStep ulich chb   e- 1*mm
         /gamos/physics/userLimits/setMaxStep uliw  world e- 1*mm"

    MAG1="$STP /gamos/field/setMagField 0.001*tesla 0 0"
    MAG2="$STP /gamos/field/setMagField 0.01*tesla 0 0"
    jgamos --dir ooh0 $WRL $CHB $PHY       $GEN $UAS $HGS $RUN  &
    jgamos --dir ooh1 $WRL $CHB $PHY $MAG1 $GEN $UAS $HGS $RUN  &
    jgamos --dir ooh2 $WRL $CHB $PHY $MAG2 $GEN $UAS $HGS $RUN  &
    wait
  

elif [ $1 == "simu" ] ; then
    pre=( 00 02 9.5 10 )
    ege=( 03 10 15 20 )
    pre=( 00 02 10 )
    ege=( 03 )

    
    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction UAClock
         #/gamos/userAction GmCountProcessesUA
	 #/gamos/userAction UAWIF
         /gamos/userAction UAInteraction"
    
    for (( j=0; j<${#ege[@]}; j++ )) ; do
	for (( i=0; i<${#pre[@]}; i++ )) ; do
	    fge='el'${ege[j]}'eV.txt'
	    ((hgm=ege[j]+2))
	    GENMONO=$(gen e- ${ege[j]}*eV 0.5*cm -15*cm)
	    GEN=$(gen e- 1*eV 0.5*cm -15*cm $fge)
	    Dens=${pre[i]}*${mTorr}*${MM}'/('${Temp}*${KbNa}')'
	    CHB=":MIXT_BY_NATOMS PYRIDINE $Dens 3  C 5  H 5  N 1
                 $(coin PYRIDINE 9*cm 4*cm 0)"
	    #furfu
	    #MM=96.0842*g/mole
	    #Dens=${pre[i]}*${mTorr}*${MM}'/('${Temp}*${KbNa}')'
	    #CHB=":MIXT_BY_NATOMS FURFURAL $Dens 3  C 5  H 4  O 2
            #     $(coin FURFURAL 9*cm 4*cm 0)"
	    UMAT=":MATE USERMAT 1 1*g/mole 1e6*g/cm3
                  $(coin USERMAT  9*cm 1*cm -18*cm)"
	    HGS="/gamos/setParam   UAExit:EnergyMax ${hgm}*eV
                 /gamos/setParam   UAExit:EnergyBins ${hgm}00
                 /gamos/userAction UAExit"
	    RUN="/run/beamOn 1000000"
	    #RUN="/gamos/userAction UAWIF /run/beamOn 10000"
	    
	    #PAR="--host dirac --ppn 1 --jobs 1 --btime 4:29:00"
	    #PAR="--host euler --ppn 10 --jobs 250 --btime 4:29:00 --seed 1400 --SEED 1400"
	    #PAR="--jobs 10 --ppn 10"
	    DIR=${ege[j]}_${pre[i]}
	    jgamos $PAR --dir oor_${DIR} $WRL $CHB $UMAT $PHYsmf $GEN $UAS $HGS $RUN  &
	    jgamos $PAR --dir oon_${DIR} $WRL $CHB       $PHYsmf $GEN $UAS $HGS $RUN  &
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
