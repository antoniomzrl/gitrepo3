

#include "LeptsElastic.hh"
#include "G4SystemOfUnits.hh"
#include "G4Positron.hh"
#include "LeptsElasticModel.hh"

using namespace std;

LeptsElastic::LeptsElastic(const G4String& processName, G4ProcessType type) :
    G4VEmProcess(processName, type), isInitialised(false)
{
  SetProcessSubType(51);
}


LeptsElastic::~LeptsElastic()
{
}


G4bool LeptsElastic::IsApplicable(const G4ParticleDefinition& p)
{
  G4DNAGenericIonsManager * instance = G4DNAGenericIonsManager::Instance();

  return( &p == G4Electron::Electron() ||
	  &p == G4Positron::Positron() ||
	  &p == G4Proton::Proton()     ||
	  &p == instance->GetIon("hydrogen") ||
	  &p == instance->GetIon("alpha++")  ||
	  &p == instance->GetIon("alpha+")   ||
	  &p == instance->GetIon("helium"));

}


void LeptsElastic::InitialiseProcess(const G4ParticleDefinition* p)
{
  if(!isInitialised)
    {
      isInitialised = true;
      SetBuildTableFlag(false);

      G4String name = p->GetParticleName();

      if(!EmModel())  {
        SetEmModel(new LeptsElasticModel);
        EmModel()->SetLowEnergyLimit(0 * eV);
        EmModel()->SetHighEnergyLimit(1. * MeV);
      }
      AddEmModel(1, EmModel());
    
    }
}


void LeptsElastic::PrintInfo()
{
  G4cout << " Total cross sections computed from " << EmModel()->GetName()
         << " model" << G4endl;
}

