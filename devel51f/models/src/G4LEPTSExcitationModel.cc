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
#include "G4LEPTSExcitationModel.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4LEPTSExcitationModel::G4LEPTSExcitationModel(const G4String& modelName) 
  : G4VLEPTSModel( modelName )
{
  theXSType = XSExcitation;
  theType = (G4String)"ElectronicExc";
  fParticleChangeForGamma = 0;
  LowestExcitationEnergy = 0;
  LowestNeutralDisociationEnergy = 0;
} // constructor


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4LEPTSExcitationModel::~G4LEPTSExcitationModel() {
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSExcitationModel::Initialise(const G4ParticleDefinition* aParticle, 
                          const G4DataVector&)
{
  Init();
  BuildPhysicsTable( *aParticle );

  fParticleChangeForGamma = GetParticleChangeForGamma();

  LowestExcitationEnergy = 0;
  LowestNeutralDisociationEnergy = 0;


}


// std::map<G4int,std::vector<G4double> > G4LEPTSExcitationModel::ReadIXS(G4String fileTXS, const G4Material* aMaterial)
// {
//   std::map<G4int,std::vector<G4double> > integralXS = G4VLEPTSModel::ReadIXS( fileTXS, aMaterial);

//   if( integralXS.size() == 0 ) return integralXS;

//   for (G4int jj=theNXSdat[aMaterial]; jj>=0; jj--) {
//     if( integralXS[XSDissociation][jj] > 0.001) LowestExcitationEnergy  = integralXS[XSTotal][jj-1];
//     if( integralXS[XSVibration][jj] > 0.001) LowestNeutralDisociationEnergy = integralXS[XSTotal][jj-1]*eV;
//     //    if( txs[5][j] > 0.001) LowestExcitationEnergy         = txs[0][j-1];
//     //  if( txs[6][j] > 0.001) LowestNeutralDisociationEnergy = txs[0][j-1]*eV;  
//   }

//   if( verboseLevel >= 1) G4cout << "        LowestExcitationEnergy: " << LowestExcitationEnergy << G4endl
// 	 << "LowestNeutralDisociationEnergy: " << LowestNeutralDisociationEnergy/eV
// 	 << G4endl;
  
//   return integralXS;
// }



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4double G4LEPTSExcitationModel::CrossSectionPerVolume(const G4Material* mate,
                                         const G4ParticleDefinition* aParticle,
                                         G4double kineticEnergy,
                                         G4double,
                                         G4double)
{
  return 1./GetMeanFreePath( mate, aParticle, kineticEnergy );

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void G4LEPTSExcitationModel::SampleSecondaries(std::vector<G4DynamicParticle*>*,
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
  const G4Material* aMat = mateCuts->GetMaterial();
  G4double eMax = min(theIonisPot[aMat], P0KinEn);
  //Energylost = SampleEnergyLoss(aMat, eMin, eMax);
  Energylost = theElostDistr[aMat]->Sample(eMin/eV, eMax/eV)*eV;

  Edep = Energylost;

  //G4ThreeVector P1Dir = SampleNewDirection(aMat, P0Dir, P0KinEn/eV, Energylost/eV);
  G4double ang = theDiffXS[aMat]->SampleAngle(P0KinEn/eV, Energylost/eV);
  G4ThreeVector P1Dir = SampleNewDirection(P0Dir, ang);

  G4double P1KinEn = P0KinEn - Edep;

  fParticleChangeForGamma->ProposeMomentumDirection( P1Dir);
  fParticleChangeForGamma->SetProposedKineticEnergy( P1KinEn);
  fParticleChangeForGamma->ProposeLocalEnergyDeposit( Edep);

}