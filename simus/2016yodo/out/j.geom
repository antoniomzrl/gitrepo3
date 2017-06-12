
//WORLD 
:MATE vacuum 1 1*g/mole 1e-25*g/cm3 
:VOLU world BOX 15*cm 15*cm 15*cm vacuum 
:COLOUR world 1 1 1 
:ROTM rm0 0 0 0 
:ROTM rmx 90 0 0 
:ROTM rmy 0 90 0 
:ROTM rmz 0 0 90 
:VOLU cuba BOX 10*cm 10*cm 10*cm G4_WATER 
:PLACE cuba 1 world rm0 0 0 0 
:COLOUR cuba 0.4 0.4 0.8 
//SEED_I125 
:ROTM rmt 0 0 0 
:VOLU gseio TUBE 0 0.4*mm 2*mm vacuum 
:COLOUR gseio 1 0 0 
:PLACE gseio 0 cuba rmt -0.5*cm -0.2*cm 0*cm 
:PLACE gseio 1 cuba rmt 1.5*cm 1*cm 0*cm 
:PLACE gseio 2 cuba rmt -1*cm 2*cm 0*cm 
:PLACE gseio 3 cuba rmt 1.5*cm 0*cm 0.5*cm 
:PLACE gseio 4 cuba rmt 2*cm 0.5*cm 0.5*cm 
:PLACE gseio 5 cuba rmt -1*cm 1.05*cm 0.5*cm 
:PLACE gseio 6 cuba rmt 1.5*cm 1.5*cm 0.5*cm 
:PLACE gseio 7 cuba rmt 0*cm 2*cm 0.5*cm 
:PLACE gseio 8 cuba rmt -1.05*cm 2.4*cm 0.5*cm 
:PLACE gseio 9 cuba rmt 1.05*cm 2.4*cm 0.5*cm 
:PLACE gseio 10 cuba rmt -0.5*cm -1*cm 1*cm 
:PLACE gseio 11 cuba rmt 1.5*cm -0.45*cm 1*cm 
:PLACE gseio 12 cuba rmt -0.5*cm -0.2*cm 1*cm 
:PLACE gseio 13 cuba rmt -1.45*cm 0.05*cm 1*cm 
:PLACE gseio 14 cuba rmt 1.5*cm 1*cm 1*cm 
:PLACE gseio 15 cuba rmt -1*cm 2*cm 1*cm 
:PLACE gseio 16 cuba rmt 2.15*cm 1.8*cm 1*cm 
:PLACE gseio 17 cuba rmt -1.45*cm -0.4*cm 1.5*cm 
:PLACE gseio 18 cuba rmt 1.5*cm -0.05*cm 1.5*cm 
:PLACE gseio 19 cuba rmt 2*cm 0.5*cm 1.5*cm 
:PLACE gseio 20 cuba rmt -1*cm 1*cm 1.5*cm 
:PLACE gseio 21 cuba rmt -2*cm 1.5*cm 1.5*cm 
:PLACE gseio 22 cuba rmt 1.5*cm 1.5*cm 1.5*cm 
:PLACE gseio 23 cuba rmt 0*cm 1.95*cm 1.5*cm 
:PLACE gseio 24 cuba rmt -1.05*cm 2.4*cm 1.5*cm 
:PLACE gseio 25 cuba rmt 1.05*cm 2.4*cm 1.5*cm 
:PLACE gseio 26 cuba rmt -0.5*cm -1*cm 2*cm 
:PLACE gseio 27 cuba rmt 1.5*cm -0.45*cm 2*cm 
:PLACE gseio 28 cuba rmt -0.5*cm -0.2*cm 2*cm 
:PLACE gseio 29 cuba rmt -1.5*cm 0*cm 2*cm 
:PLACE gseio 30 cuba rmt 1.5*cm 1*cm 2*cm 
:PLACE gseio 31 cuba rmt -1*cm 1.95*cm 2*cm 
:PLACE gseio 32 cuba rmt 2.15*cm 1.8*cm 2*cm 
:PLACE gseio 33 cuba rmt -1.45*cm -0.4*cm 2.5*cm 
:PLACE gseio 34 cuba rmt 1.5*cm -0.05*cm 2.5*cm 
:PLACE gseio 35 cuba rmt 2.05*cm 0.45*cm 2.5*cm 
:PLACE gseio 36 cuba rmt -1*cm 1*cm 2.5*cm 
:PLACE gseio 37 cuba rmt -2*cm 1.5*cm 2.5*cm 
:PLACE gseio 38 cuba rmt 1.5*cm 1.5*cm 2.5*cm 
:PLACE gseio 39 cuba rmt 0*cm 2.05*cm 2.5*cm 
:PLACE gseio 40 cuba rmt -1.05*cm 2.4*cm 2.5*cm 
:PLACE gseio 41 cuba rmt 1.05*cm 2.4*cm 2.5*cm 
:PLACE gseio 42 cuba rmt -0.5*cm -1*cm 3*cm 
:PLACE gseio 43 cuba rmt 1.5*cm -0.5*cm 3*cm 
:PLACE gseio 44 cuba rmt -0.5*cm -0.2*cm 3*cm 
:PLACE gseio 45 cuba rmt -1.5*cm 0*cm 3*cm 
:PLACE gseio 46 cuba rmt 1.5*cm 1*cm 3*cm 
:PLACE gseio 47 cuba rmt -1.05*cm 1.9*cm 3*cm 
:PLACE gseio 48 cuba rmt 2.15*cm 1.8*cm 3.08*cm 
:PLACE gseio 49 cuba rmt -1.45*cm -0.4*cm 3.5*cm 
:PLACE gseio 50 cuba rmt 1.55*cm -0.1*cm 3.5*cm 
:PLACE gseio 51 cuba rmt 1*cm 0.4*cm 3.5*cm 
:PLACE gseio 52 cuba rmt 2*cm 0.5*cm 3.5*cm 
:PLACE gseio 53 cuba rmt -1.05*cm 0.85*cm 3.5*cm 
:PLACE gseio 54 cuba rmt 1.5*cm 1.45*cm 3.5*cm 
:PLACE gseio 55 cuba rmt -2*cm 1.5*cm 3.5*cm 
:PLACE gseio 56 cuba rmt 0*cm 2*cm 3.5*cm 
:PLACE gseio 57 cuba rmt 1.05*cm 2.4*cm 3.5*cm 
:PLACE gseio 58 cuba rmt -0.5*cm -0.2*cm 4*cm 
:PLACE gseio 59 cuba rmt -1.5*cm 0*cm 4*cm 
:PLACE gseio 60 cuba rmt 1.5*cm 0.95*cm 4*cm 
:PLACE gseio 61 cuba rmt -1.05*cm 1.85*cm 4*cm 
:PLACE gseio 62 cuba rmt 1.05*cm 0.35*cm 4.5*cm 
:PLACE gseio 63 cuba rmt -1.05*cm 0.8*cm 4.5*cm 
:PLACE gseio 64 cuba rmt 0*cm 1.95*cm 4.5*cm 
:VOLU sph_0_3.2_1.3 ORB 2*mm G4_WATER 
:PLACE sph_0_3.2_1.3 1 cuba rm0 0*cm 3.2*cm 1.3*cm 
:COLOUR sph_0_3.2_1.3 1 1 1 
:VOLU sph_-1.4_4.4_4.5 ORB 2*mm G4_WATER 
:PLACE sph_-1.4_4.4_4.5 1 cuba rm0 -1.4*cm 4.4*cm 4.5*cm 
:COLOUR sph_-1.4_4.4_4.5 1 1 1 
:VOLU sph_0.3_0_5.3 ORB 2*mm G4_WATER 
:PLACE sph_0.3_0_5.3 1 cuba rm0 0.3*cm 0*cm 5.3*cm 
:COLOUR sph_0.3_0_5.3 1 1 1 
:VOLU sph_0.2_0.8_2 ORB 2*mm G4_WATER 
:PLACE sph_0.2_0.8_2 1 cuba rm0 0.2*cm 0.8*cm 2*cm 
:COLOUR sph_0.2_0.8_2 1 1 1 
:VOLU sph_-0.7_0.3_2 ORB 2*mm G4_WATER 
:PLACE sph_-0.7_0.3_2 1 cuba rm0 -0.7*cm 0.3*cm 2*cm 
:COLOUR sph_-0.7_0.3_2 1 1 1 
:VOLU sph_-4.9_2.9_5.3 ORB 2*mm G4_WATER 
:PLACE sph_-4.9_2.9_5.3 1 cuba rm0 -4.9*cm 2.9*cm 5.3*cm 
:COLOUR sph_-4.9_2.9_5.3 1 1 1 
:VOLU sph_-8.2_-3.5_0 ORB 2*mm G4_WATER 
:PLACE sph_-8.2_-3.5_0 1 cuba rm0 -8.2*cm -3.5*cm 0*cm 
:COLOUR sph_-8.2_-3.5_0 1 1 1 
:VOLU sph_4.4_0.6_2 ORB 2*mm G4_WATER 
:PLACE sph_4.4_0.6_2 1 cuba rm0 4.4*cm 0.6*cm 2*cm 
:COLOUR sph_4.4_0.6_2 1 1 1 
:VOLU sph_0.0_-10.0_8.0 ORB 2*mm G4_WATER 
:PLACE sph_0.0_-10.0_8.0 1 cuba rm0 0.0*cm -10.0*cm 8.0*cm 
:COLOUR sph_0.0_-10.0_8.0 1 1 1 
