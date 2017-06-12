
#include "LeptsIonisationSDIModel.hh"
#include "CLHEP/Units/PhysicalConstants.h"

LeptsSingleIonisationModel::~LeptsSingleIonisationModel() { }
LeptsDoubleIonisationModel::~LeptsDoubleIonisationModel() { }
LeptsInternalIonisationModel::~LeptsInternalIonisationModel() { }

LeptsSingleIonisationModel::LeptsSingleIonisationModel(const G4String& modelName)
  : LeptsBaseModel( modelName) {
  SetDeexcitationFlag(true);
  theXSType = XSIonisationS;
  theType = (G4String)"IonisationS";

  bDXS    = true;
  bDXSMT  = true;
  bELD    = true;
}

LeptsInternalIonisationModel::LeptsInternalIonisationModel(const G4String& modelName)
  : LeptsBaseModel( modelName) {
  SetDeexcitationFlag(true);
  theXSType = XSIonisationI;
  theType = (G4String)"IonisationI";

  bDXS    = true;
  bDXSMT  = true;
  bELD    = true;
}

LeptsDoubleIonisationModel::LeptsDoubleIonisationModel(const G4String& modelName)
  : LeptsBaseModel( modelName) {
  SetDeexcitationFlag(true);
  theXSType = XSIonisationD;
  theType = (G4String)"IonisationD";

  bDXS    = true;
  bDXSMT  = true;
  bELD    = true;
}


void LeptsSingleIonisationModel::
SampleSecondaries(std::vector<G4DynamicParticle*>* fvect,
		  const G4MaterialCutsCouple* mateCuts,
		  const G4DynamicParticle* aDynamicParticle, G4double, G4double) {
  const G4Material* aMat = mateCuts->GetMaterial();
  G4double E0 = aDynamicParticle->GetKineticEnergy();
  G4ThreeVector Dir0 = aDynamicParticle->GetMomentumDirection();

  G4double ElostIo = E0;
  if(E0 > theIonisPot[aMat]) {
    ElostIo = theElostDistribution[aMat]->Sample(theIonisPot[aMat]/eV*1.1, E0/eV)*eV;
  }
  
  //G4ThreeVector Dir1 = SampleNewDirection(aMat, Dir0, E0/eV, ElostIo/eV);
  G4double ang = SampleAngle(aMat, E0/eV, ElostIo/eV);
  G4ThreeVector Dir1 = SampleNewDirection(Dir0, ang);

  G4double E1 = std::max(0., E0 - ElostIo);

  fParticleChange->ProposeMomentumDirection( Dir1);
  fParticleChange->SetProposedKineticEnergy( E1);

  G4double Edep=0, E2;

  Edep = theIonisPot[aMat];
  E2 = max(0.0, (ElostIo - theIonisPot[aMat]) );

  if( E2 <= theLowestEnergyLimit)
    Edep += E2;
  else {
    G4double cp0 = std::sqrt(E0*(E0 + 2.*electron_mass_c2));
    G4double cp1 = std::sqrt(E1*(E1 + 2.*electron_mass_c2));
    G4ThreeVector P2Momentum = cp0*Dir0 -cp1*Dir1;
    G4ThreeVector Dir2 = P2Momentum / P2Momentum.mag();
    Dir2.rotateUz(Dir0);
    G4DynamicParticle* e2 = new G4DynamicParticle(G4Electron::Electron(), Dir2, E2);
    fvect->push_back(e2);
  }

  fParticleChange->ProposeLocalEnergyDeposit(Edep);
}





