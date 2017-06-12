//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#include "G4EmLEPTSPhysics.hh"

#include "G4LivermoreRayleighModel.hh"
#include "G4LivermoreComptonModel.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4GammaConversion.hh"
#include "G4RayleighScattering.hh"
#include "G4ComptonScattering.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4OpRayleigh.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"

#include "G4DNAElastic.hh"
#include "G4DNAIonisation.hh"
#include "G4DNAExcitation.hh"
#include "G4DNADissociation.hh"
#include "G4DNAVibExcitation.hh"
#include "G4DNARotExcitation.hh"
#include "G4DNAAttachment.hh"
#include "G4DNAPositronium.hh"
#include "G4DNAChargeDecrease.hh"
#include "G4DNAChargeIncrease.hh"
#include "G4DNAThermalisation.hh"
#include "G4DNAAnnihilation.hh"

#include "G4GenericIon.hh"
#include "G4LossTableManager.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"

//e- e+:
#include "G4LEPTSElasticModel.hh"
#include "G4LEPTSIonisationModel.hh"
#include "G4LEPTSExcitationModel.hh"
#include "G4LEPTSVibExcitationModel.hh"
#include "G4LEPTSAttachmentModel.hh"
#include "G4LEPTSPositroniumModel.hh"
//H+ H0:
#include "G4LEPTSIonisationIonModel.hh"
#include "G4LEPTSExcitationIonModel.hh"
#include "G4LEPTSChargeTransferModel.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4EmLEPTSPhysics::G4EmLEPTSPhysics( const G4String& name)
  : G4VPhysicsConstructor(name)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4EmLEPTSPhysics::ConstructParticle()
{
  G4Gamma::Gamma();
  G4Electron::Electron();
  G4Positron::Positron();

  // baryons
  G4Proton::Proton();

  G4GenericIon::GenericIonDefinition();

  G4DNAGenericIonsManager * genericIonsManager;
  genericIonsManager=G4DNAGenericIonsManager::Instance();
  genericIonsManager->GetIon("alpha++");
  genericIonsManager->GetIon("alpha+");
  genericIonsManager->GetIon("helium");
  genericIonsManager->GetIon("hydrogen");
  genericIonsManager->GetIon("carbon");
  genericIonsManager->GetIon("nitrogen");
  genericIonsManager->GetIon("oxygen");
  genericIonsManager->GetIon("iron");

}

