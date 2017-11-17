
# dens MM 0.1*atmosphere
dens() {
    MM=$1 ; PRES=$2
    Avogadro=6.02214179e+23/mole
    k_Boltzmann=8.617343e-11*MeV/kelvin
    KbNa=${k_Boltzmann}'*'${Avogadro}
    Temp=303.55*kelvin
    DENS=${PRES}*${MM}'/('${Temp}*${KbNa}')'
    echo $DENS
}


# mat water MM Dens
mat() {
    MAT=$1
    MM=$2
    DENS=$3
    AA=1.0
    echo "//MAT_${1}
         :MATE ${MAT} ${AA} ${MM} ${DENS}"
}

# gas2liq gasIXS.txt liqIXS.txt
gas2liq() {
    grep '#' $1 > ftmp
    grep -v '#' $1 | awk '{print $1,$2*$10,$3*$10,$4*$10,$5*$10,$6*$10,$7*$10,$8*$10,$9*$10}' >> ftmp
    column -t ftmp > $2
    rm ftmp
}


# wrl 1*m 1*m 1*m
wrl() {
    echo "//WORLD
         :MATE vacuum  1 1*g/mole 1e-25*g/cm3
         :ROTM rmz 0 90 0
         :ROTM rm0 0 0 0
         :VOLU world BOX ${1} ${2} ${3} vacuum
         :COLOUR world 1 1 1"
}

# chb water chbwat 1*m 1*m 1*m 0.5*m
chb() {
    echo "//CHAMBER_$1
         :VOLU   ch_${1} BOX $2 $3 $4 $1
         :PLACE  ch_${1} 1 world rm0 $5 0 0
         :COLOUR ch_${1} 0.4 0.4 0.8
         #/gamos/geometry/createRegion ch_region ch_${1}"
}

# coin water Diametr dx posx
coin() {
    echo "//coin-slab
         :VOLU   ch_${1} TUBE 0.0 ${2}/2 ${3}/2 $1
         :PLACE  ch_${1} 1 world rmz $4 0 0
         :COLOUR ch_${1} 0.6 0.6 0.0
         #/gamos/geometry/createRegion ch_region ch_${1}"
}


# phy LeptsPhysicsList
phy() {
    echo "#PHYSICS
          /gamos/physicsList $1
          #INIT
          /run/initialize"
}


# gen e- 5*keV size posx [fileg]
genbox() {
    GBOX="//GENERATOR_BOX
         :VOLU   gbox BOX $3 $3 $3 vacuum
         :PLACE  gbox 1 world rmz $4 0 0
         :COLOUR gbox 0 0 1"

    GEN="#GENERATOR_${1}_${2}
         /gamos/generator GmGenerator
         /gamos/generator/addSingleParticleSource gn $1 $2
         /gamos/generator/positionDist  gn GmGenerDistPositionInG4Volumes gbox
         /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0"

    if [ x${5} != x ] ; then
	GF="/gamos/generator/energyDist gn GmGenerDistEnergyFromFile $5 interpolate $2"
    fi
    echo $GBOX $GEN $GF
}

# gen e- 5*keV size posx [fileg]
genline() {
    GBOX="//GENERATOR_BOX
         :VOLU   gbox BOX $3*0.1 $3 $3 vacuum
         :PLACE  gbox 1 world rmz $4 0 0
         :COLOUR gbox 0 0 1"

    GEN="#GENERATOR_${1}_${2}
         /gamos/generator GmGenerator
         /gamos/generator/addSingleParticleSource gn $1 $2
         /gamos/generator/positionDist  gn GmGenerDistPositionInG4Volumes gbox
         /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0"

    if [ x${5} != x ] ; then
	GF="/gamos/generator/energyDist gn GmGenerDistEnergyFromFile $5 interpolate $2"
    fi
    echo $GBOX $GEN $GF
}


# gen e- 5*keV diametr posx [fileg [fixed]]
gen() {
    GBOX="//GENERATOR_BALL
         :VOLU   gbox ORB ${3}/2 vacuum
         :PLACE  gbox 1 world rmz $4 0 0
         :COLOUR gbox 0 0 1"

    GEN="#GENERATOR_${1}_${2}
         /gamos/generator GmGenerator
         /gamos/generator/addSingleParticleSource gn $1 $2
         /gamos/generator/positionDist  gn GmGenerDistPositionInG4Volumes gbox
         /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 0 0"

    if [ x${5} != x ] ; then
	if [ x${6} != x ] ; then
	    MET=$6
	else
	    MET="interpolate"
	fi
	GF="/gamos/generator/energyDist gn GmGenerDistEnergyFromFile $5 $MET $2"
    fi
    echo $GBOX $GEN $GF
}


