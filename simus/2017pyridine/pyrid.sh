#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

WRL=$(wrl 50*cm 50*cm 50*cm)
PHYold="/gamos/physicsList GmLEPTPhysics /run/initialize"
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
    


    
elif [ $1 == "ncol" ] ; then
    UAS="/gamos/userAction UAClock
         /gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         #/gamos/userAction UAWIF
         #/gamos/userAction UAVerbose
         /gamos/setParam   UAExit:EnergyMax 10*eV
         /gamos/setParam   UAExit:EnergyBins 100
         /gamos/userAction UAExit"
    
    GEN=$(gen e- 8*eV 0.2*um -1*cm)
    RUN="/run/beamOn 10000000"
    
    pre=( 1 0.5 0.1 0.01 )
    MT=FURFURAL_ELAST
    for (( i=0; i<${#pre[@]}; i++ )) ; do
	Dens=${pre[i]}*${mTorr}*${MM}'/('${Temp}*${KbNa}')'
	MAT=":MIXT_BY_NATOMS $MT $Dens 3  C 5  H 4  O 2"
	CHB="$(coin $MT 5*cm 24*cm 0)"

	#PAR="--jobs 10 --ppn 10"
	#PAR="--host euler --ppn 10 --jobs 250 --btime 4:29:00 --seed 1400 --SEED 1400"
	jgamos $PAR --dir ooels${i} $WRL $MAT $CHB $PHYsmf $GEN $UAS $RUN &
	jgamos $PAR --dir oodxs${i} $WRL $MAT $CHB $PHY    $GEN $UAS $RUN &
    done
    wait

    


    
elif [ $1 == "2reflex" ] ; then
    fge=( e02eV.txt e07eV.txt e10eV.txt e20eV.txt )
    ege=( 02         07        10        20       )
    pre=( 0.5       1.0  )

    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction UAClock
         #/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction"
    
    NP=1000000
    
    for (( j=0; j<${#ege[@]}; j++ )) ; do
	for (( i=0; i<${#pre[@]}; i++ )) ; do
	    #GENMONO=$(gen e- ${ege[j]}*eV 0.5*cm -15*cm)
	    GEN=$(gen e- 1*eV 0.5*cm -15*cm ${fge[j]})
	    ((hgm=ege[j]+2))
	    Dens=${pre[i]}*${mTorr}*${MM}'/('${Temp}*${KbNa}')'
	    CHBFUR=":MIXT_BY_NATOMS FURFURAL $Dens 3  C 5  H 4  O 2
                    $(coin FURFURAL 9*cm 24*cm 0)"
	    UMAT=":MATE USERMAT 1 1*g/mole 1e6*g/cm3
                  $(coin USERMAT  9*cm 1*cm -18*cm)"
	    HGS="/gamos/setParam   UAExit:EnergyMax ${hgm}*eV
                 /gamos/setParam   UAExit:EnergyBins ${hgm}00
                 /gamos/userAction UAExit"
	    RFLX="/gamos/setParam   UAReflex:RealNoPrimaries $NP
                  /gamos/userAction UAReflex
                  /run/beamOn 1000000000"
	    RUN="/run/beamOn $NP"
	    
	    #PAR="--host euler --ppn 10 --jobs 250 --btime 4:29:00 --seed 1400 --SEED 1400"
	    #PAR="--jobs 10 --ppn 10"
	    DIR=${ege[j]}_${pre[i]}
	    jgamos $PAR --dir oo_${DIR}_RA $WRL $CHBFUR       $PHYsmf $GEN $UAS $HGS $RFLX &
	    jgamos $PAR --dir oo_${DIR}_RR $WRL $CHBFUR $UMAT $PHYsmf $GEN $UAS $HGS $RUN  &
	done
    done
    wait



elif [ $1 == "diezev" ] ; then
    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction UAClock
         #/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction"
    
    GENMONO=$(gen e- 10*eV 0.5*cm -15*cm)
    GENDIST=$(gen e- 1*eV 0.5*cm -15*cm e10eV.txt)

    Dens=1.0*${mTorr}*${MM}'/('${Temp}*${KbNa}')'
    UMAT=":MATE USERMAT 1 1*g/mole 1e6*g/cm3
          $(coin USERMAT  9*cm 1*cm -18*cm)"
    HGS="/gamos/setParam   UAExit:EnergyMax 12*eV
         /gamos/setParam   UAExit:EnergyBins 1200
         /gamos/userAction UAExit"
    RUN="/run/beamOn 1000"

    for M in FURFURAL FURFURAL_DXS10 FURFURAL_NOROT FURFURAL_NOROT_DXS10 ; do
	CHB=":MIXT_BY_NATOMS $M $Dens 3  C 5  H 4  O 2
             $(coin $M 9*cm 24*cm 0)"
	#jgamos $PAR --dir oo_${DIR}svac $WRL               $PHYsmf $GENDIST $UAS $HGS $RUN  &
	#jgamos $PAR --dir oo_${DIR}mono $WRL $CHBFUR $UMAT $PHYsmf $GENMONO $UAS $HGS $RUN  &
	jgamos $PAR --dir oo_$M $WRL $CHB $UMAT $PHYsmf $GENDIST $UAS $HGS $RUN &
    done
    wait
    
fi

exit


IP  9.23 eV
MM  79.10*g/mole



Básicamente es lo mismo que hicimos en el último artículo de furfural. La distribución inicial de elctrones viene integrada porque simplemente la intensidad transmitida cuando aplicamos un potencial retardador. En el excel viene representado por el espectro a presión cero.

Después hacemos lo mismo introduciendo un par de valores de la presión, uno bajo en el que esperamos que no haya colisiones múltiples y otro más alto en el que éstas serán importantes.

Esto lo repetimos para tres energías, una muy baja (3 3V) en la que sólo habrá elásticos y rotacionales, otra media (10 eV) con excitaciones electrónicas (y también vibracionales pero por desgracia no tenemos datos para éstos, usaremos los de pirimidina) y la última por encima de la ionización (20 eV) por lo que también ionizarán.

Los datos que te envío son las secciones eficaces diferenciales e integrales calculadas y los espectros de pérdida de energía para excitación electrónica y para ionización.

Faltan las secciones eficaces vibracionales pero puedes utilizar las de pirimidina (lo mismo para la pérdida de energía vibracional y rotacional, usa los  valores de pirimidina).
