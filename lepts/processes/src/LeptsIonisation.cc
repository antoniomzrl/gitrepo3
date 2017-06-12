
#include "LeptsIonisation.hh"
#include "LeptsIonisationModel.hh"
#include "LeptsIonisationIonModel.hh"
#include "G4SystemOfUnits.hh"

#include "G4GenericIon.hh"
#include "G4Positron.hh"

using namespace std;

LeptsIonisation::LeptsIonisation(const G4String& processName,
                                 G4ProcessType type) :
    G4VEmProcess(processName, type), isInitialised(false)
{
  SetProcessSubType(53);
}


LeptsIonisation::~LeptsIonisation()
{
}


G4bool LeptsIonisation::IsApplicable(const G4ParticleDefinition& p)
{
  G4DNAGenericIonsManager * instance = G4DNAGenericIonsManager::Instance();

  return (&p == G4Electron::Electron() || &p == G4Positron::Positron()
          || &p == G4Proton::Proton() || &p == instance->GetIon("hydrogen")
          || &p == instance->GetIon("alpha++")
          || &p == instance->GetIon("alpha+")
          || &p == instance->GetIon("helium")
          //SEB
          //|| &p == instance->GetIon("carbon")
          //|| &p == instance->GetIon("nitrogen")
          //|| &p == instance->GetIon("oxygen")
          //|| &p == instance->GetIon("iron")
      || &p == G4GenericIon::GenericIonDefinition());
}


void LeptsIonisation::InitialiseProcess(const G4ParticleDefinition* p)
{
  if(!isInitialised) {
    isInitialised = true;
    SetBuildTableFlag(false);

    G4String name = p->GetParticleName();

    if(name == "e-" || name == "e+" ) {
      if(!EmModel())  {
        LeptsIonisationModel* lepts = new LeptsIonisationModel();
        SetEmModel(lepts);
        lepts->SetLowEnergyLimit(1. * eV);
        lepts->SetHighEnergyLimit(1. * MeV);
      }
      AddEmModel(1, EmModel());
    }
    else if(name == "proton" || name == "hydrogen") {
      if(!EmModel())  {
	SetEmModel( new LeptsIonisationIonModel() );
      }
      AddEmModel(1, EmModel());
    }
  }
}


void LeptsIonisation::PrintInfo()
{

}
