
PDIR=/home/roldan/penelope
PEXE=$PDIR/penslab.sh

source $HOME/lepts/conflepts.sh

CFL="
material water xsdat water density 1*g/cm3
#generator fudisco05mm
generator point gparticle e+ genergy 10*keV 
stopenergyelectron 0.4*eV
halflength 1*cm
#verbose 1
#verbosehistogram 1
verbosedetector 1
"


#$PEXE -p 3 -K 10e3 -n 10 -m waterl.mat11 -t 1 -o opnl1 -v
#time joblepts $CFL dirjob o1 gposition "-1*mm 2*um 0" beamon 10 > k1


$PEXE -p 3 -K 10e3 -n 100 -m waterl.mat11 -t 1 -o opnl2 -v
#time joblepts $CFL dirjob o2 gposition "-1*mm 0 0" beamon 100 > k2
