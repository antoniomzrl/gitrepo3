#!/bin/bash

source $HOME/lepts/bin/conflepts.sh
source $HOME/lepts/bin/gdefs.sh

GS="GOLD_SOLID"

WRL=$(wrl 5*mm 5*mm 5*mm)
MAT=$(mat $GS "197*g/mole" "19.3*g/cm3")

# coin water Diametr dx posx
COIN=$(coin $GS 1*mm 100*um 0)
# dx = 20, 40, 60, 100*um

PHY=$(phy GmLEPTSPhysics)
UAS=$(uas dep gmhist int intsp)

GENEL=$(gen e- 1*keV 10*um -2*mm Tl204elec.txt)
GENPH=$(gen e- 1*keV 10*um -2*mm Tl204phot.txt)



if [ $1 == "visgeom" ] ; then
    UAS=$(uas int)
    VIS=$(vis)
    COIN=$(coin $GS 1*mm 100*um 0)
    GENMONO=$(gen e- 100*keV 10*um -2*mm)
    ULI=$(uli $GS e- 1*keV)
    RUN="/run/beamOn 2"

    CFL="$WRL $MAT $COIN $PHY $UAS $GENEL $ULI $VIS $RUN"
    time jlepts $CFL /dir ov >& kv
fi


if [ $1 == "pru1" ] ; then
    UAS="/gamos/analysis/histo1Max *Ener* 1*MeV
         /gamos/userAction GmGenerHistosUA
         /gamos/userAction UAInteraction
         /gamos/userAction UAInteraction"
    COIN=$(coin $GS 1*mm 100*um 0)
    ULI=$(uli $GS e- 1*keV)
    GENEL=$(gen e- 200*keV 100*um -2*mm)
    RUN="/run/beamOn 100000"

    CFL="$WRL $MAT $COIN $PHY $UAS $GEN $ULI $(vis) $RUN"
    #time jlepts $CFL /dir op1 >& kp1


    GS="gold"
    #MAT=$(mat $ "197*g/mole" "19.3*g/cm3")
    MAT=":MATE vacuum 1 1*g/mole 1e-25*g/cm3
         :MATE gold 79 196.97 19.3"
    COIN=$(coin $GS 1*mm 100*um 0)
    ULI=$(uli $GS e- 1*keV)
    PHY="/gamos/physicsList GmEMPhysics
         /gamos/GmPhysics/replacePhysics electron-lowener
         /gamos/GmPhysics/replacePhysics gamma-lowener
         /run/initialize"

    CFL="$WRL $MAT $COIN $PHY $UAS $GENEL $ULI $RUN"
    time jlepts $CFL /dir op2 >& kp2
fi



if [ $1 == "pru2" ] ; then
    UAS="/gamos/setParam   UAExit:EnergyMax 1*MeV
         /gamos/setParam   UAExit:EnergyBins 200
         /gamos/userAction UAExit
         #/gamos/userAction UAWIF
         /gamos/userAction UAClock"
    COIN=$(coin $GS 1*mm 20*um 0)
    COIN2=$(coin G4_Au 1*mm '20*um' 0)
    
    RUN="/run/beamOn 1"
    #PHY="/gamos/physicsList GmEMPhysics /run/initialize"
    PHY="/gamos/physicsList GmLEPTSPhysics /run/initialize"
    time jlepts $WRL $MAT       $PHY $UAS $GENEL $ULI $RUN /dir oog >& kkg &
    time jlepts $WRL $MAT $COIN  $PHY $UAS $GENEL $ULI $RUN /dir oo1  >& kk1 &
    time jlepts $WRL $MAT $COIN2 $PHY $UAS $GENEL $ULI $RUN /dir oo2  >& kk2 &
    wait
    
    
fi


