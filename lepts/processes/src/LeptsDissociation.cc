
#include "LeptsDissociation.hh"
#include "LeptsDissociationModel.hh"


LeptsDissociation::LeptsDissociation(const G4String& processName) :
    G4VEmProcess(processName), isInitialised(false)
{
}


LeptsDissociation::~LeptsDissociation()
{
}


G4bool LeptsDissociation::IsApplicable(const G4ParticleDefinition& particleDef)
{
  return (&particleDef == G4Electron::Electron()
      || &particleDef == G4Positron::Positron());
}


void LeptsDissociation::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised)
  {
    isInitialised = true;
    SetBuildTableFlag(false);
    if(!EmModel())
    {
      SetEmModel(new LeptsDissociationModel);
      EmModel()->SetLowEnergyLimit(0.1 * CLHEP::eV);
      EmModel()->SetHighEnergyLimit(15. * CLHEP::MeV);
    }
    AddEmModel(1, EmModel());
  }
}


void LeptsDissociation::PrintInfo()
{
  G4cout << " Total cross sections computed from " << EmModel()->GetName()
         << " model" << G4endl;
}

