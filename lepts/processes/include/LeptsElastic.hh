
#ifndef LeptsElastic_h
#define LeptsElastic_h 1

#include "G4VEmProcess.hh"
#include "G4Electron.hh"


class LeptsElastic : public G4VEmProcess
{
  
public: 
  //LeptsElastic(const G4String& processName ="LeptsElastic",
  LeptsElastic(const G4String& processName,
	       G4ProcessType type = fElectromagnetic);

  virtual ~LeptsElastic();
  virtual G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void PrintInfo();

protected:
  virtual void InitialiseProcess(const G4ParticleDefinition*);

private:
  G4bool       isInitialised;
};
  
#endif
