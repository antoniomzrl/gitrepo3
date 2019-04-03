//WORLD, box 3m x 3m x 3m, vacuum
:MATE vacuum 1 1*g/mole 1e-25*g/cm3
:ROTM rmz 0 90 0
:VOLU world BOX 1.5*m 1.5*m 1.5*m vacuum 

//CHAMBER WATER, liquid density, 1m3 placed 25cm right
:MATE G4_WATER 1.0 18*g/mole 1*g/cm3 
:VOLU ch_G4_WATER BOX 0.5*m 0.5*m 0.5*m G4_WATER
:PLACE ch_G4_WATER 1 world rmz 0.25*m 0 0 

//GENERATOR_BOX, placed 40 cm left 
:VOLU gbox BOX 5*cm 5*cm 5*cm vacuum 
:PLACE gbox 1 world rmz -70*cm 0 0
