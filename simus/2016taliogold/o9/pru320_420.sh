#PBS -l nodes=1:ppn=5
#PBS -l walltime=00:01:00
test -z $PBS_O_WORKDIR || cd $PBS_O_WORKDIR
source $HOME/lepts/bin/conflepts.sh 
time gamos pru316_416.inp >& pru316_416.log &
time gamos pru317_417.inp >& pru317_417.log &
time gamos pru318_418.inp >& pru318_418.log &
time gamos pru319_419.inp >& pru319_419.log &
time gamos pru320_420.inp >& pru320_420.log &
wait

