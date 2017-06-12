

load "def.gp"

set xlabel 'z (mm)'

set ylabel 'x (mm)'
set out '50zxedep.eps'
set title 'Spatial distribution of interactions'
#plot "< egrep 'bin' gas.dat" u 4:2 ti '1' w points pt 7 ps 0.5
plot "gas.dat" u 3:1 ti '1' w points pt 7 ps 0.5

set ylabel 'y (mm)'
set out '51zyedep.eps'
set title 'Spatial distribution of interactions'
plot "gas.dat" u 3:2 ti '1' w points pt 7 ps 0.5

set ylabel 'x (mm)'
set out '53zxcharge.eps'
set title 'Spatial distribution of charge'
plot "charge.dat" u 3:2 ti '1' w points pt 7 ps 0.5

set ylabel 'y (mm)'
set out '54zycharge.eps'
set title 'Spatial distribution of charge'
plot "charge.dat" u 3:2 ti '1' w points pt 7 ps 0.5


set size 1,1
set xlabel 'x (mm)'
set ylabel 'y (mm)'

set out '52xyedep.eps'
set title 'Spatial distribution of interactions'
plot "gas.dat" u 1:2 ti '1' w points pt 7 ps 0.5

set out '55xycharge.eps'
set title 'Spatial distribution of charge'
plot "charge.dat" u 1:2 ti '1' w points pt 7 ps 0.5


