
set encoding iso_8859_1
set term postscript eps enhanced color solid "Helvetica" 18
#set size 1.5,0.7
set size 1.5,1
#set grid xtics ytics
#set nokey
#set logsc x
#set size 1,2
#set size 2,2
#set size 1.5,2

set noytics
set xlabel 'Length (mm)'
set ylabel 'Energy deposition (arbitrary units)'

#set key outside

set out 'dedx1.eps'
#plot 'o_50e3/xd.hg' u ($2+35):3 ti '50 keV' w hist lw 4
plot 'odedx_20e3/xd.hg' u ($2+90):4 ti ' 20 keV' w li

set out 'dedx2.eps'
plot 'odedx_50e3/xd.hg' u ($2+90):4 ti ' 50 keV' w li

set out 'dedx3.eps'
plot 'odedx_100e3/xd.hg' u ($2+90):4 ti '100 keV' w li



set out 'dedx.eps'
plot 'odedx_20e3/xd.hg' u ($2+90):4 ti ' 20 keV' w li, \
     'odedx_50e3/xd.hg' u ($2+90):4 ti ' 50 keV' w li
