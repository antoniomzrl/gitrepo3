#!/bin/bash

source $HOME/lepts/bin/gdefs.sh


WRL=$(wrl 50*cm 50*cm 50*cm)
Wl="WATER_LIQ"
MATg=":MATE $Wg 1.0 18*g/mole 1*g/cm3"
MATl=":MATE $Wl 1.0 18*g/mole 1*g/cm3"
CHBg=$(coin $Wg 100*cm 100*cm 50*cm)
CHBl=$(coin $Wl 100*cm 100*cm 50*cm)
CUTS="#ProdCuts /cuts/setLowEdge 250 eV /run/setCut 1 nm
      /gamos/physics/setCuts ch_region 1e-6 1e-6 1e-6"
INI="/run/initialize"

PHYg="/gamos/physicsList HadrontherapyPhysics $CUTS $INI"
PHYd="/gamos/physicsList GmDNAPhysics         $CUTS $INI"
PHYl="/gamos/physicsList GmLEPTSPhysics       $INI"

GENp=$(gen proton 10*MeV 1*um -1*cm)
GENe=$(gen e-     20*keV 5*um -1*cm)

ULIG="$(uli $Wl e- 10*eV)
      $(uli $Wg proton 1*eV)
      $(uli $Wg hydrogen 1*eV)"


UAS="#/gamos/userAction UAWIF
     #/gamos/userAction UAVerbose
     /gamos/setParam TimeMark 10
     /gamos/userAction UAClock
     /gamos/analysis/histo1Max *Ener* 50*keV
     /gamos/analysis/histo1NBins *Ener* 500
     /gamos/userAction GmGenerHistosUA
     /gamos/userAction UAInteraction
     /gamos/userAction GmCountProcessesUA"

SECO="/gamos/analysis/histo2MaxX *Ener* 50*keV
      /gamos/analysis/histo2NBinsX *Ener* 100000
      #seco
      /gamos/setParam   GmSecondaryTrackDataHistosUA:FileName seco
      /gamos/setParam   GmSecondaryTrackDataHistosUA:DataList SecoKineticEnergy
      /gamos/userAction GmSecondaryTrackDataHistosUA
      #only_seco_e-
      /gamos/filter secoF GmOnSecondaryFilter GmElectronFilter
      /gamos/setParam GmSecondaryTrackDataHistosUA_secoF:FileName secoel
      /gamos/setParam GmSecondaryTrackDataHistosUA_secoF:DataList SecoKineticEnergy
      #SecoKineticEnergy.vs.InitialPrimKineticEnergy
      /gamos/userAction GmSecondaryTrackDataHistosUA secoF"



if [ $1 == "jet" ] ; then
    WRL=$(wrl 50*cm 50*cm 50*cm)
    PHY="/gamos/physicsList GmLEPTSPhysics /run/initialize"
    PHY2="/gamos/physicsList GmLeptsPhysics /run/initialize"
    GEN=$(gen proton 10*keV 0.2*um -1*cm)
    UAS="/gamos/userAction UAWIF
         /gamos/userAction UAInteraction
         /gamos/userAction UAClock"
    RUN="/run/beamOn 5"
    CHB="$(coin G4_WATER 1*cm 1*cm 0.5*cm)"
    jgamos --dir ooj  $WRL $CHB $PHY  $GEN $UAS $ULI $RUN &
    jgamos --dir ooj2 $WRL $CHB $PHY2 $GEN $UAS $ULI $RUN &
    wait

    
#g4/dna sencondaries:
elif [ $1 == "seco" ] ; then
    UAS="#/gamos/userAction UAVerbose
         #/gamos/userAction UAWIF
         /gamos/analysis/histo1Max *Ener* 1*MeV
         /gamos/analysis/histo1NBins *Ener* 1000
         /gamos/userAction GmGenerHistosUA
         /gamos/userAction GmCountProcessesUA
         $SECO"

    ULI="$(uli $Wg e- 99*MeV)"
    RUN="/run/beamOn 10000"

    #prot 60*MeV penetr 20 mm
    GENp=$(gen proton 1*MeV 1*um -1*cm)
    # coin 1mm
    CHBg=$(coin $Wg 1*mm 1*mm 0.5*mm)

    # protons G4
    CFL="$WRL $MATg $CHBg $PHYg $UAS $(cofi contfil.txt) $GENp $ULI $RUN"
    time jlepts $CFL /dir osg >& ksg
    # protons DNA
    CFL="$WRL $MATg $CHBg $PHYd $UAS $(cofi contfil.txt) $GENp $ULI $RUN"
    time jlepts $CFL /dir osd >& ksd


