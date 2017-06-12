
#ifndef LeptsIonisationIonModel_h
#define LeptsIonisationIonModel_h
#include "LeptsBaseModel.hh"

class LeptsIonisationIonModel : public LeptsBaseModel  { 

public:
  LeptsIonisationIonModel(const G4String& modelName ="LeptsIonisationIonModel");
  ~LeptsIonisationIonModel();

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0,
                                 G4double tmax = DBL_MAX);

};

#endif
