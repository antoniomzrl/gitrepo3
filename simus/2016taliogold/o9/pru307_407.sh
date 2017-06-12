#PBS -l nodes=1:ppn=8
#PBS -l walltime=00:01:00
test -z $PBS_O_WORKDIR || cd $PBS_O_WORKDIR
source $HOME/lepts/bin/conflepts.sh 
time gamos pru300_400.inp >& pru300_400.log &
time gamos pru301_401.inp >& pru301_401.log &
time gamos pru302_402.inp >& pru302_402.log &
time gamos pru303_403.inp >& pru303_403.log &
time gamos pru304_404.inp >& pru304_404.log &
time gamos pru305_405.inp >& pru305_405.log &
time gamos pru306_406.inp >& pru306_406.log &
time gamos pru307_407.inp >& pru307_407.log &
wait

