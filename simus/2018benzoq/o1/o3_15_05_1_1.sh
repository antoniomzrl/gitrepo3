#!/bin/bash
#PBS        -l nodes=1:ppn=1
#SBATCH --nodes=1 --ntasks=1
#PBS           -l walltime=00:01:00
#SBATCH             --time=00:01:00
test -z $PBS_O_WORKDIR || cd $PBS_O_WORKDIR
source $HOME/lepts/bin/conflepts.sh 
time gamos o3_15_05_1_1.inp >& o3_15_05_1_1.log &
wait

