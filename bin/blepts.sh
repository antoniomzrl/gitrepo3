#!/bin/bash

if [ $# -lt 1 -o 'help' = "$*" -o '?' = "$*" ] ; then
    echo "Usage:  $0 -t 00:05:00 -n 8"
    exit
fi



BT="00:05:00"
BN=8
NP=1
DIR="out"
DRY="no"

while getopts t:n:N:d:z: opt ; do
    case $opt in
	t) BT=$OPTARG;;
	n) BN=$OPTARG;;
	N) NP=$OPTARG;;
	d) DIR=$OPTARG;;
	z) DRY=$OPTARG;;
	?) echo "Usage:  $0 -t 00:05:00 -n 8"
    esac
done

shift $((OPTIND-1))

FLG="$* /run/beamOn $NP /batch $BT"

for i in {00..99} ; do
    let sd=(i+1000)*10
    SEED="/gamos/random/setSeeds $sd $sd"
    jlepts $SEED $FLG /dir ${DIR}/o$i /jn j$i
done

cd $DIR

set $(echo *)
echo $# bat dirs


buildfb() {
    echo $*
    echo $1
    echo ${1}
    fj=job_${1}.sh
    for f in $* ; do
	echo '#PBS -l nodes=1:ppn='${BN}              > $fj
	echo '#PBS -l walltime='${BT}                >> $fj
	echo 'source $HOME/lepts/bin/conflepts.sh'   >> $fj
	echo 'cd $PBS_O_WORKDIR'                     >> $fj
	for dir in $* ; do
	    echo '(cd' $dir '; nohup gamos *.inp) &' >> $fj
	done
	echo 'wait'                                  >> $fj
    done
}


((n=0))
dirs=

while [ $# -gt 0 ] ; do
    dirs="$dirs $1"
    shift
    ((n++))
    if [ $n -eq $BN ] || [ $# -eq 0 ] ; then
	buildfb $dirs
	((n=0))
	#echo $dirs
	dirs=
    fi
done


if [ $DRY != "dry" ] ; then
    set -x
    cd ..
    pwd
    scp -r $DIR euler:batch
    ssh euler 'cd batch/'${DIR} '; for f in *sh ; do qsub $f ; done'
fi
