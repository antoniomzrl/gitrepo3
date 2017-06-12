
#include "LeptsDissociationModel.hh"



LeptsDissociationModel::LeptsDissociationModel(const G4String&  modelName) 
  : LeptsBaseModel( modelName )
{
  theXSType = XSDissociation;
  theType = (G4String)"Dissociation";
  //  cout << "constructor Dissociation " << modelName << endl;

  bDXS   = true;
  bDXSMT = true;
  bELD   = true;
}


LeptsDissociationModel::~LeptsDissociationModel() {
}


void LeptsDissociationModel::SampleSecondaries(std::vector<G4DynamicParticle*>* ,
					       const G4MaterialCutsCouple* mateCuts,
					       const G4DynamicParticle* aDynamicParticle,
					       G4double,
					       G4double)
{
  G4double P0KinEn = aDynamicParticle->GetKineticEnergy();

  G4double Edep=0;
  G4double Energylost=0;
  G4ThreeVector P0Dir = aDynamicParticle->GetMomentumDirection();

  G4double eMin = 0.0;
  G4double eMax = min(600.0*eV, P0KinEn);
  const G4Material* aMat = mateCuts->GetMaterial();
  //Energylost = SampleEnergyLoss(aMat, eMin, eMax);
  Energylost = theElostDistribution[aMat]->Sample(eMin/eV, eMax/eV)*eV;

  //G4ThreeVector P1Dir = SampleNewDirection(aMat, P0Dir, P0KinEn/eV, Energylost/eV);
  G4double ang = SampleAngle(aMat, P0KinEn/eV, Energylost/eV);
  G4ThreeVector P1Dir = SampleNewDirection(P0Dir, ang);

  G4double P1KinEn = P0KinEn - Energylost;
  Edep = Energylost;
  fParticleChange->ProposeMomentumDirection( P1Dir);
  fParticleChange->SetProposedKineticEnergy( P1KinEn);
  fParticleChange->ProposeLocalEnergyDeposit( Edep);
 
}
