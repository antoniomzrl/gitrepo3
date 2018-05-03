
:MATE vacuum 1 1*g/mole 0*g/cm3 
:ROTM rmz 0 90 0 
:ROTM rm0 0 0 0 
:VOLU world BOX 10*cm 10*cm 10*cm vacuum 
:COLOUR world 0.5 0.5 0.5 
:MATE USERMAT 1 1*g/mole 1e6*g/cm3 
:VOLU chu TUBE 0.0 9*cm/2 1*mm/2 USERMAT 
:PLACE chu 1 world rmz -2.1*cm 0 0 
:VOLU chbbck TUBE 0.0 9*cm/2 2*cm/2 vacuum 
:PLACE chbbck 1 world rmz -1*cm 0 0 
:COLOUR chbbck 0 1 1 
:MIXT_BY_NATOMS BENZOQUINONE 05*atmosphere/760000*108.096*g/mole/(301*kelvin*8.617343e-11*MeV/kelvin*6.02214179e+23/mole) 3 C 6 H 4 O 2 
:VOLU chb TUBE 0.0 8*cm/2 4*cm/2 BENZOQUINONE 
:PLACE chb 1 world rmz 4*cm/2 0 0 
:COLOUR chb 1 1 0 
:VOLU chbana TUBE 0.0 9*cm/2 1*cm/2 vacuum 
:PLACE chbana 1 world rmz 4.5*cm 0 0 
:COLOUR chbana 0 1 1 
:VOLU gbox ORB 1*mm vacuum 
:PLACE gbox 1 world rmz -5*mm 0 0 
:COLOUR gbox 1 0 0 
