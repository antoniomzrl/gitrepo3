
// AMR from "examples/extended/medical/dna/microdosimetry"

#include "GmEMSSDNAPhysics.hh"
#include "G4SystemOfUnits.hh"

// Geant4-DNA MODELS

#include "G4DNAElastic.hh"
#include "G4DNAChampionElasticModel.hh"
#include "G4DNAScreenedRutherfordElasticModel.hh"

#include "G4DNAExcitation.hh"
#include "G4DNAMillerGreenExcitationModel.hh"
#include "G4DNABornExcitationModel.hh"

#include "G4DNAIonisation.hh"
#include "G4DNABornIonisationModel.hh"
#include "G4DNARuddIonisationModel.hh"

#include "G4DNAChargeDecrease.hh"
#include "G4DNADingfelderChargeDecreaseModel.hh"

#include "G4DNAChargeIncrease.hh"
#include "G4DNADingfelderChargeIncreaseModel.hh"

#include "G4DNAAttachment.hh"
#include "G4DNAMeltonAttachmentModel.hh"

#include "G4DNAVibExcitation.hh"
#include "G4DNASancheExcitationModel.hh"

//

#include "G4LossTableManager.hh"
#include "G4EmConfigurator.hh"
#include "G4VEmModel.hh"
#include "G4DummyModel.hh"
#include "G4UrbanMscModel.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4SeltzerBergerModel.hh"
#include "G4eBremsstrahlungRelModel.hh"
#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4eMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#include "G4BraggIonGasModel.hh"
#include "G4BetheBlochIonGasModel.hh"
#include "G4UrbanMscModel.hh"
#include "G4MollerBhabhaModel.hh"
#include "G4IonFluctuations.hh"
#include "G4UniversalFluctuation.hh"


GmEMSSDNAPhysics::GmEMSSDNAPhysics():  G4VUserPhysicsList()
{
  defaultCutValue = 1*micrometer;
  fCutForGamma     = defaultCutValue;
  fCutForElectron  = defaultCutValue;
  fCutForPositron  = defaultCutValue;
  fCutForProton    = defaultCutValue;

  SetVerboseLevel(1);
}


GmEMSSDNAPhysics::~GmEMSSDNAPhysics()
{}


void GmEMSSDNAPhysics::ConstructParticle()
{
  ConstructBosons();
  ConstructLeptons();
  ConstructBarions();
}


void GmEMSSDNAPhysics::ConstructBosons()
{ 
  // gamma
  G4Gamma::GammaDefinition();
}

void GmEMSSDNAPhysics::ConstructLeptons()
{
  // leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
}


//DNA
#include "G4DNAGenericIonsManager.hh"
//ENDDNA

void GmEMSSDNAPhysics::ConstructBarions()
{
  //  baryons
  G4Proton::ProtonDefinition();
  G4GenericIon::GenericIonDefinition();

  // Geant4 DNA new particles
  G4DNAGenericIonsManager * genericIonsManager;
  genericIonsManager=G4DNAGenericIonsManager::Instance();
  genericIonsManager->GetIon("alpha++");
  genericIonsManager->GetIon("alpha+");
  genericIonsManager->GetIon("helium");
  genericIonsManager->GetIon("hydrogen");
}


void GmEMSSDNAPhysics::ConstructProcess()
{
  AddTransportation();
  ConstructEM();
  ConstructGeneral();
}

