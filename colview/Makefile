# apt-get install libglu1-mesa-dev libice-dev libsm-dev libxmu-dev libxi-dev
# apt-get install libgtk-3-dev libglm-dev libtiff5-dev
# apt-get install g++-mingw-w64-i686 mingw-w64-tools

INSTALLDIR = /usr/local

     CXX = g++
  DEFFLG = -DDEFAULTPATH=\"$(INSTALLDIR)/colview\"
   MYFLG = -O3 -Wall #-pg -g  #-fbounds-check
   LDOPT =

 GTK3INC = $(shell pkg-config --cflags gio-2.0 gtk+-3.0 epoxy) -I/usr/include/freetype2
 GTK3LIB = $(shell pkg-config --libs   gio-2.0 gtk+-3.0 epoxy) -lfreetype -lfontconfig


   UAINC = -I../uas/include
  CXXFLG = $(DEFFLG) $(MYFLG)   $(GTK3INC) $(UAINC)
   LDFLG =  $(LDOPT)  $(GTK3LIB) -lz #-lglut -ltiff -s -static-libgcc



SRCS = readfiles.cc readpetfiles.cc datah.cc dataobj.cc printp.cc \
       colorbartxt.cc  plotgltxt.cc \
       expose.cc keys.cc callbacks.cc saveimg.cc \
       glwidgets.cc txtwidgets.cc  timer.cc points.cc \
       icosahvtx.cc plotob.cc plotlines.cc transform.cc \
       draw.cc mouse.cc colorsize.cc butwidgets.cc \
       loadshaders.cc glinfo.cc gpures.cc 

OBJS = $(SRCS:.cc=.o)
WOBJS = $(SRCS:.cc=.obj)

all: colview

install: colview
	mkdir -p $(INSTALLDIR)/bin $(INSTALLDIR)/colview
	cp colview       $(INSTALLDIR)/bin
	cp shaders*.glsl $(INSTALLDIR)/colview
	cp tpi_*.txt     $(INSTALLDIR)/colview
	cp *tf           $(INSTALLDIR)/colview

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

