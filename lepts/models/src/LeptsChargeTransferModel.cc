
#include "LeptsChargeTransferModel.hh"

LeptsChargeTransferModel::LeptsChargeTransferModel(const G4String& modelName) 
  : LeptsBaseModel( modelName )
{
  theXSType = XSChargeTransfer;
  theType = (G4String)"ChargeTransfer";
  //  cout << "constructor ChargeTransfer " << modelName << endl;

  // bDXS    = true;
  // bDXSSEC = true;
}


LeptsChargeTransferModel::~LeptsChargeTransferModel() {
}


void LeptsChargeTransferModel::SampleSecondaries(std::vector<G4DynamicParticle*>* fvect,
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
    newT = T - Edep - secT;      // H+
  }
 
  //H0 / H+ killed:
  fParticleChange->ProposeTrackStatus(fStopAndKill);
  fParticleChange->ProposeLocalEnergyDeposit(Edep);
  
  //H+ / H0 new
  G4DynamicParticle* newp = new G4DynamicParticle(newParticleDef, Dir, newT);
  fvect->push_back(newp);
}
