load "def.gp"

set xlabel 'z (mm)'

set out '41gasz.eps'
set title 'Gas Spatial Histogram (Number of hits)'
plot 'gasz.hist'      u 2:3 w histeps ls 1
#plot 'gasz.hist'      u 2:(($3/$5)*$5) w histeps ls 1

set out '42gaszw.eps'
set title 'Gas Spatial Histogram (Energy deposited)'
plot 'gasz.hist'      u 2:5 w histeps ls 1
#plot 'gasz.hist'      u 2:(($5/$3)*$3) w histeps ls 1

set out '43gaszw.eps'
set title 'Gas Spatial Histogram (Energy/hit deposited)'
plot 'gasz.hist'      u 2:($5/$3) w histeps ls 1
