
set term pdf
set out 'dist.pdf'

plot 'dist.txt' u 1:($3/$2) w linesp pt 4 ti 'Ionis/Dose'
plot 'dist.txt' u 1:($4/$2) w linesp pt 4 ti 'Dissoc/Dose'
plot 'dist.txt' u 1:($5/$2) w linesp pt 4 ti 'Elast/Dose'
plot 'dist.txt' u 1:($6/$2) w linesp pt 4 ti 'Rotat/Dose'
plot 'dist.txt' u 1:($7/$2) w linesp pt 4 ti 'Excit/Dose'
plot 'dist.txt' u 1:($8/$2) w linesp pt 4 ti 'Vibrat/Dose'
plot 'dist.txt' u 1:($9/$2) w linesp pt 4 ti 'Attach/Dose'
