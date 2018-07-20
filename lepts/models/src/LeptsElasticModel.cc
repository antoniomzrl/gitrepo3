
#include "LeptsElasticModel.hh"

LeptsElasticModel::LeptsElasticModel(const G4String& modelName) : LeptsBaseModel( modelName)
{
  theXSType = XSElastic;
  theType = (G4String)"Elastic";
  // cout << "constructor Elastic " << modelName << endl;

  bDXS   = true;
  bDXSMT = true;
}


LeptsElasticModel::~LeptsElasticModel() {
}


void LeptsElasticModel::SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple* mateCuts,
                                 const G4DynamicParticle* aDynamicParticle,
                                 G4double, G4double)
{
  const G4Material* aMat = mateCuts->GetMaterial();
  G4double P0KinEn = aDynamicParticle->GetKineticEnergy();
  G4ThreeVector P0Dir = aDynamicParticle->GetMomentumDirection();
  //AMR 2015nov
  // if( P0KinEn < theLowestEnergyLimit ) {
  //   // fParticleChange->ProposeMomentumDirection( P0Dir );
  //   // fParticleChange->SetProposedKineticEnergy( 0.);
  //   // fParticleChange->ProposeLocalEnergyDeposit( P0KinEn);
  //   // fParticleChange->ProposeTrackStatus(fStopAndKill);
  //   //if( verboseLevel > 2 )
  //   G4cout << " ELOW " << P0KinEn/eV << " " << theLowestEnergyLimit/eV << G4endl;
  //   return;
  // }

  //static G4ParticleDefinition * electronDef = G4Electron::ElectronDefinition();
  //static G4ParticleDefinition * positronDef = G4Positron::PositronDefinition();
  //G4ParticleDefinition * particleDef = aDynamicParticle->GetDefinition();

  // G4double ang;
  // if(particleDef == electronDef || particleDef == positronDef) {
  //   ang = SampleAngle(aMat, P0KinEn/eV, 0.0);
  // }
  // else {
  //   //ang = theDiffXS[aMat]->SampleAngleMT(P0KinEn/eV, 0.0);
  //   ang = theDiffXS[aMat]->SampleAngle(P0KinEn/eV);
  // }

  G4double ang = SampleAngle(aMat, P0KinEn/eV, 0.0);
  G4ThreeVector P1Dir = SampleNewDirection(P0Dir, ang);
  G4double Elost = ElasticEnergyTransfer(P0KinEn, ang,theMassTarget[aMat],theMassProjectile[aMat]);
  G4double P1KinEn = P0KinEn - Elost;
  // cout << "elastion " << P0KinEn/eV << " " << ang << " " << Elost/eV << endl;
  // G4String pn = aDynamicParticle->GetDefinition()->GetParticleName();
  // cout << "elastang " << pn << "\t" << ang*180/pi << "\t" << P0KinEn/eV << "\t" << Elost/eV << endl;

  fParticleChange->ProposeMomentumDirection( P1Dir );
  fParticleChange->SetProposedKineticEnergy( P1KinEn);
  fParticleChange->ProposeLocalEnergyDeposit( Elost);

#ifndef DEBUG_LEPTS
  if( verboseLevel >= 2 )
    G4cout << " LeptsElasticModel::SampleSecondaries( P1Dir " << P1Dir
	   << " P0Dir " << P0Dir << " ang " << ang << G4endl
	   << " ELASTIC " << P1KinEn << " = " << P0KinEn << " - " << Elost << G4endl
	   << " ELASTIC Energylost " << Elost << " = " << P0KinEn << " " << ang << " "
	   << theMassTarget[aMat] << "  " << theMassProjectile[aMat] << G4endl
	   << " LeptsElasticModel::SampleSecondaries( SetProposedKineticEnergy " << P1KinEn
	   << " " << P0KinEn << " - " << Elost << G4endl
	   << " LeptsElasticModel::SampleSecondaries( ProposeMomentumDirection "
	   << fParticleChange->GetProposedMomentumDirection() << G4endl;
#endif
}



void LeptsElasticSMFModel::SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple* mateCuts,
                                 const G4DynamicParticle* aDynamicParticle, G4double, G4double)
{
  const G4Material* aMat = mateCuts->GetMaterial();
  G4double P0KinEn = aDynamicParticle->GetKineticEnergy();
  G4ThreeVector P0Dir = aDynamicParticle->GetMomentumDirection();

  G4double ang = SampleAngle(aMat, P0KinEn/eV, 0.0);
  G4double ElostEl = ElasticEnergyTransfer(P0KinEn, ang,theMassTarget[aMat],theMassProjectile[aMat]);
  G4double ca = cos(ang);
    
  G4double P1KinEn = abs(ca)*(P0KinEn - ElostEl);;
  G4double P1x = P0Dir.x();
  if(ca < 0) P1x = -P1x;
  G4double Elost = P0KinEn -P1KinEn;

  fParticleChange->ProposeMomentumDirection( P1x, 0.0, 0.0);
  fParticleChange->SetProposedKineticEnergy( P1KinEn);
  fParticleChange->ProposeLocalEnergyDeposit( Elost);
}
