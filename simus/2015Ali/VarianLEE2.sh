#!/bin/bash

#source $HOME/lepts/bin/conflepts.sh

source $HOME/lepts/bin/gdefs.sh

UAS="/gamos/userAction UAVerbose
     /gamos/userAction UAWIF
     /gamos/userAction GmCountProcessesUA
     /gamos/analysis/histo1Max *Ener* 7*MeV
     /gamos/userAction GmGenerHistosUA
     /gamos/userAction UAClock /gamos/setParam TimeMark 3600
     /gamos/userAction UAInteraction
     /gamos/userAction UAInteractionSp
     #/gamos/userAction GmKillAtStackingActionUA GmElectronFilter
     /gamos/setParam SPhg:Title NoInts
     /gamos/userAction UADepo
     #/gamos/userAction GmTrackDataHistosUA GmClassifierByParticle"

INI="/run/initialize"

PHY1="/gamos/physicsList GmEMPhysics
      #/gamos/GmPhysics/replacePhysics electron-lowener
      #/gamos/GmPhysics/replacePhysics gamma-lowener
      $INI"
PHY2="/gamos/physicsList GmEMExtendedPhysics
      /gamos/GmPhysics/replacePhysics electron-lowener
      /gamos/GmPhysics/replacePhysics gamma-lowener
      $INI"
PHY3="$PHY2
      /gamos/physics/removeProcessesByName msc
      /gamos/physics/addPhysics coulombScattering"
PHY="/gamos/physicsList GmLEPTSPhysics
     $INI"

GS="LEPTS_GOLD"
#WL="LEPTS_WATER"
#WL="LEPTS_WATER_LIQUID"
#WL="LEPTS_WATER"
WL="G4_WATER"

WRL=$(wrl 3*m 3*m 3*m)
MAT=$(mat $WL "18*g/mole" "1*g/cm3")
MATG=$(mat $GS "197*g/mole" "19.3*g/cm3")

CHB=$(chb $WL 0.5*m 0.5*m 0.5*m 0.5*m)

VIS=$(vis)

