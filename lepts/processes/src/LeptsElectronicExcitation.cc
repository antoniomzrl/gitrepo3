
#include "LeptsElectronicExcitation.hh"
#include "LeptsElectronicExcitationModel.hh"
#include "G4SystemOfUnits.hh"
#include "G4Positron.hh"

using namespace std;

LeptsElectronicExcitation::LeptsElectronicExcitation(const G4String& processName,
                                 G4ProcessType type) :
    G4VEmProcess(processName, type), isInitialised(false)
{
  SetProcessSubType(52);
}


LeptsElectronicExcitation::~LeptsElectronicExcitation()
{
}


G4bool LeptsElectronicExcitation::IsApplicable(const G4ParticleDefinition& p)
{

  G4DNAGenericIonsManager * instance = G4DNAGenericIonsManager::Instance();

  return( &p == G4Electron::Electron() ||
	  &p == G4Positron::Positron() ||
	  &p == G4Proton::ProtonDefinition() ||
	  &p == instance->GetIon("hydrogen") ||
	  &p == instance->GetIon("alpha++")  ||
	  &p == instance->GetIon("alpha+")   ||
	  &p == instance->GetIon("helium"));
}


void LeptsElectronicExcitation::InitialiseProcess(const G4ParticleDefinition* p)
{
  if(!isInitialised) {
    isInitialised = true;
    SetBuildTableFlag(false);

    G4String name = p->GetParticleName();

    if(!EmModel()) {
      LeptsElectronicExcitationModel* lepts = new LeptsElectronicExcitationModel();
      SetEmModel(lepts);
      lepts->SetLowEnergyLimit(1 * eV);
      lepts->SetHighEnergyLimit(1 * MeV);
    }
    AddEmModel(1, EmModel());
  }
}


void LeptsElectronicExcitation::PrintInfo()
{
  if(EmModel(2))
  {
    G4cout << " Total cross sections computed from " << EmModel(1)->GetName()
           << " and " << EmModel(2)->GetName() << " models" << G4endl;
  }
  else
  {
    G4cout << " Total cross sections computed from "
           << EmModel()->GetName()
           << G4endl;
  }
}
