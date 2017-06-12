#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

UAS="#/gamos/userAction UAVerbose
     #/gamos/userAction UAWIF
     /gamos/userAction GmCountProcessesUA
     /gamos/analysis/histo1Max *Ener* 7*MeV
     /gamos/userAction GmGenerHistosUA
     /gamos/userAction UAClock /gamos/setParam TimeMark 3600
     /gamos/userAction UAInteraction
     #/gamos/userAction UAInteractionSp
     /gamos/setParam SPhg:Title NoInts
     /gamos/userAction UADepo"

INI="/run/initialize"

PHY1="/gamos/physicsList GmEMPhysics
      /gamos/GmPhysics/replacePhysics electron-lowener
      /gamos/GmPhysics/replacePhysics gamma-lowener
      $INI"
PHY2="/gamos/physicsList GmEMExtendedPhysics
      /gamos/GmPhysics/replacePhysics electron-lowener
      /gamos/GmPhysics/replacePhysics gamma-lowener
      $INI"
PHY3="$PHY2
      /gamos/physics/removeProcessesByName msc
      /gamos/physics/addPhysics coulombScattering"

PHY="/gamos/physicsList GmLEPTSPhysics  $INI"
PHYdna="/gamos/physicsList GmDNAPhysics $INI"

WL="G4_WATER"

WRL=$(wrl 2*m 2*m 2*m)
MAT=":MATE $WL 10 18*g/mole 1*g/cm3"
CHB=$(chb $WL 0.5*m 0.5*m 0.5*m 0.5*m)
SLAB=":VOLU slab BOX 0.5*m 0.5*m 1*mm G4_Pb
      :PLACE  slab 1 world rmz -15*cm 0 0
      :COLOUR slab 1 1 0"

