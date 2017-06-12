
#include "LeptsThermalisationModel.hh"


LeptsThermalisationModel::LeptsThermalisationModel(const G4String& modelName) 
  : LeptsBaseModel( modelName )
{
  theXSType = XSThermalisation;
  theType = (G4String)"Thermalisation";
  //  cout << "constructor Thermalisation " << modelName << endl;
} 


LeptsThermalisationModel::~LeptsThermalisationModel() {
}



void LeptsThermalisationModel::SampleSecondaries(std::vector<G4DynamicParticle*>* ,
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
