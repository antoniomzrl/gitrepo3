source $HOME/lepts/conflepts.sh

CFL="
geometry brachy
maxworld 3*cm
xsectiondir $LEPTSDIR/XsectDat/
#density 1*g/cm3
#verbose 1
#verbosehistogram 1
verbosedetector -1
"

set -x

# 1000 e- tracks (5m40s)
time joblepts $CFL dirjob out7 stopenergyelectron 100*keV beamon 100000
mv out7/xdimg0100000.* .
amide xdimg0100000.hv
drawpw xdimg0100000.bcd
