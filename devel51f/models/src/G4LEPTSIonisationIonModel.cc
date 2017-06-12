
#include "G4LEPTSIonisationIonModel.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4LEPTSIonisationIonModel::G4LEPTSIonisationIonModel(const G4String& modelName) 
  : G4VLEPTSModel( modelName )
{
  SetDeexcitationFlag(true);
  fParticleChangeForGamma = 0;
  theXSType = XSIonisation;
  theType = (G4String)"Ionisation";

} // constructor

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4LEPTSIonisationIonModel::~G4LEPTSIonisationIonModel() 
{
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSIonisationIonModel::Initialise(const G4ParticleDefinition* aParticle, 
                          const G4DataVector&)
{
  Init();
  BuildPhysicsTable( *aParticle );
  fParticleChangeForGamma = GetParticleChangeForGamma();

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4double G4LEPTSIonisationIonModel::CrossSectionPerVolume(const G4Material* mate,
                                         const G4ParticleDefinition* aParticle,
                                         G4double kineticEnergy,
                                         G4double,
                                         G4double)
{
  return 1./GetMeanFreePath( mate, aParticle, kineticEnergy );

}


void G4LEPTSIonisationIonModel::SampleSecondaries(std::vector<G4DynamicParticle*>* fvect,
						  const G4MaterialCutsCouple* mateCuts,
						  const G4DynamicParticle* aDynamicParticle,
						  G4double, G4double)
{
  const G4Material* aMat = mateCuts->GetMaterial();
  G4ThreeVector Dir = aDynamicParticle->GetMomentumDirection();
  G4double Energy = aDynamicParticle->GetKineticEnergy();

  // static G4ParticleDefinition * electronDef = G4Electron::ElectronDefinition();
  // static G4ParticleDefinition * positronDef = G4Positron::PositronDefinition();
  // static G4ParticleDefinition * protonDef   = G4Proton::ProtonDefinition();
  // G4DNAGenericIonsManager     * instance    = G4DNAGenericIonsManager::Instance();
  // static G4ParticleDefinition * hydrogenDef = instance->GetIon("hydrogen");
  // G4ParticleDefinition * particleDef = aDynamicParticle->GetDefinition();

  //double MT = theMassTarget[aMat], MP = theMassProjectile[aMat];
  //double Wmax = Energy*4.0*MT*MP / ((MT+MP)*(MT+MP)); //max valid Eloss
  
  // p0 -> p1 primary
  //G4double Elost = SampleEnergyLoss(aMat, theIonisPot[aMat], Energy);
  G4double Elost = theElostDistr[aMat]->Sample(theIonisPot[aMat]/eV, Energy/eV)*eV;
  //cout << "G4LEPTSIonisationIonModel E " << Energy/eV <<  " Elost " << Elost/eV << endl;
  //G4double ang = theDiffXS[aMat]->SampleAngleMT(Energy/eV, Elost/eV);
  G4double ang = theDiffXS[aMat]->SampleAngle(Energy/eV, Elost/eV);
  G4ThreeVector newDir = SampleNewDirection(Dir, ang);
  
  G4double Edep = theIonisPot[aMat];
  G4double newEnergy = Energy -Elost;

  fParticleChangeForGamma->ProposeMomentumDirection( newDir);
  fParticleChangeForGamma->SetProposedKineticEnergy( newEnergy);
  fParticleChangeForGamma->ProposeLocalEnergyDeposit(Edep);

  //e2 ioniz
  G4double secEnergy = Elost -Edep;
  if( secEnergy > theLowestEnergyLimit) {
    G4double ang = theDiffXSSEC[aMat]->SampleAngle(Energy/eV, 0.0);
    G4ThreeVector secDir = SampleNewDirection(Dir, ang);
    G4DynamicParticle* e2 = new G4DynamicParticle(G4Electron::Electron(), secDir, secEnergy);
    fvect->push_back(e2);
  }
}
