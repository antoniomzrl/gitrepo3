
// Single, Double & Internal (K shell) Ionisation

#ifndef LeptsIonisationSDIModel_h
#define LeptsIonisationSDIModel_h
#include "LeptsBaseModel.hh"

class LeptsSingleIonisationModel : public LeptsBaseModel {
public:
  LeptsSingleIonisationModel(const G4String& modelName ="LeptsIonisationSModel");
  ~LeptsSingleIonisationModel();
  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
				 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
				 G4double tmin = 0.0, G4double tmax = DBL_MAX);
};


class LeptsDoubleIonisationModel : public LeptsBaseModel {
public:
  LeptsDoubleIonisationModel(const G4String& modelName ="LeptsIonisationDModel");
  ~LeptsDoubleIonisationModel();
  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
				 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
				 G4double tmin = 0.0, G4double tmax = DBL_MAX);
};


class LeptsInternalIonisationModel : public LeptsBaseModel {
public:
  LeptsInternalIonisationModel(const G4String& modelName ="LeptsIonisationKModel");
  ~LeptsInternalIonisationModel();
  virtual void SampleSecondaries(std::vector<G4DynamicParticle*>*,
				 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle*,
				 G4double tmin = 0.0, G4double tmax = DBL_MAX);
};

#endif
