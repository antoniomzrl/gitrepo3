
//WORLD 
:MATE vacuum 1 1*g/mole 1e-25*g/cm3 
:ROTM rmz 0 90 0 
:ROTM rm0 0 0 0 
:VOLU world BOX 50*cm 50*cm 50*cm vacuum 
:COLOUR world 1 1 1 
//CHAMBER_G4_WATER 
:VOLU ch_G4_WATER BOX 1*cm 1*cm 1*cm G4_WATER 
:PLACE ch_G4_WATER 1 world rm0 1*cm 0 0 
:COLOUR ch_G4_WATER 0.4 0.4 0.8 
//GENERATOR_BALL 
:VOLU gbox ORB 1*nm/2 vacuum 
:PLACE gbox 1 world rmz -1*cm 0 0 
:COLOUR gbox 0 0 1 