//....oooOO0OOooo.......oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void G4EmLEPTSPhysics::ConstructProcess()
{

  aParticleIterator->reset();

  while( (*aParticleIterator)() ){
    G4ParticleDefinition * particle = aParticleIterator->value();
    G4String               particleName = particle->GetParticleName();
    G4ProcessManager     * manager = particle->GetProcessManager();
    //G4cout << "particleIterator (" << particle->GetParticleName() << ") " G4endl;

    if(particleName=="e-") {
      G4DNAElastic*  elastic = new G4DNAElastic("e-_LeptsElastic");
      elastic->SetEmModel( new G4LEPTSElasticModel );
      manager->AddDiscreteProcess(elastic );

      G4DNAIonisation* ioni = new G4DNAIonisation("e-_LeptsIonisation");
      ioni->SetEmModel( new G4LEPTSIonisationModel );
      manager->AddDiscreteProcess( ioni );

      G4DNAExcitation* excit = new G4DNAExcitation("e-_LeptsElectronicExcitation");
      excit->SetEmModel( new G4LEPTSExcitationModel );
      manager->AddDiscreteProcess( excit );

      manager->AddDiscreteProcess(new G4DNADissociation("e-_LeptsDissocNeutr") );

      G4DNAVibExcitation* vibExcit = new G4DNAVibExcitation("e-_LeptsExcitVibrat");
      vibExcit->SetEmModel( new G4LEPTSVibExcitationModel );
      manager->AddDiscreteProcess( vibExcit );

      manager->AddDiscreteProcess( new G4DNARotExcitation("e-_LeptsExcitRotat") );

      G4DNAAttachment* attach = new G4DNAAttachment("e-_LeptsAttachment");
      attach->SetEmModel( new G4LEPTSAttachmentModel );
      manager->AddDiscreteProcess( attach );

      manager->AddDiscreteProcess(new G4DNAThermalisation("e-_LeptsThermalisation") );
    }
    
    else if(particleName=="e+") {
      G4DNAElastic*  elastic = new G4DNAElastic("e+_LeptsElastic");
      elastic->SetEmModel( new G4LEPTSElasticModel );
      manager->AddDiscreteProcess(elastic );

      G4DNAIonisation* ioni = new G4DNAIonisation("e+_LeptsIonisation");
      ioni->SetEmModel( new G4LEPTSIonisationModel );
      manager->AddDiscreteProcess( ioni );

      G4DNAExcitation* excit = new G4DNAExcitation("e+_LeptsElectronicExcitation");
      excit->SetEmModel( new G4LEPTSExcitationModel );
      manager->AddDiscreteProcess( excit );

      manager->AddDiscreteProcess(new G4DNADissociation("e+_LeptsDissocNeutr") );

      G4DNAVibExcitation* vibExcit = new G4DNAVibExcitation("e+_LeptsExcitVibrat");
      vibExcit->SetEmModel( new G4LEPTSVibExcitationModel );
      manager->AddDiscreteProcess( vibExcit );

      manager->AddDiscreteProcess( new G4DNARotExcitation("e+_LeptsExcitRotat") );

      manager->AddDiscreteProcess(new G4DNAPositronium("e+_LeptsPositronium") );

      manager->AddDiscreteProcess(new G4DNAAnnihilation("e+_LeptsAnnihilation") );
    }
    
    else if ( particleName == "proton" ) {
      G4DNAElastic * elast = new G4DNAElastic("proton_LeptsElastic");
      elast->SetEmModel( new G4LEPTSElasticModel );
      manager->AddDiscreteProcess( elast );

      G4DNAElastic * excit = new G4DNAElastic("proton_LeptsElectronicExcitation");
      excit->SetEmModel( new G4LEPTSExcitationIonModel );
      manager->AddDiscreteProcess( excit );

      G4DNAElastic * ionis = new G4DNAElastic("proton_LeptsIonisation");
      ionis->SetEmModel( new G4LEPTSIonisationIonModel );
      manager->AddDiscreteProcess( ionis );

      G4DNAElastic * chdec = new G4DNAElastic("proton_LeptsChargeDecrease");
      chdec->SetEmModel( new G4LEPTSChargeTransferModel );
      manager->AddDiscreteProcess( chdec );
    }
    else if (particleName == "hydrogen") {
      G4DNAElastic * elast = new G4DNAElastic("hydrogen_LeptsElastic");
      elast->SetEmModel( new G4LEPTSElasticModel );
      manager->AddDiscreteProcess( elast );

      G4DNAElastic * excit = new G4DNAElastic("hydrogen_LeptsElectronicExcitation");
      excit->SetEmModel( new G4LEPTSExcitationIonModel );
      manager->AddDiscreteProcess( excit );

      G4DNAElastic * ionis = new G4DNAElastic("hydrogen_LeptsIonisation");
      ionis->SetEmModel( new G4LEPTSIonisationIonModel );
      manager->AddDiscreteProcess( ionis );

      G4DNAElastic * chinc = new G4DNAElastic("hydrogen_LeptsChargeIncrease");
      chinc->SetEmModel( new G4LEPTSChargeTransferModel );
      manager->AddDiscreteProcess( chinc );
    } 
	    
      /*    } else if ( particleName == "proton" ) {
	    manager->AddDiscreteProcess(new G4DNAExcitation("proton_G4DNAExcitation"));
	    manager->AddDiscreteProcess(new G4DNAIonisation("proton_G4DNAIonisation"));
	    manager->AddDiscreteProcess(new G4DNAChargeDecrease("proton_G4DNAChargeDecrease"));

	    } else if ( particleName == "hydrogen" ) {
	    manager->AddDiscreteProcess(new G4DNAExcitation("hydrogen_G4DNAExcitation"));
	    manager->AddDiscreteProcess(new G4DNAIonisation("hydrogen_G4DNAIonisation"));
	    manager->AddDiscreteProcess(new G4DNAChargeIncrease("hydrogen_G4DNAChargeIncrease"));

	    } else if ( particleName == "alpha" ) {
	    manager->AddDiscreteProcess(new G4DNAExcitation("alpha_G4DNAExcitation"));
	    manager->AddDiscreteProcess(new G4DNAIonisation("alpha_G4DNAIonisation"));
	    manager->AddDiscreteProcess(new G4DNAChargeDecrease("alpha_G4DNAChargeDecrease"));

	    } else if ( particleName == "alpha+" ) {
	    manager->AddDiscreteProcess(new G4DNAExcitation("alpha+_G4DNAExcitation"));
	    manager->AddDiscreteProcess(new G4DNAIonisation("alpha+_G4DNAIonisation"));
	    manager->AddDiscreteProcess(new G4DNAChargeDecrease("alpha+_G4DNAChargeDecrease"));
	    manager->AddDiscreteProcess(new G4DNAChargeIncrease("alpha+_G4DNAChargeIncrease"));

	    } else if ( particleName == "helium" ) {
	    manager->AddDiscreteProcess(new G4DNAExcitation("helium_G4DNAExcitation"));
	    manager->AddDiscreteProcess(new G4DNAIonisation("helium_G4DNAIonisation"));
	    manager->AddDiscreteProcess(new G4DNAChargeIncrease("helium_G4DNAChargeIncrease"));
    
	    // Extension to HZE proposed by Z. Francis
      
	    } else if ( particleName == "carbon" ) {
	    manager->AddDiscreteProcess(new G4DNAIonisation("carbon_G4DNAIonisation"));
      
	    } else if ( particleName == "nitrogen" ) {
	    manager->AddDiscreteProcess(new G4DNAIonisation("nitrogen_G4DNAIonisation"));

	    } else if ( particleName == "oxygen" ) {
	    manager->AddDiscreteProcess(new G4DNAIonisation("oxygen_G4DNAIonisation"));
      
	    } else if ( particleName == "iron" ) {
	    manager->AddDiscreteProcess(new G4DNAIonisation("iron_G4DNAIonisation"));
      */
      else if (particleName == "gamma") {
      
      G4double LivermoreHighEnergyLimit = CLHEP::GeV;

      G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
      G4LivermorePhotoElectricModel* theLivermorePhotoElectricModel = 
	new G4LivermorePhotoElectricModel();
      theLivermorePhotoElectricModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      thePhotoElectricEffect->AddEmModel(0, theLivermorePhotoElectricModel);
      manager->AddDiscreteProcess(thePhotoElectricEffect);

      G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
      G4LivermoreComptonModel* theLivermoreComptonModel = 
	new G4LivermoreComptonModel();
      theLivermoreComptonModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      theComptonScattering->AddEmModel(0, theLivermoreComptonModel);
      manager->AddDiscreteProcess(theComptonScattering);

      G4GammaConversion* theGammaConversion = new G4GammaConversion();
      G4LivermoreGammaConversionModel* theLivermoreGammaConversionModel = 
	new G4LivermoreGammaConversionModel();
      theLivermoreGammaConversionModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      theGammaConversion->AddEmModel(0, theLivermoreGammaConversionModel);
      manager->AddDiscreteProcess(theGammaConversion);

      G4RayleighScattering* theRayleigh = new G4RayleighScattering();
      G4LivermoreRayleighModel* theRayleighModel = new G4LivermoreRayleighModel();
      theRayleighModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
      theRayleigh->AddEmModel(0, theRayleighModel);
      manager->AddDiscreteProcess(theRayleigh);
    }
    
  }
  
}
