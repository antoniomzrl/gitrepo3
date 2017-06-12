
#set -x
fileout=$1
shift
echo $* | tr ' ' '\n' > myhadd.tmp

root -b -p -q .x ${LEPTSDIR}/macros/myhadd.cc'("'"$fileout myhadd.tmp"'")'
