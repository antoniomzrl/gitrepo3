
plot 'ethDXS.txt' u 1:($7/330) w li, 'o2/angle.hg' u 2:4 w li

plot [:20]'./ethDXS.txt' u 1:($7/330) w linesp, './o2/angle.hg' u 2:4 w linesp
