#!/bin/bash

source $HOME/lepts/bin/gdefs.sh

WRL=$(wrl 5*cm 5*cm 5*cm)
WL="WATER_LIQ"
MAT=":MATE $WL 1.0 18*g/mole 1*g/cm3"
CHB=$(chb $WL 1*cm 1*cm 1*cm 1*cm)
 
PHY="/gamos/physicsList GmLEPTSPhysics /run/initialize"

ge=$(gen e- 10*keV 1*nm -1*cm)
gp=$(gen e+ 10*keV 1*nm -1*cm)


if [ $1 == "pru" ] ; then
    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/userAction UAClock
         /gamos/setParam UAInteractionSp:Title Edep-Depth
         /gamos/setParam UAInteractionSp:x 700 0 7*um
         /gamos/userAction UAInteractionSp
         #/gamos/userAction UAVerbose"
    CHB=$(chb $WL 1*cm 1*cm 1*cm 0)
    ge=$(genline e- 10*keV 0.2*um -1*cm)
    RUN="/run/beamOn 50"
    time jlepts $WRL $MAT $CHB $PHY $UAS $ge $RUN /dir oop > kkp

    
elif [ $1 == "jete" ] ; then
    UAS="/gamos/userAction GmCountProcessesUA
         /gamos/userAction UAInteraction
         /gamos/userAction UAWIF
         #/gamos/userAction UAVerbose"
    CHB=$(chb $WL 1*cm 1*cm 1*cm 0)
    ge=$(genline e- 10*keV 0.2*um -1*cm)
    RUN="/run/beamOn 4"
    time jlepts $WRL $MAT $CHB $PHY $UAS $ge $RUN /dir ooe > kke

    
elif [ $1 == "depx" ] ; then
    UAS="/gamos/setParam TimeMark 600
         /gamos/userAction UAClock         
         /gamos/userAction GmCountProcessesUA
         #/gamos/userAction UAInteraction
         /gamos/setParam UAInteractionSp:Title Edep-Depth
         /gamos/setParam UAInteractionSp:x 700 0 7*um
         /gamos/userAction UAInteractionSp
         #/gamos/userAction UAVerbose"

    RUN="/run/beamOn 500"
    CFL="$WRL $MAT $CHB $PHY $UAS $ge $RUN"

    #time jlepts $WRL $MAT $CHB $PHY $UAS $ge $RUN /dir ooe > kke &
    #time jlepts $WRL $MAT $CHB $PHY $UAS $gp $RUN /dir oop > kkp &
    #wait
    #exit

    RUN="/run/beamOn 5000"
    CFL="$WRL $MAT $CHB $PHY $UAS $ge $RUN"

    #plepts -h euler01 -n 8  -t 8:00:00 -N 1000 -I    1 -d o0eu $CFL
    #plepts -h dirac   -n 12 -t 8:00:00 -N 1000 -I 1001 -d o1di $CFL
    #plepts -h euler -n 8  -t 8:00:00 -N 1000 -I 2001 -d o2eu $CFL
    #plepts -h dirac -n 12 -t 8:00:00 -N 1000 -I 3001 -d o3di $CFL
    #plepts -h euler -n 8  -t 8:00:00 -N 1000 -I 4001 -d o4eu $CFL
    #plepts -h dirac -n 12 -t 8:00:00 -N 1000 -I 5001 -d o5di $CFL
    #plepts -h euler01 -n 8  -t 4:29:00 -N 2000 -I 6001 -d o6eu $CFL

    plepts -z dry -h dirac   -n 12 -t 4:29:00 -N 2000 -I 1001 -i 11000 -d ob1 $CFL
    #plepts -h euler01 -n 8  -t 4:29:00 -N 2000 -I 4001 -i 14000 -d ob2 $CFL
    #plepts -h euler01 -n 8  -t 4:29:00 -N 2000 -I 7001 -i 17000 -d ob3 $CFL
    exit
    
    hadd hisp.root o*/hisp*root
    root -b -p -q .x ${LEPTSDIR}/macros/tabhgs.cc\(\"hi.root\"\) > k
    root -b -p -q .x ${LEPTSDIR}/macros/tabhgsjoin.cc\(\"hisp.root\"\) > k
    for f in *root ; do
	root -b -p -q .x ${LEPTSDIR}/macros/tabhgsjoin.cc\(\"${f}\"\) > ${f}_tabhgs.txt
    done
fi

