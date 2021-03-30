
#ifndef LeptsElasticIonModel_h
#define LeptsElasticIonModel_h 1

#include "LeptsBaseModel.hh"

class LeptsElasticIonModel : public LeptsBaseModel { 

public:
  LeptsElasticIonModel(const G4String& modelName ="LeptsElasticIonModel");
  ~LeptsElasticIonModel();
  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0, G4double tmax = DBL_MAX);
};


class LeptsElasticSMFIonModel : public LeptsElasticIonModel {
public:
  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
				 const G4MaterialCutsCouple*,
				 const G4DynamicParticle*,
				 G4double tmin = 0.0, G4double tmax = DBL_MAX);
};


#endif
