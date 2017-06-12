
#include "LeptsPhysics.hh"

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

#include "LeptsElastic.hh"
#include "LeptsIonisation.hh"
#include "LeptsElectronicExcitation.hh"
#include "LeptsDissociation.hh"
#include "LeptsVibrationalExcitation.hh"
#include "LeptsRotationalExcitation.hh"
#include "LeptsAttachment.hh"
#include "LeptsPositronium.hh"
#include "LeptsChargeDecrease.hh"
#include "LeptsChargeIncrease.hh"
#include "LeptsThermalisation.hh"
#include "LeptsAnnihilation.hh"

#include "G4GenericIon.hh"
#include "G4LossTableManager.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"

//e- e+:
#include "LeptsElasticModel.hh"
#include "LeptsUserModel.hh"
#include "LeptsIonisationModel.hh"
#include "LeptsIonisationSDIModel.hh"
#include "LeptsElectronicExcitationModel.hh"
#include "LeptsVibrationalExcitationModel.hh"
#include "LeptsRotationalExcitationModel.hh"
#include "LeptsDissociationModel.hh"
#include "LeptsAttachmentModel.hh"
#include "LeptsPositroniumModel.hh"
//H+ H0:
#include "LeptsIonisationIonModel.hh"
#include "LeptsElectronicExcitationIonModel.hh"
#include "LeptsChargeTransferModel.hh"
#include "LeptsThermalisationModel.hh"
#include "LeptsAnnihilationModel.hh"


LeptsPhysics::LeptsPhysics( const G4String& name) : G4VPhysicsConstructor(name) { }

LeptsSMFPhysics::LeptsSMFPhysics( const G4String& name) : G4VPhysicsConstructor(name) { }



void LeptsPhysics::ConstructParticle()
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

void LeptsSMFPhysics::ConstructParticle()
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


