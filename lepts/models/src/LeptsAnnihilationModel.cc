
#include "LeptsAnnihilationModel.hh"


LeptsAnnihilationModel::LeptsAnnihilationModel(const G4String& modelName) 
  : LeptsBaseModel( modelName )
{
  theXSType = XSAnnihilation;
  theType = (G4String)"Annihilation";
    cout << "constructor Annihilation " << modelName << endl;
} 


LeptsAnnihilationModel::~LeptsAnnihilationModel() {
}



void LeptsAnnihilationModel::SampleSecondaries(std::vector<G4DynamicParticle*>* ,
                                 const G4MaterialCutsCouple* mateCuts,
                                 const G4DynamicParticle* aDynamicParticle,
                                 G4double,
                                 G4double)
{
  G4double P0KinEn = aDynamicParticle->GetKineticEnergy();

  G4double Edep=0;
  G4double Energylost=0;

  Energylost = P0KinEn;
  const G4Material* aMaterial = mateCuts->GetMaterial();

  Edep = P0KinEn;
  fParticleChange->SetProposedKineticEnergy(0.);
  fParticleChange->ProposeLocalEnergyDeposit (Edep);
  fParticleChange->ProposeTrackStatus( fStopAndKill);
 
}
