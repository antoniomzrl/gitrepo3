source $HOME/lepts/conflepts.sh

CFL="
geometry brachy
maxworld 3*cm
xsectiondir $LEPTSDIR/XsectDat/
#verbose 1
#verbosehistogram 1
#verbosedetector 1
"

set -x

# 10 e- tracks (10s)
time joblepts $CFL dirjob out4 stopenergyelectron 10*keV beamon 10 > log4
cat out4/detint.bcd source.bcd > tentracks.bcd
drawpw tentracks.bcd