genang() {
    GBOX="//GENERATOR_BALL
         :VOLU   gbox ORB ${3}/2 vacuum
         :PLACE  gbox 1 world rmz $4 0 0
         :COLOUR gbox 0 0 1"

    GEN="#GENERATOR_${1}_${2}
         /gamos/generator GmGenerator
         /gamos/generator/addSingleParticleSource gn $1 $2
         /gamos/generator/positionDist  gn GmGenerDistPositionInG4Volumes gbox
         /gamos/generator/directionDist gn GmGenerDistDirectionConst 1 1 0"

    if [ x${5} != x ] ; then
	GF="/gamos/generator/energyDist gn GmGenerDistEnergyFromFile $5 interpolate $2"
    fi
    echo $GBOX $GEN $GF
}



# gen phspfile shiftx
genvar() {
    echo "#GENERATOR_EGS_${1}
          #/gamos/setParam RTGeneratorPhaseSpace_EGS:MaxNReuse 5
          #/gamos/setParam RTGeneratorPhaseSpace_EGS:MirrorWhenReuse X
          #/gamos/setParam RTGeneratorPhaseSpace_EGS:MaxNRecycle 1
           /gamos/setParam RTGeneratorPhaseSpace_EGS:InitialDisplacement $2 0. 0.
          #/gamos/setParam RTGeneratorPhaseSpace:Histos 1
          /gamos/setParam RTGeneratorPhaseSpace_EGS:InitialRotAngles 0 90*deg 0
          /gamos/setParam RTGeneratorPhaseSpace_EGS:FileName ${1}
          /gamos/generator RTGeneratorPhaseSpace_EGS"
}


uas() { # uas dep int intsp wif v pru gmcount gmhist
    gu=/gamos/userAction
    echo "#UAS"
    for u in $* ; do
	if [ $u == "dep" ] ; then
	    echo $gu UADepo
	elif [ $u == "int" ] ; then
	    echo $gu UAInteraction
	elif [ $u == "intsp" ] ; then
	    echo $gu UAInteractionSp
	elif [ $u == "wif" ] ; then
	    echo $gu UAWIF
	elif [ $u == "v" ] ; then
	    echo $gu UAVerbose
	elif [ $u == "gmcount" ] ; then
	    echo "/gamos/setParam GmCountProcessesUA:PrintPartList 1"
	    echo $gu GmCountProcessesUA
	elif [ $u == "gmhist" ] ; then
	    echo "/gamos/analysis/histo1Max *Ener* 7*MeV
                  $gu GmGenerHistosUA"
	elif [ $u == "pru" ] ; then
	    echo $gu UserActionPrueba
	elif [ $u == "exit" ] ; then
	    echo $gu UAExit
	else
	    echo $gu $u
	fi
    done
}

# procfilt e-_G4LEPTSElastic
procfilt() {
    NF=$(echo $1 | tr -d '_')'F'
    FI="/gamos/filter     $NF GmProcessFilter $1
        /gamos/setParam   GmStepDataHistosUA_${NF}:DataList AngleChange KineticEnergyChange
        /gamos/setParam   GmSecondaryTrackDataHistosUA_${NF}:DataList InitialPrimSecoAngle SecoKineticEnergy
        /gamos/setParam   GmStepDataHistosUA_${NF}:FileName $NF
        /gamos/userAction GmStepDataHistosUA $NF
        /gamos/userAction GmSecondaryTrackDataHistosUA $NF
       "
    echo $FI
}

#mfdet volume
mfdet() {
    MFD="/gamos/scoring/createMFDetector theDet    $1
          /gamos/scoring/addScorer2MFD    theDosis  GmG4PSDoseDeposit  theDet
          /gamos/scoring/addScorer2MFD    theNProc  GmG4PSNofStep      theDet 1
          /gamos/classifier                      theCl GmClassifierByProcess
          /gamos/scoring/assignClassifier2Scorer theCl theNProc
          /gamos/scoring/printer cout1 GmPSPrinterCout
          /gamos/scoring/printer cout2 GmPSPrinterCout
          /gamos/scoring/addPrinter2Scorer cout1 theDosis
          /gamos/scoring/addPrinter2Scorer cout2 theNProc"
    echo $MFD
}

# cofi filename
cofi() {
    HD="/gamos/setParam GmTrackDataTextFileUA_GmElectronOrPositronFilter:"
    CF="${HD}FileName ${1}
        ${HD}WriteHeaderData 0
        ${HD}DataList EventID Particle InitialPosX InitialPosY InitialPosZ InitialMomX InitialMomY InitialMomZ InitialTime InitialWeight
         /gamos/userAction GmTrackDataTextFileUA GmElectronOrPositronFilter"
    echo $CF
}