GENe=$(gen e- 10*keV 5*cm -80*cm)
GENMONO=$(gen gamma 20*keV 5*cm -80*cm)
GENSPEC=$(gen gamma 1*MeV 5*cm -80*cm photonSpec5p9MeV.txt)
#GENVAR=$(genvar varian10x10.egsphsp1 -1*m)
GENVAR=$(genvar //${HOME}/varian10x10.egsphsp1 -45*cm)
pwd
if [ $1 == "examp" ] ; then
    UAS="$UAS
        /gamos/userAction UAVerbose
        /gamos/userAction UAWIF"

    ULI=$(uli $WL e- 1*MeV)

    CFL="$WRL $MAT $CHB $PHY $UAS $GENMONO $ULI $VIS /run/beamOn 10"
    time jlepts $CFL /dir oph > kph

    ULI=$(uli $WL e- 0.95*keV)
    CFL="$WRL $MAT $CHB $PHY $UAS $GENe    $ULI $VIS /run/beamOn 1"
    time jlepts $CFL /dir oel > kel

    #wrl op/g4_01.wrl
fi



if [ $1 == "gener" ] ; then
    #RUN="$VIS /run/beamOn 1000"
    #RUN="/run/beamOn 1000000"
    RUN="/run/beamOn 1000"

    CFL="$WRL $MAT $PHY $UAS"
    time jlepts $CFL $GENMONO $RUN /dir ogm > kgm
    time jlepts $CFL $GENSPEC $RUN /dir ogs > kgs
    time jlepts $CFL $GENVAR  $RUN /dir ogv > kgv

    #root og/gener.root
    # drawpw og/int4gen.bcd
fi



if [ $1 == "visgeom" ] ; then
    UAS="$UAS
         /gamos/userAction UAWIF
         /gamos/userAction UAVerbose"
    GENSPEC=$(genbox gamma 1*MeV 5*cm -100*cm photonSpec5p9MeV.txt)
    ULI="$(uli $WL e- 9*MeV)"
    RUN="$VIS /run/beamOn 100"

    
   CFL="$WRL $MAT $MATG $CHB $PHY $UAS $ULI $GENSPEC $RUN"
   #CFL="$WRL $MAT $MATG $CHB $PHY $UAS $ULI $GENVAR $RUN"   
 time jlepts $CFL /dir obb > kbb
fi


if [ $1 == "atenu" ] ; then
    UAS="/gamos/userAction UAExit
         /gamos/setParam UAExit:EnergyMax 10*MeV
         /gamos/setParam UAExit:EnergyBins 1000
         /gamos/userAction UAClock
         /gamos/setParam TimeMark 3600"
    #RUN="$VIS /run/beamOn 100"
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



if [ $1 == "simu" ] ; then
    UAS="#/gamos/userAction UAVerbose
         #/gamos/userAction UAWIF
         /gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         #/gamos/analysis/histo1Max *Ener* 7*MeV
         #/gamos/userAction GmGenerHistosUA
         /gamos/userAction UAClock /gamos/setParam TimeMark 5

         /gamos/setParam UAInteractionSp:Title Edep-Depth
         /gamos/setParam UAInteractionSp:x 110 -5*cm 105*cm
         /gamos/userAction UAInteractionSp

         /gamos/userAction UADepo
         #/gamos/userAction GmTrackDataHistosUA GmClassifierByParticle 
         #/gamos/analysis/histo1Max *Ener* 6*MeV"
    #ULI="$(uli $WL e- 100*eV) $(uli $WL e+ 100*eV)"
    ULIL="$(uli $WL e- 100*eV) $(uli $WL e+ 100*eV)"
    #ULIk="$(uli $WL e- 100*MeV) $(uli $WL e+ 100*MeV)"

    
   #CFL2="$WRL $MAT $CHB $PHY  $UAS $ULIL $GENMONO $RUN"
   #CFL1="$WRL $MAT $CHB $PHY  $UAS $ULI $GENMONO $RUN"
   #CFL3="$WRL $MAT $CHB $PHY  $UAS $ULIk $GENMONO $RUN"
    

   #time jlepts $CFL2 /dir om2 > kl2
   #time jlepts $CFL1 /dir om > kl
   #time jlepts $CFL3 /dir om3 > kl
    
   
   #CFL1="$WRL $MAT $CHB $PHY  $UAS $ULI $GENSPEC $RUN"
   #CFL3="$WRL $MAT $CHB $PHY  $UAS $ULIk $GENSPEC $RUN"

   RUN="/run/beamOn 1000"
   for((i=1;i<=8;i++)); do
       SEED="/gamos/random/setSeeds 1000$i 1000$i"
       CFL2="$WRL $MAT $CHB $PHY  $UAS $ULIL $GENSPEC $SEED $RUN"
       #CFL2="$WRL $MAT $CHB $PHY  $UAS $ULIL $GENMONO $SEED $RUN"
       time jlepts $CFL2 /dir ospecVendor100eV /jn job_$i > kspec100ev_$i &
   done
   hadd hdepo.root ospecVendor100eV/hdepo*root
   hadd hint.root ospecVendor100eV/hint*root
   hadd hisp.root ospecVendor100eV/hisp*root
   root -b -p -q .x ${LEPTSDIR}/macros/tabhgs.cc\(\"hdepo.root\"\) >> hdepo.txt
   root -b -p -q .x ${LEPTSDIR}/macros/tabhgs.cc\(\"hint.root\"\) >> hint.txt
   root -b -p -q .x ${LEPTSDIR}/macros/tabhgs.cc\(\"hisp.root\"\) >> hisp.txt
   root -b -p -q .x ${LEPTSDIR}/macros/tabhgsjoin.cc\(\"hisp.root\"\) >> hispjoin.txt
   exit
   
   CFL2="$WRL $MAT $CHB $PHY $UAS $ULIL $GENVAR $SEED $RUN"
   time jlepts $CFL2 /dir olinacsim100eV > klinacsim100ev &
   wait
    
    
#time jlepts $CFL2 /dir od > kd

    # plepts -t 00:10:00 -d opru -N 10 $CFLk
    # plepts -t 00:10:00 -d olep -N 100 $CFL1
    #plepts -t 00:10:00 -d odna -N 100 $CFL2

    # hadd ph.root  op*/hintsp.root
    # hadd phel.root  oe*/hintsp.root
    # root TFile f1("ph.root"); EdepX->Draw(); TFile f2("phel.root"); EdepX->SetLineColor(kRed); EdepX->Draw("same"); 
    exit
fi







if [ $1 == "hginter" ] ; then
    UAS="/gamos/userAction UAInteraction
         /gamos/userAction UAWIF"
    GENMONO=$(gen gamma 100*keV 5*cm -80*cm)
    ULI="$(uli $WL e- 1000*eV)"
    #RUN="/run/beamOn 10000000 /batchtime 4:0:0"
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
