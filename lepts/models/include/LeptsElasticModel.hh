
#ifndef LeptsElasticModel_h
#define LeptsElasticModel_h 1

#include "LeptsBaseModel.hh"

class LeptsElasticModel : public LeptsBaseModel { 

public:
  LeptsElasticModel(const G4String& modelName ="LeptsElasticModel");
  ~LeptsElasticModel();
  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0, G4double tmax = DBL_MAX);
};


class LeptsElasticSMFModel : public LeptsElasticModel {
public:
  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
				 const G4MaterialCutsCouple*,
				 const G4DynamicParticle*,
				 G4double tmin = 0.0, G4double tmax = DBL_MAX);
};


#endif
