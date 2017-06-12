
#ifndef LivermoreLEPTSPhysics_h
#define LivermoreLEPTSPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"


class LivermoreLEPTSPhysics : public G4VPhysicsConstructor {
  
public:
  LivermoreLEPTSPhysics(G4double highELimit = 10.*CLHEP::keV, G4int ver = 1);

  // obsolete
  LivermoreLEPTSPhysics(G4int ver, const G4String& name);

  virtual ~LivermoreLEPTSPhysics();
  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  G4int  verbose;
  G4double  theLEPTSHighEnergyLimit;

};

#endif
