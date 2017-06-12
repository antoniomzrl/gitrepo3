#!/bin/bash -x

source head.sh


Pres=1*atmosphere/760000
Temp=273.15*kelvin
Dens=${Pres}*${MM}'/('${Temp}*${KbNa}')'

FL="$CFL
   density ${Dens}
   genergy 10*eV
   #verbose parser,gamos,lepts,headers,detectorhit
   #vis    ogl
   #vis     vrml
"

time joblepts $ch2cm11 $FL dirjob op  beamon 100000 > kp
#time joblepts $ch2cm11 $FL dirjob op2 beamon 1000 > kp2
