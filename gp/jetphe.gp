set encoding iso_8859_1
set term postscript eps enhanced color solid "Helvetica" 18
#set term postscript eps enhanced "Helvetica" 18

set style line 1 pt 5 ps .6
set style line 2 
set style line 3 pt 5 ps .6
set style line 4 

set xlabel 'x (mm)' 
set ylabel 'y (mm)'
#set xrange [0:3]
#set yrange [-1.5:1.5]
set size ratio 1 1.5,1.5

set out 'jet1b.eps'
plot 'gasphot.d' u 1:3 w poi ls 3 noti, \
     'ii.d'      u 1:3 w dots ls 1 noti

set out 'jet1.eps'
plot 'gasphot.d' u 1:2 w poi ls 3 noti, \
     'ii.d'      u 1:2 w dots ls 1 noti

set out 'jet2.eps'
set xrange [-5:5]
set yrange [-5:5]
replot

set out 'jet3.eps'
set xrange [-3:3]
set yrange [-3:3]
replot

set out 'jet4.eps'
set xrange [-2:2]
set yrange [-2:2]
plot 'gasphot.d' u 1:2 w poi ls 3 noti , \
     'ii.d'      u 1:2 w poi ls 1 noti