elif [ $1 == "conti" ] ; then
    GENp=$(gen proton 60*MeV 100*um -1*cm)
    CHBg=$(coin $Wg 1*cm 100*um 50*um)
    CHBl=$(coin $Wl 1*cm 10*cm 5*cm)

    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction UAWIF
         /gamos/userAction UAInteraction
         /gamos/userAction GmCountProcessesUA"
    ULI="$(uli $Wg e- 99*MeV)"
    RUN="/run/beamOn 100000"

    # protons
    CFL="$WRL $MATg $CHBg $PHYg $UAS $(cofi contfil.txt) $GENp $ULI $RUN"
    time jlepts $CFL /dir oc /jn prot >& kp

    # clip
    #clipgf -i oc/contfil.txt -o oc/contfil2.txt -r 5 5.2 -9 9 -9 9

    # Continu e-
    N=$(cat oc/contfil.txt | wc -l)
    echo $N

    RUN="/run/beamOn $N"
    GEN="/gamos/setParam GmGeneratorFromTextFile:FileName contfil.txt
         /gamos/generator GmGeneratorFromTextFile"
    ULI=$(uli $Wl e- 1000*eV)
    CFL="$WRL $MATl $CHBl $PHYl $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir oc /jn elec >& ke

    RUN="/run/beamOn $Nb"
    GEN="/gamos/setParam GmGeneratorFromTextFile:FileName contfil2.txt
         /gamos/generator GmGeneratorFromTextFile"
    ULI=$(uli $Wl e- 10*eV)
    CFL="$WRL $MATl $CHBl $PHYl $UAS $GEN $ULI $RUN"
    #time jlepts $CFL /dir oc /jn elecb >& keb


elif [ $1 == "01mm" ] ; then
    GENp=$(gen proton 60*MeV 1*nm -1*cm)
    CHBg=$(coin $Wg 10*cm 0.1*mm 0.1/2*mm)
    CHBl=$(coin $Wl 10*cm 0.1*mm 0.1/2*mm)
    #CHBl=$(coin $Wl 10*cm 10*cm 5*cm)

    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction UAWIF
         /gamos/userAction UAInteraction
         /gamos/userAction GmCountProcessesUA"

    ULI="$(uli $Wg e- 99*MeV)"
    RUN="/run/beamOn 1000"

    # protons
    CFL="$WRL $MATg $CHBg $PHYg $UAS $(cofi contfil.txt) $GENp $ULI $RUN"
    time jlepts $CFL /dir oc /jn prot >& kp

    N=$(cat oc/contfil.txt | wc -l)
    echo $N
    RUN="/run/beamOn $N"
    GEN="/gamos/setParam GmGeneratorFromTextFile:FileName contfil.txt
         /gamos/generator GmGeneratorFromTextFile"
    ULI=$(uli $Wl e- 1*eV)
    CFL="$WRL $MATl $CHBl $PHYl $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir oc /jn Aelec >& ke

    ULI=$(uli $Wl e- 0.4*eV)
    CFL="$WRL $MATl $CHBl $PHYl $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir oc /jn Belec >& ke
fi



