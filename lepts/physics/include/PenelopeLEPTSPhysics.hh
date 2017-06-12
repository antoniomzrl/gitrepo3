
#ifndef PenelopeLEPTSPhysics_h
#define PenelopeLEPTSPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"


class PenelopeLEPTSPhysics : public G4VPhysicsConstructor {
  
public:
  PenelopeLEPTSPhysics(G4double highELimit = 10*CLHEP::keV, G4int ver = 1);

  // obsolete
  PenelopeLEPTSPhysics(G4double highELimit, G4int ver, const G4String& name);

  virtual ~PenelopeLEPTSPhysics();
  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  G4int  verbose;

};


#endif






