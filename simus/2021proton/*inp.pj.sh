#PBS -l nodes=1:ppn=1
#PBS -l walltime=12:00:00
echo hostname: $HOSTNAME
test -n "$PBS_O_WORKDIR" && cd $PBS_O_WORKDIR
source $HOME/lepts/bin/conflepts.sh 51f
gamos *inp >& *inp.log &
wait
