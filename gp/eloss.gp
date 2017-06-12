
load "def.gp"

set size 1.5, 0.75

#set logscale y
set xlabel 'Energy (eV)'

#set title 'Inelastic Energy Loss Spectrum'
set out '10elossXst.eps'
plot 'elossXst.hist'      u 2:4 w histeps lw 5

set out '11elossGen.eps'
plot 'elossGen.hist'       u 2:4 w histeps lw 5

set out '12elossXst.eps'
plot [:100]'elossXst.hist'      u 2:4 w histeps lw 5

set out '13elossGen.eps'
plot [:100]'elossGen.hist'       u 2:4 w histeps lw 5

set out '14elossGen.eps'
plot [:100]'elossXst.hist'      u 2:4 w histeps lw 5, \
           'elossGen.hist'      u 2:4 w histeps lw 5


#set out '12elossGenVsEff.eps'
#set title 'Inelastic Energy Loss Histogram'
#plot 'elossGen.hist' u 2:3 w histeps ls 1, \
#     'elossEff.hist' u 2:3 w histeps ls 2


