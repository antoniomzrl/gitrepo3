#!/bin/bash

source $HOME/lepts/bin/conflepts.sh

set -x
cd $HOME/lepts
#git clone https://github.com/antoniomzrl/gitrepoapp.git
git pull

cd srcapp
if [ x$1 == xclean ] ; then
    make clean
fi
make
