
#include "LeptsUserModel.hh"

LeptsUserModel::LeptsUserModel(const G4String& modelName) : LeptsBaseModel( modelName)
{
  theXSType = XSUserProc;
  theType = (G4String)"UserProc";
}


LeptsUserModel::~LeptsUserModel() {
}



void LeptsUserModel::SampleSecondaries(std::vector<G4DynamicParticle*>*,
                                 const G4MaterialCutsCouple* mateCuts,
                                 const G4DynamicParticle* aDynamicParticle, G4double, G4double)
{
   G4String mateName = mateCuts->GetMaterial()->GetName();
   G4ThreeVector Dir = aDynamicParticle->GetMomentumDirection();
   G4cout << "xxxUserModel1: " << mateName << " " << Dir << G4endl;
   
   if( Dir.x() < 0) {
     fParticleChange->ProposeMomentumDirection(1,0,0);
     //G4ThreeVector Pos = aDynamicParticle->GetPosition();
     //fParticleChange->ProposePosition(-10.,0.,0.);
     G4cout << "xxxUserModel2 xxxlt0" << G4endl;
   }

   G4ThreeVector Dir2 = aDynamicParticle->GetMomentumDirection();
   G4cout << "xxxUserModel3: " << mateName << " " << Dir2 << G4endl;
}
