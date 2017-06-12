
     CXX = g++ -std=c++11  -g #-fbounds-check -Wall
CXXFLAGS = 
 LDGTKGL = 
 LDFLAGS = -lm

all: joblepts jgamos mkhg joinhg clipgf sphgbcd medvar mfactor dxstab idxs penjoin

install: all
	mkdir -p ../bin
	mv joblepts jgamos mkhg joinhg clipgf sphgbcd medvar ../bin


joblepts: joblepts.o
	$(CXX) -o $@ $^ $(LDFLAGS)

jgamos: jgamos.o
	$(CXX) -o $@ $^ $(LDFLAGS)

sphgbcd: sphgbcd.o
	$(CXX) -o $@ $^ $(LDFLAGS)

mkhg: mkhg.o histogram.o
	$(CXX) -o $@ $^ $(LDFLAGS)

joinhg: joinhg.o histogram.o
	$(CXX) -o $@ $^ $(LDFLAGS)

clipgf: clipgf.o
	$(CXX) -o $@ $^ $(LDFLAGS)

medvar: medvar.f90
	f95 -o $@ $^
mfactor: mfactor.f90
	f95 -o $@ $^
dxstab: dxstab.f90
	f95 -o $@ $^
idxs: idxs.f90
	f95 -o $@ $^
penjoin: penjoin.f90
	f95 -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $^

clean:
	rm -f *.o *.obj *.exe *~ a.out
