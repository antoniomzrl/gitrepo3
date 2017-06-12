
source $HOME/lepts/bin/conflepts.sh 51b

if [ $# == 0 ] ; then
    set -x
    cd ${LEPTSDIR}/uas
    make -j4
    cd ${LEPTSDIR}/lepts
    make -j4
    cd ${LEPTSDIR}/visu
    make -j4 install
    cd ${LEPTSDIR}/util
    make -j4 install
    exit
elif [ $1 == euler ] ; then
    set -x
    ssh euler 'cd lepts; git pull; bin/mkall.sh'
    exit
elif [ $1 == symlinks ] ; then
    set -x
    cd ${G4INSTALL}/source/processes/electromagnetic
    mv lepts lepts_BCK_$(date +%Y-%m-%d-%T)
    ln -s ${LEPTSDIR}/lepts .
    ls -l
    exit
fi

