# 60 um medidas / TRAX / sin ph / el / el+ph

set datafile separator ","


plot [:600][0:80]'TlAuMedidas.csv' u 1:6 w li ti 'Medidas', \
               'daria60um.csv'  u 1:($2*7e6) w li lw 3 ti 'Daria-TRAX', \
	       'TlAuSimus3.csv' u ($1*1000):($10/16000) w li lw 3 ti 'G4 noph', \
	       'TlAuSimus3.csv' u ($1*1000):($9/16000) w li lw 3 ti 'G4 el', \
	       'TlAuSimus3.csv' u ($1*1000):(($8+$9)/16000) w li lw 3 ti 'G4 el+ph', \
	       'Penel.csv'      u ($1/1000):($2*1e6) w li lw 3 lt 8 ti 'Penelope'
pause -1


set logsc y

plot [:600][1:]'TlAuMedidas.csv' u 1:6 w li ti 'Medidas', \
               'daria60um.csv'  u 1:($2*7e6) w li lw 3 ti 'Daria-TRAX', \
	       'TlAuSimus3.csv' u ($1*1000):($10/16000) w li lw 3 ti 'G4 noph', \
	       'TlAuSimus3.csv' u ($1*1000):($9/16000) w li lw 3 ti 'G4 el', \
	       'TlAuSimus3.csv' u ($1*1000):(($8+$9)/16000) w li lw 3 ti 'G4 el+ph', \
	       'Penel.csv'      u ($1/1000):($2*1e6) w li lw 3 lt 8 ti 'Penelope'
	       
pause -1

