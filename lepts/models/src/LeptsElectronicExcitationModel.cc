
#include "LeptsElectronicExcitationModel.hh"


LeptsElectronicExcitationModel::LeptsElectronicExcitationModel(const G4String& modelName) 
  : LeptsBaseModel( modelName )
{
  theXSType = XSExcitation;
  theType = (G4String)"ElectronicExc";
  LowestElectronicExcitationEnergy = 0;
  LowestNeutralDisociationEnergy = 0;
  //  cout << "constructor ElectronicExcitation " << modelName << endl;

  bDXS   = true;
  bDXSMT = true;
  bELD   = true;
}


LeptsElectronicExcitationModel::~LeptsElectronicExcitationModel() {
}


void LeptsElectronicExcitationModel::SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple* mateCuts,
                                 const G4DynamicParticle* aDynamicParticle, G4double, G4double)
{
  G4double P0KinEn = aDynamicParticle->GetKineticEnergy();

  G4double Edep=0;
  G4double Energylost=0;
  G4ThreeVector P0Dir = aDynamicParticle->GetMomentumDirection();

  G4double eMin = 0.0;
  const G4Material* aMat = mateCuts->GetMaterial();
  G4double eMax = min(theIonisPot[aMat], P0KinEn);
  //Energylost = SampleEnergyLoss(aMat, eMin, eMax);
  Energylost = theElostDistribution[aMat]->Sample(eMin/eV, eMax/eV)*eV;

  Edep = Energylost;

  //G4ThreeVector P1Dir = SampleNewDirection(aMat, P0Dir, P0KinEn/eV, Energylost/eV);
  G4double ang = SampleAngle(aMat, P0KinEn/eV, Energylost/eV);
  G4ThreeVector P1Dir = SampleNewDirection(P0Dir, ang);

  G4double P1KinEn = P0KinEn - Edep;

  fParticleChange->ProposeMomentumDirection( P1Dir);
  fParticleChange->SetProposedKineticEnergy( P1KinEn);
  fParticleChange->ProposeLocalEnergyDeposit( Edep);

}




void LeptsElectronicExcitationSMFModel::SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple* mateCuts,
                                 const G4DynamicParticle* aDynamicParticle, G4double, G4double)
{
  G4double P0KinEn = aDynamicParticle->GetKineticEnergy();

  G4ThreeVector P0Dir = aDynamicParticle->GetMomentumDirection();

  G4double eMin = 0.0;
  const G4Material* aMat = mateCuts->GetMaterial();
  G4double eMax = min(theIonisPot[aMat], P0KinEn);
  G4double ElostEx = theElostDistribution[aMat]->Sample(eMin/eV, eMax/eV)*eV;

  G4double ang = SampleAngle(aMat, P0KinEn/eV, ElostEx/eV);
  G4double ca = cos(ang);
    
  G4double P1KinEn = abs(ca)*(P0KinEn - ElostEx);;
  G4double P1x = P0Dir.x();
  if(ca < 0) P1x = -P1x;
  G4double Elost = P0KinEn -P1KinEn;
  
  fParticleChange->ProposeMomentumDirection( P1x, 0.0, 0.0);
  fParticleChange->SetProposedKineticEnergy( P1KinEn);
  fParticleChange->ProposeLocalEnergyDeposit( Elost);
}
