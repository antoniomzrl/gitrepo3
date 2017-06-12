set encoding iso_8859_1
set term postscript eps enhanced color solid "Helvetica" 18

set xlabel 'Energy (keV)' 
set ylabel 'XS (arbitrary units)'
set logsc y
set noytics

set out 'xs.eps'

plot 'ph.xs' u 1:4 w li ti 'PhotoElectric' lw 2, \
     'ph.xs' u 1:3 w li ti 'Compton'       lw 2, \
     'ph.xs' u 1:2 w li ti 'Rayleigh'      lw 2
