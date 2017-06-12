
#ifndef __LeptsPhysics__
#define __LeptsPhysics__

#include "G4VPhysicsConstructor.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ios.hh"
// Geant461: #include "g4std/iomanip"
#include "G4Decay.hh"
#include "G4UserSpecialCuts.hh"
#include "G4StepLimiter.hh"
#include "globals.hh"


class LeptsPhysics : public G4VPhysicsConstructor {
  
public:
  LeptsPhysics(const G4String& name="LeptsPhysics");
  virtual ~LeptsPhysics(){};

  virtual void ConstructParticle();
  virtual void ConstructProcess();

protected:
  // Physics

};


// strong mag field
class LeptsSMFPhysics : public G4VPhysicsConstructor {
  
public:
  LeptsSMFPhysics(const G4String& name="LeptsSMFPhysics");
  virtual ~LeptsSMFPhysics(){};

  virtual void ConstructParticle();
  virtual void ConstructProcess();

protected:
  // Physics

};


#endif
