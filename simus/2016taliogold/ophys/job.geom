
//WORLD 
:MATE vacuum 1 1*g/mole 1e-25*g/cm3 
:ROTM rmz 0 90 0 
:ROTM rm0 0 0 0 
:VOLU world BOX 5*mm 5*mm 5*mm vacuum 
:COLOUR world 1 1 1 
//coin-slab 
:VOLU ch_G4_Au TUBE 0.0 1*mm/2 20*um/2 G4_Au 
:PLACE ch_G4_Au 1 world rmz 0 0 0 
:COLOUR ch_G4_Au 0.6 0.6 0.0 
//GENERATOR_BALL 
:VOLU gbox ORB 10*um/2 vacuum 
:PLACE gbox 1 world rmz -2*mm 0 0 
:COLOUR gbox 0 0 1 
