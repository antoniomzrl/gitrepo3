set encoding iso_8859_1
set term postscript eps enhanced color solid "Helvetica" 22
#set size 1.5,0.7
#set size 2,1
#set grid xtics mxtics ytics
#set key outside
#set grid xtics ytics
set nokey
set linestyle 1 lw 3 pt 0 ps 5
set noytics


set xlabel 'x (mm)'
set ylabel 'Energy (arbitrary units)'


set out 'xd.eps'
set title 'Spatial Histogram / Energy deposited'
plot [-0.0001:26]'xd.hg' u 2:4 w histeps ls 1

set out 'yd.eps'
set title 'Spatial Histogram / Energy deposited'
plot [-3:3]'gasz.hg' u 2:4 w histeps ls 1
