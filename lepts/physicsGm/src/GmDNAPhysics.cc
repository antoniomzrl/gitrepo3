
#include "GmDNAPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmDNAPhysics.hh"
#include "G4ParticleTable.hh"
#include "G4VEmProcess.hh"
#include "G4VEmModel.hh"


GmDNAPhysics::GmDNAPhysics() 
: G4VModularPhysicsList()
{
  currentDefaultCut   = 1.0*micrometer;
  cutForGamma         = currentDefaultCut;
  cutForElectron      = currentDefaultCut;
  cutForPositron      = currentDefaultCut;

  SetVerboseLevel(1);

  // EM physics
  theEmDNAPhysics = new G4EmDNAPhysics();
  /*  theHighEnergyModel = "livermore";
  if( theHighEnergyModel == "penelope" ) {
    theEmHighEnergyPhysics = new G4EmPenelopeLEPTSPhysics();
  } else if( theHighEnergyModel == "livermore" ) {
    theEmHighEnergyPhysics = new G4EmLivermoreLEPTSPhysics();
  } else if( theHighEnergyModel == "standard" ) {
    theEmHighEnergyPhysics = new G4EmStandardLEPTSPhysics();
    }*/

}


GmDNAPhysics::~GmDNAPhysics()
{
}


void GmDNAPhysics::ConstructParticle()
{
  theEmDNAPhysics->ConstructParticle();
  //  theEmHighEnergyPhysics->ConstructParticle();
}


void GmDNAPhysics::ConstructProcess()
{
  // Transportation
  //
  AddTransportation();

  // Electromagnetic physics list
  //
  theEmDNAPhysics->ConstructProcess();
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
	G4Exception("GmDNAPhysics::ConstructProcess",
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


#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

void GmDNAPhysics::SetCuts()
{
 // fixe lower limit for cut
 G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100*eV, 1*GeV);
  
 // set cut values for gamma at first and for e- second and next for e+,
 // because some processes for e+/e- need cut values for gamma
 SetCutValue(cutForGamma, "gamma");
 SetCutValue(cutForElectron, "e-");
 SetCutValue(cutForPositron, "e+");
}


void GmDNAPhysics::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}


void GmDNAPhysics::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}


void GmDNAPhysics::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}
