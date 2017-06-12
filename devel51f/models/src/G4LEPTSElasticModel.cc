//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#include "G4LEPTSElasticModel.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LEPTSElasticModel::G4LEPTSElasticModel(const G4String& modelName) 
  : G4VLEPTSModel( modelName )
{
  theXSType = XSElastic;
  theType = (G4String)"Elastic";
  fParticleChangeForGamma = 0;
} // constructor


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LEPTSElasticModel::~G4LEPTSElasticModel() {
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSElasticModel::Initialise(const G4ParticleDefinition* aParticle, 
                          const G4DataVector&)
{
  Init();
  BuildPhysicsTable( *aParticle );

  fParticleChangeForGamma = GetParticleChangeForGamma();
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4double G4LEPTSElasticModel::CrossSectionPerVolume(const G4Material* mate,
						    const G4ParticleDefinition* aParticle,
						    G4double kineticEnergy,
						    G4double,
						    G4double)
{
  if( kineticEnergy < theLowestEnergyLimit ) return DBL_MAX;
  return 1./GetMeanFreePath( mate, aParticle, kineticEnergy );

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSElasticModel::SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple* mateCuts,
                                 const G4DynamicParticle* aDynamicParticle,
                                 G4double, G4double)
{
  const G4Material* aMat = mateCuts->GetMaterial();
  G4double P0KinEn = aDynamicParticle->GetKineticEnergy();
  G4ThreeVector P0Dir = aDynamicParticle->GetMomentumDirection();

  //AMR 2015nov
  // if( P0KinEn < theLowestEnergyLimit ) {
  //   // fParticleChangeForGamma->ProposeMomentumDirection( P0Dir );
  //   // fParticleChangeForGamma->SetProposedKineticEnergy( 0.);
  //   // fParticleChangeForGamma->ProposeLocalEnergyDeposit( P0KinEn);
  //   // fParticleChangeForGamma->ProposeTrackStatus(fStopAndKill);
  //   //if( verboseLevel > 2 )
  //   G4cout << " ELOW " << P0KinEn/eV << " " << theLowestEnergyLimit/eV << G4endl;
  //   return;
  // }

  static G4ParticleDefinition * electronDef = G4Electron::ElectronDefinition();
  static G4ParticleDefinition * positronDef = G4Positron::PositronDefinition();
  G4ParticleDefinition * particleDef = aDynamicParticle->GetDefinition();

  // G4double ang;
  // if(particleDef == electronDef || particleDef == positronDef) {
  //   ang = SampleAngle(aMat, P0KinEn/eV, 0.0);
  // }
  // else {
  //   //ang = theDiffXS[aMat]->SampleAngleMT(P0KinEn/eV, 0.0);
  //   ang = theDiffXS[aMat]->SampleAngle(P0KinEn/eV);
  // }

  G4double ang = theDiffXS[aMat]->SampleAngle(P0KinEn/eV, 0.0);
  G4ThreeVector P1Dir = SampleNewDirection(P0Dir, ang);

  G4double Elost = ElasticEnergyTransfer(P0KinEn, ang,theMassTarget[aMat],theMassProjectile[aMat]);
  G4double P1KinEn = P0KinEn - Elost;

  // cout << "elastion " << P0KinEn/eV << " " << ang << " " << Elost/eV << endl;
  // G4String pn = aDynamicParticle->GetDefinition()->GetParticleName();
  // cout << "elastang " << pn << "\t" << ang*180/pi << "\t" << P0KinEn/eV << "\t" << Elost/eV << endl;

  fParticleChangeForGamma->ProposeMomentumDirection( P1Dir );
  fParticleChangeForGamma->SetProposedKineticEnergy( P1KinEn);
  fParticleChangeForGamma->ProposeLocalEnergyDeposit( Elost);
  //
#ifdef DEBUG_LEPTS
  if( verboseLevel >= 2 )
    G4cout << " G4LEPTSElasticModel::SampleSecondaries( P1Dir " << P1Dir
	   << " P0Dir " << P0Dir << " ang " << ang << G4endl
	   << " ELASTIC " << P1KinEn << " = " << P0KinEn << " - " << Energylost << G4endl
	   << " ELASTIC Energylost " << Energylost << " = " << P0KinEn << " " << ang << " "
	   << theMassTarget[aMat] << "  " << theMassProjectile[aMat] << G4endl
	   << " G4LEPTSElasticModel::SampleSecondaries( SetProposedKineticEnergy " << P1KinEn
	   << " " << P0KinEn << " - " << Energylost << G4endl
	   << " G4LEPTSElasticModel::SampleSecondaries( ProposeMomentumDirection "
	   << fParticleChangeForGamma->GetProposedMomentumDirection() << G4endl;
#endif
  //-  G4ParticleDefinition * particleDefDef = aTrack.GetDefinition();
  //-  G4String partName = particleDefDef->GetParticleName();
  //  G4ThreeVector pos, pos0, dpos;
  //-  G4StepPoG4int * PostPoG4int = aStep.GetPostStepPoG4int();
  //-  G4ThreeVector r = PostPoG4int->GetPosition();
  //TypeOfInteraction=-10;
}
