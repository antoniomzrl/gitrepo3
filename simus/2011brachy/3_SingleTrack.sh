
source $HOME/lepts/conflepts.sh

CFL="
geometry brachy
maxworld 3*cm
xsectiondir $LEPTSDIR/XsectDat
#verbose 1
#verbosehistogram 1
#verbosedetector 1
"

# 1 track (65 sec)
time joblepts $CFL dirjob out3 stopenergyelectron 1*eV beamon 1 > log3
cat out3/detint.bcd source.bcd > singletrack.bcd
drawpw singletrack.bcd
