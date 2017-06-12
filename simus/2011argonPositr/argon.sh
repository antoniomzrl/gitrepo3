source $HOME/lepts/conflepts.sh

CFL="
material argon
xsdat argon
density 0.8*kg/m3

generator point
#generator fudisco05mm
genergy 10*keV
beamon 5

stopenergyelectron 11.5*eV
halflength 14*cm
vis vrml

#verbose 1
#verbosehistogram 1
#verbosedetector 1
"

#time joblepts $CFL gposition "-1*cm 0 0" dirjob o1 > k1
#drawpw o1/detint.bcd
#exit

#gdirection "1 0.5 0" mag "0.1*tesla 0 0"

rm -rf o1 o2 k1 k2
time joblepts $CFL gparticle e- dirjob o1 gposition "-1*mm 0 0" > k1
time joblepts $CFL gparticle e+ dirjob o2 gposition "-1*mm -10*mm 0" > k2


cat o1/detint.bcd o2/detint.bcd > ep.bcd
drawpw ep.bcd &
# wrl o1/g4_01.wrl &
