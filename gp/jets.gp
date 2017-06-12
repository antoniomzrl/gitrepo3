set encoding iso_8859_1
set term postscript eps enhanced color solid "Helvetica" 18
#set term postscript eps enhanced "Helvetica" 18



set style line 1 pt 7 ps 1.3
set style line 2 pt 7 ps 2
set style line 3 pt 7 ps 0.4
set style line 4 pt 7 ps 1

set xlabel 'x (mm)' 
set ylabel 'y (mm)'
#set xrange [0:3]
#set yrange [-1.5:1.5]
#set size ratio 1 1.2,1.2

set out '31jet.eps'
plot 'ii.d' u 1:3 ti '' w dots
set out '32jet.eps'
plot 'ii.d' u 1:( $5>0  && $5<50  ? ($3):1/0) ti ' <  50 eV' w points ls 3 , \
     'ii.d' u 1:( $7>50 && $7<100 ? ($3):1/0) ti '50-100 eV' w points ls 4 , \
     'ii.d' u 1:( $7>100          ? ($3):1/0) ti ' > 100 eV' w points ls 1
set out '33jet.eps'
plot 'ii.d' u 1:(floor($0)%5==0 && $7>0  && $7<50  ? ($3):1/0) ti ' <  50 eV' w points ls 3 , \
     'ii.d' u 1:(floor($0)%5==0 && $7>50 && $7<100 ? ($3):1/0) ti '50-100 eV' w points ls 4 , \
     'ii.d' u 1:(floor($0)%5==0 && $7>100          ? ($3):1/0) ti ' > 100 eV' w points ls 1
set out '34jet.eps'
plot 'ii.d' u 1:(floor($0)%10==0 && $7>0  && $7<50  ? ($3):1/0) ti ' <  50 eV' w points ls 3 , \
     'ii.d' u 1:(floor($0)%10==0 && $7>50 && $7<100 ? ($3):1/0) ti '50-100 eV' w points ls 4 , \
     'ii.d' u 1:(floor($0)%10==0 && $7>100          ? ($3):1/0) ti ' > 100 eV' w points ls 1
