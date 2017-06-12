
#ifndef LeptsAttachmentModel_h
#define LeptsAttachmentModel_h 1

#include "LeptsBaseModel.hh"

class LeptsAttachmentModel : public LeptsBaseModel {
  
public:
  LeptsAttachmentModel(const G4String& processName ="LeptsAttachmentModel");
  ~LeptsAttachmentModel();

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0,
                                 G4double tmax = DBL_MAX);

};


#endif
