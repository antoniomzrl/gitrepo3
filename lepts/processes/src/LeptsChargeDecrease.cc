
#include "LeptsChargeDecrease.hh"
#include "LeptsChargeTransferModel.hh"
#include "G4SystemOfUnits.hh"


using namespace std;

LeptsChargeDecrease::LeptsChargeDecrease(const G4String& processName,
                                         G4ProcessType type) :
    G4VEmProcess(processName, type), isInitialised(false)
{
  SetProcessSubType(56);
}


LeptsChargeDecrease::~LeptsChargeDecrease()
{
}

G4bool LeptsChargeDecrease::IsApplicable(const G4ParticleDefinition& p)
{

  G4DNAGenericIonsManager * instance = G4DNAGenericIonsManager::Instance();

  return (&p == G4Proton::ProtonDefinition() ||
	  &p == instance->GetIon("alpha++") ||
	  &p == instance->GetIon("alpha+"));
}


void LeptsChargeDecrease::InitialiseProcess(const G4ParticleDefinition* p)
{
  if( !isInitialised) {
    isInitialised = true;
    SetBuildTableFlag(false);

    G4String name = p->GetParticleName();
    if( !EmModel()) {
      SetEmModel(new LeptsChargeTransferModel );
      EmModel()->SetLowEnergyLimit(100 * eV);
      EmModel()->SetHighEnergyLimit(100 * MeV);
    }
    AddEmModel(1, EmModel());

  }
}


void LeptsChargeDecrease::PrintInfo()
{
  G4cout << " Total cross sections computed from " << EmModel()->GetName()
         << " model" << G4endl;
}
