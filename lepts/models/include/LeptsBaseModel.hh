
#ifndef LeptsBaseModel_hh
#define LeptsBaseModel_hh

#include "G4ios.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4VEmModel.hh"
#include "G4PhysicsTable.hh"
#include "G4PhysicsLogVector.hh"
#include "G4Element.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"
#include "G4Gamma.hh"
#include "G4DNAGenericIonsManager.hh"
#include "G4Step.hh"

#include "G4EnergyLossTables.hh"
#include "G4UnitsTable.hh"
#include <iostream> 
#include "G4ParticleChangeForGamma.hh"

using namespace std;


#include "LeptsDiffXS.hh"
#include "LeptsDistribution.hh"
#include "LeptsElossDistribution.hh"
#include "G4ForceCondition.hh"
#include <map>
#include <vector> 


#include <CLHEP/Units/SystemOfUnits.h>
using namespace CLHEP;
// enum XSType {XSEnergy=0, 
// 	     XSTotal=1,        XSElastic=2,     XSInelastic=3,
// 	     XSIonisation=4,   XSExcitation=5,  XSDissociation=6, XSVibration=7,
// 	     XSAttachment=8,   XSPositronium=8, XSChargeTransfer=8,
// 	     XSAnnihilation=9, XSRotation=9
// };

enum XSType {
  XSEnergy,
  XSElastic,
  XSRotation,
  XSExcitation,
  XSDissociation,
  XSVibration,
  XSAttachment,
  XSThermalisation,
  XSPositronium,
  XSAnnihilation,
  XSChargeTransfer,
  XSIonisation, XSIonisationS, XSIonisationI, XSIonisationD,
  XSUserProc
};

		       
class LeptsBaseModel : public G4VEmModel {

public: 
  
  LeptsBaseModel(const G4String& processName);
  ~LeptsBaseModel();

  virtual void Initialise(const G4ParticleDefinition*, const G4DataVector&);

  void BuildPhysicsTable(const G4ParticleDefinition& aParticleType);
  //G4ParticleWithCuts::SetCut()
  G4double GetMeanFreePath(const G4Material* mate,
			   const G4ParticleDefinition* aParticle,
			   G4double kineticEnergy );
  
  G4ThreeVector SampleNewDirection(const G4Material* aMaterial,
				   G4ThreeVector Dir, G4double e, G4double el);
  G4double SampleAngle(const G4Material* aMaterial, G4double e, G4double el);
  G4double SampleAngle(const G4Material* aMaterial, G4double e);
  G4ThreeVector SampleNewDirection(G4ThreeVector Dir, G4double ang);
  
  LeptsBaseModel& operator=(const LeptsBaseModel &right); //hide assignment operator
  LeptsBaseModel(const LeptsBaseModel& );

  // main method to compute cross section per Volume
  virtual G4double CrossSectionPerVolume(const G4Material*, const G4ParticleDefinition*,
                                         G4double kineticEnergy, G4double cutEnergy = 0.0,
                                         G4double maxEnergy = DBL_MAX);


  
protected:
  void Init();
  G4bool ReadMasterFile(G4String, G4String, const G4Material*,
			string&, string&, string&, string&, string&);
  bool GetValues(string, string, string,
		 string&, string&, string&, string&, string&,
		 double&, double&, double&);
  bool bIXS, bDXS, bDXSMT, bDXSSEC, bELD, bUSER;
    
  std::map<G4int,std::vector<G4double> > ReadIXS(G4String, const G4Material* );
  std::map<G4int,std::vector<G4double> > ReadIXS(G4String, const G4Material*, G4String );
  void BuildMeanFreePathTable(const G4Material* aMaterial, std::map< G4int,
			      std::vector<G4double> >& integralXS );
  double ElasticAngle(G4double, G4double, G4double, G4double);
  G4double ElasticEnergyTransfer(G4double, G4double, G4double, G4double);
  G4double FractionET(G4double, G4double, G4double);
  void InitMassArrays(const G4String);
  double AdjustAngle(double, double, double, double, double);

  
protected:
  G4PhysicsTable * theMeanFreePathTable;
  
  G4double theLowestEnergyLimit;
  G4double theHighestEnergyLimit;
  G4int    theNumbBinTable;

  std::map<const G4Material*, G4double > theIonisPot;
  std::map<const G4Material*, G4double > theIonisPotInt;
  std::map<const G4Material*, G4double > theIonisPotInt2;
  std::map<const G4Material*, G4double > theMolecularMass;

  std::map<const G4Material*, G4double> theMassTarget;  //M*c2
  std::map<const G4Material*, G4double> theMassProjectile;  //M*2
  
  std::map<const G4Material*, LeptsDiffXS*> theDiffXS;
  std::map<const G4Material*, LeptsDiffXS*> theDiffXSROT;
  std::map<const G4Material*, LeptsDiffXS*> theDiffXSSEC;
  //std::map<const G4Material*, LeptsDistribution*> theRMTDistribution;
  std::map<const G4Material*, LeptsDiffXS*> theDiffXSMT;
  
  std::map<const G4Material*, LeptsElossDistribution*> theElostDistribution;

  std::map<const G4Material*, G4int> theNXSdat;
  std::map<const G4Material*, G4int> theNXSsub;
   

  XSType theXSType;
  G4String theType;
  G4int verboseLevel;

  G4ParticleChangeForGamma * fParticleChange;
  G4double         LowestElectronicExcitationEnergy;
  G4double         LowestNeutralDisociationEnergy;

};


#endif // LeptsBaseModel_hh