# GmLEPTSPhysics e-
if [ $1 == "batlepts" ] ; then
    UAS="/gamos/analysis/histo1Max *Ener* 1*MeV
         /gamos/userAction GmGenerHistosUA
         /gamos/userAction UAExit"
    ULI=$(uli $GS e- 40*eV)
    COIN=$(coin $GS 1*mm 20*um 0)
    PHY=$(phy GmLEPTSPhysics)
    RUN="/run/beamOn 100000 /batchtime 4:00:00"

    for((i=0;i<100;i++)) ; do
	let sd=(i+1000)*100
	SEED="/gamos/random/setSeeds $sd $sd"

	for dx in 1e-6 20 40 60 100 ; do
	    DX=${dx}'*um'
	    COIN=$(coin $GS 1*mm $DX 0)
	    DIR="/dir o_${dx}nm_${i}"
	    CFL="$WRL $MAT $COIN $PHY $UAS $GENEL $SEED $ULI $RUN $DIR"
	    time jlepts $CFL >& kp
	done
    done
fi


# GmEMPhysics e- / Coulomb

if [ $1 == "bigsimu" ] ; then
    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/analysis/histo1Max *Ener* 1*MeV
         /gamos/analysis/histo1NBins *Ener* 10
         /gamos/userAction GmGenerHistosUA
         #/gamos/userAction UAWIF
         /gamos/userAction UAExit
         /gamos/setParam UAExit:EnergyMax 1e6
         /gamos/setParam UAExit:EnergyBins 100
         /gamos/userAction UAClock
         /gamos/setParam TimeMark 3600
         /gamos/userAction UAWIF
         /gamos/userAction UAInteractionSp
         /gamos/setParam SPhg:x 1100 -10*um 100*um"

    VAC=":MATE vacuum 1 1*g/mole 1e-25*g/cm3"
    MAT="$VAC :MATE gold 79 196.97*g/mole 19.3*g/cm3"
    ULI=$(uli gold e- 1*keV)

    INI="/run/initialize"

    PHY="/gamos/physicsList GmEMExtendedPhysics
         /gamos/GmPhysics/replacePhysics electron-lowener
         /gamos/GmPhysics/replacePhysics gamma-lowener"
    PHY1="$PHY $INI"
    PHY2="$PHY $INI
          /gamos/physics/removeProcessesByName msc
          /gamos/physics/addPhysics coulombScattering"

    PHY3="$PHY
          /cuts/setLowEdge 250 eV
          /run/setCut 1e-3 nm
          /gamos/physics/setCuts ch_region 1e-9 1e-9 1e-9
          $INI"

    #RUN="/run/beamOn 100000000 /batchtime 14:00:00"
    #RUN="/run/beamOn 10000000  /batchtime 2:00:00"
    #RUN="/run/beamOn 100000  #02:00:00"
    RUN="/run/beamOn 1000000  /batchtime 48:00:00"
    RUN="/run/beamOn 100000  /batchtime 5:00:00"
    DXS="1e-9 020 040 060 100"
    DXS="20 0"
    RUN="/run/beamOn 100000"

   ##for((i=0;i<10;i++)) ; do
    for((i=0;i<1;i++)) ; do
	let sd=(i+1000)*100
	SEED="/gamos/random/setSeeds $sd $sd"

	for dx in $DXS ; do
	    DX=${dx}'*um'

	    if [ $dx == "0" ] ; then
		COIN="//"
		COIN2="//"
	    else
		COIN=$(coin gold 1*mm $DX 0)
		COIN2=$(coin G4_Au 1*mm $DX 0)
	    fi

	    # e-
	    #DIR="/dir oel1_${dx}um_${i}"
	    #CFL="$WRL $MAT $COIN $PHY1 $UAS $GENEL $SEED $ULI $RUN $DIR &"
	    #time jlepts $CFL

	    # e- Coulomb
	    MAT=":MATE gold 79 196.97*g/mole 19.3*g/cm3"
	    DIR="/dir oec_${dx}um_${i}"
	    VER="/version 51c"
	    CFL="$WRL $MAT $COIN $PHY2 $UAS $GENEL $SEED $ULI $RUN $DIR $VER"
	    time jlepts $CFL >& koec_$dx

	    # e- Coulomb G4_Au
	    MAT="//"
	    ULI="#"
	    DIR="/dir oecau_${dx}um_${i}"
	    CFL="$WRL $MAT $COIN2 $PHY2 $UAS $GENEL $SEED $ULI $RUN $DIR $VER"
	    time jlepts $CFL >& koecau_$dx

	    # lowcuts
	    #DIR="/dir oel3_${dx}um_${i}"
	    #CFL="$WRL $MAT $COIN $PHY3 $UAS $GENEL $SEED $ULI $RUN $DIR"
	    #time jlepts $CFL

	    # lepts
	    #MAT="$VAC :MATE LEPTS_GOLD_SOLID 79 196.97*g/mole 19.3*g/cm3"
	    #ULI=$(uli LEPTS_GOLD_SOLID e- 1*keV)
	    #PHY="/gamos/physicsList GmLEPTSPhysics $INI"
	    #DIR="/dir oel4_${dx}um_${i} /jn el4_${dx}um_${i}"
	    #COIN=$(coin LEPTS_GOLD_SOLID 1*mm $DX 0)
	    #CFL="$WRL $MAT $COIN $PHY $UAS $GENEL $SEED $ULI $RUN $DIR"
	    #time jlepts $CFL

	    # phot
	    #DIR="/dir oph_${dx}um_${i}"
	    #CFL="$WRL $MAT $COIN $PHY2 $UAS $GENPH $SEED $ULI $RUN $DIR"
	    #time jlepts $CFL #>& kp
	done
    done
