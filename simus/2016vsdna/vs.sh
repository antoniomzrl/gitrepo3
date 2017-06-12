#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

WRL=$(wrl 50*cm 50*cm 50*cm)

CUTS="#ProdCuts /cuts/setLowEdge 250 eV /run/setCut 1 nm
      /gamos/physics/setCuts ch_region 1e-6 1e-6 1e-6"
INI="/run/initialize"

#PHYg="/gamos/physicsList HadrontherapyPhysics $CUTS $INI"
PHYg="/gamos/physicsList GmEMPhysics $INI"
PHYd="/gamos/physicsList GmDNAPhysics         $CUTS $INI"
PHYl="/gamos/physicsList GmLEPTSPhysics       $INI"

UAS="#/gamos/userAction UAVerbose         
     #/gamos/userAction UAInteraction
     /gamos/userAction GmCountProcessesUA
     /gamos/setParam TimeMark 600
     /gamos/userAction UAClock
     /gamos/setParam UAInteractionSp:Title 1MeV-protons
     /gamos/setParam UAInteractionSp:x 300 0 30*um
     /gamos/userAction UAInteractionSp
     /gamos/userAction UAEloss

     /gamos/analysis/histo1Max *EnergyLost* 5*keV
     /gamos/analysis/histo1NBins *EnergyLost* 5000
     /gamos/analysis/histo2MaxX *KineticEnergy* 1.
     /gamos/analysis/histo2NBinsX *KineticEnergy* 200
     /gamos/analysis/histo2MaxY *KineticEnergy* 0.02
     /gamos/analysis/histo2NBinsY *KineticEnergy* 200
     /gamos/analysis/histo1Max *.prof.AccumulatedEnergyLost* 0.02
     /gamos/analysis/histo1NBins *.prof.AccumulatedEnergyLost* 1000
     /gamos/analysis/addFileFormat root
     /gamos/analysis/addFileFormat TXT
     #/gamos/setParam GmStepDataHistosUA:DataList FinalXS
     #/gamos/setParam GmStepDataHistosUA:DataList AccumulatedEnergyLost
     #/gamos/userAction GmStepDataHistosUA

     /gamos/setParam GmStepDataHistosUA_GmClassifierByProcess:DataList AccumulatedEnergyLost
     /gamos/userAction GmStepDataHistosUA GmClassifierByProcess
     "


GENp=$(gen proton 1*MeV 1*nm -1*cm)
GENe=$(gen e-     10*keV 1*nm -1*cm)
WL="G4_WATER"
CHB=$(coin $WL 1*cm 10*nm 5*nm)

#ULI="$(uli $WL e- 9.9*keV)"
CFLg="$WRL $CHB $PHYg $ULI"
CFLd="$WRL $CHB $PHYd $ULI"
CFLl="$WRL $CHB $PHYl $ULI"
#ULI="$(uli $WL proton 0.999999*MeV) $(uli $WL hydrogen 0.999999*MeV)"
#ULI="/gamos/userAction GmKillAtStackingActionUA GmElectronFilter"


