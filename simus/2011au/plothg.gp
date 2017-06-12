set encoding iso_8859_1
set term postscript eps enhanced color solid "Helvetica" 22
#set size 1.5,0.7
#set size 2,1
#set grid xtics mxtics ytics
#set key outside
#set grid xtics ytics
set nokey
#set noytics

#set ylabel ''


set xlabel "{/Symbol q} (degrees)"
set logsc y
set title 'Angular distribution {/Symbol g}'
set out 'tag.eps'
plot [0:180]'wdag.hg' u 2:3 w histeps lw 2
set out 'tag3.eps'
plot [0:180]'wdag.hg' u 2:($3/(sin(($2)/180*3.14))) w histeps lw 2
#set out 'tag2.eps'
#plot [0:180]'wdag2.hg' u 2:4 w histeps lw 2
unset logsc y

set out 'tae.eps'
set xlabel "{/Symbol q} (degrees)"
set title 'Angular distribution e-'
plot [0:180]'wdae.hg' u 2:3 w histeps lw 2
set out 'tae3.eps'
plot  'wdae.hg' u 2:($3/(sin(($2)/180*3.14))) w histeps lw 2
set out 'tae2.eps'
plot [0:180]'wdae2.hg' u 2:4 w histeps lw 2


set xlabel 'E (keV)'

set out 'te000.eps'
set title 'e- {/Symbol q} = 0 deg'
plot 'te0.hg' u ($2/1000):3 w histeps lw 2

set out 'te015.eps'
set title 'e- {/Symbol q} = 15 deg'
plot 'te15.hg' u ($2/1000):3 w histeps lw 2

set out 'te030.eps'
set title '{/Symbol q} = 30 deg'
plot 'te30.hg' u ($2/1000):3 w histeps lw 2

set out 'te045.eps'
set title '{/Symbol q} = 45 deg'
plot 'te45.hg' u ($2/1000):3 w histeps lw 2

set out 'te060.eps'
set title '{/Symbol q} = 60 deg'
plot 'te60.hg' u ($2/1000):3 w histeps lw 2

set out 'te075.eps'
set title '{/Symbol q} = 75 deg'
plot 'te75.hg' u ($2/1000):3 w histeps lw 2

set out 'te090.eps'
set title '{/Symbol q} = 90 deg'
plot 'te90.hg' u ($2/1000):3 w histeps lw 2

set out 'te105.eps'
set title '{/Symbol q} = 105 deg'
plot 'te105.hg' u ($2/1000):3 w histeps lw 2

set out 'te120.eps'
set title '{/Symbol q} = 120 deg'
plot 'te120.hg' u ($2/1000):3 w histeps lw 2

set out 'te135.eps'
set title '{/Symbol q} = 135 deg'
plot 'te135.hg' u ($2/1000):3 w histeps lw 2

set out 'te150.eps'
set title '{/Symbol q} = 150 deg'
plot 'te150.hg' u ($2/1000):3 w histeps lw 2

set out 'te165.eps'
set title '{/Symbol q} = 165 deg'
plot 'te165.hg' u ($2/1000):3 w histeps lw 2

set out 'te180.eps'
set title '{/Symbol q} = 180 deg'
plot 'te180.hg' u ($2/1000):3 w histeps lw 2

set out 'tg180.eps'
set title '{/Symbol q} = 180 deg'
plot 'tg180.hg' u ($2/1000):3 w histeps lw 2



set out 're000.eps'
set title 'e- {/Symbol q} = 0 deg'
plot 're0.hg' u ($2/1000):3 w histeps lw 2
set out 'rwe000.eps'
set title 'e- {/Symbol q} = 0 deg'
plot 'rwe0.hg' u ($2/1000):3 w histeps lw 2
set out 'rw2e000.eps'
set title 'e- {/Symbol q} = 0 deg'
plot 'rw2e0.hg' u ($2/1000):3 w histeps lw 2
set out 'rw3e000.eps'
set title 'e- {/Symbol q} = 0 deg'
plot 'rw3e0.hg' u ($2/1000):3 w histeps lw 2

set out 're180.eps'
set title '{/Symbol q} = 180 deg'
plot 're180.hg' u ($2/1000):3 w histeps lw 2
set out 'rwe180.eps'
set title '{/Symbol q} = 180 deg'
plot 'rwe180.hg' u ($2/1000):3 w histeps lw 2
set out 'rw2e180.eps'
set title '{/Symbol q} = 180 deg'
plot 'rw2e180.hg' u ($2/1000):3 w histeps lw 2
set out 'rw3e180.eps'
set title '{/Symbol q} = 180 deg'
plot 'rw3e180.hg' u ($2/1000):3 w histeps lw 2

set out 'rg180.eps'
set title '{/Symbol q} = 180 deg'
plot 'rg180.hg' u ($2/1000):3 w histeps lw 2
set out 'rwg180.eps'
set title '{/Symbol q} = 180 deg'
plot 'rwg180.hg' u ($2/1000):3 w histeps lw 2
set out 'rw2g180.eps'
set title '{/Symbol q} = 180 deg'
plot 'rw2g180.hg' u ($2/1000):3 w histeps lw 2
set out 'rw3g180.eps'
set title '{/Symbol q} = 180 deg'
plot 'rw3g180.hg' u ($2/1000):3 w histeps lw 2
