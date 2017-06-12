
#ifndef GmDNAPhysics_h
#define GmDNAPhysics_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;


class GmDNAPhysics: public G4VModularPhysicsList {
public:
  GmDNAPhysics();
  ~GmDNAPhysics();

  void ConstructParticle();
  void ConstructProcess();

  void SetCuts();
  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForPositron(G4double);
      
private:
  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;
  G4double currentDefaultCut;
    
  G4VPhysicsConstructor*  theEmDNAPhysics;
  G4VPhysicsConstructor*  theEmHighEnergyPhysics;

  G4String theHighEnergyModel;    
};

#endif

