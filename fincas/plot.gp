# derivative functions.  Return 1/0 for first point, otherwise delta y or (delta y)/(delta x)
d(y) = ($0 == 0) ? (y1 = y, 1/0) : (y2 = y1, y1 = y, y1-y2)
d2(x,y) = ($0 == 0) ? (x1 = x, y1 = y, 1/0) : (x2 = x1, x1 = x, y2 = y1, y1 = y, (y1-y2)/(x1-x2))
# offset for derivatives (half the x spacing)
#dx = 0.25
#plot 'data.dat' title 'data', '' u ($1-dx):(d($2)) title '1v derivative', \
#     '' u ($1-dx):(d2($1,$2)) title '2v derivative'


set xdata time
set timefmt "%d/%m/%y"
set grid
set term canvas size 1000,600 enhanced fsize 8 jsdir './js' standalone mousing
#background "#eeeeff" standalone mousing 
set out 'index.html'
#plot [][0:]'bving.txt' u 1:2 w li lw 2 notitle
plot [][:]'bving.txt' u 1:2 w histep notitle, 'bving.txt' u 1:(d($2)) w histep notitle


set term canvas size 1000,1000 enhanced fsize 8 jsdir './js' standalone mousing
set out 'index2.html'
plot 'bving.txt' u 1:(d($2)) w histep notitle