void LeptsPhysics::ConstructProcess()
{

  aParticleIterator->reset();

  while( (*aParticleIterator)() ){
    G4ParticleDefinition * particle = aParticleIterator->value();
    G4String               particleName = particle->GetParticleName();
    G4ProcessManager     * manager = particle->GetProcessManager();
    //G4cout << "particleIterator (" << particle->GetParticleName() << ") " G4endl;

    if(particleName=="e-") {
      LeptsElastic * elastic = new LeptsElastic("e-_LeptsElastic");
      elastic->SetEmModel( new LeptsElasticModel);
      manager->AddDiscreteProcess( elastic);

      LeptsIonisation * ioni = new LeptsIonisation("e-_LeptsIonisation");
      ioni->SetEmModel( new LeptsIonisationModel);
      manager->AddDiscreteProcess( ioni);

      LeptsIonisation * ionis = new LeptsIonisation("e-_LeptsIonisationS");
      ionis->SetEmModel( new LeptsSingleIonisationModel);
      manager->AddDiscreteProcess( ionis);

      LeptsIonisation * ionid = new LeptsIonisation("e-_LeptsIonisationD");
      ionid->SetEmModel( new LeptsDoubleIonisationModel);
      manager->AddDiscreteProcess( ionid);

      LeptsIonisation * ionik = new LeptsIonisation("e-_LeptsIonisationK");
      ionik->SetEmModel( new LeptsInternalIonisationModel);
      manager->AddDiscreteProcess( ionik);

      
      LeptsElectronicExcitation * excit = new LeptsElectronicExcitation("e-_LeptsElectronicExcitation");
      excit->SetEmModel( new LeptsElectronicExcitationModel);
      manager->AddDiscreteProcess( excit);

      LeptsDissociation * dissoc = new LeptsDissociation("e-_LeptsDissocNeutr");
      dissoc->SetEmModel( new LeptsDissociationModel);
      manager->AddDiscreteProcess(dissoc);

      LeptsVibrationalExcitation * vib = new LeptsVibrationalExcitation("e-_LeptsExcitVibrat");
      vib->SetEmModel( new LeptsVibrationalExcitationModel);
      manager->AddDiscreteProcess( vib);

      LeptsRotationalExcitation * rot = new LeptsRotationalExcitation("e-_LeptsExcitRotat");
      rot->SetEmModel( new LeptsRotationalExcitationModel);
      manager->AddDiscreteProcess( rot);

      LeptsAttachment * attach = new LeptsAttachment("e-_LeptsAttachment");
      attach->SetEmModel( new LeptsAttachmentModel);
      manager->AddDiscreteProcess( attach);

      LeptsThermalisation * therm = new LeptsThermalisation("e-_LeptsThermalisation");
      therm->SetEmModel( new LeptsThermalisationModel);
      manager->AddDiscreteProcess( therm);
      
      LeptsElastic * userproc = new LeptsElastic("e-_LeptsUserProc");
      userproc->SetEmModel( new LeptsUserModel);
      manager->AddDiscreteProcess( userproc);
    }

    else if(particleName=="e+") {
      LeptsElastic * elastic = new LeptsElastic("e+_LeptsElastic");
      elastic->SetEmModel( new LeptsElasticModel);
      manager->AddDiscreteProcess( elastic);

      LeptsIonisation * ioni = new LeptsIonisation("e+_LeptsIonisation");
      ioni->SetEmModel( new LeptsIonisationModel);
      manager->AddDiscreteProcess( ioni);

      LeptsIonisation * ionis = new LeptsIonisation("e+_LeptsIonisationS");
      ionis->SetEmModel( new LeptsSingleIonisationModel);
      manager->AddDiscreteProcess( ionis);

      LeptsIonisation * ionid = new LeptsIonisation("e+_LeptsIonisationD");
      ionid->SetEmModel( new LeptsDoubleIonisationModel);
      manager->AddDiscreteProcess( ionid);

      LeptsIonisation * ionik = new LeptsIonisation("e+_LeptsIonisationK");
      ionik->SetEmModel( new LeptsInternalIonisationModel);
      manager->AddDiscreteProcess( ionik);

      LeptsElectronicExcitation * excit = new LeptsElectronicExcitation("e+_LeptsElectronicExcitation");
      excit->SetEmModel( new LeptsElectronicExcitationModel);
      manager->AddDiscreteProcess( excit);

      LeptsDissociation * dissoc = new LeptsDissociation("e+_LeptsDissocNeutr");
      dissoc->SetEmModel( new LeptsDissociationModel);
      manager->AddDiscreteProcess(dissoc);

      LeptsVibrationalExcitation * vib = new LeptsVibrationalExcitation("e+_LeptsExcitVibrat");
      vib
	->SetEmModel( new LeptsVibrationalExcitationModel);
      manager->AddDiscreteProcess( vib);

      LeptsRotationalExcitation * rot = new LeptsRotationalExcitation("e+_LeptsExcitRotat");
      rot->SetEmModel( new LeptsRotationalExcitationModel);
      manager->AddDiscreteProcess( rot);

      LeptsPositronium * positr = new LeptsPositronium("e+_LeptsPositronium");
      positr->SetEmModel( new LeptsPositroniumModel);
      manager->AddDiscreteProcess( positr);

      LeptsAnnihilation * annih = new LeptsAnnihilation("e+_LeptsAnnihilation");
      annih->SetEmModel( new LeptsAnnihilationModel );
      manager->AddDiscreteProcess( annih);
          
      LeptsElastic * userproc = new LeptsElastic("e-_LeptsUserProc");
      userproc->SetEmModel( new LeptsUserModel);
      manager->AddDiscreteProcess( userproc);
    }
    
    else if ( particleName == "proton" ) {
      LeptsElastic * elast = new LeptsElastic("proton_LeptsElastic");
      elast->SetEmModel( new LeptsElasticModel);
      manager->AddDiscreteProcess( elast);

      LeptsElastic * excit = new LeptsElastic("proton_LeptsElectronicExcitation");
      excit->SetEmModel( new LeptsElectronicExcitationIonModel );
      manager->AddDiscreteProcess( excit );

      LeptsElastic * ionis = new LeptsElastic("proton_LeptsIonisation");
      ionis->SetEmModel( new LeptsIonisationIonModel );
      manager->AddDiscreteProcess( ionis);

      LeptsElastic * chdec = new LeptsElastic("proton_LeptsChargeDecrease");
      chdec->SetEmModel( new LeptsChargeTransferModel);
      manager->AddDiscreteProcess( chdec);
    }
    else if (particleName == "hydrogen") {
      LeptsElastic * elast = new LeptsElastic("hydrogen_LeptsElastic");
      elast->SetEmModel( new LeptsElasticModel);
      manager->AddDiscreteProcess( elast);

      LeptsElastic * excit = new LeptsElastic("hydrogen_LeptsElectronicExcitation");
      excit->SetEmModel( new LeptsElectronicExcitationIonModel);
      manager->AddDiscreteProcess( excit);

      LeptsElastic * ionis = new LeptsElastic("hydrogen_LeptsIonisation");
      ionis->SetEmModel( new LeptsIonisationIonModel);
      manager->AddDiscreteProcess( ionis);

      LeptsElastic * chinc = new LeptsElastic("hydrogen_LeptsChargeIncrease");
      chinc->SetEmModel( new LeptsChargeTransferModel );
      manager->AddDiscreteProcess( chinc);
    } 
	    
      /*    } else if ( particleName == "proton" ) {
	    manager->AddDiscreteProcess(new LeptsExcitation("proton_LeptsExcitation"));
	    manager->AddDiscreteProcess(new LeptsIonisation("proton_LeptsIonisation"));
	    manager->AddDiscreteProcess(new LeptsChargeDecrease("proton_LeptsChargeDecrease"));

	    } else if ( particleName == "hydrogen" ) {
	    manager->AddDiscreteProcess(new LeptsExcitation("hydrogen_LeptsExcitation"));
	    manager->AddDiscreteProcess(new LeptsIonisation("hydrogen_LeptsIonisation"));
	    manager->AddDiscreteProcess(new LeptsChargeIncrease("hydrogen_LeptsChargeIncrease"));

	    } else if ( particleName == "alpha" ) {
	    manager->AddDiscreteProcess(new LeptsExcitation("alpha_LeptsExcitation"));
	    manager->AddDiscreteProcess(new LeptsIonisation("alpha_LeptsIonisation"));
	    manager->AddDiscreteProcess(new LeptsChargeDecrease("alpha_LeptsChargeDecrease"));

	    } else if ( particleName == "alpha+" ) {
	    manager->AddDiscreteProcess(new LeptsExcitation("alpha+_LeptsExcitation"));
	    manager->AddDiscreteProcess(new LeptsIonisation("alpha+_LeptsIonisation"));
	    manager->AddDiscreteProcess(new LeptsChargeDecrease("alpha+_LeptsChargeDecrease"));
	    manager->AddDiscreteProcess(new LeptsChargeIncrease("alpha+_LeptsChargeIncrease"));

	    } else if ( particleName == "helium" ) {
	    manager->AddDiscreteProcess(new LeptsExcitation("helium_LeptsExcitation"));
	    manager->AddDiscreteProcess(new LeptsIonisation("helium_LeptsIonisation"));
	    manager->AddDiscreteProcess(new LeptsChargeIncrease("helium_LeptsChargeIncrease"));
    
	    // Extension to HZE proposed by Z. Francis
      
	    } else if ( particleName == "carbon" ) {
	    manager->AddDiscreteProcess(new LeptsIonisation("carbon_LeptsIonisation"));
      
	    } else if ( particleName == "nitrogen" ) {
	    manager->AddDiscreteProcess(new LeptsIonisation("nitrogen_LeptsIonisation"));

	    } else if ( particleName == "oxygen" ) {
	    manager->AddDiscreteProcess(new LeptsIonisation("oxygen_LeptsIonisation"));
      
	    } else if ( particleName == "iron" ) {
	    manager->AddDiscreteProcess(new LeptsIonisation("iron_LeptsIonisation"));
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




void LeptsSMFPhysics::ConstructProcess()
{

  aParticleIterator->reset();

  while( (*aParticleIterator)() ){
    G4ParticleDefinition * particle = aParticleIterator->value();
    G4String               particleName = particle->GetParticleName();
    G4ProcessManager     * manager = particle->GetProcessManager();
    //G4cout << "particleIterator (" << particle->GetParticleName() << ") " G4endl;

    if(particleName=="e-") {
      LeptsElastic * elastic = new LeptsElastic("e-_LeptsElasticSMF");
      elastic->SetEmModel( new LeptsElasticSMFModel);
      manager->AddDiscreteProcess( elastic);
      
      LeptsIonisation * ioni = new LeptsIonisation("e-_LeptsIonisationSMF");
      ioni->SetEmModel( new LeptsIonisationSMFModel);
      manager->AddDiscreteProcess( ioni);

      LeptsElectronicExcitation * excit = new LeptsElectronicExcitation("e-_LeptsElectronicExcitationSMF");
      excit->SetEmModel( new LeptsElectronicExcitationSMFModel);
      manager->AddDiscreteProcess( excit);

      LeptsDissociation * dissoc = new LeptsDissociation("e-_LeptsDissocNeutr");
      dissoc->SetEmModel( new LeptsDissociationModel);
      manager->AddDiscreteProcess(dissoc);

      LeptsVibrationalExcitation * vib = new LeptsVibrationalExcitation("e-_LeptsExcitVibratSMF");
      vib->SetEmModel( new LeptsVibrationalExcitationSMFModel);
      manager->AddDiscreteProcess( vib);

      LeptsRotationalExcitation * rot = new LeptsRotationalExcitation("e-_LeptsExcitRotatSMF");
      rot->SetEmModel( new LeptsRotationalExcitationSMFModel);
      manager->AddDiscreteProcess( rot);

      LeptsAttachment * attach = new LeptsAttachment("e-_LeptsAttachment");
      attach->SetEmModel( new LeptsAttachmentModel);
      manager->AddDiscreteProcess( attach);

      LeptsThermalisation * therm = new LeptsThermalisation("e-_LeptsThermalisation");
      therm->SetEmModel( new LeptsThermalisationModel);
      manager->AddDiscreteProcess( therm);
      
      LeptsElastic * userproc = new LeptsElastic("e-_LeptsUserProc");
      userproc->SetEmModel( new LeptsUserModel);
      manager->AddDiscreteProcess( userproc);
    }
    
    else if(particleName=="e+") {
      LeptsElastic * elastic = new LeptsElastic("e+_LeptsElastic");
      elastic->SetEmModel( new LeptsElasticModel);
      manager->AddDiscreteProcess( elastic);

      LeptsIonisation * ioni = new LeptsIonisation("e+_LeptsIonisation");
      ioni->SetEmModel( new LeptsIonisationModel);
      manager->AddDiscreteProcess( ioni);

      LeptsElectronicExcitation * excit = new LeptsElectronicExcitation("e+_LeptsElectronicExcitation");
      excit->SetEmModel( new LeptsElectronicExcitationModel);
      manager->AddDiscreteProcess( excit);

      LeptsDissociation * dissoc = new LeptsDissociation("e+_LeptsDissocNeutr");
      dissoc->SetEmModel( new LeptsDissociationModel);
      manager->AddDiscreteProcess( dissoc);

      LeptsVibrationalExcitation * vibExcit = new LeptsVibrationalExcitation("e+_LeptsExcitVibrat");
      vibExcit->SetEmModel( new LeptsVibrationalExcitationModel);
      manager->AddDiscreteProcess( vibExcit);

      LeptsRotationalExcitation * rot = new LeptsRotationalExcitation("e+_LeptsExcitRotat");
      rot->SetEmModel( new LeptsRotationalExcitationModel);
      manager->AddDiscreteProcess( rot);

      LeptsPositronium * positr = new LeptsPositronium("e+_LeptsPositronium");
      positr->SetEmModel( new LeptsPositroniumModel);
      manager->AddDiscreteProcess( positr);

      LeptsAnnihilation * annih = new LeptsAnnihilation("e+_LeptsAnnihilation");
      annih->SetEmModel( new LeptsAnnihilationModel );
      manager->AddDiscreteProcess( annih);
    }
    
    else if ( particleName == "proton" ) {
      LeptsElastic * elast = new LeptsElastic("proton_LeptsElastic");
      elast->SetEmModel( new LeptsElasticModel);
      manager->AddDiscreteProcess( elast);

      LeptsElastic * excit = new LeptsElastic("proton_LeptsElectronicExcitation");
      excit->SetEmModel( new LeptsElectronicExcitationIonModel );
      manager->AddDiscreteProcess( excit );

      LeptsElastic * ionis = new LeptsElastic("proton_LeptsIonisation");
      ionis->SetEmModel( new LeptsIonisationIonModel );
      manager->AddDiscreteProcess( ionis);

      LeptsElastic * chdec = new LeptsElastic("proton_LeptsChargeDecrease");
      chdec->SetEmModel( new LeptsChargeTransferModel);
      manager->AddDiscreteProcess( chdec);
    }
    else if (particleName == "hydrogen") {
      LeptsElastic * elast = new LeptsElastic("hydrogen_LeptsElastic");
      elast->SetEmModel( new LeptsElasticModel);
      manager->AddDiscreteProcess( elast);

      LeptsElastic * excit = new LeptsElastic("hydrogen_LeptsElectronicExcitation");
      excit->SetEmModel( new LeptsElectronicExcitationIonModel);
      manager->AddDiscreteProcess( excit);

      LeptsElastic * ionis = new LeptsElastic("hydrogen_LeptsIonisation");
      ionis->SetEmModel( new LeptsIonisationIonModel);
      manager->AddDiscreteProcess( ionis);

      LeptsElastic * chinc = new LeptsElastic("hydrogen_LeptsChargeIncrease");
      chinc->SetEmModel( new LeptsChargeTransferModel );
      manager->AddDiscreteProcess( chinc);
    } 
	    
      /*    } else if ( particleName == "proton" ) {
	    manager->AddDiscreteProcess(new LeptsExcitation("proton_LeptsExcitation"));
	    manager->AddDiscreteProcess(new LeptsIonisation("proton_LeptsIonisation"));
	    manager->AddDiscreteProcess(new LeptsChargeDecrease("proton_LeptsChargeDecrease"));

	    } else if ( particleName == "hydrogen" ) {
	    manager->AddDiscreteProcess(new LeptsExcitation("hydrogen_LeptsExcitation"));
	    manager->AddDiscreteProcess(new LeptsIonisation("hydrogen_LeptsIonisation"));
	    manager->AddDiscreteProcess(new LeptsChargeIncrease("hydrogen_LeptsChargeIncrease"));

	    } else if ( particleName == "alpha" ) {
	    manager->AddDiscreteProcess(new LeptsExcitation("alpha_LeptsExcitation"));
	    manager->AddDiscreteProcess(new LeptsIonisation("alpha_LeptsIonisation"));
	    manager->AddDiscreteProcess(new LeptsChargeDecrease("alpha_LeptsChargeDecrease"));

	    } else if ( particleName == "alpha+" ) {
	    manager->AddDiscreteProcess(new LeptsExcitation("alpha+_LeptsExcitation"));
	    manager->AddDiscreteProcess(new LeptsIonisation("alpha+_LeptsIonisation"));
	    manager->AddDiscreteProcess(new LeptsChargeDecrease("alpha+_LeptsChargeDecrease"));
	    manager->AddDiscreteProcess(new LeptsChargeIncrease("alpha+_LeptsChargeIncrease"));

	    } else if ( particleName == "helium" ) {
	    manager->AddDiscreteProcess(new LeptsExcitation("helium_LeptsExcitation"));
	    manager->AddDiscreteProcess(new LeptsIonisation("helium_LeptsIonisation"));
	    manager->AddDiscreteProcess(new LeptsChargeIncrease("helium_LeptsChargeIncrease"));
    
	    // Extension to HZE proposed by Z. Francis
      
	    } else if ( particleName == "carbon" ) {
	    manager->AddDiscreteProcess(new LeptsIonisation("carbon_LeptsIonisation"));
      
	    } else if ( particleName == "nitrogen" ) {
	    manager->AddDiscreteProcess(new LeptsIonisation("nitrogen_LeptsIonisation"));

	    } else if ( particleName == "oxygen" ) {
	    manager->AddDiscreteProcess(new LeptsIonisation("oxygen_LeptsIonisation"));
      
	    } else if ( particleName == "iron" ) {
	    manager->AddDiscreteProcess(new LeptsIonisation("iron_LeptsIonisation"));
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
