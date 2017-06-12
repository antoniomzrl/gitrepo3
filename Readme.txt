#/home/roldan/gamos5/scripts5


# (1) gcc 4.8.1
H=/home/localsoft
PA=$H/gcc-v4.8.1/bin
LI=$H/gmp-v5.1.3/lib:$H/mpfr-v3.1.2/lib:$H/mpc-v1.0.1/lib:$H/gcc-v4.8.1/lib64
export PATH=$PA:$PATH
export LD_LIBRARY_PATH=$LI:$LD_LIBRARY_PATH


# (2) euler:
root/config/Makefile.linuxx8664gcc
Quitar -Wl,--no-undefined
#LDFLAGS       = -m64 $(OPT) $(EXTRA_LDFLAGS) -Wl,--no-undefined -Wl,--as-needed
LDFLAGS       = -m64 $(OPT) $(EXTRA_LDFLAGS) -Wl,--as-needed

# (3) euler:
root/graf2d/asimage/src/libAfterImage/Makefile
añadir freetype2...
INCS_EXTRA = -I/usr/include/freetype2 -I/usr/include/freetype2/freetype -I/usr/include/freetype/freetype2
Y compilar en master pq en los nodos no hay jpeglib.h

# (3) euler:
cd GAMOS.5.1.0/source/GamosCore/GamosApplication

g++ -W -Wall -ansi -pedantic -Wno-non-virtual-dtor -Wno-long-long  -Wwrite-strings -Wpointer-arith -Woverloaded-virtual -pipe  -O2 -DGAMOS_NO_FFTW -fPIC -DG4OPTIMISE -Iinclude  -I/home/roldan/gamos510/external/geant4/geant4.10.00.p02.gamos/source/externals/clhep/include -DG4VERBOSE -DG4_STORE_TRAJECTORY -DG4UI_USE_TERMINAL -DG4UI_USE -DG4VIS_USE_ASCIITREE -DG4VIS_USE_DAWNFILE -DG4VIS_USE_GMOCRENFILE -DG4VIS_USE_HEPREPFILE -DG4VIS_USE_RAYTRACER -DG4VIS_USE_VRMLFILE -DG4VIS_USE_DAWN -DG4VIS_USE_OPENGLX -DG4VIS_USE_OPENGL -DG4VIS_USE_VRML -DG4VIS_USE  -I/usr/include/X11/extensions -I/usr/include/X11 -I/home/roldan/gamos510/external/geant4/geant4.10.00.p02.gamos/include -I/home/roldan/gamos510/GAMOS.5.1.0/source -I/home/roldan/gamos510/GAMOS.5.1.0/source -I/home/roldan/gamos510/external/root/5.34.10/root/include -I/home/roldan/gamos510/external/root/5.34.10/root/cint -I/home/roldan/gamos510/external/geant4/geant4.10.00.p02.gamos/source/visualization/OpenGL/include -c -o /home/roldan/gamos510/GAMOS.5.1.0/tmp/Linux-g++/gamos/exe/gamos.o gamos.cc
