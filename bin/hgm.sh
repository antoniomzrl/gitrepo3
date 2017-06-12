
#set -x

if [ $1 == "help" ] ; then
    echo hgm.sh rebin   fil.root 10
    echo hgm.sh tabjoin fil.root

elif [ $1 == "rebin" ] ; then
    f=$2
    N=$3
    #time root -b -p -q .x ${LEPTSDIR}/macros/hgrebin.cc\(\"$f\",$N\)
    time root -b -p -q .x ${LEPTSDIR}/macros/hgrebin.cc'("'$f'",'$N')'

elif [ $1 == "tabjoin" ] ; then
    f=$2
    time root -b -p -q .x ${LEPTSDIR}/macros/hgtabjoin.cc'("'$f'")'

elif [ $1 == "tab" ] ; then
    f=$2
    time root -b -p -q .x ${LEPTSDIR}/macros/tabhgs.cc'("'$f'")'

elif [ $1 == "tabh" ] ; then
    f=$2
    time root -b -p -q .x ${LEPTSDIR}/macros/tabhgshor.cc'("'$f'")'

#elif [ $1 == "tabinteg" ] ; then
#    f=$2
#    time root -b -p -q .x ${LEPTSDIR}/macros/tabinteghgs.cc'("'$f'")'
fi
