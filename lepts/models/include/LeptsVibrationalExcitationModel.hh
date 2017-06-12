
#ifndef LeptsVibrationalExcitationModel_h
#define LeptsVibrationalExcitationModel_h
#include "LeptsBaseModel.hh"

class LeptsVibrationalExcitationModel : public LeptsBaseModel  { 

public:
  LeptsVibrationalExcitationModel(const G4String& modelName ="LeptsVibrationalExcitationModel");
  ~LeptsVibrationalExcitationModel();

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0,
                                 G4double tmax = DBL_MAX);
};

class LeptsVibrationalExcitationSMFModel : public LeptsVibrationalExcitationModel { 

public:
  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0, G4double tmax = DBL_MAX);
};



#endif
