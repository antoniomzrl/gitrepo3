
#ifndef LeptsElectronicExcitationModel_h
#define LeptsElectronicExcitationModel_h
#include "LeptsBaseModel.hh"

class LeptsElectronicExcitationModel : public LeptsBaseModel {
  
public:
  LeptsElectronicExcitationModel(const G4String& modelName ="LeptsElectronicExcitationModel");
  ~LeptsElectronicExcitationModel();

  //virtual std::map<G4int,std::vector<G4double> > ReadIXS(G4String, const G4Material* aMaterial);
  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,  const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*, G4double tmin = 0.0, G4double tmax = DBL_MAX);

};

class LeptsElectronicExcitationSMFModel : public LeptsElectronicExcitationModel {
  
public:
  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,  const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*, G4double tmin = 0.0, G4double tmax = DBL_MAX);
};


#endif
