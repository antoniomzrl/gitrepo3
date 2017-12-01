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
MM=96.0842*g/mole
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
    

    
if [ $1 == "mat" ] ; then
    UAS="#/gamos/userAction UAWIF
         /gamos/userAction UAInteraction
         /gamos/userAction UAClock"
    RUN="/run/beamOn 5"
    
    CHB1="$(coin G4_WATER 1*cm 1*cm 0.5*cm)"
    CHB2=":MIXT_BY_NATOMS H2O 1*g/cm3 2  O 1 H 2
         $(coin H2O 1*cm 1*cm 0.5*cm)"
    CHB3=":MIXT_BY_NATOMS FURFURAL 1.16*g/cm3 3  C 5  H 4  O 2
         $(coin FURFURAL 1*cm 1*cm 0.5*cm)"
    
    CHB4=":MIXT_BY_NATOMS FURFURAL_PRU 1.16*g/cm3 3  C 5  H 4  O 2
         $(coin FURFURAL_PRU 1*cm 1*cm 0.5*cm)"
    CHB5=":MIXT_BY_NATOMS FURFURAL_ELD20 1.16*g/cm3 3  C 5  H 4  O 2
         $(coin FURFURAL_ELD20 1*cm 1*cm 0.5*cm)"
    CHB6=":MIXT_BY_NATOMS FURFURAL_ISO100 1.16*g/cm3 3  C 5  H 4  O 2
         $(coin FURFURAL_ISO100 1*cm 1*cm 0.5*cm)"

    jgamos --dir oo1 $WRL $CHB1 $PHY $GEN $UAS $ULI $RUN &
    jgamos --dir oo2 $WRL $CHB2 $PHY $GEN $UAS $ULI $RUN &
    jgamos --dir oo3 $WRL $CHB3 $PHY $GEN $UAS $ULI $RUN &
    #jgamos --dir oo4 $WRL $CHB4 $PHY $GEN $UAS $ULI $RUN &
    #jgamos --dir oo5 $WRL $CHB5 $PHY $GEN $UAS $ULI $RUN &
    #jgamos --dir oo6 $WRL $CHB6 $PHY $GEN $UAS $ULI $RUN &
    wait

    
elif [ $1 == "hgsp" ] ; then
    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/setParam TimeMark 600
         /gamos/userAction UAClock
         /gamos/setParam UAInteractionSp:Title 10_KeV_electr_furfural
         /gamos/setParam UAInteractionSp:x 300 0*um 3*um
         /gamos/userAction UAInteractionSp"

    MATS="FURFURAL FURFURAL_ELD120 FURFURAL_ELD0 FURFURAL_ISO100 FURFURAL_ISO3k"
    for mat in $MATS ; do
	CHB=":MIXT_BY_NATOMS $mat 1.16*g/cm3 3  C 5  H 4  O 2
             $(coin $mat 1*cm 1*cm 0.5*cm)"
	CFL="$WRL $CHB $PHY $UAS $ULI $GEN"

	time jlepts $CFL /gamos/userAction UAWIF /run/beamOn 10 /dir oo_$mat > kk_$mat &
	#plepts -h euler -n 8 -N 100 -I 1001 -d oo_$mat $CFL /run/beamOn 1000
    done

    wait
    #hgm.sh tabjoin hgsp1e5.root

    
elif [ $1 == "txs" ] ; then
    DX="14*cm"
    #Pres=0.5*${mTorr}
    #Dens=${Pres}*${MM}'/('${Temp}*${KbNa}')'
    Dens=2.55932*mg/m3 
    MAT=":MIXT_BY_NATOMS FURFURAL $Dens 3  C 5  H 4  O 2"
    GEN=$(gen e- 20*eV 0.5*cm -10*cm)
    CHB="$(coin FURFURAL 5*cm $DX 0)"
    
    UAS="#/gamos/userAction UAWIF
         #/gamos/userAction UAVerbose
         /gamos/userAction UAClock
         /gamos/setParam STPW:Density ${Dens}
         /gamos/setParam STPW:Length  ${DX}
         /gamos/setParam STPW:MolecularMass $MM
         /gamos/userAction UAInteraction"
    #ULI="$(uli FURFURAL e- 0.12*eV)"
    RUN="/run/beamOn 1000000"
    jgamos $PAR --dir ootxs $WRL $MAT $CHB $PHYsmf $GEN $UAS $ULI $RUN


