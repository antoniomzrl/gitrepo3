
#include "GmLeptsPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "LeptsPhysics.hh"
#include "PenelopeLEPTSPhysics.hh"
#include "LivermoreLEPTSPhysics.hh"
#include "StandardLEPTSPhysics.hh"
#include "G4ParticleTable.hh"
#include "G4VEmProcess.hh"
#include "G4VEmModel.hh"


GmLeptsPhysics::GmLeptsPhysics() : G4VModularPhysicsList()
{
  currentDefaultCut   = 1.0*micrometer;
  cutForGamma         = currentDefaultCut;
  cutForElectron      = currentDefaultCut;
  cutForPositron      = currentDefaultCut;

  SetVerboseLevel(1);

  // EM physics
  theEmLEPTSPhysics = new LeptsPhysics();
  theHighEnergyModel = "livermore";
  if( theHighEnergyModel == "penelope" ) {
    theEmHighEnergyPhysics = new PenelopeLEPTSPhysics();
  } else if( theHighEnergyModel == "livermore" ) {
    theEmHighEnergyPhysics = new LivermoreLEPTSPhysics();
  } else if( theHighEnergyModel == "standard" ) {
    theEmHighEnergyPhysics = new StandardLEPTSPhysics();
  }
  G4cout << "GmLeptsPhysics list" << G4endl;
}


GmLeptsSMFPhysics::GmLeptsSMFPhysics() : G4VModularPhysicsList()
{
  currentDefaultCut   = 1.0*micrometer;
  cutForGamma         = currentDefaultCut;
  cutForElectron      = currentDefaultCut;
  cutForPositron      = currentDefaultCut;

  SetVerboseLevel(1);

  // EM physics
  theEmLEPTSPhysics = new LeptsSMFPhysics();
  theHighEnergyModel = "livermore";
  if( theHighEnergyModel == "penelope" ) {
    theEmHighEnergyPhysics = new PenelopeLEPTSPhysics();
  } else if( theHighEnergyModel == "livermore" ) {
    theEmHighEnergyPhysics = new LivermoreLEPTSPhysics();
  } else if( theHighEnergyModel == "standard" ) {
    theEmHighEnergyPhysics = new StandardLEPTSPhysics();
  }
  G4cout << "GmLeptsSMFPhysics list" << G4endl;
}


GmLeptsPhysics::~GmLeptsPhysics()
{
}

GmLeptsSMFPhysics::~GmLeptsSMFPhysics()
{
}


void GmLeptsPhysics::ConstructParticle()
{
  theEmLEPTSPhysics->ConstructParticle();
  //  theEmHighEnergyPhysics->ConstructParticle();
}


void GmLeptsSMFPhysics::ConstructParticle()
{
  theEmLEPTSPhysics->ConstructParticle();
  //  theEmHighEnergyPhysics->ConstructParticle();
}


void GmLeptsPhysics::ConstructProcess()
{
  // Transportation
  //
  AddTransportation();

  // Electromagnetic physics list
  //
  theEmLEPTSPhysics->ConstructProcess();
  //  theEmHighEnergyPhysics->ConstructProcess();

  /*  G4double highELimit = 10.*keV;

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){

    G4ParticleDefinition * particle = theParticleIterator->value();
    if( particle != G4Electron::Electron() && G4Positron::Positron() ) continue;x
    G4String               pName = particle->GetParticleName();
    G4ProcessManager     * pManager = particle->GetProcessManager();
    G4ProcessVector* procList = pManager->GetProcessList();
    for( G4int ii = 0; ii < procList->size(); ii++) {
      G4VEmProcess* emProc = static_cast<G4VEmProcess*>((*procList)[ii]);
      if( !emProc ) {
	G4Exception("GmLeptsPhysics::ConstructProcess",
		    "",
		    FatalException,
		    (G4String("Process is not Electromagnetic ") + (*procList)[ii]->GetProcessName()).c_str());
      }
      for( G4int ii = 0; ii < 10; ii++ ) {
	G4VEmModel* emModel = emProc->GetModelByIndex(ii,false);
	G4cout << ii << " PROC " << emProc->GetProcessName() << " P_MODEL: " << emModel << G4endl;
	if( emModel ) {
	  G4cout << " PROC " << emProc->GetProcessName() << " MODEL: " << emModel->GetName() << G4endl;
	  emModel->SetHighEnergyLimit(highELimit);
	}
      }
    }
  }
  */

}


void GmLeptsSMFPhysics::ConstructProcess()
{
  // Transportation
  //
  AddTransportation();

  // Electromagnetic physics list
  //
  theEmLEPTSPhysics->ConstructProcess();
 

}



#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

void GmLeptsPhysics::SetCuts()
{
 // fixe lower limit for cut
 G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100*eV, 1*GeV);
  
 // set cut values for gamma at first and for e- second and next for e+,
 // because some processes for e+/e- need cut values for gamma
 SetCutValue(cutForGamma, "gamma");
 SetCutValue(cutForElectron, "e-");
 SetCutValue(cutForPositron, "e+");
}


void GmLeptsSMFPhysics::SetCuts()
{
 // fixe lower limit for cut
 G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100*eV, 1*GeV);
  
 // set cut values for gamma at first and for e- second and next for e+,
 // because some processes for e+/e- need cut values for gamma
 SetCutValue(cutForGamma, "gamma");
 SetCutValue(cutForElectron, "e-");
 SetCutValue(cutForPositron, "e+");
}


void GmLeptsPhysics::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}

void GmLeptsSMFPhysics::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}


void GmLeptsPhysics::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}


void GmLeptsSMFPhysics::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}


void GmLeptsPhysics::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}

void GmLeptsSMFPhysics::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}
