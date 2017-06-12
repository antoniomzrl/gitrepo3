

gs=`echo $HOME/gamos*${1}/GAMOS*/config/confgamos.sh`
#gs=$(echo $HOME/gamos*/GAMOS*/config/confgamos.sh)
for g in $gs ; do
    echo found "  " $g
done

cfg=${gs##* }
echo select " " $cfg

if test -f $cfg ; then
    source $cfg
fi

export LEPTSDIR=$HOME/lepts
export PATH=$PATH:$LEPTSDIR/bin

export GAMOS_SEARCH_PATH=${GAMOS_SEARCH_PATH}:${LEPTSDIR}/spectr
export LEPTSXSD=${LEPTSDIR}/xsd/lepts

type gamos
#echo $GAMOS_SEARCH_PATH

#dune
#alias wrl="wine $LEPTSDIR/3DExplor/explor3d.exe"
alias wrl="view3dscene --geometry 1300x1300"

export COLVIEWPATH=${HOME}/lepts/colview
