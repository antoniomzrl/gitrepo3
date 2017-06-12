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
#include "G4LEPTSDissociationModel.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4LEPTSDissociationModel::G4LEPTSDissociationModel(const G4String&  modelName) 
  : G4VLEPTSModel( modelName )
{
  theXSType = XSDissociation;
  theType = (G4String)"Dissociation";
  fParticleChangeForGamma = 0;
} // constructor


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4LEPTSDissociationModel::~G4LEPTSDissociationModel() {
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSDissociationModel::Initialise(const G4ParticleDefinition* aParticle, 
                          const G4DataVector&)
{
  Init();
  BuildPhysicsTable( *aParticle );

  fParticleChangeForGamma = GetParticleChangeForGamma();

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4double G4LEPTSDissociationModel::CrossSectionPerVolume(const G4Material* mate,
                                         const G4ParticleDefinition* aParticle,
                                         G4double kineticEnergy,
                                         G4double,
                                         G4double)
{
  return 1./GetMeanFreePath( mate, aParticle, kineticEnergy );

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSDissociationModel::SampleSecondaries(std::vector<G4DynamicParticle*>* ,
                                 const G4MaterialCutsCouple* mateCuts,
                                 const G4DynamicParticle* aDynamicParticle,
                                 G4double,
                                 G4double)
{
  G4double P0KinEn = aDynamicParticle->GetKineticEnergy();

  G4double Edep=0;
  G4double Energylost=0;
  G4ThreeVector P0Dir = aDynamicParticle->GetMomentumDirection();

  G4double eMin = 0.0;
  G4double eMax = min(600.0*eV, P0KinEn);
  const G4Material* aMat = mateCuts->GetMaterial();
  //Energylost = SampleEnergyLoss(aMat, eMin, eMax);
  Energylost = theElostDistr[aMat]->Sample(eMin/eV, eMax/eV)*eV;

  //G4ThreeVector P1Dir = SampleNewDirection(aMat, P0Dir, P0KinEn/eV, Energylost/eV);
  G4double ang = theDiffXS[aMat]->SampleAngle(P0KinEn/eV, Energylost/eV);
  G4ThreeVector P1Dir = SampleNewDirection(P0Dir, ang);

  G4double P1KinEn = P0KinEn - Energylost;
  Edep = Energylost;
  fParticleChangeForGamma->ProposeMomentumDirection( P1Dir);
  fParticleChangeForGamma->SetProposedKineticEnergy( P1KinEn);
  fParticleChangeForGamma->ProposeLocalEnergyDeposit( Edep);
 
}
