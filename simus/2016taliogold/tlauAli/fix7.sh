#!/bin/bash

# execute as root (sudo -i)
# bash fix7.sh
# send me by email /home/alitraore/log.txt

#apt-get -y install libexpat1-dev g++ dpkg-dev libx11-dev libxft-dev libxmu-dev libxmu-dev libxi-dev

apt-get -y install libglu1-mesa-dev libice-dev libsm-dev libxmu-dev libxi-dev libgtk*dev
apt-get -y install libgtk2.0-dev libgtkglext1-dev libtiff5-dev g++-mingw-w64-i686

cd /home/roldan/lepts
rm log*
git pull                            >& log1
source bin/conflepts.sh
rm bin/drawpw* bin/lcv*
gmanager.sh cleandevel cleanuas     >$ log2
gmanager.sh compiledevel compileuas >& log3
(cd visu; make install)             >& log4

cat log*                > /home/alitraore/log.txt
chown alitraore.alitraore /home/alitraore/log.txt