# # histo E vs r
# pedro2() {
# /gamos/setParam GmStepDataHistosUA_GmElectronFilter:DataList AccumulatedEnergyLost.vs.(sqrt(sqr(FinalPosX-VertexPosX)+sqr(FinalPosY-VertexPosY)+sqr(FinalPosZ-VertexPosZ))
# /gamos/setParam GmStepDataHistosUA_GmElectronFilter:FileName evr.root
# /gamos/userAction GmStepDataHistosUA GmElectronFilter
# }


# # cont file w filter x-y
# pedro() {
# /gamos/filter XF GmNumericDataFilter InitialPosX -10 2
# /gamos/filter YF GmNumericDataFilter InitialPosY -12 3

# /gamos/setParam GmTrackDataTextFileUA_GmElectronOrPositronFilter:FileName contfil.txt 
# /gamos/setParam GmTrackDataTextFileUA_GmElectronOrPositronFilter:WriteHeaderData 0 
# /gamos/setParam GmTrackDataTextFileUA_GmElectronOrPositronFilter_XF_YF:DataList EventID Particle InitialPosX InitialPosY InitialPosZ InitialMomX InitialMomY InitialMomZ InitialTime InitialWeight 
# /gamos/userAction GmTrackDataTextFileUA GmElectronOrPositronFilter XF YF
# }



# cofi filename xmin xmax
cofixf() {
    HD="/gamos/setParam GmTrackDataTextFileUA_GmElectronOrPositronFilter_XF:"
    CF="/gamos/filter XF GmNumericDataFilter InitialPosX $2 $3
        ${HD}FileName ${1}
        ${HD}WriteHeaderData 0
        ${HD}DataList EventID Particle InitialPosX InitialPosY InitialPosZ InitialMomX InitialMomY InitialMomZ InitialTime InitialWeight
         /gamos/userAction GmTrackDataTextFileUA GmElectronOrPositronFilter"
    echo $CF
}


# uli water e- 1*keV
uli() {
    echo "#USERLIMITS
         /gamos/physics/userLimits/setMinEKin ulimit_${1}_${2} ch_${1} $2 $3"
    if [ x${4} != x ] ; then
        echo "/gamos/physics/userLimits/setMinEKin ulimit_${1}_${4} ch_${1} $4 $5"
    fi
    echo "/gamos/physics/userLimits/print"
}

# efi 20*eV
efi() {
    EFI="#ENERGY_FIlTER
         /gamos/filter energyF GmKineticEnergyFilter 0. ${1}
         /gamos/userAction GmKillAtStackingActionUA energyF"
    echo $EFI
}

vis() {
    echo "#VISU
         /vis/scene/create
         /vis/open VRML2FILE
         /vis/viewer/flush
         /tracking/storeTrajectory 1
         /vis/scene/add/trajectories
         /vis/scene/endOfEventAction accumulate
         /vis/viewer/update
         /vis/viewer/flush"
}


# sphg um xl xu n1 yl yu n2 zl zu n3
sphg() {
    echo "/gamos/setParam SPhg:Units '${1}'
          /gamos/setParam SPhg:x $2 $3 $4
          /gamos/setParam SPhg:y $5 $6 $7 
          /gamos/setParam SPhg:z $8 $9 ${10}"
}



#bldfj dir files
bldfj() {
    local NJ fj f fi
    
    NJ=$#
    fj=$(basename ${1} .inp)'.pj.sh'
    echo "("$NJ")" $fj "("$*")"

    echo '#PBS -l nodes=1:ppn='${NJ}                      > $fj
    echo '#PBS -l walltime='${BT}                        >> $fj
    echo 'echo hostname: $HOSTNAME'                      >> $fj
    echo 'test -n "$PBS_O_WORKDIR" && cd $PBS_O_WORKDIR' >> $fj
    echo 'source $HOME/lepts/bin/conflepts.sh' $GV       >> $fj
    for fi in $* ; do
	echo 'gamos' $fi '>&' $fi'.log &'                >> $fj
    done
    echo 'wait'                                          >> $fj
}


