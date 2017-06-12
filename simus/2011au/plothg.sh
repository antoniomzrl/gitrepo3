


mkdir -p plots

for d in $* ; do
    cd $d
    echo 'xxx ' $d
    gnuplot ../plothg.gp
    for f in *eps ; do
	mv $f ../${d}_plots/${d}_$f
    done
    cd -
done
