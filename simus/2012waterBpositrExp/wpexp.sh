#!/bin/bash

source $HOME/lepts/conflepts.sh

# camara 20cm
ch20cm="
   worldbox  15*cm
   chamber   ch_water,waterB,5*cm,10*cm,0    #chamber chname,material,R,L/2,PosX
   generator gdisc,vacio,0.75*mm,1*mm,-10.5*cm
   detector  detdisc,gold,1*cm,1*mm,11*cm
   detector2 detdisc2,gold,1*cm,1*mm,-11*cm
   diafragm  detdiaf,gold,4*cm,10*um,12*cm
"

Avogadro=6.02214179e+23/mole
k_Boltzmann=8.617343e-11*MeV/kelvin
KbNa=${k_Boltzmann}'*'${Avogadro}
MM=18*g/mole


#for Pmt in 0.4 ; do
#for E in 10 ; do
for Pmt in 0.2 0.4 0.6 1.0 ; do
for E in 05 10 30 50 ; do

    Pres=${Pmt}*atmosphere/760000
    Temp=303.55*kelvin
    Dens=${Pres}*${MM}'/('${Temp}*${KbNa}')'
    Ein=${E}*eV

    FL="
      density  ${Dens}
      gparticle e+
      genergy  ${Ein}
      xsdat    waterB
      material waterB
      #vis      vrml
      #verbose parser,gamos,lepts,headers,histograms,detectorhits
      #verbose histograms

      #beamon   100000

      #beamon   10000000
      #batch    00:20:00

      beamon   1000000000
      batch    26:00:00
    "

    DO='E'${E}'_P'${Pmt}
    time joblepts $ch20cm $FL dirjob $DO > k${DO}
done
done

