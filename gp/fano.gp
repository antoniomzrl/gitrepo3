
set encoding iso_8859_1
set term postscript eps enhanced color solid "Helvetica" 22
#set size 1.5,0.7
#set size 2,1
set grid xtics ytics
set nokey


set title 'Distribution of charges generated'
set xlabel 'Number of charges (j)'
set ylabel 'p(j)'
set out 'ncharg.eps'
plot 'out10000/ncharg.hist' u 2:4 ti '' w hist lw 2

set title 'Distribution of W'
set xlabel 'W (eV)'
set ylabel 'p(w)'
set out 'w.eps'
plot 'out10000/w.hist' u 2:4 ti '' w hist
