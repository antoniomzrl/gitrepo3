
#ifndef LeptsThermalisationModel_h
#define LeptsThermalisationModel_h
#include "LeptsBaseModel.hh"

class LeptsThermalisationModel : public LeptsBaseModel {
  
public:
  LeptsThermalisationModel(const G4String& modelName ="LeptsThermalisationModel");
  ~LeptsThermalisationModel();

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0,
                                 G4double tmax = DBL_MAX);

};

#endif
