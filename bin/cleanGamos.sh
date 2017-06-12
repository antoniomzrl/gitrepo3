
cd $GAMOSDIR

find . -name tmp -exec rm -r {} \;
find . -name "*.o" -exec rm {} \;
find . -name "*.d" -exec rm {} \;
