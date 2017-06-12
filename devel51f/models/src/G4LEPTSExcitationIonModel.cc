
#include "G4LEPTSExcitationIonModel.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4LEPTSExcitationIonModel::G4LEPTSExcitationIonModel(const G4String& modelName) 
  : G4VLEPTSModel( modelName )
{
  theXSType = XSExcitation;
  theType = (G4String)"ElectronicExc";
  fParticleChangeForGamma = 0;
  LowestExcitationEnergy = 0;
  LowestNeutralDisociationEnergy = 0;
} // constructor


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4LEPTSExcitationIonModel::~G4LEPTSExcitationIonModel() {
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSExcitationIonModel::Initialise(const G4ParticleDefinition* aParticle, 
                          const G4DataVector&)
{
  Init();
  BuildPhysicsTable( *aParticle );

  fParticleChangeForGamma = GetParticleChangeForGamma();

  LowestExcitationEnergy = 0;
  LowestNeutralDisociationEnergy = 0;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4double G4LEPTSExcitationIonModel::CrossSectionPerVolume(const G4Material* mate,
                                         const G4ParticleDefinition* aParticle,
                                         G4double kineticEnergy,
                                         G4double,
                                         G4double)
{
  return 1./GetMeanFreePath( mate, aParticle, kineticEnergy );

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSExcitationIonModel::SampleSecondaries(std::vector<G4DynamicParticle*>*,
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
  G4double Elost = theElostDistr[aMat]->Sample(0.0, Energy/eV)*eV;
  
  //G4double ang = theDiffXS[aMat]->SampleAngleMT(Energy/eV, Elost/eV);
  G4double ang = theDiffXS[aMat]->SampleAngle(Energy/eV, Elost/eV);

  G4double Edep  = Elost;
  G4double newEnergy = Energy - Elost;
  G4ThreeVector newDir = SampleNewDirection(Dir, ang);
  
  fParticleChangeForGamma->ProposeMomentumDirection( newDir);
  fParticleChangeForGamma->SetProposedKineticEnergy( newEnergy);
  fParticleChangeForGamma->ProposeLocalEnergyDeposit( Edep);
}
