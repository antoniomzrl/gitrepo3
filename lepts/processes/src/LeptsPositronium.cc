
#include "LeptsPositronium.hh"
#include "LeptsPositroniumModel.hh"


LeptsPositronium::LeptsPositronium(const G4String& processName) 
  : G4VEmProcess( processName ),isInitialised(false)
{
} // constructor


LeptsPositronium::~LeptsPositronium() {
}


G4bool LeptsPositronium::IsApplicable(const G4ParticleDefinition& particleDef) {
  return( &particleDef == G4Positron::Positron() );
}


void LeptsPositronium::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) 
  {
    isInitialised = true;
    SetBuildTableFlag(false);
    if(!EmModel()) SetEmModel(new LeptsPositroniumModel);
    EmModel()->SetLowEnergyLimit(0.1*CLHEP::eV);
    EmModel()->SetHighEnergyLimit(15.*CLHEP::MeV);
    AddEmModel(1, EmModel());

  } 
}


void LeptsPositronium::PrintInfo()
{
  G4cout
    << " Total cross sections computed from " << EmModel()->GetName() << " model"
    << G4endl;
}         

