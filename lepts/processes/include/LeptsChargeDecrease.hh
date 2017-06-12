
#ifndef LeptsChargeDecrease_h
#define LeptsChargeDecrease_h 1

#include "G4VEmProcess.hh"

#include "G4DNAGenericIonsManager.hh"
#include "G4Proton.hh"


class LeptsChargeDecrease : public G4VEmProcess
{
  
public: 
  LeptsChargeDecrease(const G4String& processName ="LeptsChargeDecrease",
		      G4ProcessType type = fElectromagnetic);

  virtual ~LeptsChargeDecrease();
  virtual G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void PrintInfo();

protected:
  virtual void InitialiseProcess(const G4ParticleDefinition*);

private:
  G4bool       isInitialised;
};
  
#endif
