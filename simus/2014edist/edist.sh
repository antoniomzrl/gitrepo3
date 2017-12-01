#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

CUTS="#ProdCuts
      #/cuts/setLowEdge 1 eV
      #/run/setCut 1 cm
      #/gamos/physics/setCuts ch_region 1 1 1"
INI="/run/initialize"
PHYg="/gamos/physicsList HadrontherapyPhysics $CUTS $INI"
PHYd="/gamos/physicsList GmDNAPhysics         $CUTS $INI"
PHYl="/gamos/physicsList GmLEPTSPhysics       $INI"

WRL=$(wrl 20*cm 20*cm 20*cm)
Wg="G4_WATER"
Wl="LEPTS_WATER"

mTorr=atmosphere/760000
Pres=1.5*${mTorr}
Dens=$(dens 18*g/mole ${Pres})

MATg=":MATE $Wg 1.0 18*g/mole ${Dens}"
MATl=":MATE $Wl 1.0 18*g/mole ${Dens}"
CHBg=$(coin $Wg 15*cm 14*cm 7*cm)
CHBl=$(coin $Wl 15*cm 14*cm 7*cm)


#helix, 1 Torr 0.01 T:
if [ $1 == "setup" ] ; then
    #GEN=$(genang e- 2*eV 5*um -1*cm)
    #MAG="/gamos/field/setMagField 0.1*tesla 0 0"
    #Pres=1.5*${mTorr}
    GEN=$(genang e- 10*keV 5*um -1*cm)
    MAG="/gamos/field/setMagField 0.01*tesla 0 0"
    STP="/gamos/physics/userLimits/setMaxStep uliw ch_LEPTS_WATER e- 1*mm"
    Pres=1000*${mTorr}

    Dens=$(dens 18*g/mole ${Pres})

    MATg=":MATE $Wg 1.0 18*g/mole ${Dens}"
    MATl=":MATE $Wl 1.0 18*g/mole ${Dens}"
    CHBg=$(coin $Wg 15*cm 14*cm 7*cm)
    CHBl=$(coin $Wl 15*cm 14*cm 7*cm)

    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/userAction UAExit"
    ULIl=$(uli $Wl e- 0.5*eV)
    ULIg=$(uli $Wg e- 0.5*eV)

    RUN="/run/beamOn 10"
    CFL="$WRL $MATl $CHBl $PHYl $UAS $GEN $ULIl $(vis) $STP $MAG $RUN"
    jgamos --dir oosl $CFL
    CFL="$WRL $MATg $CHBg $PHYd $UAS $GEN $ULIg $(vis)      $MAG $RUN"
    time jlepts --dir oosd $CFL
fi



if [ $1 == "ang" ] ; then
    CHBg=$(coin $Wg 1*cm 1*mm 0)
    CHBl=$(coin $Wl 1*cm 1*mm 0)

    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/setParam UAExit:EnergyMax  120
         /gamos/setParam UAExit:EnergyBins 1200
         /gamos/userAction UAExit"
    ULIl=$(uli $Wl e- 0.5*eV)
    ULIg=$(uli $Wg e- 0.5*eV)
    GEN=$(gen e- 100*eV 1*nm -1*cm)
    RUN="/run/beamOn 10000000"
    CFL="$WRL $MATl $CHBl $PHYl $UAS $GEN $ULIl $RUN"
    time jlepts $CFL /dir oal >& kal &
    CFL="$WRL $MATg $CHBg $PHYd $UAS $GEN $ULIg $RUN"
    time jlepts $CFL /dir oad >& kad
fi



if [ $1 == "gen100eV" ] ; then
    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/setParam UAExit:EnergyMax  120
         /gamos/setParam UAExit:EnergyBins 1200
         /gamos/userAction UAExit"
    ULIl=$(uli $Wl e- 0.5*eV)
    ULIg=$(uli $Wg e- 0.5*eV)
    GEN=$(gen e- 100*eV 1*nm -1*cm)
    MAG="/gamos/field/setMagField 0.1*tesla 0 0"
    RUN="/run/beamOn 1000000"
    #RUN="/run/beamOn 1000000 /batchtime 2:00:00"
    CFL="$WRL $MATl $CHBl $PHYl $UAS $GEN $ULIl $MAG $RUN"
    time jlepts $CFL /dir osl >& ksl &
    CFL="$WRL $MATg $CHBg $PHYd $UAS $GEN $ULIg $MAG $RUN"
    time jlepts $CFL /dir osd >& ksd
    CFL="$WRL $MATg $CHBg $PHYg $UAS $GEN $ULg $MAG $RUN"
    #time jlepts $CFL /dir osg >& ksg
