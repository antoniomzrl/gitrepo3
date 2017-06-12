
#ifndef LeptsElectronicExcitation_h
#define LeptsElectronicExcitation_h 1

#include "G4VEmProcess.hh"

#include "G4DNAGenericIonsManager.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"


class LeptsElectronicExcitation : public G4VEmProcess {
  
public:
  LeptsElectronicExcitation(const G4String& processName ="LeptsExcitation",
			    G4ProcessType type = fElectromagnetic);

  virtual ~LeptsElectronicExcitation();
  virtual G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void PrintInfo();

protected:
  virtual void InitialiseProcess(const G4ParticleDefinition*);

private:
  G4bool       isInitialised;
};
  
#endif
