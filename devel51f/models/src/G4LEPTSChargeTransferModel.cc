
#include "G4LEPTSChargeTransferModel.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LEPTSChargeTransferModel::G4LEPTSChargeTransferModel(const G4String& modelName) 
  : G4VLEPTSModel( modelName )
{
  theXSType = XSChargeTransfer;
  theType = (G4String)"ChargeTransfer";
  fParticleChangeForGamma = 0;
} // constructor


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LEPTSChargeTransferModel::~G4LEPTSChargeTransferModel() {
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSChargeTransferModel::Initialise(const G4ParticleDefinition* aParticle, 
                          const G4DataVector&)
{
  Init();
  BuildPhysicsTable( *aParticle );

  fParticleChangeForGamma = GetParticleChangeForGamma();
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4double G4LEPTSChargeTransferModel::CrossSectionPerVolume(const G4Material* mate,
						    const G4ParticleDefinition* aParticle,
						    G4double kineticEnergy,
						    G4double,
						    G4double)
{
  if( kineticEnergy < theLowestEnergyLimit ) return DBL_MAX;
  return 1./GetMeanFreePath( mate, aParticle, kineticEnergy );

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSChargeTransferModel::SampleSecondaries(std::vector<G4DynamicParticle*>* fvect,
						   const G4MaterialCutsCouple* mateCuts,
						   const G4DynamicParticle* aDynamicParticle,
						   G4double, G4double)
{
  const G4Material* aMaterial = mateCuts->GetMaterial();
  G4ThreeVector Dir = aDynamicParticle->GetMomentumDirection();
  G4double T = aDynamicParticle->GetKineticEnergy();

  static G4ParticleDefinition * protonDef   = G4Proton::ProtonDefinition();
  G4DNAGenericIonsManager     * instance    = G4DNAGenericIonsManager::Instance();
  static G4ParticleDefinition * hydrogenDef = instance->GetIon("hydrogen");
  G4ParticleDefinition * particleDef = aDynamicParticle->GetDefinition();
  G4ParticleDefinition * newParticleDef;
  G4double Edep, newT;
   

  if( particleDef == protonDef) {
    newParticleDef = hydrogenDef;
    Edep  = theIonisPot[aMaterial];
    newT = T - Edep;
  }
  else if( particleDef == hydrogenDef) {
    double M    = proton_mass_c2 + electron_mass_c2;   //H0 mass*c2
    double vc   = T*(T+2*M)/(T+M)/(T+M);               //(v/c)^2  H0
    double secT = electron_mass_c2*(1/sqrt(1-vc) -1);
    //secondary e-
    G4DynamicParticle* e2 = new G4DynamicParticle(G4Electron::Electron(), Dir, secT);
    fvect->push_back(e2);

    newParticleDef = protonDef;
    Edep  = 13.65*eV;            // H0 -> H+ ionisPot
    newT = T -Edep -secT;        // H+
  }
 
  //H0 / H+ killed:
  fParticleChangeForGamma->ProposeTrackStatus(fStopAndKill);
  fParticleChangeForGamma->ProposeLocalEnergyDeposit(Edep);
  
  //H+ / H0 new
  G4DynamicParticle* newp = new G4DynamicParticle(newParticleDef, Dir, newT);
  fvect->push_back(newp);
}
