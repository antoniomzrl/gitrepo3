
#ifndef LeptsVibrationalExcitation_h
#define LeptsVibrationalExcitation_h 1

#include "G4VEmProcess.hh"


class LeptsVibrationalExcitation : public G4VEmProcess
{
  
public: 
  LeptsVibrationalExcitation(const G4String& processName ="LeptsVibrationalExcitation",
			     G4ProcessType type = fElectromagnetic);

  virtual ~LeptsVibrationalExcitation();
  virtual G4bool IsApplicable(const G4ParticleDefinition&);
  virtual void PrintInfo();
  static int ProcessSubType() {
    return 54;
  }

protected:
  virtual void InitialiseProcess(const G4ParticleDefinition*);

private:
  G4bool       isInitialised;

};

#endif
