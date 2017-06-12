
#include "LeptsRotationalExcitationModel.hh"


LeptsRotationalExcitationModel::LeptsRotationalExcitationModel(const G4String&  modelName) 
 : LeptsBaseModel( modelName )
{
  theXSType = XSRotation;
  theType = (G4String)"RotationalExc";
  //cout << "constructor RotationalExcitation " << modelName << endl;
  
  bDXS = true;
  bELD = true;
} 


LeptsRotationalExcitationModel::~LeptsRotationalExcitationModel() {
}


void LeptsRotationalExcitationModel::SampleSecondaries(std::vector<G4DynamicParticle*>* ,
                                 const G4MaterialCutsCouple* mateCuts,
                                 const G4DynamicParticle* aDynamicParticle, G4double, G4double)
{
  const G4Material * aMat = mateCuts->GetMaterial();
  G4double P0KinEn = aDynamicParticle->GetKineticEnergy();

  G4double Energylost=0;
  G4ThreeVector P0Dir = aDynamicParticle->GetMomentumDirection();

  G4double Edep = theElostDistribution[aMat]->Sample(0.0, P0KinEn/eV)*eV;
  Energylost = Edep;

  G4double ang = SampleAngle(aMat, P0KinEn/eV, Energylost/eV);
  G4ThreeVector P1Dir = SampleNewDirection(P0Dir, ang);
  
  G4double P1KinEn = P0KinEn - Edep;

  fParticleChange->ProposeMomentumDirection( P1Dir);
  fParticleChange->SetProposedKineticEnergy( P1KinEn);
  fParticleChange->ProposeLocalEnergyDeposit( Edep);
}


void LeptsRotationalExcitationSMFModel::SampleSecondaries(std::vector<G4DynamicParticle*>* ,
					 const G4MaterialCutsCouple* mateCuts,
					 const G4DynamicParticle* aDynamicParticle, G4double, G4double)
{
  const G4Material * aMat = mateCuts->GetMaterial();
  
  G4double P0KinEn = aDynamicParticle->GetKineticEnergy();
  G4ThreeVector P0Dir = aDynamicParticle->GetMomentumDirection();

  G4double Edep = theElostDistribution[aMat]->Sample(0.0, P0KinEn/eV)*eV;

  G4double ElostRot = Edep;
  G4double ang = SampleAngle(aMat, P0KinEn/eV, ElostRot/eV);
  G4double ca = cos(ang);
    
  G4double P1KinEn = abs(ca)*(P0KinEn - ElostRot);;
  G4double P1x = P0Dir.x();
  if(ca < 0) P1x = -P1x;
  G4double Elost = P0KinEn -P1KinEn;
  
  fParticleChange->ProposeMomentumDirection( P1x, 0.0, 0.0);
  fParticleChange->SetProposedKineticEnergy( P1KinEn);
  fParticleChange->ProposeLocalEnergyDeposit( Elost);
}
