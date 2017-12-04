
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
   //G4cout << "xxxUserModel: " << mateName << " " << Dir << endl;
   if( Dir.x() < 0) {
     //Dir = -Dir;
     //G4ThreeVector Pos = aDynamicParticle->GetMomentumDirection();
     fParticleChange->ProposeMomentumDirection(1,0,0);
   }
}
