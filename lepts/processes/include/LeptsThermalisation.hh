
#ifndef LeptsThermalisation_h
#define LeptsThermalisation_h 1

#include "G4VEmProcess.hh"

class LeptsThermalisation : public G4VEmProcess
{ 

public:
  LeptsThermalisation(const G4String& processName ="LeptsThermalisation");
  ~LeptsThermalisation();

  G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void InitialiseProcess(const G4ParticleDefinition*);
  virtual void PrintInfo();
  
private:
  G4bool       isInitialised;

};

#endif
