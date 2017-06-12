
#ifndef LeptsAnnihilation_h
#define LeptsAnnihilation_h 1

#include "G4VEmProcess.hh"

class LeptsAnnihilation : public G4VEmProcess
{
  
public:
  LeptsAnnihilation(const G4String& processName ="LeptsAnnihilation");
  ~LeptsAnnihilation();

  G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void InitialiseProcess(const G4ParticleDefinition*);
  virtual void PrintInfo();
  
private:
  G4bool       isInitialised;

};

#endif
