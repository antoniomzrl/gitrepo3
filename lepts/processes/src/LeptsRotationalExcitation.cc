
#include "LeptsRotationalExcitation.hh"
#include "LeptsRotationalExcitationModel.hh"


LeptsRotationalExcitation::LeptsRotationalExcitation(const G4String& processName) 
  : G4VEmProcess( processName ),isInitialised(false)
{
} // constructor


LeptsRotationalExcitation::~LeptsRotationalExcitation() {
}


G4bool LeptsRotationalExcitation::IsApplicable(const G4ParticleDefinition& particleDef) {
  return( &particleDef == G4Electron::Electron() ||
	  &particleDef == G4Positron::Positron() );
}


void LeptsRotationalExcitation::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) 
  {
    isInitialised = true;
    SetBuildTableFlag(false);
    if(!EmModel()) SetEmModel(new LeptsRotationalExcitationModel);
    EmModel()->SetLowEnergyLimit(0.1*CLHEP::eV);
    EmModel()->SetHighEnergyLimit(15.*CLHEP::MeV);
    AddEmModel(1, EmModel());

  } 
}


void LeptsRotationalExcitation::PrintInfo()
{
  G4cout
    << " Total cross sections computed from " << EmModel()->GetName() << " model"
    << G4endl;
}         

