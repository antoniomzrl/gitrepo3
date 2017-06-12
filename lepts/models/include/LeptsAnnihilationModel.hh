
#ifndef LeptsAnnihilationModel_h
#define LeptsAnnihilationModel_h
#include "LeptsBaseModel.hh"

class LeptsAnnihilationModel : public LeptsBaseModel {

public:
  LeptsAnnihilationModel(const G4String& modelName ="LeptsAnnihilationModel");
  ~LeptsAnnihilationModel();

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0,
                                 G4double tmax = DBL_MAX);

};

#endif