fi


exit


hadd ph20.root  oph_020_*/hexit.root
hadd ph40.root  oph_040_*/hexit.root
hadd ph60.root  oph_060_*/hexit.root
hadd ph100.root oph_100_*/hexit.root
hadd ph0.root   oph_1e-6_*/hexit.root

hadd el20.root  oel3_020*/hexit.root
hadd el40.root  oel3_040*/hexit.root
hadd el60.root  oel3_060*/hexit.root
hadd el100.root oel3_100*/hexit.root
hadd el0.root   oel3_1e-6_*/hexit.root


root -b -p -q .x tabhgs.cc\(\"ph0.root\"\) | grep E30deg >> SimuTalGol.txt
root -b -p -q .x tabhgs.cc\(\"ph20.root\"\) | grep E30deg >> SimuTalGol.txt
root -b -p -q .x tabhgs.cc\(\"ph40.root\"\) | grep E30deg >> SimuTalGol.txt
root -b -p -q .x tabhgs.cc\(\"ph60.root\"\) | grep E30deg >> SimuTalGol.txt
root -b -p -q .x tabhgs.cc\(\"ph100.root\"\) | grep E30deg >> SimuTalGol.txt

root -b -p -q .x tabhgs.cc\(\"el0.root\"\) | grep E30deg >> SimuTalGol.txt
root -b -p -q .x tabhgs.cc\(\"el20.root\"\) | grep E30deg >> SimuTalGol.txt
root -b -p -q .x tabhgs.cc\(\"el40.root\"\) | grep E30deg >> SimuTalGol.txt
root -b -p -q .x tabhgs.cc\(\"el60.root\"\) | grep E30deg >> SimuTalGol.txt
root -b -p -q .x tabhgs.cc\(\"el100.root\"\) | grep E30deg >> SimuTalGol.txt

root -b -p -q .x tabhgs.cc\(\"el20s.root\"\) | grep E30deg >> SimuTalGol.txt
root -b -p -q .x tabhgs.cc\(\"el40s.root\"\) | grep E30deg >> SimuTalGol.txt
root -b -p -q .x tabhgs.cc\(\"el60s.root\"\) | grep E30deg >> SimuTalGol.txt
root -b -p -q .x tabhgs.cc\(\"el100s.root\"\) | grep E30deg >> SimuTalGol.txt



gStyle->SetCanvasPreferGL(true);
TFile f1("z.root");
E30deg->Draw();
TFile f2("v.root");
E30deg->SetLineColor(kRed)
E30deg->Draw("same"); 
