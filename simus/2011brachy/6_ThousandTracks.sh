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

# 1000 e- tracks (5m40s)
time joblepts $CFL dirjob out6 stopenergyelectron 100*keV beamon 1000
cat source.bcd out6/detint.bcd > thousandtracks.bcd
