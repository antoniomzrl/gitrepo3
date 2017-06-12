
#include "LeptsAnnihilation.hh"
#include "G4Positron.hh"
#include "LeptsAnnihilationModel.hh"

LeptsAnnihilation::LeptsAnnihilation(const G4String& processName) 
  : G4VEmProcess( processName ),isInitialised(false)
{
}


LeptsAnnihilation::~LeptsAnnihilation() {
}


G4bool LeptsAnnihilation::IsApplicable(const G4ParticleDefinition& particleDef) {
  return( &particleDef == G4Positron::Positron() );
}


void LeptsAnnihilation::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) 
  {
    isInitialised = true;
    SetBuildTableFlag(false);
    if(!EmModel()) SetEmModel(new LeptsAnnihilationModel);
    EmModel()->SetLowEnergyLimit(0.1*CLHEP::eV);
    EmModel()->SetHighEnergyLimit(15.*CLHEP::MeV);
    AddEmModel(1, EmModel());

  } 
}

void LeptsAnnihilation::PrintInfo()
{
  G4cout
    << " Total cross sections computed from " << EmModel()->GetName() << " model"
    << G4endl;
}         

