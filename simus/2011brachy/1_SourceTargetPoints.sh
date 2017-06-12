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

# source points
time joblepts $CFL dirjob out1 stopenergyelectron 1e9*MeV beamon 10000 > log1
mv out1/detint.bcd source.bcd

# target points (15 sec)
time joblepts $CFL dirjob out1b stopenergyelectron 1e9*MeV beamon 100000 geometry brachygamma > log1b

mv out1b/detint.bcd target.bcd
cat source.bcd target.bcd > sourcetarget.bcd
drawpw sourcetarget.bcd
