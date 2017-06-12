
#cmd="zip -r"
#ext=".zip"
cmd="tar cvfz"
ext="tgz"

$cmd gamos210.${ext} gamos210
$cmd      src.${ext} src* gtkglext 
$cmd    lepts.${ext} bin* work* conf* gp XsectDat Doc


exit
zip -r gamos.zip gamos210 \
    -x "*.cc" -x "*.icc" -x "*.cxx" -x "*.cpp" -x "*.C" -x "*.hh" \
    -x "*.c" -x "*.h" -x "*.Po" -x "*.mk" -x "*.gmk" -x "*.in" \
    -x "*.o" -x "*.d" -x "*akefile*" -x "*.html"
