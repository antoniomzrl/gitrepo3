
:MATE vacuum 1 1*g/mole 1e-25*g/cm3 
:MIXT_BY_NATOMS G4_WATER_VAPOR 1*g/cm3 2 O 1 H 2 
:MATE iridium 77 192.217 22.42*g/cm3 
:MIXT steel316L 8.03*g/cm3 9 G4_Fe 0.68745 G4_Cr 0.19 G4_Ni 0.0925 G4_Mn 0.02 G4_Si 0.0075 G4_N 0.001 G4_C 0.0008 G4_P 0.00045 G4_S 0.0003 
:ROTM rm0 0 0 0 
:ROTM rmx 90 0 0 
:ROTM rmy 0 90 0 
:ROTM rmz 0 0 90 
:VOLU world BOX 70*cm 70*cm 70*cm vacuum 
:VOLU sphewat ORB 55*cm G4_WATER_VAPOR 
:PLACE sphewat 1 world rm0 0 0 0 
:COLOUR sphewat 0 1 0 
:VOLU cilv TUBE 0 0.9*mm/2 3.5*mm/2+0.065 vacuum 
:PLACE cilv 1 sphewat rmy 0 0 0 
:COLOUR cilv 1 0 0 
:VOLU theTarget ORB sqrt(005)/2*mm G4_WATER_VAPOR 
:PLACE theTarget 1 sphewat rm0 0 005*mm 0 
:COLOUR theTarget 1 0 0 
