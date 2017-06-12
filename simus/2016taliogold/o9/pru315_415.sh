#PBS -l nodes=1:ppn=8
#PBS -l walltime=00:01:00
test -z $PBS_O_WORKDIR || cd $PBS_O_WORKDIR
source $HOME/lepts/bin/conflepts.sh 
time gamos pru308_408.inp >& pru308_408.log &
time gamos pru309_409.inp >& pru309_409.log &
time gamos pru310_410.inp >& pru310_410.log &
time gamos pru311_411.inp >& pru311_411.log &
time gamos pru312_412.inp >& pru312_412.log &
time gamos pru313_413.inp >& pru313_413.log &
time gamos pru314_414.inp >& pru314_414.log &
time gamos pru315_415.inp >& pru315_415.log &
wait