UASPEDRO="/gamos/analysis/histo1Max *Pos* 0.005
         /gamos/analysis/histo1Min *Pos* 0.
         /gamos/analysis/histo2MaxX *Pos* 0.005
         /gamos/analysis/histo2MaxY *Pos* 0.005
         /gamos/analysis/histo2MinX *Pos* 0.
         /gamos/analysis/histo2MinY *Pos* 0.
         /gamos/filter protonF GmParticleFilter proton 
         /gamos/setParam GmStepDataHistosUA_GmElectronFilter:DataList FinalPosPerp FinalPosPerp.vs.FinalPosZ
         /gamos/setParam GmStepDataHistosUA_GmElectronFilter:FileName leptsProton
         /gamos/setParam GmStepDataHistosUA_protonF:DataList FinalPosPerp FinalPosPerp.vs.FinalPosZ log10(FinalPosPerp+1.e-10)
         /gamos/setParam GmStepDataHistosUA_protonF:FileName leptsProton
         /gamos/userAction GmStepDataHistosUA GmElectronFilter
         /gamos/userAction GmStepDataHistosUA protonF
         /gamos/analysis/histo1Min *log10* -8
         /gamos/analysis/histo1Max *log10* 0
         /gamos/analysis/histo1Max *:Tr* 0.005
         /gamos/analysis/histo1Min *log10(InitialK* -8
         /gamos/analysis/histo1Max *log10(InitialK* 0
         /gamos/setParam GmTrackDataHistosUA_GmElectronFilter:DataList log10(TrackLength+1.e-10) TrackLength log10(InitialKineticEnergy)
         /gamos/setParam GmTrackDataHistosUA_GmElectronFilter:FileName leptsProton
         /gamos/userAction GmTrackDataHistosUA GmElectronFilter"




echo "------------------------------>" $1

if [ $1 == "artipedro" ] ; then
    CUTS="#ProdCuts
          /cuts/setLowEdge 1 eV
          /run/setCut 1e-6 nm
          /gamos/physics/setCuts ch_region 1e-6 1e-6 1e-6"

    GENp=$(genline proton 1*MeV 0.2*um -1*cm)
    GENp=$(genline proton 1*MeV 0.2*um -1*cm)
    CHBg=$(coin $Wg 10*mm 10*mm 5*mm)
    CHBl=$(coin $Wl 10*mm 10*mm 5*mm)
    #CHBg=$(coin $Wg 10*mm 2*um 0)
    #CHBl=$(coin $Wl 10*mm 2*um 0)

    UAS="$UASPEDRO
         #/gamos/userAction UAVerbose
         /gamos/userAction UAWIF
         /gamos/userAction UAInteraction
         /gamos/userAction GmCountProcessesUA"


    #proton
    RUN="/run/beamOn 1"

    #proton g4
    CFLp="$WRL $MATg $CHBg $PHYg $UAS $GENp $ULI2 $RUN"
    time jlepts $CFLp /dir opg /jn protgeant >& kpg
    #cat opg/protgeant1.bcd > 1protg4-1MeV.bcd

    PHYg2="$PHYg
          /gamos/physics/removeProcessesByName msc
          /gamos/physics/addPhysics coulombScattering"
    CFLp="$WRL $MATg $CHBg $PHYg2 $UAS $GENp $ULI2 $RUN"
    #time jlepts $CFLp /dir opg2 /jn protg2 >& kpg2

    #proton dna
    ULI="$(uli $Wg e- 99*MeV)"
    CFLp="$WRL $MATg $CHBg $PHYd $UAS $(cofi contfil.txt) $GENp $ULI $RUN"
    time jlepts $CFLp /dir op /jn prot >& kp

    #electr conti
    N=$(cat op/contfil.txt | wc -l)
    echo 'NL =' $N

    RUN="/run/beamOn $N"
    GEN="/gamos/setParam GmGeneratorFromTextFile:FileName contfil.txt
         /gamos/generator GmGeneratorFromTextFile"
    ULI2="$(uli $Wl e- 0.2*eV)"
    CFL="$WRL $MATl $CHBl $PHYl $UAS $GEN $ULI2 $RUN"
    time jlepts $CFL /dir op /jn elec >& ke

    #cat op/elec1.bcd op/prot1.bcd > 1protlepts-1MeV.bcd

elif [ $1 == "dxs" ] ; then
    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/userAction UAClock
         /gamos/setParam TimeMark 5
         /gamos/userAction UAWIF
         #/gamos/userAction UAVerbose"

    GEN=$(gen proton 10*MeV 1*nm -1*cm)

    WL="WATER_LIQ"
    MAT=":MATE $WL 1.0 18*g/mole 1*g/cm3"
    CHB="$(coin $WL 5*um 5*um 0)"
    ULI="$(uli $WL e- 1*GeV)"

    RUN="/run/beamOn 1000"
    
    CFL="$WRL $MAT $CHB $PHYl $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir oop >& kkp


elif [ $1 == "jetslab" ] ; then
    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/userAction UAClock
         /gamos/setParam TimeMark 5
         /gamos/userAction UAWIF
         #/gamos/userAction UAVerbose"

    GEN=$(gen proton 1*MeV 9*um -1*cm)

    WL="G4_WATER"
    MAT=":MATE $WL 1.0 18*g/mole 1*g/cm3"
    CHB="$(coin $WL 5*mm 4*um 0)"
    ULI="$(uli $WL e- 1*eV)"

    RUN="/run/beamOn 10"
    SEED="/gamos/random/setSeeds 3 3"
    CFL="$WRL $MAT $CHB $PHYl $UAS $GEN $ULI $SEED $RUN"
    time jlepts $CFL /dir oop >& kkp


elif [ $1 == "1p" ] ; then
    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/userAction UAClock
         /gamos/setParam TimeMark 5
         #/gamos/userAction UAVerbose
         /gamos/userAction UAWIF"
    GEN=$(gen proton 2*keV 20*nm -1*cm)

    WL="G4_WATER"
    MAT=":MATE $WL 1.0 18*g/mole 1*g/cm3"
    CHB="$(coin $WL 1*cm 1*cm 0.5*cm)"
    ULI="$(uli $WL e- 0.5*eV)"
    
    RUN="/run/beamOn 5"
    SEED="/gamos/random/setSeeds 333 35"
    CFL="$WRL $MAT $CHB $PHYl $UAS $GEN $ULI $SEED $RUN"
    time jlepts $CFL /dir oop >& kkp

elif [ $1 == "elec" ] ; then
    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/userAction UAClock
         /gamos/setParam TimeMark 5
         #/gamos/userAction UAVerbose
         #/gamos/userAction UAWIF"
    GEN=$(gen e- 50*eV 20*nm -1*cm)

    WL="G4_WATER"
    MAT=":MATE $WL 1.0 18*g/mole 1*g/cm3"
    CHB="$(coin $WL 1*cm 1*cm 0.5*cm)"
    #ULI="$(uli $WL e- 10*eV)"
    
    RUN="/run/beamOn 1000"
    CFL="$WRL $MAT $CHB $PHYl $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir ooe >& kke

    
elif [ $1 == "rnd" ] ; then
    UAS="$UAS /gamos/userAction UAInteraction"
    GEN=$(gen proton 10*keV 1*nm -1*cm)
    RUN="/run/beamOn 1"
    #RND="/gamos/random/setEngine MTwistEngine"
    SEED="$RND /gamos/random/setSeeds 1001 1001"
    time jlepts $WRL $MAT $CHB $PHYl $UAS $GEN $ULI $SEED $RUN /dir oor1 >& kkr1 &
    SEED="$RND /gamos/random/setSeeds 1002 1001"
    time jlepts $WRL $MAT $CHB $PHYl $UAS $GEN $ULI $SEED $RUN /dir oor2 >& kkr2 &
    wait


elif [ $1 == "pru" ] ; then
    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction UAWIF
         /gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteractionSp
         /gamos/setParam TimeMark 1
         /gamos/userAction UAClock"
    WL="WATER_LIQ"
    MAT=":MATE $WL 1.0 18*g/mole 1*g/cm3"
    CHB=$(chb $WL 1*cm 1*cm 1*cm 1*cm)
    GEN=$(gen proton 0.1*MeV 1*nm -1*cm)
 
    RUN="/run/beamOn 1"
    CFL="$WRL $MAT $CHB $PHYl $UAS $GEN $ULI $SEED $RUN"
    time jlepts $CFL /dir oop >& kkp

    

elif [ $1 == "hgseco" ] ; then
    ANA="/gamos/analysis/histo1Max  *Ener* 1.5*keV
         /gamos/analysis/histo1Min  *Pos* 0
         /gamos/analysis/histo1Max  *Pos* 60*um
         /gamos/analysis/histo2MaxY *Ener* 1.5*keV
         /gamos/analysis/histo2MaxX *Pos* 60*um
         /gamos/analysis/histo2MinX *Pos* 0"

    SECO="/gamos/setParam GmTrackDataHistosUA_GmElectronFilter_GmClassifierByCreatorProcess:FileName sec
          /gamos/setParam GmTrackDataHistosUA_GmElectronFilter_GmClassifierByCreatorProcess:DataList
                 InitialKineticEnergy                 InitialPosX
                 InitialPosX.vs.InitialKineticEnergy  InitialPosX.prof.InitialKineticEnergy
          /gamos/userAction GmTrackDataHistosUA GmElectronFilter GmClassifierByCreatorProcess"

    UAS="$SECO $ANA
         /gamos/setParam TimeMark 10 /gamos/userAction UAClock
         /gamos/setParam UAInteractionSp:Title 1MeV-protons
         /gamos/setParam UAInteractionSp:x 600 0 60*um
         /gamos/userAction UAInteractionSp
         /gamos/userAction GmCountProcessesUA
         #/gamos/userAction UAInteraction"
    WL="WATER_LIQ"
    MAT=":MATE $WL 1.0 18*g/mole 1*g/cm3"
    CHB=$(chb $WL 1*cm 1*cm 1*cm 1*cm)
    GEN=$(gen proton 1*MeV 1*nm -1*cm)
    #ULI="/gamos/userAction GmKillAtStackingActionUA GmElectronFilter"
    ULI="$(uli $WL e- 99*MeV)"
    
    RUN="/run/beamOn 1000"
    CFL="$WRL $MAT $CHB $PHYl $UAS $SECO $GEN $ULI $SEED $RUN"
    time jlepts $CFL /dir ooseco >& kkseco
    #plepts -h local -n 10 -N 10 -I 1001 -d oosec $CFL
    #plepts -h dirac -n 12 -N 100 -I 1001 -d oosec -t 1:00:00 $CFL
    #plepts -h euler -n 8 -N 100 -I 1001 -d oosec -t 1:00:00 $CFL


elif [ $1 == "pjet" ] ; then
    UAS="#/gamos/userAction UAVerbose
         /gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/userAction UAClock
         /gamos/setParam TimeMark 5
         /gamos/userAction UAWIF"
    
    WL="WATER_LIQ"
    MAT=":MATE $WL 1.0 18*g/mole 1*g/cm3"
    CHB="$(coin $WL 1*cm 1*cm 0.5*cm)"
    ULI="/gamos/userAction GmKillAtStackingActionUA GmElectronFilter"
    GEN=$(gen proton 1*MeV 0.5*um -1*cm)

    RUN="/run/beamOn 50"
    SEED="/gamos/random/setSeeds 3 3"
    CFL="$WRL $MAT $CHB $PHYl $UAS $SEED $GEN $ULI $RUN"
    #time jlepts $CFL /dir oop >& kkp &

    SEED="/gamos/random/setSeeds 3 3"
    GEN=$(gen proton 0.1*MeV 2*um -1*cm)
    #ULI="$(uli $WL proton 11*eV) $(uli $WL hydrogen 11*eV)"
    ULI=
    RUN="/run/beamOn 5"
    CFL="$WRL $MAT $CHB $PHYl $UAS $GEN $ULI $RUN"
    time jlepts $CFL /dir oope >& kkpe &

    wait


elif [ $1 == "hgsp" ] ; then
    UAS="#/gamos/userAction UAVerbose
         #/gamos/userAction UAWIF
         #/gamos/userAction GmCountProcessesUA
         #/gamos/userAction UAInteraction
         /gamos/setParam TimeMark 600
         /gamos/userAction UAClock
         /gamos/setParam UAInteractionSp:Title 1MeV-protons
         /gamos/setParam UAInteractionSp:x 2000 0 10*um
         /gamos/userAction UAInteractionSp"
    WL="G4_WATER"
    #MAT=":MATE $WL 1.0 18*g/mole 1*g/cm3"
    CHB=$(chb $WL 1*cm 1*cm 1*cm 1*cm)
    GEN=$(gen proton 250*keV 1*nm -1*cm)
    RUN="/run/beamOn 500"
    CFL="$WRL $MAT $CHB $PHYl $UAS $GEN"

    #ULIK="/gamos/userAction GmKillAtStackingActionUA GmElectronFilter"
    #time jlepts $CFL $ULIK /run/beamOn 10 /dir oo >& kk 
    #plepts -h local -n 8 -N 8 -I 1101 -d oo $CFL /run/beamOn 5

    #plepts -h euler -n 8 -t 12:00:00 -N 1000 -I 1000 -i 1000 -d ooprot1 $CFL $RUN
    #plepts -h euler -n 8 -t 12:00:00 -N 1000 -I 2000 -i 2000 -d ooprot2 $CFL $RUN
    #plepts -h euler -n 8 -t 12:00:00 -N 1000 -I 3000 -i 3000 -d ooprot3 $CFL $RUN
    #plepts -h euler -n 8 -t 12:00:00 -N 1000 -I 4000 -i 4000 -d ooprot4 $CFL $RUN
    #plepts -h euler -n 8 -t 12:00:00 -N 1000 -I 5000 -i 5000 -d ooprot5 $CFL $RUN
    plepts -h euler -n 8 -t 12:00:00 -N 1000 -I 6000 -i 6000 -d ooprot6 $CFL $RUN


    exit

    
    time hadd hgsp1.root ooprot1/hgsp*root
    time hadd hgsp2.root ooprot2/hgsp*root
    time hadd hgsp3.root ooprot3/hgsp*root
    time hadd hgsp4.root ooprot4/hgsp*root
    time hadd hgsptot.root ghsp1.root ghsp2.root ghsp3.root ghsp4.root
    hgm.sh rebin   hgsptot.root 10
    hgm.sh tabjoin rbhgsptot.root

fi


exit

    
k1pc   : GamosRandom: sees before nTries 140731638045568 72197726691653200 1001
k2pc   : GamosRandom: sees before nTries 140720744836592 72197959993222736 1001

k1dirac: GamosRandom: sees before nTries 140737263929472 214746351141      1001
k2dirac: GamosRandom: sees before nTries 140734926265808 214746351141      1001

k1euler: GamosRandom: sees before nTries        11855840 263738927522      1001
k2euler: GamosRandom: sees before nTries        11855840 263738927522      1001