void GmEMSSDNAPhysics::ConstructEM()
{

  theParticleIterator->reset();

  while( (*theParticleIterator)() )
  {

    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    // *********************************
    // 1) Processes for the World region
    // *********************************

    if (particleName == "e-") {

      // STANDARD msc 
      G4eMultipleScattering* msc = new G4eMultipleScattering();
      G4VMscModel* modMsc = new G4UrbanMscModel();
      modMsc->SetActivationLowEnergyLimit(1*MeV);
      msc->SetEmModel(modMsc, 1);

      pmanager->AddProcess(msc, -1, 1, 1);

      // STANDARD ionisation
      G4VEmModel* modIoni = new G4MollerBhabhaModel();
      modIoni->SetActivationLowEnergyLimit(1*MeV);
      G4eIonisation* eIoni = new G4eIonisation();
      eIoni->SetEmModel(modIoni, 1);
      pmanager->AddProcess(eIoni, -1, 2, 2);

      // STANDARD bremsstrahlung
      G4SeltzerBergerModel* modBrem = new G4SeltzerBergerModel();
      G4eBremsstrahlungRelModel* modBrem2 = new G4eBremsstrahlungRelModel();
      G4eBremsstrahlung* eBrems = new G4eBremsstrahlung();
      modBrem->SetLowEnergyLimit(eBrems->MinKinEnergy());
      modBrem->SetHighEnergyLimit(modBrem2->LowEnergyLimit());
      modBrem2->SetHighEnergyLimit(eBrems->MaxKinEnergy());
      modBrem2->SetActivationLowEnergyLimit(1*MeV);
      G4VEmFluctuationModel* fm = 0;
      eBrems->AddEmModel(1, modBrem, fm);
      eBrems->AddEmModel(2, modBrem2, fm);
      pmanager->AddProcess(eBrems, -1, 3, 3);

      // DNA elastic is not active in the world 
      G4DNAElastic* theDNAElasticProcess = new G4DNAElastic("e-_G4DNAElastic");
      pmanager->AddDiscreteProcess(theDNAElasticProcess);

      // DNA excitation is not active in the world 
      G4DNAExcitation* dnaex = new G4DNAExcitation("e-_G4DNAExcitation");
      pmanager->AddDiscreteProcess(dnaex);

      // DNA ionisation is not active in the world 
      G4DNAIonisation* dnaioni = new G4DNAIonisation("e-_G4DNAIonisation");
      pmanager->AddDiscreteProcess(dnaioni);

      // DNA attachment is not active in the world 
      G4DNAAttachment* dnaatt = new G4DNAAttachment("e-_G4DNAAttachment");
      pmanager->AddDiscreteProcess(dnaatt);

      // DNA vib. excitation is not active in the world 
      G4DNAVibExcitation* dnavib =
          new G4DNAVibExcitation("e-_G4DNAVibExcitation");
      pmanager->AddDiscreteProcess(dnavib);

    } else if ( particleName == "proton" ) {

      // STANDARD msc is active in the world 
      G4hMultipleScattering* msc = new G4hMultipleScattering();
      pmanager->AddProcess(msc, -1, 1, 1);

      // STANDARD ionisation is active in the world 
      G4hIonisation* hion = new G4hIonisation();
      hion->SetEmModel(new G4BraggIonGasModel(), 1);
      hion->SetEmModel(new G4BetheBlochIonGasModel(), 2);
      pmanager->AddProcess(hion, -1, 2, 2);

      // DNA excitation is not active in the world 
      G4DNAExcitation* dnaex = new G4DNAExcitation("proton_G4DNAExcitation");
      pmanager->AddDiscreteProcess(dnaex);

      // DNA ionisation is not active in the world 
      G4DNAIonisation* dnaioni = new G4DNAIonisation("proton_G4DNAIonisation");
      pmanager->AddDiscreteProcess(dnaioni);

      // DNA charge decrease is ACTIVE in the world since
      // no corresponding STANDARD process exist
      pmanager->AddDiscreteProcess(
          new G4DNAChargeDecrease("proton_G4DNAChargeDecrease"));

    } else if ( particleName == "hydrogen" ) {

      // DNA processes are ACTIVE in the world since
      // no corresponding STANDARD processes exist
      pmanager->AddDiscreteProcess(
          new G4DNAIonisation("hydrogen_G4DNAIonisation"));
      pmanager->AddDiscreteProcess(
          new G4DNAExcitation("hydrogen_G4DNAExcitation"));
      pmanager->AddDiscreteProcess(
          new G4DNAChargeIncrease("hydrogen_G4DNAChargeIncrease"));

    } else if (particleName == "GenericIon")
    { // THIS IS NEEDED FOR STANDARD ALPHA G4ionIonisation PROCESS

      // STANDARD msc is active in the world 
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);

      // STANDARD ionisation is active in the world 
      G4ionIonisation* hion = new G4ionIonisation();
      hion->SetEmModel(new G4BraggIonGasModel(),1);
      hion->SetEmModel(new G4BetheBlochIonGasModel(), 2);
      pmanager->AddProcess(hion, -1, 2, 2);

    } else if ( particleName == "alpha" ) {

      // STANDARD msc is active in the world 
      G4hMultipleScattering* msc = new G4hMultipleScattering();
      pmanager->AddProcess(msc, -1, 1, 1);

      // STANDARD ionisation is active in the world 
      G4ionIonisation* hion = new G4ionIonisation();
      hion->SetEmModel(new G4BraggIonGasModel(),1);
      hion->SetEmModel(new G4BetheBlochIonGasModel(), 2);
      pmanager->AddProcess(hion, -1, 2, 2);

      // DNA excitation is not active in the world 
      G4DNAExcitation* dnaex = new G4DNAExcitation("alpha_G4DNAExcitation");
      pmanager->AddDiscreteProcess(dnaex);

      // DNA ionisation is not active in the world 
      G4DNAIonisation* dnaioni = new G4DNAIonisation("alpha_G4DNAIonisation");
      pmanager->AddDiscreteProcess(dnaioni);

      // DNA charge decrease is ACTIVE in the world since no
      // corresponding STANDARD process exist
      pmanager->AddDiscreteProcess(
          new G4DNAChargeDecrease("alpha_G4DNAChargeDecrease"));

    } else if ( particleName == "alpha+" ) {

      // DNA processes are ACTIVE in the world since no
      // corresponding STANDARD processes exist
      pmanager->AddDiscreteProcess(
          new G4DNAExcitation("alpha+_G4DNAExcitation"));
      pmanager->AddDiscreteProcess(
          new G4DNAIonisation("alpha+_G4DNAIonisation"));
      pmanager->AddDiscreteProcess(
          new G4DNAChargeDecrease("alpha+_G4DNAChargeDecrease"));
      pmanager->AddDiscreteProcess(
          new G4DNAChargeIncrease("alpha+_G4DNAChargeIncrease"));

    } else if ( particleName == "helium" ) {

      // DNA processes are ACTIVE in the world since no
      // corresponding STANDARD processes exist
      pmanager->AddDiscreteProcess(
          new G4DNAExcitation("helium_G4DNAExcitation"));
      pmanager->AddDiscreteProcess(
          new G4DNAIonisation("helium_G4DNAIonisation"));
      pmanager->AddDiscreteProcess(
          new G4DNAChargeIncrease("helium_G4DNAChargeIncrease"));

    }
  }

}

void GmEMSSDNAPhysics::ConstructGeneral()
{ }

void GmEMSSDNAPhysics::SetCuts()
{
  if (verboseLevel >0)
  {
    G4cout << "GmEMSSDNAPhysics::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }  

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma 
  SetCutValue(fCutForGamma, "gamma");
  SetCutValue(fCutForElectron, "e-");
  SetCutValue(fCutForPositron, "e+");
  SetCutValue(fCutForProton, "proton");

  if (verboseLevel>0) { DumpCutValuesTable(); }
}
