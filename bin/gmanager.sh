#!/bin/bash

VER=51f

while getopts "gvV:" opt ; do
    case $opt in
	g) export G4DEBUG=1;;
	v) export CPPVERBOSE=1;;
	V) VER=$OPTARG;;
	?) echo "Usage error" $opt
    esac
done

echo VERSION: $VER
gms=$(echo $HOME/gamos*${VER})
export GAMOSREP=${gms##* }
echo gms: $gms
echo GAMOSREP: $GAMOSREP

if [ $GAMOSREP == $HOME/gamos51c ] ; then
    export GAMOS_VER=GAMOS.5.1.0
    export GEANT4_VER=10.01.gamos
    export ROOT_VER=5.34.30
    DEVELDIR=$LEPTSDIR/devel51c
elif [ $GAMOSREP == $HOME/gamos51d ] ; then
    export GAMOS_VER=GAMOS.5.1.0
    export GEANT4_VER=10.01.gamos
    export ROOT_VER=5.34.30
    DEVELDIR=$LEPTSDIR/devel51c
elif [ $GAMOSREP == $HOME/gamos51f ] ; then
    export GAMOS_VER=GAMOS.5.1.0
    export GEANT4_VER=10.02.gamos
    export ROOT_VER=5.34.34
    DEVELDIR=$LEPTSDIR/devel51f
fi	

echo GAMOS_VER $GAMOS_VER
echo GEANT4_VER $GEANT4_VER
echo ROOT_VER $ROOT_VER
echo DEVELDIR $DEVELDIR

export GAMOSWEB=http://fismed.ciemat.es/GAMOS/download/${GAMOS_VER}/uncompiled
export PROCS=${PROCS:-8}
export LINUX_SYSTEM=Linux-g++


CMDS="help  get  untar  cleantar
      compileroot   cleanroot  
      compilegeant4 cleangeant4
      compilegamos  cleangamos
      compiledevel  cleandevel  syncdevel   eulerdevel
      compilelepts  cleanlepts  updateeuler updateceta
      compileuas    cleanuas"

help() {
    GREEN='\033[1;32m'
    NC='\033[0m'
    printf "\n $GREEN $CMDS $NC \n"
}

get() {
    mkdir -p ${GAMOSREP}
    cd ${GAMOSREP}
    wget -N ${GAMOSWEB}/${GAMOS_VER}.tgz
    wget -N ${GAMOSWEB}/root_v${ROOT_VER}.source.tar.gz
    wget -N ${GAMOSWEB}/geant4.${GEANT4_VER}.tar.gz
    wget -N ${GAMOSWEB}/geant4.${GEANT4_VER}.data.tar.gz
}


untar() {
    cd ${GAMOSREP}
    tar xfz ${GAMOSREP}/${GAMOS_VER}.tgz

    mkdir -p external/geant4
    cd external/geant4
    tar xfz ${GAMOSREP}/geant4.${GEANT4_VER}.tar.gz
    tar xfz ${GAMOSREP}/geant4.${GEANT4_VER}.data.tar.gz

    cd ${GAMOSREP}
    mkdir -p external/root/${ROOT_VER}
    cd external/root/${ROOT_VER}
    tar xfz ${GAMOSREP}/root_v${ROOT_VER}.source.tar.gz
    #source ./dependencySearch.sh
}


logfi() {
    echo $GAMOSREP/$(date +%F-%T)'-'$(echo $1)'.log'
}


compileroot() {
    cd ${GAMOSREP}/external/root/${ROOT_VER}/root
    ./configure --enable-builtin-freetype
    time make -j $PROCS >& $(logfi compileroot)
}


compilegeant4() {
    cd ${GAMOSREP}/external/geant4/geant4.${GEANT4_VER}
    sed s:MY_GAMOS_DIR:${GAMOSREP}:g geant4conf.sh > ttt
    mv ttt geant4conf.sh
    sed s:MY_GAMOS_DIR:${GAMOSREP}:g geant4conf.csh > ttt
    mv ttt geant4conf.csh  
    source ./geant4conf.sh
    cd source
    time make -j $PROCS >& $(logfi compilegeant4)
}


compilegamos() {
    echo ${GAMOSREP}/${GAMOS_VER}
    cd ${GAMOSREP}/${GAMOS_VER}/config

    # sed s:MY_GAMOS_DIR:${GAMOSREP}:g confgamos.csh > ttt
    # mv ttt confgamos.csh
    # sed s:MY_LINUX_SYSTEM:${LINUX_SYSTEM}:g confgamos.csh > ttt
    # mv ttt confgamos.csh 
    # sed s:MY_G4INSTALL:${GAMOSREP}/external/geant4/geant4.${GEANT4_VER}:g confgamos.csh > ttt
    # mv ttt confgamos.csh 
    # sed s:MY_ROOT_DIR:${ROOTSYS}:g confgamos.csh > ttt
    # mv ttt confgamos.csh 
    # sed s:MY_GAMOS_DIR:${GAMOSREP}:g confgamos.sh > ttt
    # mv ttt confgamos.sh
    # sed s:MY_LINUX_SYSTEM:${LINUX_SYSTEM}:g confgamos.sh > ttt
    # mv ttt confgamos.sh 
    # sed s:MY_G4INSTALL:${GAMOSREP}/external/geant4/geant4.${GEANT4_VER}:g confgamos.sh > ttt
    # mv ttt confgamos.sh 
    # sed s:MY_ROOT_DIR:${ROOTSYS}:g confgamos.sh > ttt
    # mv ttt confgamos.sh

    source ./confgamos.sh

    cd ${GAMOSREP}/${GAMOS_VER}/source
    export OGLFLAGS="-I${GAMOSREP}/external/geant4/geant4.${GEANT4_VER}/source/visualization/OpenGL/include"
    time make -j $PROCS >& $(logfi gamossource)
    cd ${GAMOSREP}/${GAMOS_VER}/analysis
    time make -j $PROCS >& $(logfi gamosanalysis)
}


cleantar() {
    rm ${GAMOS_VER}.tgz
    rm geant4.${GEANT4_VER}.tar.gz
    rm geant4.${GEANT4_VER}.data.tar.gz
    rm root_v${ROOT_VER}.source.tar.gz
}


cleangamos() {
    rm -rf ${GAMOSREP}/${GAMOS_VER}/tmp
    rm -rf ${GAMOSREP}/${GAMOS_VER}/lib
    cd ${GAMOSREP}/${GAMOS_VER}/source
    make clean
}


cleanroot() {
    cd ${GAMOSREP}/external/root/${ROOT_VER}/root
    make clean
    rm -f $(find . -name "*.o")
    rm -f $(find . -name "*.d")
    rm -f lib/*rootmap
}


cleangeant4() {
    rm -r ${GAMOSREP}/external/geant4/geant4.${GEANT4_VER}/lib
    rm -r ${GAMOSREP}/external/geant4/geant4.${GEANT4_VER}/bin
    rm -r ${GAMOSREP}/external/geant4/geant4.${GEANT4_VER}/tmp
}



DSTA=$G4INSTALL/source/processes/electromagnetic/dna
DSTB=$G4INSTALL/source/physics_lists/constructors
DSTC=$GAMOSINSTALL/source/GamosCore/GamosPhysics
syncdevel() {
    SYNC="rsync -av --exclude='*~'"
    $SYNC $DEVELDIR/processes/         $DSTA/processes/
    $SYNC $DEVELDIR/models/            $DSTA/models/
    $SYNC $DEVELDIR/electromagnetic/   $DSTB/electromagnetic/
    $SYNC $DEVELDIR/OtherPhysicsLists/ $DSTC/OtherPhysicsLists/
    $SYNC $LEPTSDIR/xsd/lepts/         $G4LEDATA/lepts/
}
compiledevel() {
    syncdevel
    cd $DSTA ; make -j $PROCS
    cd $DSTB ; make -j $PROCS
    cd $DSTC ; make -j $PROCS
}
cleandevel() {
    cd $DSTA ; make clean ; echo xxx ; pwd
    cd $DSTB ; make clean
    cd $DSTC ; make clean
}
eulerdevel() {
    cd ${LEPTSDIR}
    gitcompush.sh
    ssh euler 'cd lepts; git pull; source bin/conflepts.sh; gmanager.sh compiledevel'}
}

compileuas() {
    cd $LEPTSDIR/uas ; make -j $PROCS
}
cleanuas() {
    cd $LEPTSDIR/uas ; make clean
}


compilelepts() {
    cd ${LEPTSDIR}/lepts
    make -j
}
cleanlepts() {
    cd ${LEPTSDIR}/lepts
    make clean
}

update() {
    cd ${LEPTSDIR}
    gitcompush.sh
    ssh $1 'cd lepts; git pull; source bin/conflepts.sh; gmanager.sh compilelepts compileuas'
}

updateeuler() {
    update euler
}

updateceta() {
    update amunoz@login-hpc.ceta-ciemat.es
}

#euler:
if [ -e /home/localsoft/gcc-v4.9.1 ] ; then
    # problem OGLX
    OGLINC=source/visualization/OpenGL/include
    export OGLFLAGS="-I${GAMOSREP}/external/geant4/geant4.${GEANT4_VER}/${OGLINC}"
fi



for cmd in $CMDS ; do
    for arg in $* ; do
	if [ $arg == $cmd ] ; then
	    echo $cmd
	    set -x
	    time $cmd
	    set +x
	fi
    done
done


# (2) root/config/Makefile.linuxx8664gcc
#Quitar -Wl,--no-undefined
#LDFLAGS       = -m64 $(OPT) $(EXTRA_LDFLAGS) -Wl,--no-undefined -Wl,--as-needed
#LDFLAGS       = -m64 $(OPT) $(EXTRA_LDFLAGS) -Wl,--as-needed