elif [ $1 == "dxs1" ] ; then
    Pres=10*${mTorr}
    Dens=${Pres}*${MM}'/('${Temp}*${KbNa}')'
    MAT=":MIXT_BY_NATOMS FURFURAL $Dens 3  C 5  H 4  O 2"
    CHB="$(coin FURFURAL 5*cm 50*um 0)"
    GEN=$(gen e- 20*eV 0.2*um -1*cm)
    RUN="/run/beamOn 2000000"   
    
    jgamos --dir oodxs1 $WRL $MAT $CHB $PHY    $GEN $UAS $ULI      $RUN &
    jgamos --dir oodxs2 $WRL $MAT $CHB $PHYsmf $GEN $UAS $ULI      $RUN &
    jgamos --host euler --btime 4:0:00 --dir oodxs2b $WRL $MAT $CHB $PHYsmf $GEN $UAS $ULI $RUN &
    jgamos --host dirac --btime 4:0:00 --dir oodxs2c $WRL $MAT $CHB $PHYsmf $GEN $UAS $ULI $RUN &
    #jgamos --dir oodxs3 $WRL $MAT $CHB $PHY   $GEN $UAS $ULI $MAG $RUN &
    wait


    
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
    RUN="/run/beamOn 100000"

    #for M in FURFURAL FURFURAL_DXS10 FURFURAL_NOROT FURFURAL_NOROT_DXS10 ; do
    for M in FURFURAL ; do
	CHB=":MIXT_BY_NATOMS $M $Dens 3  C 5  H 4  O 2
             $(coin $M 9*cm 24*cm 0)"
	#jgamos $PAR --dir oo_${DIR}svac $WRL               $PHYsmf $GENDIST $UAS $HGS $RUN  &
	#jgamos $PAR --dir oo_${DIR}mono $WRL $CHBFUR $UMAT $PHYsmf $GENMONO $UAS $HGS $RUN  &
	jgamos $PAR --dir oo_$M $WRL $CHB $UMAT $PHYsmf $GENDIST $UAS $HGS $RUN &
    done
    wait
    
fi

exit


e envío las distribuciones de energía de los haces de electrones. Tienen 2, 8 y 20 eV.
Estos haces tienen que atravesar un cámara de 140 mm de larga
a una temperatura de 301 K
en la que hay un campo magnético intenso cuyo efecto es impedir que se deflecten por
lo que después de la colisión todos siguen hacia delante pero tienen una pérdida adicional
de energía igual al producto de la energía después de la colisión por el coseno
del ángulo de dispersión.

La presión de furfural en la cámara es de:
3,253 mTorr  para el haz de 2 eV
2,264 mTorr para el de 8 eV
3,144 para el de 20 mTorr

Se trata de obtener el espectro a la salida de la cámara.
Ten en cuenta que los que tengan una dispersión mayor de 90º salen hacia atrás y al volver al punto de entrada son repelidos por el potencial acelerador y enviados de nuevo a la salida. Aunque el experimento es pulsado, la anchura del impulso es de 1 milisegundo por lo que a todos los que salgan hacia atrás les da tiempo a volver y ser detectados. Haz la simulación con buena estadística (son relativamente cortas)
Envíame el primer resultado cuando lo tengas para ver cómo va la estadística.


presiones (mTorr):   3.253     2.264     3.144
densidades(ng/cm3): 16.65      11.59    16.093
          (mg/m3)

pres 1 atm --> dens 3.9e+06(mg/m3) = 3.9 (Kg/m3)
aire a 1 atm 1,29 (Kg/m3)



¿Podrías hacer de nuevo una simulación con furfural para 2 y 10 eV?.
Te adjunto la distribución inicial (integrada) de electrones.

La última vez nos quedamos atascados con el tema de los que se dan la vuelta debido al campo magnético. Recuerda que suponemos que el campo es muy intenso y no deflectan sino que tienen un pérdida de energía hacia alante (si el ángulo es menor de 90) o hacia atrás si es mayor de 90.

Los que van hacia atrás se encuentran de nuevo el potencial acelerador al llegar al extremos y son acelerados de nuevo. Aquí nos entró la duda de si recuperaban toda la energía que tenían inicialmente o si sólo parte de ella. Esto realmente depende del campo eléctrico real que vean en el extremo. Vamos a suponer, inicialmente, que es uniforme y por lo tanto recuperan la energía inicial.

En ese caso los que lleguen al extremo son relanzados con la distribución inicial y así sucesivamente hasta que todos salgan por el extremo del detector. La longitud de la cámara es de 240 mm y en principio vamos a considerar dos presiones 0,5 y 1 mTorr. 
