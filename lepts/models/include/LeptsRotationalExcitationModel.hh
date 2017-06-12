
#ifndef LeptsRotationalExcitationModel_h
#define LeptsRotationalExcitationModel_h
#include "LeptsBaseModel.hh"

class LeptsRotationalExcitationModel : public LeptsBaseModel 
{ 
public:
  LeptsRotationalExcitationModel(const G4String& modelName ="LeptsRotationalExcitationModel");
  ~LeptsRotationalExcitationModel();

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0, G4double tmax = DBL_MAX);
};



class LeptsRotationalExcitationSMFModel : public LeptsRotationalExcitationModel { 

public:
  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0, G4double tmax = DBL_MAX);
};

#endif
