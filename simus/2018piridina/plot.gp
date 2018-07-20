set datafile separator ","

plot 'pirid15eV.csv' u 1:($2*33) w li ti '0mTorr',   '' u ($5*1e6):6 w li lw 3
pause -1
plot 'pirid15eV.csv' u 1:($3*39) w li ti '2mTorr',   '' u ($5*1e6):7 w li lw 3
pause -1
plot 'pirid15eV.csv' u 1:($4*39) w li ti '9.5mTorr', '' u ($5*1e6):8 w li lw 3
pause -1
