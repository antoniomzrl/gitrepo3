
#ifndef LeptsDissociationModel_h
#define LeptsDissociationModel_h
#include "LeptsBaseModel.hh"

class LeptsDissociationModel : public LeptsBaseModel {
  
public:
  LeptsDissociationModel(const G4String& modelName ="LeptsDissociationModel");
  ~LeptsDissociationModel();

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0,
                                 G4double tmax = DBL_MAX);

};

#endif
