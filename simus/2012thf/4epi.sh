#!/bin/bash

source head.sh

# Energ / pres / intensidad
Temp=293.15*kelvin



ES="10 9 8 7 6 5 4 3 2 1.5 1.002"
PS="10 7 4 2 1 0.7 0.4 0.2 0.1 0.07 0.04 0.02 0.01"
FL="batch 1:00:00 beamon 1000000"
for E in ${ES} ; do
    for P in ${PS} ; do
	Eo=${E}*eV
	Pres=${P}*atmosphere/760000
	Dens=${Pres}*${MM}'/('${Temp}*${KbNa}')'
	of=o_e${E}_p${P}
	time joblepts $ch2cm11 $CFL $FL genergy ${Eo} density ${Dens} dirjob $of
	#time joblepts $chpeq $CFL genergy ${Eo} density ${Dens} dirjob $of
    done
done

exit

FL="genergy 1*eV beamon 100000"
Pres=4*atmosphere/760000
Dens=${Pres}*${MM}'/('${Temp}*${KbNa}')'
time joblepts $ch2cm11 $CFL $FL density ${Dens} dirjob $okk > kk
exit