
// AMR physics_lists/constructors/electromagnetic/include/G4EmStandardPhysicsSS.hh

#ifndef EMSSPhysics_h
#define EMSSPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EMSSPhysics : public G4VPhysicsConstructor
{
public:
  EMSSPhysics(G4int ver = 1);

  virtual ~EMSSPhysics();

  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  G4int  verbose;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif






