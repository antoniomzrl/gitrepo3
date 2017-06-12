
#ifndef LeptsChargeIncrease_h
#define LeptsChargeIncrease_h 1

#include "G4VEmProcess.hh"
#include "G4DNAGenericIonsManager.hh"

class LeptsChargeIncrease : public G4VEmProcess
{
  
public: 
  LeptsChargeIncrease(const G4String& processName ="LeptsChargeIncrease",
		      G4ProcessType type = fElectromagnetic);

  virtual ~LeptsChargeIncrease();
  virtual G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void PrintInfo();

protected:
  virtual void InitialiseProcess(const G4ParticleDefinition*);

private:
  G4bool       isInitialised;

};
  
#endif
