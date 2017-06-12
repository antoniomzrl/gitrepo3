
#  FC = ifort
# FLG = -cpp -g

FC  = gfortran -std=legacy -O3 #-g
FLG =

#FS = ./penelope2005/fsource
#FS = ./penelope2006/fsource
#FS = ./penelope2008.1/fsource
FS = $(HOME)/penelope2011/fsource

OBJS = $(FS)/penelope.o $(FS)/pengeom.o $(FS)/penvared.o $(FS)/timer.o \
       $(FS)/rita.f # 2011

all: material penslab

penslab: penslab.o $(OBJS)
	$(FC) -o $@ $^

material: $(FS)/material.o
	$(FC) -o $@ $^

%.o: %.f
	$(FC) -c $(FLG) -o $@ $<

clean:
	rm -f *.o *~ a.out $(FS)/*.o penslab material
