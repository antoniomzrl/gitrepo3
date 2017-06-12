#!/bin/bash

# penslab.sh -p 1 -K 1e4 -S spetrumfile.txt -m watv.mat -t 10 -n 100 -s seed -v -X /.../.../penslab
#            -x 0.1 -y 0.2 -z -10

# defaults
PAR=1 # electron, 2=ph, 3=positron
E0=1e3
Sfile='xx'
seed="1"
NN=10
FM=watv.mat
TH=10
z0=-10
x0=0
y0=0
VV=0
OUT=.
DIRNAME=`dirname $0`
BASNAME=`basename $0 .sh`
EXE=${DIRNAME}/${BASNAME}


while getopts "p:K:S:s:n:m:t:x:y:z:o:v" flag
do
    case $flag in
	p) PAR="$OPTARG" ;;
	K) E0="$OPTARG" ;;
	S) Sfile="$OPTARG" ;;
	s) seed="$OPTARG" ;;
	n) NN="$OPTARG" ;;
	m) FM="$OPTARG" ;;
	t) TH="$OPTARG" ;;
	x) x0="$OPTARG" ;;
	y) y0="$OPTARG" ;;
	z) z0="$OPTARG" ;;
	X) EXE="$OPTARG" ;;
	o) OUT="$OPTARG" ;;
	v) VV=1 ;;
	?) echo "Flag incorrecto: $flag" ; help="si";;
    esac
done


TMPFILE='TMP-'$E0'-'$NN'-'$TH'-'$PAR'-'$seed

mkdir -p $OUT
cp $FM $OUT
cd $OUT

if [ $Sfile == "xx" ] ; then
    EE="E0     $E0"
else
    EE="SFILE  $Sfile $E0"
fi

cat <<EOF > $TMPFILE
TITLE   penslab.sh
KPAR   $PAR
$EE
Z0     $z0 $x0 $y0
POLAR  0.0 0.0
SIMPAR 1 1 1 0.001 0.001 -1 -1
PFNAME $FM
THICKN $TH
DSMAX  1e-4
NSIMSH $NN
TIME   999999
RSEED  $seed $seed
VERBOS $VV
EOF

set -x
exec "$EXE" < $TMPFILE
exit


# water vapor Eloss
../penslab.sh -p 3 -K 2e4 -n 100 -m watv.mat -t 10 -v


# Stopping Power

MAT=waterVap.mat
DZ=0.001
N=100000000
egs="9999 9000 8000 7000 6000 5000 4500 4000 3500 3000 2500 \
     2000 1500 1000 0750 0500 0400 0350 0300 0250 0220 0200 \
     0190 0180 0170 0160 0150 0140 0130 0120 0110 0100 0095 \
     0090 0085 0080 0075 0070 0065 0060 0055 0050 0045 0040 \
     0035 0030 0025 0020 0015 0012 0010 0009 0008 0007 0005"
for E in $egs ; do
   echo '#PBS -l nodes=1:hz32'                    > job.kk
   echo 'source /opt/intel.8.1/bin/ifortvars.sh' >> job.kk
   echo 'set -x; cd $PBS_O_WORKDIR'              >> job.kk
   echo "time ../penslab.sh $MAT $E $N $DZ"      >> job.kk
   qsub -joe -N penSPW_$E                         < job.kk
done

