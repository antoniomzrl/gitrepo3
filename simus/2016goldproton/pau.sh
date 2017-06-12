#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

VAC=":MATE vacuum 1 1*g/mole 1e-25*g/cm3"
WRL=$(wrl 5*cm 5*cm 5*cm)
COIN=$(coin G4_Au 1*mm 100*um 0)


GENflat=":VOLU   gbox ORB 10*um/2 vacuum
         :PLACE  gbox 1 world rmz -2*mm 0 0  :COLOUR gbox 0 0 1
         /gamos/generator GmGenerator
         /gamos/generator/addSingleParticleSource gn proton 1*MeV
         /gamos/generator/positionDist gn GmGenerDistPositionInG4Volumes gbox
         /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0
         /gamos/generator/energyDist gn GmGenerDistEnergyRandomFlat 0 100*keV"

GENp=$(gen proton 1*MeV 1*nm -1*cm)

#INI="/run/setCut 0.1 um /run/initialize"

# /gamos/physicsList HadronTherapyPhysics
# defaults:G4EmStandardPhysics_option3, G4DecayPhysics and G4RadioactiveDecayPhysics.
# This default physics can be changed with the command
# /HT/Physics/replacePhysics PHYSICS_LIST
# where PHYSICS_LIST can be
# • standard_opt3 : G4EmStandardPhysics_option3(
# • LowE_Livermore : G4EmLivermorePhysics
# • LowE_Penelope :G4EmPenelopePhysics
# • local_ion_ion_inelastic: LocalIonIonInelasticPhysics
# • QGSP_BIC_EMY: QGSP_BIC_EMY
# This list has several commands to select cuts:
# /HT/Physics/setCuts VALUE UNIT : set the cuts for all volumes and particles
# /HT/Physics/setGCut VALUE UNIT : set the cuts for gammas for all volumes
# /HT/Physics/setECut VALUE UNIT : set the cuts for electrons for all volumes
# /HT/Physics/setPCut VALUE UNIT : set the cuts for positrons for all volumes

HGSECO="/gamos/analysis/histo1Max   *Energy* 10*keV
        /gamos/analysis/histo1NBins *Energy* 100
        /gamos/filter secoeF GmOnSecondaryFilter GmElectronFilter
        /gamos/setParam GmSecondaryTrackDataHistosUA_secoeF:FileName secoe
        /gamos/setParam GmSecondaryTrackDataHistosUA_secoeF:DataList SecoKineticEnergy
        #SecoKineticEnergy.vs.InitialPrimKineticEnergy
        /gamos/userAction GmSecondaryTrackDataHistosUA secoeF

        /gamos/filter secogF GmOnSecondaryFilter GmGammaFilter
        /gamos/setParam GmSecondaryTrackDataHistosUA_secogF:FileName secog
        /gamos/setParam GmSecondaryTrackDataHistosUA_secogF:DataList SecoKineticEnergy
        /gamos/userAction GmSecondaryTrackDataHistosUA secogF"

CUTS="/HT/Physics/setCuts 1e-5 nm"
INI="/run/initialize"

PHYl="/gamos/physicsList GmLEPTSPhysics       $CUTS $INI"
PHYg="/gamos/physicsList HadrontherapyPhysics $CUTS $INI
      #/gamos/physics/replacePhysics standard_opt3
      #/gamos/physics/replacePhysics LowE_Livermore
      #/gamos/physics/replacePhysics LowE_Penelope
      #/gamos/physics/replacePhysics local_ion_ion_inelastic
      #/gamos/physics/replacePhysics QGSP_BIC_EMY
      #/gamos/physics/replacePhysics xxxxxx"

UAS="#/gamos/userAction UAWIF
     /gamos/userAction UAInteraction
     /gamos/userAction GmCountProcessesUA
     #/gamos/analysis/histo1Max *Ener* 3*MeV
     #/gamos/analysis/histo1NBins *Ener* 3000
     #/gamos/userAction GmGenerHistosUA
     /gamos/setParam   UAExit:EnergyMax 3*MeV
     /gamos/setParam   UAExit:EnergyBins 3000
     /gamos/userAction UAExit
     #/gamos/setParam TimeMark 10
     /gamos/userAction UAClock"

if [ $1 == "prucut" ] ; then
    COIN=$(coin G4_Au 10*mm 15*um 0)
    CUT="/HT/Physics/setCuts 0.1e-5 nm"
    PHYg="/gamos/physicsList HadrontherapyPhysics $CUT /run/initialize"
    RUN="/run/beamOn 100"

    CFL="$WRL $COIN $PHYg $UAS $ULI $GENp $RUN"
    time jlepts $CFL /dir oocut > kkcut

    
elif [ $1 == "simu" ] ; then
    #DXS="1 2 3 4 5 6 7"
    DXS="6"
    #EGS="1 2 3"
    EGS="1"
    #RUN="/run/beamOn 100000"
    RUN="/run/beamOn 1"
    ULI="/gamos/userAction GmKillAtStackingActionUA GmElectronFilter"
    
    for E in $EGS ; do
	GENp=$(gen proton ${E}*MeV 1*nm -1*cm)
	for dx in $DXS ; do
	    if [ $dx == "0" ] ; then
		COIN="//"
	    else
		COIN=$(coin G4_Au 1*mm ${dx}'*um' 0)
	    fi
	    CFL="$WRL $COIN $PHYg $UAS $ULI $HGSECO $GENp $RUN"
	    jgamos --dir oo oo_${E}MeV_${dx}um $CFL &
	    #plepts -h euler -n 8 -N 100 -I 1001 -t 4:29:00 -d ooe_${dx}um $CFL
	done
    done
    wait


elif [ $1 == "sphg" ] ; then
    PHY="/gamos/physicsList GmEMPhysics $INI"
    UAS="#/gamos/userAction UAVerbose
         #/gamos/userAction UAWIF
         /gamos/userAction UAClock
         /gamos/setParam UAInteractionSp:Title xxx
         /gamos/setParam UAInteractionSp:x 100 0 200*um
         /gamos/userAction UAInteractionSp"
    COIN=$(chb G4_Au 1*mm 1*mm 1*mm 1*mm)
    #RUN="$(vis) /run/beamOn 10"
    RUN="/run/beamOn 1000000"

    CFL="$WRL $COIN $PHY $UAS $ULI $GENEL $RUN"
    #jlepts $CFL /dir oosp > kksp &
    plepts -h euler -n 8 -N 48 -I 1001 -d oosp -t 4:29:00 $CFL

    ULI="/gamos/userAction GmKillAtStackingActionUA GmGammaFilter"
    CFL="$WRL $COIN $PHY $UAS $ULI $GENEL $RUN"
    #jlepts $CFL /dir oosp2 > kksp2 &
    plepts -h euler -n 8 -N 48 -I 1001 -d oosp2 -t 4:29:00 $CFL

    wait
fi


exit
 
