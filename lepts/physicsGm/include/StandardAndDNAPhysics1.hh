
// AMR from "examples/extended/medical/dna/microdosimetry"
#ifndef StandardAndDNAPhysics1_h
#define StandardAndDNAPhysics1_h 1

#include "G4VUserPhysicsList.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"


class StandardAndDNAPhysics1: public G4VUserPhysicsList {
  
public:

  StandardAndDNAPhysics1();
  virtual ~StandardAndDNAPhysics1();

  void SetGammaCut(G4double);
  void SetElectronCut(G4double);
  void SetPositronCut(G4double);
  void SetProtonCut(G4double);
  
protected:
  // these methods construct particles 
  void ConstructBosons();
  void ConstructLeptons();
  void ConstructBarions();

  // these methods construct physics processes and register them
  void ConstructGeneral();
  void ConstructEM();

  // Construct particle and physics
  virtual void ConstructParticle();
  virtual void ConstructProcess();
  
  // set cuts
  void SetCuts();
    
private:
  G4double fCutForGamma;
  G4double fCutForElectron;
  G4double fCutForPositron;
  G4double fCutForProton;
  
};
#endif