#plepts [-r dry] -v 51c -h bhost -t 00:10:00 -n 8 -N 100 -I InitSeed -i seedshift -d dirout $CFL
plepts() {
    local OPTIND BH BT BN NJ IS is DIR RUN DRY FLG SEED i n files
    if [ $# -lt 1 -o 'help' = "$*" -o '?' = "$*" ] ; then
	echo "Usage error" $*
	exit
    fi

    GV="51f"      # gamos version
    BH="euler"    # batch host
    BT="00:05:00" # batch time
    BN=8          # ppn
    NJ=100        # No Jobs
    IS=1
    is=1
    DIR="out"
    RUN="y"

    while getopts "v:h:t:n:N:I:i:d:r:" opt ; do
	case $opt in
	    v) GV=$OPTARG;;
	    h) BH=$OPTARG;;
	    t) BT=$OPTARG;;
	    n) BN=$OPTARG;;
	    N) NJ=$OPTARG;;
	    I) IS=$OPTARG;;
	    i) is=$OPTARG;;
	    d) DIR=$OPTARG;;
	    r) RUN=$OPTARG;;
	    ?) echo "Usage error" $opt
	esac
    done

    shift $((OPTIND-1))
    FLG="$* /batch $BT"
    
    # for i in {001..999} ; do
    # 	test $i -gt $NJ && break
    # 	sd=${IS}${i}
    # 	#SEED="/gamos/random/setSeeds $sd $sd"
    # 	SEED="/gamos/random/restoreSeeds $sd $sd"
    # 	jlepts $SEED $FLG /dir ${DIR} /jn ${DIR}_${i}
    # done
    for ((i=${IS},j=${is}; i<$((NJ+IS)); i++,j++)) ; do
	#SEED="/gamos/random/setSeeds $i $j"
	#sd=$(printf "s%05di%05d\n" "$i" "$j")
	jn=${DIR}
	jgamos --norun --SEED $i --seed $j $FLG /dir ${DIR}
    done
    
    cd $DIR
    rm *.sh
    set $(echo *inp)
    let n=0
    files=

    while [ $# -gt 0 ] ; do
	files="$files $1"
	shift
	((n++))
	if [ $n -eq $BN ] || [ $# -eq 0 ] ; then
	    bldfj $files
	    let n=0
	    files=
	fi
    done

    set -x
    if [ $BH == "local" ] ; then
	for f in *.pj.sh ; do time bash $f ; done
	exit
    fi
    
    cd ..
    set -x
    scp -rq $DIR ${BH}:batch
    set +x
    
    if [ $RUN != "dry" ] ; then
	ssh ${BH} 'cd batch/'${DIR} '; for f in *.pj.sh ; do qsub $f ; done'
    fi
}



# parjob -h euler -t 00:05:00 -n 8 -N 100 -d dir -I 10 -i 10  cmd flgs flgs ...
parjob() {
    local OPTIND HOST BTIME PPN NJOBS DIR SEED seed EXECMD JF i ii

    #defaults
    HOST=local ; PPN=8 ; NJOBS=100 ; BTIME="00:05:00"
    DIR=outdir ; SEED=1 ; seed=1

    #loong opts
    for arg in "$@"; do
	shift
	case "$arg" in
	    "--host")  set -- "$@" "-h" ;;
	    "--ppn")   set -- "$@" "-p" ;;
	    "--jobs")  set -- "$@" "-j" ;;
	    "--btime") set -- "$@" "-b" ;;
	    "--dir")   set -- "$@" "-d" ;;
	    "--SEED")  set -- "$@" "-S" ;;
	    "--seed")  set -- "$@" "-s" ;;
	    *)         set -- "$@" "$arg"
	esac
    done

    #parse opts
    while getopts "h:p:j:b:d:S:s:" opt ; do
	case $opt in
	    h) HOST=$OPTARG;;
	    p) PPN=$OPTARG;;
	    j) NJOBS=$OPTARG;;
	    b) BTIME=$OPTARG;;
	    d) DIR=$OPTARG;;
	    S) SEED=$OPTARG;;
	    s) seed=$OPTARG;;
	    ?) echo "Usage error" $opt
	esac
    done

    shift $((OPTIND-1))
    EXECMD="$*"
    echo "EXECMD: " $EXECMD
    
    mkdir -p $DIR

    for ((i=0; i<${NJOBS}; i++)) ; do
	if [ $((i%PPN)) == 0 ] ; then
	    ii=$(printf "%04d" $i)
	    JF=$DIR/job${ii}
	    echo "#PBS -l nodes=1:ppn=$PPN"                     > $JF
	    echo "#PBS -l walltime=${BTIME}"                   >> $JF
	    echo 'test -n $PBS_O_WORKDIR && cd $PBS_O_WORKDIR' >> $JF
	fi

	FLGS="-s $((i+seed)) -o ood${i}"
	echo "time $EXECMD $FLGS" '>&' log${i} '&'             >> $JF
	
	if [ $(((i+1)%PPN)) == 0 ] || [ $((i+1)) == $NJOBS ] ; then
	    echo 'wait'                                        >> $JF
	fi
    done
    
    if [ $HOST != "local" ] ; then
	scp -r $DIR ${HOST}:batch
	ssh $HOST 'cd batch/'${DIR} '; for j in job* ; do qsub $j ; done'
    fi
}
