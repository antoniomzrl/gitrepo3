
#ifndef LeptsRotationalExcitation_h
#define LeptsRotationalExcitation_h 1

#include "G4VEmProcess.hh"

class LeptsRotationalExcitation : public G4VEmProcess
{
  
public:
  LeptsRotationalExcitation(const G4String& processName ="LeptsRotationalExcitation");
  ~LeptsRotationalExcitation();

  G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void InitialiseProcess(const G4ParticleDefinition*);
  virtual void PrintInfo();
  
private:
  G4bool       isInitialised;

};

#endif