GENe=$(gen e- 10*keV 5*cm -80*cm)
GENMONO=$(gen gamma 1*MeV 5*cm -80*cm)
#GENSPEC=$(gen gamma 1*MeV 5*cm -80*cm photonSpec5p9MeV.txt)
GENSPEC=$(genbox gamma 1*MeV 5*cm -70*cm photonSpec5p9MeV.txt)
GENVAR=$(genvar //${HOME}/varian10x10.egsphsp1 -45*cm)


if [ $1 == "examp" ] ; then
    UAS="$UAS
        /gamos/userAction UAVerbose
        /gamos/userAction UAWIF"

    ULI=$(uli $WL e- 1*MeV)

    CFL="$WRL $MAT $CHB $PHY $UAS $GENMONO $ULI $(vis) /run/beamOn 10"
    #time jlepts $CFL /dir oph > kph

    ULI=$(uli $WL e- 0.95*keV)
    CFL="$WRL $MAT $CHB $PHY $UAS $GENe    $ULI $(vis) /run/beamOn 1"
    time jlepts $CFL /dir oel > kel

    #wrl op/g4_01.wrl
fi



if [ $1 == "gener" ] ; then
    UAS="$UAS #/gamos/userAction UAWIF
         /gamos/userAction UAExit
         /gamos/setParam UAExit:EnergyMax 10*MeV
         /gamos/setParam UAExit:EnergyBins 1000"
    MAT=":MATE $WL 10 18*g/mole 1e-20*g/cm3"
    CHB=$(chb $WL 0.5*m 0.5*m 0.5*m 0.5*m)

    RUN="/run/beamOn 4000000"
    
    CFL="$WRL $MAT $CHB $PHY $UAS"
    #time jlepts $CFL $GENMONO $RUN /dir ogm > kgm &
    #time jlepts $CFL $GENSPEC $RUN /dir ogs > kgs &
    time jlepts $CFL $GENVAR  $RUN /dir ogv > kgv &
    wait

    #root og*/gener.root  og*/hexit.root
    # drawpw og/int4gen.bcd

    #root
    #TFile f1("ogv/hexit.root"); E->Draw(); TFile f2("ogs/hexit.root"); E->SetLineColor(kRed); E->Draw("same"); 
fi



if [ $1 == "visgeom" ] ; then
    UAS="$UAS /gamos/userAction UAWIF"
    ULI="$(uli $WL e- 9*MeV) $(uli $WL e+ 9*MeV)"
    RUN="/run/beamOn 100000"
    #RUN="$(vis) /run/beamOn 100"
    CFL="$WRL $MAT $CHB $SLAB $PHY2 $UAS $ULI"
    time jlepts $CFL $GENSPEC $RUN /dir ogs > kg
    time jlepts $CFL $GENVAR $RUN /dir ogv > kv
    #time jlepts $CFL $GENe $RUN /dir oge > ke

    #SEED="/gamos/random/setSeeds 108100 108100"
    #time jlepts $CFL $GENVAR $SEED $RUN /dir ogv2 >& kv2
fi


if [ $1 == "atenu" ] ; then
    UAS="/gamos/userAction UAExit
         /gamos/setParam UAExit:EnergyMax 10*MeV
         /gamos/setParam UAExit:EnergyBins 1000
         /gamos/userAction UAClock
         /gamos/setParam TimeMark 3600"
    #RUN="$(vis) /run/beamOn 100"
    RUN="/run/beamOn 10000000"
    PLA=$(chb G4_Pb 0.5*m 0.5*m 1*cm 0.5*m)
    ULI="$(uli G4_Pb e- 9*MeV) $(uli G4_Pb e+ 9*MeV)"

    for dx in 000 001 002 004 006 010 020 040 060 100 200 400 600 ; do
	PLA=$(chb G4_Pb 0.5*m 0.5*m ${dx}'*mm' 0.5*m)
	if [ $dx == 000 ] ; then
	    CFL="$WRL $MAT      $PHY1 $UAS      $GENSPEC $RUN"
	else
	    CFL="$WRL $MAT $PLA $PHY1 $UAS $ULI $GENSPEC $RUN"
	fi

	jlepts $CFL /dir oat /jn oat${dx} /batch 08:00:00
	#scp -r oat euler:batch
	#ssh euler 'cd batch/oat; for f in *sh ; do qsub $f; done'
    done
exit
    for f in *root ; do
	root -b -p -q .x ${LEPTSDIR}/macros/tabhgs.cc\(\"${f}\"\) >> tabhgs.txt
    done
fi



if [ $1 == "interac" ] ; then
    UAS="/gamos/userAction UAClock
         #/gamos/userAction UAWIF
         /gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/userAction UAInteractionSp
         /gamos/setParam UAInteractionSp:Title Edep-Depth
         /gamos/setParam UAInteractionSp:x 110 -5*cm 105*cm"

    ULI="$(uli $WL e- 10*MeV) $(uli $WL e+ 10*MeV)"
    RUN="/run/beamOn 1000000"
    GEN=$(gen gamma 50*keV 5*cm -80*cm)
    time jlepts $WRL $MAT $CHB $PHY $UAS $ULI $GEN $RUN /dir oo1 > kk1 &
    time jlepts $WRL      $CHB $PHY $UAS $ULI $GEN $RUN /dir oo2 > kk2 &
    wait
fi


if [ $1 == "dvl" ] ; then
    UAS="#/gamos/userAction UAVerbose
         #/gamos/userAction UAWIF
         /gamos/userAction UAClock
         /gamos/setParam TimeMark 1
         /gamos/userAction UAInteraction"
    ULI=$(uli $WL e- 100*eV)
    GEN=$(gen e- 100*keV 1*nm -80*cm)
    RUN="/run/beamOn 10"

    CFL1="$WRL $MAT $CHB $PHY    $UAS $ULI $GEN $RUN"
    CFL2="$WRL $MAT $CHB $PHYdna $UAS $ULI $GEN $RUN"
    time jlepts $CFL1 /dir ol > kl
    time jlepts $CFL2 /dir od > kd
fi


if [ $1 == "simu" ] ; then
    UAS="#/gamos/analysis/histo1Max *Ener* 7*MeV
         #/gamos/userAction GmGenerHistosUA
         #/gamos/userAction UAWIF
         #/gamos/userAction UAVerbose
         /gamos/setParam TimeMark 1
         /gamos/userAction UAClock
         /gamos/userAction UAInteraction
         /gamos/setParam UAInteractionSp:Title Edep-Depth
         /gamos/setParam UAInteractionSp:x 110 -5*cm 105*cm
         /gamos/userAction UAInteractionSp"
    PHYs="/gamos/physicsList GmEMPhysics          /run/initialize"
    PHYh="/gamos/physicsList HadronTherapyPhysics /run/initialize"
    PHYl="/gamos/physicsList GmLEPTSPhysics       /run/initialize"

    #RUN="/run/beamOn 6000"
    RUN="/run/beamOn 10000000"
    ULI="$(uli $WL e- 10*MeV) $(uli $WL e+ 10*MeV)"

    CFL="$WRL $MAT $CHB $PHYs $UAS $ULI $GENVAR $RUN"
    #time jlepts $CFL /dir oos > kks &
    CFL="$WRL $MAT $CHB $PHYs $UAS $GENVAR $RUN"
    #time jlepts $CFL /dir oos2 > kks2 &
    CFL="$WRL $MAT $CHB $PHYl $UAS $ULI $GENVAR $RUN"
    time jlepts $CFL /dir ool > kkl &
    CFL="$WRL      $CHB $PHYl $UAS $ULI $GENVAR $RUN"
    time jlepts $CFL /dir oolb > kklb &

    RUN="/run/beamOn 30"
    CFL="$WRL $MAT $CHB $PHYl $UAS  $GENVAR $RUN"
    #plepts -v 51d -h euler -n 8 -t 4:29:00 -N 100 -I 1000 -i 1000 -d ooll1 $CFL
    #plepts -v 51d -h euler -n 8 -t 4:29:00 -N 100 -I 2000 -i 2000 -d ooll2 $CFL
    wait
fi



if [ $1 == "hginter" ] ; then
    UAS="/gamos/userAction UAInteraction
         /gamos/userAction UAWIF"
    GENMONO=$(gen gamma 100*keV 5*cm -80*cm)
    ULI="$(uli $WL e- 1000*eV)"
    #RUN="/run/beamOn 10000000 /batch 4:0:0"
    RUN="/run/beamOn 1"

    CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO $RUN"
    time jlepts $CFL /dir oi1 > k1

    PHY="/gamos/physicsList GmDNAPhysics"
    CFL="$MAT $WRL $WRP $CHB $PHY $UAS $STPE $INI $GENMONO $RUN"
    time jlepts $CFL /dir oi2 > k2
fi






if [ $1 == "genfile" ] ; then
    GFF="/gamos/setParam GmGeneratorFromTextFile:FileName ../genfile.txt
	 /gamos/generator GmGeneratorFromTextFile"
    RUN="/run/beamOn 11"
    STPE="/gamos/setParam StopEnergyElectron 1.27*MeV"
    UAS="/gamos/userAction UAWIF"
    CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $GFF $RUN"
    time jlepts $CFL /dir ogf > kgf
    exitg
    #drawpw ogf/int.bcd
fi


if [ $1 == "contfile" ] ; then
# (1) 2 fotones & e- hasta:
    RUN="/run/beamOn 1"
    STPE="/gamos/setParam StopEnergyElectron 0.5*MeV"
    UAS="/gamos/userAction UAWIF"
    CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $GENMONO $RUN"
    time jlepts $CFL /dir ocf1 > kcf1

# (2a) fotones & stop e- en fichero:
    STPE="/gamos/setParam StopEnergyElectron 10*MeV"
    UAS="/gamos/setParam ContinuationFileName elgen.txt
         /gamos/userAction UAContinuationFile
         /gamos/userAction UAWIF"
    CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $GENMONO $RUN"
    time jlepts $CFL /dir ocf2a > kcf2a

# (2b) cont a partir del fichero:
    Nl=`cat ocf2a/elgen.txt | wc -l`
    RUN="/run/beamOn $Nl"
    GFF="/gamos/setParam GmGeneratorFromTextFile:FileName ../ocf2a/elgen.txt
	 /gamos/generator GmGeneratorFromTextFile"
    STPE="/gamos/setParam StopEnergyElectron 0.5*MeV"
    UAS="/gamos/userAction UAWIF"
    CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $GFF $RUN"
    time jlepts $CFL /dir ocf2b > kcf2b

    cat ocf1/int.bcd                > 1.bcd
    cat ocf2a/int.bcd ocf2b/int.bcd > 2.bcd
fi



if [ $1 == "nintel" ] ; then
    UAS="/gamos/userAction UAInteraction"
    RUN="/run/beamOn 10"
    STPE="/gamos/setParam StopEnergyElectron 1*eV"
    ge10k=$(gen e- 10*keV 5*cm -80*cm)

    CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $ge10k $RUN"
    time jlepts $CFL /dir oni > k
exit
    # G4-DNA:
    MAT=":ROTM rmz 0 90 0
         //MAT-water-gold-vacuum
         :MATE vacuum  1. 1.008*g/mole   1.e-25*g/cm3
         :MATE gold   79. 196.967*g/mole 19.3*g/cm3
         //:MATE G4_WATER  10. 18*g/mole 1*g/cm3
         //:MATE_TEMPERATURE G4_WATER 273.15*kelvin"
    CHB="//CHAMBER-water-1m3
         :VOLU   chwater BOX 0.5*m 0.5*m 0.5*m G4_WATER
         :PLACE  chwater 1 world rmz 0 0 0
         :COLOUR chwater 0.6 0.6 0.0"
    PHY="/gamos/physicsList GmDNAPhysics"
    ge10k=$(gen e- 10*keV 5*cm -80*cm)

    CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $ge10k $RUN"
    time jlepts $CFL /dir oni2 > k2

    # g4-DNA-Sally
    PHY="/gamos/physicsList SallyDNAPhysicsList"
    CFL="$MAT $WRL $CHB $PHY $UAS $STPE $INI $ge10k $RUN"
    time jlepts $CFL /dir oni3 > k3
fi
