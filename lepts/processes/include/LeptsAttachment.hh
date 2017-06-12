
#ifndef LeptsAttachment_h
#define LeptsAttachment_h 1

#include "G4VEmProcess.hh"


class LeptsAttachment : public G4VEmProcess
{
public: 

  LeptsAttachment(const G4String& processName ="LeptsAttachment",
		  G4ProcessType type = fElectromagnetic);

  virtual ~LeptsAttachment();
  virtual G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void PrintInfo();

protected:
  virtual void InitialiseProcess(const G4ParticleDefinition*);

private:
  G4bool       isInitialised;
};
  
#endif
