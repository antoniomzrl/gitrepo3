# apt-get install libgtkglext1-dev libtiff5-dev g++-mingw-w64-i686 mingw-w64-tools
# apt-get install libglu1-mesa-dev libice-dev libsm-dev libxmu-dev libxi-dev libgtk2.0-dev
# apt-get install libglm-dev libglew-dev 

# cd win32dev/gtk
# find -name '*.pc' | while read pc; do sed -e "s@^prefix=.*@prefix=$PWD@" -i "$pc"; done
# cd win32dev/gtkglext
# idem

INSTALLDIR = /usr/local

     CXX = g++
  DEFFLG = -DDEFAULTPATH=\"$(INSTALLDIR)/colview\"
   MYFLG = -O3 -Wall #-pg -g  #-fbounds-check
   LDOPT = 
 CCGTKGL = $(shell pkg-config --cflags gtkglext-1.0)
 LDGTKGL = $(shell pkg-config --libs   gtkglext-1.0)
 GLEWINC = $(shell pkg-config --cflags glew) -I/usr/include/freetype2
 GLEWLIB = $(shell pkg-config --libs   glew) -lfreetype
 GTK3INC = $(shell pkg-config --cflags gtk+-3.0)
 GTK3LIB = $(shell pkg-config --libs   gtk+-3.0)	

    WCXX = i686-w64-mingw32-g++ #i586-mingw32msvc-g++ x86_64-w64-mingw32-g++
  WMYFLG = -mconsole -g #-mwindows -O3 -mms-bitfields
  WLDOPT = -static-libgcc -static-libstdc++ -s -Wl,-static #-Wl,--verbose -m32 
WCCGTKGL = $(shell ./my-pkg-config --cflags gtkglext-1.0) -I../win32dev/tiff/include
WLDGTKGL = $(shell ./my-pkg-config --libs   gtkglext-1.0)
WGLEWINC = -I../win32dev/glew/include -I ../win32dev/glm -I ../win32/freetype2/include
WGLEWLIB = -L../win32dev/glew/lib -lglew32 -L ../win32/freetype2/lib -lfreetype

   UAINC = -I../uas/include

 CXXFLG = $(DEFFLG) $(MYFLG)   $(GLEWINC)  $(CCGTKGL) $(UAINC)
#CXXFLG = $(DEFFLG) $(MYFLG)   $(GLEWINC)  $(GTK3INC) $(UAINC)
 WCXXFLG =           $(WMYFLG) $(WGLEWINC) $(WCCGTKGL) $(UAINC)

   LDFLG =  $(LDOPT)  $(GLEWLIB)  $(LDGTKGL) -lz  #-lglut -ltiff -s -static-libgcc
#  LDFLG =  $(LDOPT)  $(GLEWLIB)  $(GTK3LIB) -lz  #-lglut -ltiff -s -static-libgcc
  WLDFLG = $(WLDOPT) $(WGLEWLIB) $(WLDGTKGL) -lz.dll


SRCS = readfiles.cc readpetfiles.cc datah.cc dataobj.cc printp.cc \
       colorbartxt.cc  plotgltxt.cc \
       gpures.cc sphere.cc \
       expose.cc keys.cc callbacks.cc saveimg.cc \
       glwidgets.cc txtwidgets.cc  timer.cc points.cc \
       icosahvtx.cc plotob.cc plotlines.cc transform.cc \
       draw.cc mouse.cc colorsize.cc butwidgets.cc \
       loadshaders.cc glinfo.cc

OBJS = $(SRCS:.cc=.o)
WOBJS = $(SRCS:.cc=.obj)

all: colview

install: colview
	mkdir -p $(INSTALLDIR)/bin $(INSTALLDIR)/colview
	cp colview       $(INSTALLDIR)/bin
	cp shaders*.glsl $(INSTALLDIR)/colview
	cp tpi_*.txt     $(INSTALLDIR)/colview
	cp DejaVuSansMono.ttf $(INSTALLDIR)/colview

dist: colview
	mkdir -p        ../bincolview
	cp colview      ../bincolview
	cp shaders.glsl ../bincolview
	cp tpi_*.txt    ../bincolview
	cp install.sh   ../bincolview

installw: colview.exe
	mkdir -p ../wincolview
	mv colview.exe   ../wincolview
	cp shaders*.glsl ../wincolview
	cp tpi_*.txt     ../wincolview

v2n: v2n.o
	$(CXX) -o $@ $^ $(LDFLAGS)

colview: $(OBJS) colview.o
	$(CXX) -o $@ $^ $(LDFLG)

colview.exe: $(WOBJS) colview.obj
	$(WCXX) -o $@ $^ $(WLDFLG)


%.o: %.cc
	$(CXX) $(CXXFLG) -c -o $@ $^

%.obj: %.cc
	$(WCXX) $(WCXXFLG) -c -o $@ $^


clean:
	rm -f *.o *.obj *.exe *~  a.out


#make 'CXX=g++ -DDEBUG -g'
#g++ demowidget.cc -lglut -lfltk_gl

# objdump -p colview.exe | grep DLL

