
#include "LeptsElectronicExcitationIonModel.hh"

LeptsElectronicExcitationIonModel::LeptsElectronicExcitationIonModel(const G4String& modelName) 
  : LeptsBaseModel( modelName )
{
  theXSType = XSExcitation;
  theType = (G4String)"ElectronicExc";
  LowestElectronicExcitationEnergy = 0;
  LowestNeutralDisociationEnergy = 0;
  //  cout << "constructor ElectronicExcitationIon " << modelName << endl;

  bDXS   = true;
  bELD   = true;
}


LeptsElectronicExcitationIonModel::~LeptsElectronicExcitationIonModel() {
}





void LeptsElectronicExcitationIonModel::SampleSecondaries(std::vector<G4DynamicParticle*>*,
						  const G4MaterialCutsCouple* mateCuts,
						  const G4DynamicParticle* aDynamicParticle,
						  G4double, G4double)
{
  const G4Material* aMat = mateCuts->GetMaterial();
  G4double        Energy = aDynamicParticle->GetKineticEnergy();
  G4ThreeVector      Dir = aDynamicParticle->GetMomentumDirection();

  // static G4ParticleDefinition * electronDef = G4Electron::ElectronDefinition();
  // static G4ParticleDefinition * positronDef = G4Positron::PositronDefinition();
  // static G4ParticleDefinition * protonDef   = G4Proton::ProtonDefinition();
  // G4DNAGenericIonsManager     * instance    = G4DNAGenericIonsManager::Instance();
  // static G4ParticleDefinition * hydrogenDef = instance->GetIon("hydrogen");
  // G4ParticleDefinition* particleDef = aDynamicParticle->GetDefinition();
  // if(particleDef == protonDef) {}

  // G4double ElostI = SampleEnergyLoss(aMat, 0.0, Energy);
  // G4double angE = theDiffXS[aMat]->SampleAngleMT(Energy/eV, 0.0);
  // G4double ElostE = Energy * FractionET(angE, theMassTarget[aMat], theMassProjectile[aMat]);
  // G4double Elost = ElostI + ElostE;
  // G4double Edep  = Elost;
  // G4double newEnergy = Energy - Elost;
  // double ang = ElasticAngle(Energy, Elost, theMassTarget[aMat], theMassProjectile[aMat]);
  // G4ThreeVector newDir = SampleNewDirection(Dir, ang);

  //G4double Elost = SampleEnergyLoss(aMat, 0.0, Energy);
  G4double Elost = theElostDistribution[aMat]->Sample(0.0, Energy/eV)*eV;
  
  G4double ang = SampleAngle(aMat, Energy/eV, Elost/eV);

  G4double Edep  = Elost;
  G4double newEnergy = Energy - Elost;
  G4ThreeVector newDir = SampleNewDirection(Dir, ang);
  
  fParticleChange->ProposeMomentumDirection( newDir);
  fParticleChange->SetProposedKineticEnergy( newEnergy);
  fParticleChange->ProposeLocalEnergyDeposit( Edep);
}
