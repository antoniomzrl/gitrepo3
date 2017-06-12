
#include "LeptsIonisationModel.hh"
#include "CLHEP/Units/PhysicalConstants.h"

LeptsIonisationModel::LeptsIonisationModel(const G4String& modelName) 
  : LeptsBaseModel( modelName )
{
  SetDeexcitationFlag(true);
  theXSType = XSIonisation;
  theType = (G4String)"Ionisation";
  //  cout << "constructor Ionisation " << modelName << endl;

  bDXS   = true;
  bELD   = true;
}


LeptsIonisationModel::~LeptsIonisationModel() 
{
}




void LeptsIonisationModel::SampleSecondaries(std::vector<G4DynamicParticle*>* fvect,
                                 const G4MaterialCutsCouple* mateCuts,
                                 const G4DynamicParticle* aDynamicParticle,
                                 G4double, G4double)
{
  const G4Material* aMat = mateCuts->GetMaterial();
  G4double E0 = aDynamicParticle->GetKineticEnergy();
  G4ThreeVector Dir0 = aDynamicParticle->GetMomentumDirection();

  G4double ElostIo = E0;
  if(E0 > theIonisPot[aMat]) {
    //EnlostIo = SampleEnergyLoss(aMat, theIonisPot[aMat], E0);
    ElostIo = theElostDistribution[aMat]->Sample(theIonisPot[aMat]/eV*1.1, E0/eV)*eV;
  }
  
  G4double ang = SampleAngle(aMat, E0/eV, ElostIo/eV);
  G4ThreeVector Dir1 = SampleNewDirection(Dir0, ang);

  G4double E1 = std::max(0., E0 - ElostIo);

  fParticleChange->ProposeMomentumDirection( Dir1);
  fParticleChange->SetProposedKineticEnergy( E1);

  G4double Edep=0, E2;

  if( ElostIo < theIonisPotInt[aMat]) {  // External Ionisation
    Edep = theIonisPot[aMat];
    E2 = max(0.0, (ElostIo - theIonisPot[aMat]) );
  }
  else {  // Auger    
    G4double theIIP;
    if( ElostIo > theIonisPotInt2[aMat]) theIIP = theIonisPotInt2[aMat];
    else                                    theIIP = theIonisPotInt[aMat];
    Edep = 35*eV;
    E2 = max(0.0, (ElostIo - theIIP) );
    //cout << "Auger IIP:" << theIIP/eV << " K0 " << E0/eV
    //	 << " El " << ElostIo/eV << " K2 " << E2/eV << endl;
    
    G4double E3 = max(0.0, theIIP - Edep);
    G4ThreeVector Dir3;
    Dir3.setX( G4UniformRand() );
    Dir3.setY( G4UniformRand() );
    Dir3.setZ( G4UniformRand() );
    Dir3 /= Dir3.mag();

    G4DynamicParticle* e3 = new G4DynamicParticle(G4Electron::Electron(), Dir3, E3);
    fvect->push_back(e3);
  }

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




void LeptsIonisationSMFModel::SampleSecondaries(std::vector<G4DynamicParticle*>* fvect,
                                 const G4MaterialCutsCouple* mateCuts,
                                 const G4DynamicParticle* aDynamicParticle,
                                 G4double, G4double)
{
  const G4Material* aMat = mateCuts->GetMaterial();
  G4double E0 = aDynamicParticle->GetKineticEnergy();
  G4ThreeVector Dir0 = aDynamicParticle->GetMomentumDirection();

  G4double ElostIo = E0;
  if(E0 > theIonisPot[aMat]) {
    //EnlostIo = SampleEnergyLoss(aMat, theIonisPot[aMat], E0);
    ElostIo = theElostDistribution[aMat]->Sample(theIonisPot[aMat]/eV*1.1, E0/eV)*eV;
  }
  
  G4double ang = SampleAngle(aMat, E0/eV, ElostIo/eV);
  G4ThreeVector Dir1 = SampleNewDirection(Dir0, ang);

  G4double E1 = std::max(0., E0 - ElostIo);
  G4double ca = cos(ang);
  E1 *= abs(ca);
  G4double dirx = 1;
  if(ca < 0) dirx = -dirx;
  //  G4double Elost = E0 -E1;
  
  fParticleChange->ProposeMomentumDirection( dirx, 0.0, 0.0);
  fParticleChange->SetProposedKineticEnergy( E1);
  //   fParticleChange->ProposeLocalEnergyDeposit( Elost);
  
  G4double Edep=0, E2;

  if( ElostIo < theIonisPotInt[aMat]) {  // External Ionisation
    Edep = theIonisPot[aMat];
    E2 = max(0.0, (ElostIo - theIonisPot[aMat]) );
  }
  else {  // Auger    
    G4double theIIP;
    if( ElostIo > theIonisPotInt2[aMat]) theIIP = theIonisPotInt2[aMat];
    else                                    theIIP = theIonisPotInt[aMat];
    Edep = 35*eV;
    E2 = max(0.0, (ElostIo - theIIP) );
    //cout << "Auger IIP:" << theIIP/eV << " K0 " << E0/eV
    //	 << " El " << ElostIo/eV << " K2 " << E2/eV << endl;
    
    G4double E3 = max(0.0, theIIP - Edep);
    G4ThreeVector Dir3;
    Dir3.setX( G4UniformRand() );
    Dir3.setY( G4UniformRand() );
    Dir3.setZ( G4UniformRand() );
    Dir3 /= Dir3.mag();

    G4DynamicParticle* e3 = new G4DynamicParticle(G4Electron::Electron(), Dir3, E3);
    fvect->push_back(e3);
  }

  if( E2 <= theLowestEnergyLimit) Edep += E2;
  else {
    G4double cp0 = std::sqrt(E0*(E0 + 2.*electron_mass_c2));
    G4double cp1 = std::sqrt(E1*(E1 + 2.*electron_mass_c2));
    G4ThreeVector P2Momentum = cp0*Dir0 -cp1*Dir1;
    G4ThreeVector Dir2 = P2Momentum / P2Momentum.mag();
    Dir2.rotateUz(Dir0);

    G4double cas = Dir2.x();
    E2 *= abs(cas);
    G4double dirx2 = 1;
    if(cas < 0) dirx2 = -dirx2;
    //    G4double Elost = E0 -E1;
    Dir2 = G4ThreeVector(dirx2,0,0);
    G4DynamicParticle* e2 = new G4DynamicParticle(G4Electron::Electron(), Dir2, E2);
    fvect->push_back(e2);
  }

  //  fParticleChange->ProposeLocalEnergyDeposit(Edep);
}





// void LeptsIonisationSMFModel::SampleSecondaries(std::vector<G4DynamicParticle*>* fvect,
// 						const G4MaterialCutsCouple* mateCuts,
// 						const G4DynamicParticle* aDynamicParticle,
// 						G4double, G4double)
// {
//   G4double P0KinEn = aDynamicParticle->GetKineticEnergy();

//   G4double ElostIo=0;
//   G4ThreeVector P0Dir = aDynamicParticle->GetMomentumDirection();

//   const G4Material* aMat = mateCuts->GetMaterial();
//   if(P0KinEn <= theIonisPot[aMat]) {
//     ElostIo = P0KinEn;
//   }
//   else {
//     ElostIo = theElostDistribution[aMat]->Sample(theIonisPot[aMat]/eV*1.1, P0KinEn/eV)*eV;
//   }
  
//   G4double ang = theDiffXS[aMat]->SampleAngle(P0KinEn/eV, ElostIo/eV);
//   G4double ca = cos(ang);
    
//   G4double P1KinEn = abs(ca)*(P0KinEn - ElostIo);;
//   G4double P1x = P0Dir.x();
//   if(ca < 0) P1x = -P1x;
//   G4double Elost = P0KinEn -P1KinEn;
  
//   fParticleChange->ProposeMomentumDirection( P1x, 0.0, 0.0);
//   fParticleChange->SetProposedKineticEnergy( P1KinEn);
//   fParticleChange->ProposeLocalEnergyDeposit( Elost);

// }
