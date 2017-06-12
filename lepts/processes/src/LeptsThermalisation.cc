
#include "LeptsThermalisation.hh"
#include "LeptsThermalisationModel.hh"


LeptsThermalisation::LeptsThermalisation(const G4String& processName) 
  : G4VEmProcess( processName ),isInitialised(false)
{
} // constructor


LeptsThermalisation::~LeptsThermalisation() {
}


G4bool LeptsThermalisation::IsApplicable(const G4ParticleDefinition& particleDef) {
  return( &particleDef == G4Electron::Electron() );
}


void LeptsThermalisation::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) 
  {
    isInitialised = true;
    SetBuildTableFlag(false);
    if(!EmModel()) SetEmModel(new LeptsThermalisationModel);
    EmModel()->SetLowEnergyLimit(0.1*CLHEP::eV);
    EmModel()->SetHighEnergyLimit(15.*CLHEP::MeV);
    AddEmModel(1, EmModel());

  } 
}

void LeptsThermalisation::PrintInfo()
{
  G4cout
    << " Total cross sections computed from " << EmModel()->GetName() << " model"
    << G4endl;
}         