fi


if [ $1 == "casivac" ] ; then
    Pres=1e-10*${mTorr}
    Dens=$(dens 18*g/mole ${Pres})
    MATl=":MATE $Wl 1.0 18*g/mole ${Dens}"
    CHBl=$(coin $Wl 15*cm 14*cm 7*cm)

    UAS="/gamos/userAction UAClock /gamos/setParam TimeMark 10
         /gamos/userAction GmCountProcessesUA
         /gamos/analysis/histo1Max *Ener* 120*eV
         /gamos/analysis/histo1NBins *Ener* 120
         /gamos/userAction GmGenerHistosUA
         /gamos/setParam UAExit:EnergyMax  120
         /gamos/setParam UAExit:EnergyBins 120
         /gamos/userAction UAExit"
    ULIl=$(uli $Wl e- 1*eV)
    RUN="/run/beamOn 100000"

    GEN=$(gen e- 1*eV 1*nm -1*cm spectremis.txt interpolate)
    CFL="$WRL $MATl $CHBl $PHYl $UAS $GEN $ULIl $MAG $RUN"
    time jlepts $CFL /dir o1 > k1
    GEN=$(gen e- 1*eV 1*nm -1*cm spectremis.txt fixed)
    CFL="$WRL $MATl $CHBl $PHYl $UAS $GEN $ULIl $MAG $RUN"
    time jlepts $CFL /dir o2 > k2
fi


if [ $1 == "simu" ] ; then
    Pres=1.5*${mTorr}
    #Pres=0
    Dens=$(dens 18*g/mole ${Pres})
    MATg=":MATE $Wg 1.0 18*g/mole ${Dens}"
    MATl=":MATE $Wl 1.0 18*g/mole ${Dens}"
    CHBg=$(coin $Wg 15*cm 14*cm 7*cm)
    CHBl=$(coin $Wl 15*cm 14*cm 7*cm)

    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction UAClock /gamos/setParam TimeMark 10
         /gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/setParam UAExit:EnergyMax  120
         /gamos/setParam UAExit:EnergyBins 120
         /gamos/userAction UAExit"
    ULIl=$(uli $Wl e- 1*eV)
    ULIg=$(uli $Wg e- 1*eV)
    GEN=$(gen e- 1*eV 1*nm -1*cm spectremis.txt)
    RUN="/run/beamOn 10000"
    #RUN="/run/beamOn 10000 /batchtime 2:00:00"
    MAG="/gamos/field/setMagField 0.1*tesla 0 0"

    #for((i=0;i<100;i++)) ; do
	let sd=(i+1000)*100
	SEED="/gamos/random/setSeeds $sd $sd"
	CFL="$WRL $MATl $CHBl $PHYl $UAS $GEN $SEED $ULIl $MAG $RUN"
	#time jlepts $CFL /dir osl_${i} &
	CFL="$WRL $MATg $CHBg $PHYd $UAS $GEN $SEED $ULIg $MAG $RUN"
	time jlepts $CFL /dir osd_${i}
    #done
fi


exit

Te adjunto una distribución de energía de electrones tal como se obtiene en un espectrómetro de campo retardador (ya lo has visto otras veces, para cada energía del potencial retardador, eje x, se obtiene la intensidad d eelectrones que tienen una energía superior a ésta, es decir es la integral del espectro de energías. 

En una columnan está la distribución en vacío y en la otra con 1,5 mTorr ed agua. hay que hacer una prueba con LEPTS y GEANT4-DNA para comparar ambos. Es decir, dada la distribución en vacío, qué distribución obtiene cada programa cuando metemos 1.mTorr en la cámara de colisión. 

El sistema es de campo magnético intenso (PDF adjunto). Recuerda que los electrones van confinados por el campo por lo que no son dispersados sini que tienen un pérdida de energía dada por la proyección de la velocidad. Los que colisionan con un ángulo de más de noventa van hacia atrá con la pérdida correspondiente y son rebotados hacia alante al llegar de nuevo al filamento. Si tienes dudas lo l¡hblamos.
