if ((ARG1 eq "") || (ARG3 ne "")) {
print "usage (from shell)  :  gnuplot -c ", ARG0, " \"file_1\" [\"file_2\"]\n";
print "usage (from gnuplot):  call \"", ARG0, "\" \"file_1\" [\"file_2\"]\n";
exit
}

set terminal GPVAL_TERM size 700, 700 font "Courier Bold,14"
if (GPVAL_VERSION >= 5) set colorsequence default

set key outside
set tics font "Helvetica,12"
set xrange [0:]
set ylabel "arbitrary units"
#set format y ""
set title font "Arial, 16"
set datafile commentschars "%"

do for [i=1:ARGC] {

  FILE=value("ARG".i)

  set terminal GPVAL_TERM i-1
  print "Plotting file ",FILE

  call "lepts.gp" FILE

  set logscale y
  plot for [IDX=0:10] FILE i IDX using 1:2 w lines title columnheader(1)      
    
  pause -1
}

