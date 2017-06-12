
#ifndef LeptsElectronicExcitationIonModel_h
#define LeptsElectronicExcitationIonModel_h
#include "LeptsBaseModel.hh"

class LeptsElectronicExcitationIonModel : public LeptsBaseModel {

public:
  LeptsElectronicExcitationIonModel(const G4String& modelName ="LeptsElectronicExcitationIonModel");
  ~LeptsElectronicExcitationIonModel();


  //pedro (no hace falta)
  //virtual std::map<G4int,std::vector<G4double> > ReadIXS(G4String, const G4Material* aMaterial);

  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
                                 G4double tmin = 0.0,
                                 G4double tmax = DBL_MAX);

};

#endif
