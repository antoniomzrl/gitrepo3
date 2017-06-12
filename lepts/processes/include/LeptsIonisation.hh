
#ifndef LeptsIonisation_h
#define LeptsIonisation_h 1

#include "G4VEmProcess.hh"
#include "G4DNAGenericIonsManager.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"


class LeptsIonisation : public G4VEmProcess
{
  
public: 
  LeptsIonisation(const G4String& processName ="LeptsIonisation",
		  G4ProcessType type = fElectromagnetic);

  virtual ~LeptsIonisation();
  virtual G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void PrintInfo();

protected:
  virtual void InitialiseProcess(const G4ParticleDefinition*);

private:
  G4bool       isInitialised;

};
  
#endif
