set datafile separator ","

set terminal GPVAL_TERM size 800, 700 font "Courier Bold,14"
if (GPVAL_VERSION >= 5) set colorsequence default

set key outside
set tics font "Helvetica,12"
set xlabel "eV"
set xrange [0:]
set ylabel "arbitrary units"
set format y ""


plot 'oosmf_90_00/oosmf_90_00_1_1Analyser.root.csv' u ($1*1e6):5 w li lw 3 ti 'p=0', 'oosmf_90_05/oosmf_90_05_1_1Analyser.root.csv' u ($1*1e6):5 w li lw 3 ti 'p=5', 'oosmf_90_10/oosmf_90_10_1_1Analyser.root.csv' u ($1*1e6):5 w li lw 3 ti 'p=10', 'oosmf_90_15/oosmf_90_15_1_1Analyser.root.csv' u ($1*1e6):5 w li lw 3 ti 'p=15', 'oosmf_90_20/oosmf_90_20_1_1Analyser.root.csv' u ($1*1e6):5 w li lw 3 ti 'p=20', 'experim/90eV_integrated e-source P0.txt' u ($1*1e0):($2*1000) '%lf %lf' w li lw 3 ti 'p=0 (exp)'
pause -1
