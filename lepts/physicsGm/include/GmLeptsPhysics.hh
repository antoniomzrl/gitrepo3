
#ifndef GmLeptsPhysics_h
#define GmLeptsPhysics_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;

class GmLeptsPhysics: public G4VModularPhysicsList {
public:
  GmLeptsPhysics();
  ~GmLeptsPhysics();

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
    
  G4VPhysicsConstructor * theEmLEPTSPhysics;
  G4VPhysicsConstructor * theEmHighEnergyPhysics;

  G4String theHighEnergyModel;    
};


class GmLeptsSMFPhysics: public G4VModularPhysicsList {
public:
  GmLeptsSMFPhysics();
  ~GmLeptsSMFPhysics();

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
    
  G4VPhysicsConstructor * theEmLEPTSPhysics;
  G4VPhysicsConstructor * theEmHighEnergyPhysics;

  G4String theHighEnergyModel;    
};


#endif

