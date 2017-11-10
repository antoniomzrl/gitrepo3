
//WORLD 
:MATE vacuum 1 1*g/mole 1e-25*g/cm3 
:ROTM rmz 0 90 0 
:ROTM rm0 0 0 0 
:VOLU world BOX 3*cm 3*cm 3*cm vacuum 
:COLOUR world 1 1 1 
:MATE PYRIMIDINE 1.0 80*g/mole 1.016*g/cm3 
//CHAMBER_PYRIMIDINE 
:VOLU ch_PYRIMIDINE BOX 1*cm 1*cm 1*cm PYRIMIDINE 
:PLACE ch_PYRIMIDINE 1 world rm0 1*cm 0 0 
:COLOUR ch_PYRIMIDINE 0.4 0.4 0.8 
//GENERATOR_BOX 
:VOLU gbox BOX 50*nm*0.1 50*nm 50*nm vacuum 
:PLACE gbox 1 world rmz -5*mm 0 0 
:COLOUR gbox 0 0 1 
