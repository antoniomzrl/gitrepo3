
#include "LeptsAttachmentModel.hh"

// constructor
LeptsAttachmentModel::LeptsAttachmentModel(const G4String& modelName) 
  : LeptsBaseModel( modelName )
{
  theXSType = XSAttachment;
  theType = (G4String)"Attachment";
  //  cout << "constructor Attachment " << modelName << endl;

} // constructor


LeptsAttachmentModel::~LeptsAttachmentModel() {
}



void LeptsAttachmentModel::SampleSecondaries(std::vector<G4DynamicParticle*>* ,
                                 const G4MaterialCutsCouple*,
                                 const G4DynamicParticle* aDynamicParticle,
                                 G4double,
                                 G4double)
{
  G4double P0KinEn = aDynamicParticle->GetKineticEnergy();

  fParticleChange->SetProposedKineticEnergy(0.);
  fParticleChange->ProposeLocalEnergyDeposit (P0KinEn);
  fParticleChange->ProposeTrackStatus( fStopAndKill);
 
}
