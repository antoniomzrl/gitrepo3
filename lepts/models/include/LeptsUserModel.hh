
#ifndef LeptsUserModel_h
#define LeptsUserModel_h 1

#include "LeptsBaseModel.hh"

class LeptsUserModel : public LeptsBaseModel { 

public:
  LeptsUserModel(const G4String& modelName ="LeptsUserModel");
  ~LeptsUserModel();
  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0, G4double tmax = DBL_MAX);
};


class LeptsUserSMFModel : public LeptsUserModel {
public:
  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
				 const G4MaterialCutsCouple*,
				 const G4DynamicParticle*,
				 G4double tmin = 0.0, G4double tmax = DBL_MAX);
};


#endif
