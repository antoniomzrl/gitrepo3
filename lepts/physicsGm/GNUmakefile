
name := lepts-phl-gm

G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

MYFLAGS =  #-Ofast -w #-Wunused-variable #-w
CXXFLAGS += $(MYFLAGS)
CPPFLAGS += -I../processes/include \
            -I../models/include \
            -I../physics/include \
            -I$(GAMOSINSTALL)/source/GamosCore/GamosPhysics/PhysicsList/include
EXTRALIBS += -lGamosCore_GamosUtils \
             -lGamosCore_GamosPhysics_PhysicsList \
             -llepts-phl -llepts-models -llepts-processes

