set encoding iso_8859_1
set term postscript eps enhanced color solid "Helvetica" 14


#set xtics ( 0, 1000,2000,3000)
#set ytics ( 1000,2000,3000)
set xlabel 'X position (mm)'
set ylabel 'Y position (mm)'
#set grid xtics ytics
set nogrid

set linestyle 1 pt 7 ps 1.1
set linestyle 2 pt 7 ps 2
set linestyle 3 pt 7 ps 0.3
set linestyle 4 pt 7 ps 0.7


##set key outside
##set key left
#set xrange [:4]
#set yrange [-2.5:2.5]

set out 'pen1.eps'
plot 'xyzE.dat' u 3:( floor($0)%3==0 && $5<50           ? $1:1/0) ti ' <  50 eV' w points ls 3 , \
     'xyzE.dat' u 3:( floor($0)%3==0 && $5>50 && $3<100 ? $1:1/0) ti '50-100 eV' w points ls 4 , \
     'xyzE.dat' u 3:( floor($0)%3==0 && $5>100          ? $1:1/0) ti ' > 100 eV' w points ls 1

set out 'pen2.eps'
plot 'xyzE.dat' u 3:( floor($0)%3==0 && $5<50           ? $2:1/0) ti ' <  50 eV' w points ls 3 , \
     'xyzE.dat' u 3:( floor($0)%3==0 && $5>50 && $3<100 ? $2:1/0) ti '50-100 eV' w points ls 4 , \
     'xyzE.dat' u 3:( floor($0)%3==0 && $5>100          ? $2:1/0) ti ' > 100 eV' w points ls 1


#set out 'cl.eps'
#plot 'cloudzx.dat' u ($1)/1000:( floor($0)%3==0 && $3<50           ? ($2)/1000:1/0) ti ' <  50 eV' w points ls 3 , \
#     'cloudzx.dat' u ($1)/1000:( floor($0)%3==0 && $3>50 && $3<100 ? ($2)/1000:1/0) ti '50-100 eV' w points ls 4 , \
#     'cloudzx.dat' u ($1)/1000:( floor($0)%3==0 && $3>100          ? ($2)/1000:1/0) ti ' > 100 eV' w points ls 1 





#set out 'pen1b.eps'
#plot 'xyzE.dat' u 3:( floor($0)%10==0 && $5<50           ? $1:1/0) ti ' <  50 eV' w points pt 6 ps 0.4 , \
#     'xyzE.dat' u 3:( floor($0)%10==0 && $5>50 && $3<100 ? $1:1/0) ti '50-100 eV' w points pt 7 ps 0.8 , \
#     'xyzE.dat' u 3:( floor($0)%10==0 && $5>100          ? $1:1/0) ti ' > 100 eV' w points pt 5 ps 1.2

#set out 'pen1c.eps'
#plot 'xyzE.dat' u 3:( floor($0)%10==0 && $5<50           ? $1:1/0) ti ' <  50 eV' w points pt 6 ps 1 , \
#     'xyzE.dat' u 3:( floor($0)%10==0 && $5>50 && $3<100 ? $1:1/0) ti '50-100 eV' w points pt 7 ps 1.1 , \
#     'xyzE.dat' u 3:( floor($0)%10==0 && $5>100          ? $1:1/0) ti ' > 100 eV' w points pt 5 ps 1.1
