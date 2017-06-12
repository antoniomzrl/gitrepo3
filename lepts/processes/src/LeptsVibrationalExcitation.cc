
#include "LeptsVibrationalExcitation.hh"
#include "LeptsVibrationalExcitationModel.hh"
#include "G4SystemOfUnits.hh"
#include "G4Positron.hh"


using namespace std;

LeptsVibrationalExcitation::LeptsVibrationalExcitation(const G4String& processName,
  G4ProcessType type):G4VEmProcess (processName, type),
    isInitialised(false)
{
  SetProcessSubType(54);
}

 
LeptsVibrationalExcitation::~LeptsVibrationalExcitation()
{}


G4bool LeptsVibrationalExcitation::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Electron::Electron() || &p == G4Positron::Positron());
}


void LeptsVibrationalExcitation::InitialiseProcess(const G4ParticleDefinition* p)
{
  if(!isInitialised) 
  {
    isInitialised = true;
    SetBuildTableFlag(false);
    
    if( !EmModel() ) {
      SetEmModel(new LeptsVibrationalExcitationModel);
      EmModel()->SetLowEnergyLimit(2*eV);
      EmModel()->SetHighEnergyLimit(100*eV);
    }
    AddEmModel(1, EmModel());
  } 
}


void LeptsVibrationalExcitation::PrintInfo()
{
     G4cout
      << " Total cross sections computed from " 
      << EmModel()->GetName() 
      << G4endl;
}
