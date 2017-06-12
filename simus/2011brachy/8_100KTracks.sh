source $HOME/lepts/conflepts.sh

CFL="
geometry brachy
maxworld 3*cm
xsectiondir $LEPTSDIR/XsectDat/
#density 1*g/cm3
#verbose 1
#verbosehistogram 1
verbosedetector -1
saveimg 1 0000
#batch 240:0:0
"

set -x

#for see in 1000 30 ; do
for see in 1000 ; do
    time joblepts $CFL dirjob o_${see} stpe ${see}'*eV' beamon 100000000
done

#for((i=1;i<=1;i++)) ; do
#    let sd=i 
#    echo $sd $i
#    time joblepts $CFL dirjob o_${sd} stpe 100'*keV' beamon 500
#done

