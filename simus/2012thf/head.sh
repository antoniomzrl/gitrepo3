#!/bin/bash

source $HOME/lepts/conflepts.sh

Avogadro=6.02214179e+23/mole
k_Boltzmann=8.617343e-11*MeV/kelvin
KbNa=${k_Boltzmann}'*'${Avogadro}
MM=72.11*g/mole
#Dens = Pres * MM / (Kb*Na*Temp)

#chamber chname,material,R,L/2,PosX


# camara de L=10cm
ch10cm="
   worldbox  7*cm
   chamber   ch_thf,THF,1*cm,5*cm,0
   generator gdisc,vacio,0.75*mm,1*mm,-6*cm
   detector  detdisc,gold,1*mm,1*mm,59*mm
   diafragm  detdiaf,gold,0.75*mm,10*um,5.01*cm
"


# camara de L=2.11cm
ch2cm11="
   worldbox  2.5*cm
   chamber   ch_thf,THF,10*mm,11.05*mm,0
   generator gdisc,vacio,0.75*mm,1*mm,-15*mm
   detector  detdisc,gold,1*mm,1*mm,20.05*mm
   diafragm  detdiaf,gold,0.75*mm,10*um,11.2*mm
"


# camara de L=2.11cm haz fino y det pequeño
chpeq="
   worldbox  2.5*cm
   chamber   ch_thf,THF,10*mm,11.05*mm,0
   generator gdisc,vacio,1*um,1*mm,-15*mm
   detector  detdisc,gold,1*um,1*mm,20.05*mm
   diafragm  detdiaf,gold,1*um,10*um,11.2*mm
"


CFL="
   xsdat     THF
   gparticle e+
   beamon    100
"
