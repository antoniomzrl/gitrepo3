# export CPPVERBOSE=1
# export G4DEBUG=1
# make 'CXX=g++32'

# make -j4 lib ; make

name := leptsuas
G4TARGET := $(name)
G4EXLIB := true


.PHONY: all
#all: lib bin
#all: lib plugin plugin_check
all: lib

#include $(G4INSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

MYFLAGS =  -Ofast -w #-Wunused-variable #-w

include $(GAMOSINSTALL)/config/gamos_libraries.gmk
CXXFLAGS += $(MYFLAGS)
CPPFLAGS += -I$(GAMOSINSTALL)/source/GamosCore/GamosPhysics/OtherPhysicsLists/include

#EXTRALIBS += -lGamosBase -lGamosSD -lGamosGenerator -lGamosFactories \
#             -lGamosUserActionMgr -lGamosUtils
#gamos220
#EXTRALIBS += -Wl,--no-as-needed -ldl -lGamosSD -lGamosGenerator -lGamosUserActionMgr -lGamosUtils
#gamos300
#CXXFLAGS += -Wunused
#G4-DNA
#CPPFLAGS += -I$(G4INSTALL)/source/processes/electromagnetic/dna/processes/include \
#            -I$(G4INSTALL)/source/processes/electromagnetic/dna/models/include/ \
#            -I$(G4INSTALL)/source/processes/electromagnetic/dna/utils/include

limpia:
	rm  -f *~ */*~

# make plugin_check
# SealPluginDump

euler:
	(cd .. ; gitcom.sh ; git push)
	ssh euler lepts/bin/bldapp.sh
