
#include "LeptsVibrationalExcitationModel.hh"


LeptsVibrationalExcitationModel::LeptsVibrationalExcitationModel(const G4String& modelName) 
  : LeptsBaseModel( modelName )
{
  theXSType = XSVibration;
  theType = (G4String)"VibrationalExc";
  //cout << "constructor VibrationalExcitation " << modelName << endl;

  bDXS   = true;
  bDXSMT = true;
  bELD   = true;
} 


LeptsVibrationalExcitationModel::~LeptsVibrationalExcitationModel() {
}




void LeptsVibrationalExcitationModel::SampleSecondaries(std::vector<G4DynamicParticle*>*,
							const G4MaterialCutsCouple* mateCuts,
							const G4DynamicParticle* aDynamicParticle,
							G4double, G4double)
{
  G4double P0KinEn = aDynamicParticle->GetKineticEnergy();

  G4double Edep=0;
  G4double Energylost=0;
  G4ThreeVector P0Dir = aDynamicParticle->GetMomentumDirection();
  const G4Material* aMat = mateCuts->GetMaterial();

  Edep = theElostDistribution[aMat]->Sample(0.0, P0KinEn/eV)*eV;
  Energylost = Edep;

  //G4ThreeVector P1Dir = SampleNewDirection(aMat, P0Dir, P0KinEn/eV, Energylost/eV);
  G4double ang = SampleAngle(aMat, P0KinEn/eV, Energylost/eV);
  G4ThreeVector P1Dir = SampleNewDirection(P0Dir, ang);

  G4double P1KinEn = P0KinEn - Edep;

  fParticleChange->ProposeMomentumDirection( P1Dir);
  fParticleChange->SetProposedKineticEnergy( P1KinEn);
  fParticleChange->ProposeLocalEnergyDeposit( Edep);
}



void LeptsVibrationalExcitationSMFModel::SampleSecondaries(std::vector<G4DynamicParticle*>*,
							const G4MaterialCutsCouple* mateCuts,
							const G4DynamicParticle* aDynamicParticle,
							G4double, G4double)
{
  G4double P0KinEn = aDynamicParticle->GetKineticEnergy();

  G4ThreeVector P0Dir = aDynamicParticle->GetMomentumDirection();
  const G4Material* aMat = mateCuts->GetMaterial();

  G4double Edep = theElostDistribution[aMat]->Sample(0.0, P0KinEn/eV)*eV;
  G4double ElostVib = Edep;

  G4double ang = SampleAngle(aMat, P0KinEn/eV, ElostVib/eV);
  G4double ca = cos(ang);
    
  G4double P1KinEn = abs(ca)*(P0KinEn - ElostVib);;
  G4double P1x = P0Dir.x();
  if(ca < 0) P1x = -P1x;
  G4double Elost = P0KinEn -P1KinEn;

  fParticleChange->ProposeMomentumDirection( P1x, 0.0, 0.0);
  fParticleChange->SetProposedKineticEnergy( P1KinEn);
  fParticleChange->ProposeLocalEnergyDeposit( Elost);
}