void LeptsInternalIonisationModel::
SampleSecondaries(std::vector<G4DynamicParticle*>* fvect,
		  const G4MaterialCutsCouple* mateCuts,
		  const G4DynamicParticle* aDynamicParticle, G4double, G4double) {
  const G4Material* aMat = mateCuts->GetMaterial();
  G4double E0 = aDynamicParticle->GetKineticEnergy();
  G4ThreeVector Dir0 = aDynamicParticle->GetMomentumDirection();

  G4double ElostIo = E0;
  if(E0 > theIonisPot[aMat]) {
    ElostIo = theElostDistribution[aMat]->Sample(theIonisPot[aMat]/eV*1.1, E0/eV)*eV;
  }
  
  //G4ThreeVector Dir1 = SampleNewDirection(aMat, Dir0, E0/eV, ElostIo/eV);
  G4double ang = SampleAngle(aMat, E0/eV, ElostIo/eV);
  G4ThreeVector Dir1 = SampleNewDirection(Dir0, ang);

  G4double E1 = std::max(0., E0 - ElostIo);

  fParticleChange->ProposeMomentumDirection( Dir1);
  fParticleChange->SetProposedKineticEnergy( E1);

  G4double Edep=0, E2;

  // Auger    
  G4double theIIP;
  if( ElostIo > theIonisPotInt2[aMat]) theIIP = theIonisPotInt2[aMat];
  else                                 theIIP = theIonisPotInt[aMat];
  Edep = 35*eV;
  E2 = max(0.0, (ElostIo - theIIP) );
  //cout << "Auger IIP:" << theIIP/eV << " K0 " << E0/eV
  //	 << " El " << ElostIo/eV << " K2 " << E2/eV << endl;
    
  G4double E3 = max(0.0, theIIP - Edep);
  G4ThreeVector Dir3( G4UniformRand(), G4UniformRand(), G4UniformRand() );
  Dir3 /= Dir3.mag();

  G4DynamicParticle* e3 = new G4DynamicParticle(G4Electron::Electron(), Dir3, E3);
  fvect->push_back(e3);


  if( E2 <= theLowestEnergyLimit) Edep += E2;
  else {
    G4double cp0 = std::sqrt(E0*(E0 + 2.*electron_mass_c2));
    G4double cp1 = std::sqrt(E1*(E1 + 2.*electron_mass_c2));
    G4ThreeVector P2Momentum = cp0*Dir0 -cp1*Dir1;
    G4ThreeVector Dir2 = P2Momentum / P2Momentum.mag();
    Dir2.rotateUz(Dir0);
    G4DynamicParticle* e2 = new G4DynamicParticle(G4Electron::Electron(), Dir2, E2);
    fvect->push_back(e2);
  }

  fParticleChange->ProposeLocalEnergyDeposit(Edep);
}



void LeptsDoubleIonisationModel::
SampleSecondaries(std::vector<G4DynamicParticle*>* fvect,
		  const G4MaterialCutsCouple* mateCuts,
		  const G4DynamicParticle* aDynamicParticle, G4double, G4double) {
  const G4Material* aMat = mateCuts->GetMaterial();
  G4double E0 = aDynamicParticle->GetKineticEnergy();
  G4ThreeVector Dir0 = aDynamicParticle->GetMomentumDirection();

  //G4double theDoubleIonisPot = theIonisPot[aMat] * 4.0;

  // for(double e=1; e<7e6;) {
  //   if(e<100) e+=1;
  //   else      e*=1.1;
  //   double el = theElostDistribution[aMat]->SampleMeanEnergyLost(e);
  //   cout << "eldiod " << e << " " << el << endl;
  // }
    
  G4double ElostIo = E0;
  //if(E0 > theDoubleIonisPot) {
  //ElostIo = theElostDistribution[aMat]->Sample(theDoubleIonisPot/eV*1.1, E0/eV)*eV;
  ElostIo = theElostDistribution[aMat]->SampleMeanEnergyLost(E0/eV)*eV;
  //}
    
  G4double ang = SampleAngle(aMat, E0/eV, ElostIo/eV);
  G4ThreeVector Dir1 = SampleNewDirection(Dir0, ang);

  G4double E1 = std::max(0., E0 - ElostIo);

  fParticleChange->ProposeMomentumDirection( Dir1);
  fParticleChange->SetProposedKineticEnergy( E1);

  //G4double Edep = theDoubleIonisPot;
  //G4double E2   = max(0.0, (ElostIo - Edep) );
  G4double E2 = (26.3253 -10.79)*eV;
  G4double Edep = ElostIo -2*E2;
  
  if( E2 <= theLowestEnergyLimit)
    Edep += E2;
  else {
    G4ThreeVector Dir2( G4UniformRand(), G4UniformRand(), G4UniformRand() );
    Dir2 /= Dir2.mag();
    G4DynamicParticle * e2 = new G4DynamicParticle(G4Electron::Electron(),  Dir2, E2);
    G4DynamicParticle * e3 = new G4DynamicParticle(G4Electron::Electron(), -Dir2, E2);
    fvect->push_back(e2);
    fvect->push_back(e3);
  }

  fParticleChange->ProposeLocalEnergyDeposit(Edep);
}