if [ $1 == "jet1" ] ; then
    CHB="$(coin $WL 1*cm 1*cm 0.5*cm)"
    CFLl="$WRL $CHB $PHYl $UAS $ULI"
    RUN="/run/beamOn 1000"
    #time jlepts $CFLl $GENp $ULI $RUN /dir oopjet > kk

    ULI="$(uli $WL e- 1*keV)"

    GENe="$(gen e-  10*keV 1*nm -1*cm) /gamos/setParam UAInteractionSp:x 100 0 5*um"
    #GENe="$(gen e- 0.1*MeV 1*nm -1*cm) /gamos/setParam UAInteractionSp:x 100 0 500*um"
    #GENe="$(gen e-   1*MeV 1*nm -1*cm) /gamos/setParam UAInteractionSp:x 100 0 5*mm"
    UAS="/gamos/userAction UAInteractionSp
         /gamos/userAction UAInteraction
         /gamos/userAction UAClock"

    time jlepts $WRL $CHB $PHYg $GENe $UAS $ULI $RUN /version 51c /dir ooegc > kkgc &
    time jlepts $WRL $CHB $PHYd $GENe $UAS $ULI $RUN /version 51c /dir ooedc > kkdc &
    time jlepts $WRL $CHB $PHYl $GENe $UAS $ULI $RUN /version 51c /dir ooelc > kklc &

    time jlepts $WRL $CHB $PHYg $GENe $UAS $ULI $RUN /version 51f /dir ooegf > kkgf &
    time jlepts $WRL $CHB $PHYd $GENe $UAS $ULI $RUN /version 51f /dir ooedf > kkdf &
    time jlepts $WRL $CHB $PHYl $GENe $UAS $ULI $RUN /version 51f /dir ooelf > kklf &
    wait

elif [ $1 == "jet2" ] ; then
    CHB="$(coin $WL 1*cm 1*cm 0.5*cm)"
    CFLl="$WRL $CHB $PHYl $UAS $ULI"
    RUN="/run/beamOn 10"
    GENe="$(gen e-  10*keV 1*nm -1*cm)"
    UAS="/gamos/userAction UAWIF
         /gamos/userAction UAInteraction
         /gamos/userAction UAClock"

    time jlepts $WRL $CHB $PHYl $GENe $UAS $ULI $RUN /dir oo > kk &
    wait

    
elif [ $1 == "jet" ] ; then
    CHB="$(coin $WL 1*cm 1*cm 0.5*cm)"
    CFLl="$WRL $CHB $PHYl $UAS $ULI"
    RUN="/run/beamOn 10"
    #time jlepts $CFLl $GENp $ULI $RUN /dir oopjet > kk

    GENe="$(gen e-  10*keV 1*nm -1*cm) /gamos/setParam UAInteractionSp:x 100 0 5*um"
    UAS="/gamos/userAction UAInteraction
         /gamos/userAction UAClock
         /gamos/userAction UAWIF"

    #time jlepts $WRL $CHB $PHYl $GENe $UAS $RUN /version 51f /dir ooelf > kklf &
    #time jlepts $WRL $CHB $PHYd $GENe $UAS $RUN /version 51f /dir ooedf > kkdf &
    #time jlepts $WRL $CHB $PHYd $GENe $UAS $RUN /dir ood >& kkd &
    time jlepts $WRL $CHB $PHYl $GENe $UAS $RUN /dir oo  >& kk &
    wait

    
elif [ $1 == "prudistro" ] ; then
    DX="0.2*nm"
    #MT="G4_WATER"
    MT="PRUDISTRO"
    MAT=":MATE $MT 8 18*g/mole 1*g/cm3"
    CHB=$(coin $MT 1*cm ${DX} ${DX}/2)
    ULI="$(uli $MT e- 1.9999*keV)"
    GEN=$(gen e- 2*keV 1*nm -1*cm)
    UAS="#/gamos/userAction UAVerbose
         #/gamos/userAction UAWIF
         /gamos/userAction UAClock
         /gamos/userAction UAInteraction
         /gamos/analysis/histo1Max *EnergyLost* 1*keV
         /gamos/analysis/histo1NBins *EnergyLost* 1000
         /gamos/analysis/addFileFormat root
         /gamos/analysis/addFileFormat TXT
        #/gamos/setParam GmStepDataHistosUA:DataList AccumulatedEnergyLost
        #/gamos/userAction GmStepDataHistosUA
         /gamos/setParam GmStepDataHistosUA_GmClassifierByProcess:DataList AccumulatedEnergyLost
         /gamos/userAction GmStepDataHistosUA GmClassifierByProcess"
    RUN="/run/beamOn 10000000"
    CFL="$WRL $MAT $CHB $PHYl $UAS $ULI $GEN $RUN"
    #time jlepts $CFL /dir oosl > kksl
    plepts -h local -n 8 -N 8 -I 1001 -d oosl $CFL
    #plepts -h dirac -n 12 -N 100 -I 1001 -d oosl $CFL


