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

# 100 e- tracks (60s)
time joblepts $CFL dirjob out5 stopenergyelectron 100*keV beamon 100 > log5
cat out5/detint.bcd source.bcd > hundredtracks.bcd
drawpw hundredtracks.bcd
