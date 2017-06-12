
#ifndef LeptsIonisationModel_h
#define LeptsIonisationModel_h
#include "LeptsBaseModel.hh"

class LeptsIonisationModel : public LeptsBaseModel {
  
public:
  LeptsIonisationModel(const G4String& modelName ="LeptsIonisationModel");
  ~LeptsIonisationModel();

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0, G4double tmax = DBL_MAX);

};



class LeptsIonisationSMFModel : public LeptsIonisationModel {
  
public:
  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0, G4double tmax = DBL_MAX);

};

#endif