elif [ $1 == "slab" ] ; then
    #RUN="$(vis) /run/beamOn 100"
    RUN="/run/beamOn 1000000"
    UAS="$UAS
         /gamos/userAction UAInteraction
         /gamos/setParam UAExit:EnergyMax 10*keV
         /gamos/setParam UAExit:EnergyBins 1000
         /gamos/userAction UAExit"

    time jlepts $CFLd $GENe $UAS $RUN /dir ooed > kked &
    time jlepts $CFLl $GENe $UAS $RUN /dir ooel > kkel &
    time jlepts $CFLg $GENe $UAS $RUN /dir ooeg > kkeg &
    wait

elif [ $1 == "stpw" ] ; then
    DX="0.2*nm"
    DENS="1*g/cm3"
    MM="18*g/mole"
    CHB=$(coin $WL 1*cm ${DX} ${DX}/2)
    UAS="/gamos/setParam STPW:Density  ${DENS}
         /gamos/setParam STPW:Length  ${DX}
         /gamos/setParam STPW:MolecularMass ${MM}
         /gamos/userAction UAInteraction
         /gamos/userAction UAClock"
    
    RUN="/run/beamOn 10000000"
    Eis="0.1 0.3 1.0 3 10 30 100 300 1e3"

    for Ei in ${Eis} ; do
	E="${Ei}*keV"
	GEN=$(gen e- ${E} 1*nm -1*cm)
	#time jlepts $WRL $CHB $PHYd $UAS $GEN $RUN /dir ood_${Ei} > kkd_${Ei} &
	jgamos  --dir ool_${Ei} $WRL $CHB $PHYl $UAS $GEN $RUN
    done
    wait



elif [ $1 == "vsphy" ] ; then
    CHB="$(coin $WL 1*cm 1*cm 0.5*cm)"
    GEN="$(gen e- 1.5*MeV 1*nm -1*cm)"
    RUN="/run/beamOn 1"
    UAS="/gamos/userAction UAInteraction
         /gamos/userAction GmCountProcessesUA
         /gamos/userAction UAClock
         /gamos/userAction UAWIF
         #/gamos/userAction UAVerbose
         /tracking/verbose 1"
    ULI="$(uli $WL e- 0.9*MeV)"
    
    PHY1="/gamos/physicsList GmEMPhysics      $INI"
    PHY2="/gamos/physicsList GmDNAPhysics     $INI"
    PHY3="/gamos/physicsList GmLEPTSPhysics   $INI"
    PHY4="/gamos/physicsList StandardAndDNAPhysics1 $INI"
    PHY5="/gamos/physicsList StandardAndDNAPhysics  $INI"
    PHY6="/gamos/physicsList GmEMSSDNAPhysics $INI"
    PHY7="/gamos/physicsList EMSSPhysics      $INI"

    #time jlepts $WRL $CHB $PHY1 $GEN $UAS $ULI $RUN /dir oo1 > kk1 &
    #time jlepts $WRL $CHB $PHY2 $GEN $UAS $ULI $RUN /dir oo2 > kk2 &
    #time jlepts $WRL $CHB $PHY3 $GEN $UAS $ULI $RUN /dir oo3 > kk3 &
    #time jlepts $WRL $CHB $PHY4 $GEN $UAS $ULI $RUN /dir oo4 > kk4 &

    #time jlepts $WRL $CHB $PHY5 $GEN $UAS $ULI $RUN /dir oo5 > kk5 &
    #time jlepts $WRL $CHB $PHY6 $GEN $UAS $ULI $RUN /dir oo6 > kk6 &
    time jlepts $WRL $CHB $PHY7 $GEN $UAS $ULI $RUN /dir oo7 > kk7 &
    wait
fi
