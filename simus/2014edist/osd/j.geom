
//WORLD 
:MATE vacuum 1 1*g/mole 1e-25*g/cm3 
:ROTM rmz 0 90 0 
:ROTM rm0 0 0 0 
:VOLU world BOX 20*cm 20*cm 20*cm vacuum 
:COLOUR world 1 1 1 
:MATE G4_WATER 1.0 18*g/mole 1000*atmosphere/760000*18*g/mole/(303.55*kelvin*8.617343e-11*MeV/kelvin*6.02214179e+23/mole) 
//coin-slab 
:VOLU ch_G4_WATER TUBE 0.0 15*cm/2 14*cm/2 G4_WATER 
:PLACE ch_G4_WATER 1 world rmz 7*cm 0 0 
:COLOUR ch_G4_WATER 0.6 0.6 0.0 
//GENERATOR_BALL 
:VOLU gbox ORB 5*um/2 vacuum 
:PLACE gbox 1 world rmz -1*cm 0 0 
:COLOUR gbox 0 0 1 
