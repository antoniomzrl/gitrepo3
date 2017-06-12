
#include "LeptsIonisationIonModel.hh"


LeptsIonisationIonModel::LeptsIonisationIonModel(const G4String& modelName) 
  : LeptsBaseModel( modelName )
{
  SetDeexcitationFlag(true);
  theXSType = XSIonisation;
  theType = (G4String)"Ionisation";
  //  cout << "constructor IonisationIon " << modelName << endl;

  bDXS    = true;
  bDXSSEC = true;
  bELD  = true;
}


LeptsIonisationIonModel::~LeptsIonisationIonModel() 
{
}



void LeptsIonisationIonModel::SampleSecondaries(std::vector<G4DynamicParticle*>* fvect,
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
  G4double Elost = theElostDistribution[aMat]->Sample(theIonisPot[aMat]/eV, Energy/eV)*eV;
  G4double ang = SampleAngle(aMat, Energy/eV, Elost/eV);
  G4ThreeVector newDir = SampleNewDirection(Dir, ang);
  
  G4double Edep = theIonisPot[aMat];
  G4double newEnergy = Energy -Elost;

  fParticleChange->ProposeMomentumDirection( newDir);
  fParticleChange->SetProposedKineticEnergy( newEnergy);
  fParticleChange->ProposeLocalEnergyDeposit(Edep);

  //e2 ioniz
  G4double secEnergy = Elost -Edep;
  if( secEnergy > theLowestEnergyLimit) {
    G4double angsec = SampleAngle(aMat, Energy/eV, 0.0);
    G4ThreeVector secDir = SampleNewDirection(Dir, angsec);
    G4DynamicParticle* e2 = new G4DynamicParticle(G4Electron::Electron(), secDir, secEnergy);
    fvect->push_back(e2);
  }
}
