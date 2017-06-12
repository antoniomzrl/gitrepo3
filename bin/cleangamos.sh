set -x
rm -f $GAMOSDIR/install.log

find $GAMOSDIR -name tmp       -exec rm -rf {} \;
find $GAMOSDIR -name examples  -exec rm -rf {} \;
find $GAMOSDIR -name tutorials -exec rm -rf {} \;

find $GAMOSDIR -name "*.o"     -exec rm {} \;
find $GAMOSDIR -name "*.d"     -exec rm {} \;
find $GAMOSDIR -name "*.C"     -exec rm {} \;
find $GAMOSDIR -name "*.cc"    -exec rm {} \;
find $GAMOSDIR -name "*.cxx"   -exec rm {} \;
find $GAMOSDIR -name "*.cpp"   -exec rm {} \;
