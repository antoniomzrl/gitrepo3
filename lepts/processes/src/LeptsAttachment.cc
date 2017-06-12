
#include "LeptsAttachment.hh"
#include "LeptsAttachmentModel.hh"

#include "G4SystemOfUnits.hh"
#include "G4Positron.hh"

using namespace std;

LeptsAttachment::LeptsAttachment(const G4String& processName,
  G4ProcessType type):G4VEmProcess (processName, type),
    isInitialised(false)
{
  SetProcessSubType(55);
}
 
LeptsAttachment::~LeptsAttachment()
{}

G4bool LeptsAttachment::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Electron::Electron() || &p == G4Positron::Positron());
}

void LeptsAttachment::InitialiseProcess(const G4ParticleDefinition* p)
{
  if(!isInitialised) 
  {
    isInitialised = true;
    SetBuildTableFlag(false);

    G4String name = p->GetParticleName();

    if(!EmModel())  {
        SetEmModel(new LeptsAttachmentModel);
        EmModel()->SetLowEnergyLimit(1.*eV);
        EmModel()->SetHighEnergyLimit(1.*MeV);
      }
      AddEmModel(1, EmModel());   
  } 
}


void LeptsAttachment::PrintInfo()
{
  G4cout << " Total cross sections computed from "
         << EmModel()->GetName()
         << G4endl;
}         

