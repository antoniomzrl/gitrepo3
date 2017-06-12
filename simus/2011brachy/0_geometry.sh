source $HOME/lepts/conflepts.sh

CFL="
geometry brachygamma
maxworld 3*cm
xsectiondir $LEPTSDIR/XsectDat/
#verbose 1
#verbosehistogram 1
#verbosedetector 1
"

rm -rf log0 out0
set -x

# vrml plot
time joblepts $CFL dirjob out0 stopenergyelectron 1e9*MeV beamon 10 vis vrml > log0
mv out0/g4_01.wrl geombrachy.wrl
wrl geombrachy.wrl
