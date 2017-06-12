#!/bin/bash

source $HOME/lepts/conflepts.sh

GEN="
   generator g_box,vacio,5*cm,5*cm,-80*cm
   gparticle gamma
   genergy   1*MeV
   #gspectrum photonSpec5p9MeV.txt,fixed,1*MeV
   gspectrum photonSpec5p9MeV.txt,interpolate,1*MeV
"

GEN1MeV="
   generator g_box,vacio,5*cm,5*cm,-80*cm
   gparticle gamma
   genergy   1*MeV
"

CFL="
   worldbox  1.5*m
   wrapper   invisible
   #chamber  chname,material,R,L/2,PosX
   chamber   ch_box,water,0.5*m,0.5*m,0

   xsdat  water  material water   density 1*g/cm3
   stopenergyelectron 100*eV
   sphg  101,-505,505,101,-505,505,101,-505,505

   #verbose parser,gamos,lepts,headers,histograms,detectorhits
   verbose histograms
   #vis  vrml

   #beamon   1000000000
   #batch    26:00:00
   #beamon   10
"


if [ $# -lt 1 ] ; then
    grep then $0
    exit
fi



if [ $1 == "useraction" ] ; then
    set -x
    #joblepts $GEN $CFL stpe 100*eV beamon 1 dirjob o
    time joblepts $GEN $CFL ghg 7*MeV stpe 9*MeV beamon 1000 dirjob o > k
    exit
#root EdX.Draw(); EdX2.Draw("same");
fi


if [ $1 == "nanovol" ] ; then
    set -x
    joblepts $GEN $CFL stpe 9*MeV contfile nanovol.txt beamon 1000000 dirjob on > kn
    #joblepts $CFL stpe 5*keV generator file,nanovol.txt beamon 10 dirjob on > kn2
exit
fi



if [ $1 == "longsim" ] ; then
    SPHG="sphg  100,-500,500,10,-5,5,10,-5,5"
    set -x
   #time joblepts $GEN $CFL $SPHG ghg 8*MeV stpe 10*MeV beamon 10000 dirjob oph > kph
   #exit
    # e- 100*eV
    # phot
    for((i=1;i<101;i++)) ; do
	let sd=(i+10000)*100
	joblepts $GEN $CFL $SPHG ghg 8*MeV stpe 10*MeV beamon 10000000 batch 2:00:00 seed $sd dirjob OP_$sd > k
    done
exit

    for((i=1;i<101;i++)) ; do
	let sd=(i+10000)*100
	joblepts $GEN $CFL $SPHG ghg 8*MeV stpe 100*eV beamon 10000 batch 24:00:00 seed $sd dirjob OE_$sd > k
    done



    # hadd gener1e8.root op*/gener.root
exit
fi



if [ $1 == "longsimpru" ] ; then
    echo $1
    set -x

    #time joblepts $CFL stpe 10*MeV beamon 100000  > k1
    #time joblepts $CFL stpe 100*keV beamon 100  > k1
    #time joblepts $CFL stpe  10*keV beamon 100  > k2
    #time joblepts $CFL stpe   1*keV beamon 100  > k3
    #time joblepts $CFL stpe  100*eV beamon 100  > k4
    #time joblepts $CFL stpe   10*eV beamon 100  > k5
    #time joblepts $CFL stpe    1*eV beamon 100  > k6
    #time joblepts $CFL stpe  0.5*eV beamon 100  > k7
    #time joblepts $CFL stpe  0.1*eV beamon 100  > k8
    #egrep "StopE|events" k*
    #exit

    SPHG="sphg  100,-500,500,10,-5,5,10,-5,5"

    #joblepts $GEN1MeV $CFL $SPHG stpe 10*MeV beamon 100 dirjob oph > kph1
    #joblepts $GEN $CFL $SPHG stpe 10*MeV beamon 100 dirjob oph > kph2
    time joblepts $GEN $CFL $SPHG ghg 8*MeV stpe 10*MeV beamon 1000 dirjob oph > kph
exit

    #for((i=1;i<1001;i++)) ; do
for((i=1002;i<1033;i++)) ; do
	let sd=(i+10000)*100
	joblepts $GEN $CFL $SPHG stpe 100*eV beamon 1000 batch 24:00:00 seed $sd dirjob os2_$sd > ks2
    done
exit

    sphgbcd -hg "100,-500,500,2,-5,25,2,-5,25" -o 1e8mc om_*/sphg.bcd
    sphgbcd -hg "100,-500,500,2,-5,25,2,-5,25" -o 1e8sp os_*/sphg.bcd
    #time joblepts $GEN $CFL $SPHG stpe 10*MeV beamon 1000000 dirjob oph > kph
exit


    for((i=1;i<101;i++)) ; do
	let sd=(i+1000)*100
	joblepts $GEN $CFL $SPHG stpe 100*eV beamon 100000 batch 5:00:00 seed $sd dirjob oel_$sd
    done
exit
    sphgbcd -hg "100,-500,500,2,-5,25,2,-5,25" -o 10e7ph   batch/oph*/sphg.bcd
    sphgbcd -hg "100,-500,500,2,-5,25,2,-5,25" -o 10e6phel batch/oel*/sphg.bcd
fi


if [ $1 == "pruxs" ] ; then
    rm -rf oxs 
    ch="worldbox  1.5*m
        #chamber  chname,material,R,L/2,PosX
        chamber   ch_water,water,50*cm,0.5*cm,0
        detector  detdisc,gold,2*mm,1*cm,40*cm
        #verbose headers
       "
    ge="gparticle gamma gspectrum none genergy 1*MeV
        generator g_box,vacio,1*um,1*cm,-15*cm
        vis wrl
        #verbose parser,gamos,lepts,headers,histograms,detectorhits"

    joblepts $CFL $ch $ge stpe 9*MeV beamon 100 dirjob oxs
exit 
   #for dx in 10 1 0.1 ; do
    for dx in 10 1 0.1 ; do
	CH='ch_water,water,5*cm,'$dx'/2*mm,0'
	time joblepts $CFL $ch chamber $CH stpe 9*MeV beamon 100000 dirjob oxs |grep bxs
    done
fi

if [ $1 == "pru1cm" ] ; then
    echo $1
    set -x
    ch="worldbox 2*m  wrapper invisible  chamber ch_water,water,0.5*m,0.5*cm,0"
    ge="gparticle gamma gspectrum none genergy 1*MeV
        generator g_box,vacio,1*um,1*cm,-15*cm
        #vis wrl
        #verbose parser,gamos,lepts,headers,histograms,detectorhits
        verbose histograms"
    
    time joblepts $CFL1 $ch $ge stpe 9*MeV beamon 1000000 dirjob o > k
exit
    joblepts $CFL1 $ch $ge stpe 9*MeV beamon 26 dirjob o1 > k1
    joblepts $CFL1 $ch $ge stpe 9*MeV beamon 27 dirjob o2 > k2
fi



if [ $1 == "emited" ] ; then
    echo $1
    set -x
    ch="worldbox 2*m  wrapper invisible  chamber ch_water,water,5*cm,0.5*cm,-20*cm"
    ge="gparticle gamma gspectrum none genergy 1*MeV
        generator g_box,vacio,5*cm,5*cm,25*cm
        #vis wrl
        verbose parser,gamos,lepts,headers,histograms,detectorhits
        #verbose histograms"

    joblepts $CFL $ch $GEN ghg 10*MeV stpe 9*MeV beamon 100000 dirjob o > k
  exit

    for i in 1 2 3 4 ; do
        #time joblepts $CFL $ch $ge stpe 9*MeV beamon 100 dirjob o > k
	joblepts $CFL $ch $GEN ghg 7*MeV stpe 9*MeV beamon 100000 dirjob o${i} > k${i}
    done
    #hadd geners.root o*/gener.root
fi


if [ $1 == "pruphl" -o $1 == "all" ] ; then
    echo $1
    set -x
    Ge="gparticle e- gspectrum none genergy 100*keV
        generator g_box,vacio,1*mm,1*mm,-0.6*m 
        vis wrl verbose parser,gamos,lepts,headers,histograms,detectorhits"
    joblepts $CFL $Ge                      beamon 1 dirjob o1 > k1
    joblepts $CFL $Ge physics GmEMPhysics  beamon 1 dirjob o2 > k2
    joblepts $CFL $Ge physics GmDNAPhysics chamber ch_box,G4_WATER,0.5*m,0.5*m,0 beamon 1 dirjob o3 > k3
fi


if [ $1 == "penph" -o $1 == "all" ] ; then
    echo $1 penet phot
    joblepts $CFL stpe 9*MeV genergy 1*MeV beamon 10000 dirjob op > kp
    mv op/detint.bcd ph-1MeV.bcd
    joblepts $CFL stpe 9*MeV genergy 6*MeV beamon 10000 dirjob op > kp
    mv op/detint.bcd ph-6MeV.bcd

    joblepts $CFL stpe 9*MeV genergy 6*MeV beamon 10000 dirjob op > kp
    mv op/detint.bcd ph-Spectr.bcd
exit
fi


if [ $1 == "test" -o $1 == "all" ] ; then
    echo $1 genfile
    joblepts $CFL stpe 9*MeV generator file,../genfile.txt dirjob o1 > k1
    mv o1/detint.bcd 1testgen.bcd

    echo $1 contfile
    GEN="generator g_box,vacio,1*mm,1*mm,0 genergy 70*keV"
    joblepts $CFL stpe 5*keV beamon 1 dirjob o2 > k2
    mv o2/detint.bcd 2atcf.bcd

    joblepts $CFL stpe 1*MeV beamon 1 contfile contfil.txt dirjob o2b >> k2
    joblepts $CFL stpe 5*keV generator file,contfil.txt beamon 10 dirjob o2b >> k2
    mv o2b/detint.bcd 2btcf.bcd

    echo $1 penet elec
    GSMALL="gparticle e- generator g_box,vacio,1*mm,1*mm,0"
    joblepts $CFL $GSMALL stpe 1*keV genergy 1*MeV beamon 10 dirjob o3 > k3
    mv o3/detint.bcd 3penel1MeV.bcd
    joblepts $CFL $GSMALL stpe 1*keV genergy 6*MeV beamon 10 dirjob o3b > k3b
    mv o3b/detint.bcd 3penel6MeV.bcd
fi


if [ $1 == "visuset" -o $1 == "all" ] ; then
    echo $1
    joblepts $CFL stpe 9*MeV beamon 10 vis vrml dirjob o4 > k4
    mv o4/g4_01.wrl 4setup.wrl
    joblepts $CFL stpe 9*MeV beamon 50 vis vrml wrapper visible dirjob o4b >> k4
    mv o4b/g4_01.wrl 4setup2.wrl

    joblepts $CFL stpe 9*MeV beamon 10000 dirjob o4c >> k4
    cat o4c/detint*bcd > 4foton2.bcd
    mv o4c/detint.bcd 4foton.bcd
fi



if [ $1 == "caceresminijet" -o $1 == "all" ] ; then
    echo $1
    rm -rf o5
    set -x

    mch="chamber ch_box,water,1.5*cm,1.5*cm,0"

    time joblepts $CFL $mch stpe 9*MeV beamon 1000000 contfile contelec.txt dirjob o5 > k5f
    mv o5/detint.bcd 5efots.bcd
    time clipgf -i o5/contelec.txt -o o5/contclip.txt -r -5 5 0 15 -5 5
    
    time joblepts $CFL stpe 15*keV generator file,contclip.txt beamon 10000 dirjob o5 >> k5
    mv o5/detint3.bcd 5eelecs.bcd
    cat 5efots.bcd 5eelecs.bcd > 5eall.bcd
fi



if [ $1 == "caceres" -o $1 == "all" ] ; then
    echo $1
    rm -rf o5
    set -x

    time joblepts $CFL stpe stpe 9*MeV beamon 1 contfile c1.txt dirjob o1
    time joblepts $CFL stpe 0.5*eV generator file,c1.txt beamon 22 dirjob o1
 exit


    time joblepts $CFL stpe 9*MeV beamon 1000000 dirjob oc > kc
    mv oc/sphg.bcd EdepHg.bcd



    time clipgf -i oc/contelec.txt -o oc/contclip.txt -r -500 400 50 100 50 100
    time joblepts $CFL generator file,contclip.txt beamon 10000 dirjob oc
    mv oc/detint.bcd 2electr.bcd

    


    time joblepts $CFL stpe 9*MeV beamon 30000 contfile contelec.txt dirjob o5 > k5c
    mv o5/detint.bcd  5c1foton.bcd
    mv o5/detint2.bcd 5c2fotonwrap.bcd
    cat 5c1foton.bcd 5c2fotonwrap.bcd > 5c3all.bcd


    time joblepts $CFL stpe 9*MeV beamon 50000 contfile contelec.txt dirjob o5 > k5d
    mv o5/detint.bcd 5d1fots.bcd

    time clipgf -i o5/contelec.txt -o o5/contclip.txt -r -260 -240 -500 500 -500 500
    #time clipgf -i o5/contelec.txt -o o5/contclip.txt -r -260 -240 105 115 -5 5
    time joblepts $CFL stpe 15*keV generator file,contclip.txt beamon 10000 dirjob o5 >> k5e
    mv o5/detint3.bcd 5d2elecs.bcd
fi

