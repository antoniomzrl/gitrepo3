
#ifndef LeptsChargeTransferModel_h
#define LeptsChargeTransferModel_h 1

#include "LeptsBaseModel.hh"

class LeptsChargeTransferModel : public LeptsBaseModel {
  
public:
  LeptsChargeTransferModel(const G4String& modelName ="LeptsChargeTransferModel");
  ~LeptsChargeTransferModel();

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0,
                                 G4double tmax = DBL_MAX);

};


#endif
