
export LEPTSDIR=$HOME/lepts

export PATH=$PATH:$LEPTSDIR/bin

# source $LEPTSDIR/gamos210/GAMOS.2.1.0/config/confgamos.sh
# source $LEPTSDIR/gamos220/GAMOS.2.2.0/config/confgamos.sh
#source $LEPTSDIR/../gamos/GAMOS.3.0.0/config/confgamos.sh
source $LEPTSDIR/../gamos300/GAMOS.3.0.0/config/confgamos.sh



export GAMOS_SEARCH_PATH=${GAMOS_SEARCH_PATH}:$LEPTSDIR/XsectDat

alias wrl="wine $LEPTSDIR/3DExplor/explor3d.exe"
#dune
#view3dscene


#  cd external/geant4/geant4.9.4.p01.gamos/
#  rm -r tmp lib
#  source geant4conf.sh 
#  cd source/
#  make -j3

# cd GAMOS.3.0.0/source
# make -j6
