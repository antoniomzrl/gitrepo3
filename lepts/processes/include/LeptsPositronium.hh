
#ifndef LeptsPositronium_h
#define LeptsPositronium_h 1

#include "G4VEmProcess.hh"

class LeptsPositronium : public G4VEmProcess
{
  
public:
  LeptsPositronium(const G4String& processName ="LeptsPositronium");
  ~LeptsPositronium();

  G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void InitialiseProcess(const G4ParticleDefinition*);
  virtual void PrintInfo();
  
private:
  G4bool       isInitialised;

};

#endif
