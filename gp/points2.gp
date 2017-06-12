set encoding iso_8859_1
set term postscript color solid "Helvetica" 14
set nogrid
#set key outside
#set size 2,2

set linestyle 1 pt 7 ps 0.5
set linestyle 2 pt 7 ps 0.5
set linestyle 3 pt 7 ps 0.5
set linestyle 4 pt 7 ps 0.5
set linestyle 5 pt 7 ps 0.5
set linestyle 6 pt 7 ps 0.5
set linestyle 7 pt 7 ps 0.1

#set out 'pru.eps'
#plot 1 ls 1, 2 ls 2, 3 ls 3, 4 ls 4, 5 ls 5, 6 ls 6, \
#     7 ls 7, 8 ls 8, 9 ls 9, 10 ls 10, 11 ls 11

set xlabel 'z (mm)'
set ylabel 'x (mm)'

set out '61zxIonization.eps'
set title 'Ionization points'
plot "ionization.dat" u 3:1 ti 'Ionization' w points ls 1

set out '62zxExcitation.eps'
set title 'Excitation points'
plot "excitation.dat" u 3:1 ti 'Excitation' w points ls 2

set out '63zxDissociation.eps'
set title 'Dissociation points'
plot "dissociation.dat" u 3:1 ti 'Dissociation' w points ls 3

set out '64zxElastic.eps'
set title 'Elastic points'
plot "elastic.dat" u 3:1 ti 'Elastic' w points ls 7

set out '65zx.eps'
set title 'Ionization + Excitation + Dissociation'
plot "excitation.dat"   u 3:1 ti 'Excitation'   w points ls 2, \
     "dissociation.dat" u 3:1 ti 'Dissociation' w points ls 3, \
     "ionization.dat"   u 3:1 ti 'Ionization'   w points ls 1

set out '66zx.eps'
set title 'Ionization + Excitation + Dissociation + Elastic'
plot "excitation.dat"   u 3:1 ti 'Excitation'   w points ls 2, \
     "dissociation.dat" u 3:1 ti 'Dissociation' w points ls 3, \
     "ionization.dat"   u 3:1 ti 'Ionization'   w points ls 1, \
     "elastic.dat"      u 3:1 ti 'Elastic'      w points ls 7

set out 'kk.ps'
set title 'Ionization + Excitation + Dissociation + Elastic'
plot [:]"excitation.dat"   u 3:1 ti 'Excitation'   w points ls 2, \
     "dissociation.dat" u 3:1 ti 'Dissociation' w points ls 3, \
     "ionization.dat"   u 3:1 ti 'Ionization'   w points ls 1, \
     "elastic.dat"      u 3:1 ti 'Elastic'      w points ls 7
