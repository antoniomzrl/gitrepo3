
#ifndef LeptsDissociation_h
#define LeptsDissociation_h 1

#include "G4VEmProcess.hh"

class LeptsDissociation : public G4VEmProcess
{
  
public:
  LeptsDissociation(const G4String& processName ="LeptsDissociation");
  ~LeptsDissociation();

  G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void InitialiseProcess(const G4ParticleDefinition*);
  virtual void PrintInfo();
  
private:
  G4bool       isInitialised;

};

#endif
