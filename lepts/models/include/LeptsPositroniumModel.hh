
#ifndef LeptsPositroniumModel_h
#define LeptsPositroniumModel_h
#include "LeptsBaseModel.hh"

class LeptsPositroniumModel : public LeptsBaseModel  {
  
public:
  LeptsPositroniumModel(const G4String& modelName ="LeptsPositroniumModel");
  ~LeptsPositroniumModel();

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0,
                                 G4double tmax = DBL_MAX);

};

#endif
