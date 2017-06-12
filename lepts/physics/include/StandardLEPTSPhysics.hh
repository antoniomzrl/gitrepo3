
#ifndef StandardLEPTSPhysics_h
#define StandardLEPTSPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"


class StandardLEPTSPhysics : public G4VPhysicsConstructor {
  
public:
  StandardLEPTSPhysics(G4double highELimit=10.*CLHEP::keV, G4int ver = 0);

  // obsolete
  StandardLEPTSPhysics(G4int ver, const G4String& name);

  virtual ~StandardLEPTSPhysics();
  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  G4int  verbose;

};


#endif






