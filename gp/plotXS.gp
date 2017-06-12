
set encoding iso_8859_1
set term postscript eps enhanced color solid "Helvetica" 18

#set grid xtics mxtics ytics
#set key outside

set size 2,2
#set nokey
set key outside
set xlabel "{/Symbol q} (degrees)"
set ylabel "Energy"
set logscale y
set ytics ("" 1)

set out 'dxs1.eps'
set hidden3d
set title 'H_2O elastic differential XS'
splot 'Waterdxs2.dat'  ti '' w li

set ylabel "DXS"

set out 'dxs2.eps'
plot 'Waterdxs.dat' u 1:2  ti '1 eV'    w li lw 3, \
     'Waterdxs.dat' u 1:3  ti '1.5 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:4  ti '2 eV'    w li lw 3, \
     'Waterdxs.dat' u 1:5  ti '3 eV'    w li lw 3, \
     'Waterdxs.dat' u 1:6  ti '4 eV'    w li lw 3, \
     'Waterdxs.dat' u 1:7  ti '5 eV'    w li lw 3, \
     'Waterdxs.dat' u 1:8  ti '7.5 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:9  ti '10 eV'   w li lw 3, \
     'Waterdxs.dat' u 1:10 ti '20 eV'   w li lw 3, \
     'Waterdxs.dat' u 1:11 ti '30 eV'   w li lw 3, \
     'Waterdxs.dat' u 1:12 ti '40 eV'   w li lw 3, \
     'Waterdxs.dat' u 1:13 ti '50 eV'   w li lw 3, \
     'Waterdxs.dat' u 1:14 ti '75 eV'   w li lw 3, \
     'Waterdxs.dat' u 1:15 ti '100 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:16 ti '150 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:17 ti '200 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:18 ti '300 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:19 ti '400 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:20 ti '500 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:21 ti '750 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:22 ti '1 keV'   w li lw 3, \
     'Waterdxs.dat' u 1:23 ti '1.5 keV' w li lw 3, \
     'Waterdxs.dat' u 1:24 ti '2 keV'   w li lw 3, \
     'Waterdxs.dat' u 1:25 ti '3 keV'   w li lw 3, \
     'Waterdxs.dat' u 1:26 ti '4 keV'   w li lw 3, \
     'Waterdxs.dat' u 1:27 ti '5 keV'   w li lw 3, \
     'Waterdxs.dat' u 1:28 ti '7.5 keV' w li lw 3, \
     'Waterdxs.dat' u 1:29 ti '10 keV'  w li lw 3


set out 'dxs3.eps'
plot 'Waterdxs.dat' u 1:2  ti '1 eV'    w li lw 3, \
     'Waterdxs.dat' u 1:3  ti '1.5 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:4  ti '2 eV'    w li lw 3, \
     'Waterdxs.dat' u 1:5  ti '3 eV'    w li lw 3, \
     'Waterdxs.dat' u 1:6  ti '4 eV'    w li lw 3, \
     'Waterdxs.dat' u 1:7  ti '5 eV'    w li lw 3, \
     'Waterdxs.dat' u 1:8  ti '7.5 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:9  ti '10 eV'   w li lw 3, \
     'Waterdxs.dat' u 1:10 ti '20 eV'   w li lw 3, \
     'Waterdxs.dat' u 1:11 ti '30 eV'   w li lw 3, \
     'Waterdxs.dat' u 1:12 ti '40 eV'   w li lw 3, \
     'Waterdxs.dat' u 1:13 ti '50 eV'   w li lw 3, \
     'Waterdxs.dat' u 1:14 ti '75 eV'   w li lw 3, \
     'Waterdxs.dat' u 1:15 ti '100 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:16 ti '150 eV'  w li lw 3

set out 'dxs4.eps'
plot 'Waterdxs.dat' u 1:17 ti '200 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:18 ti '300 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:19 ti '400 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:20 ti '500 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:21 ti '750 eV'  w li lw 3, \
     'Waterdxs.dat' u 1:22 ti '1 keV'   w li lw 3, \
     'Waterdxs.dat' u 1:23 ti '1.5 keV' w li lw 3, \
     'Waterdxs.dat' u 1:24 ti '2 keV'   w li lw 3, \
     'Waterdxs.dat' u 1:25 ti '3 keV'   w li lw 3, \
     'Waterdxs.dat' u 1:26 ti '4 keV'   w li lw 3, \
     'Waterdxs.dat' u 1:27 ti '5 keV'   w li lw 3, \
     'Waterdxs.dat' u 1:28 ti '7.5 keV' w li lw 3, \
     'Waterdxs.dat' u 1:29 ti '10 keV'  w li lw 3
