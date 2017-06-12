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
#ifndef G4VLEPTSModel_hh
#define G4VLEPTSModel_hh

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
using namespace std;


#include "G4LEPTSDiffXS.hh"
#include "G4LEPTSDistribution.hh"
#include "G4LEPTSElossDistr.hh"
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

enum XSType {XSEnergy=0,        XSElastic=1,        XSRotation=2,
	     XSIonisation=3,    XSExcitation=4,     XSDissociation=5, XSVibration=6,
	     XSAttachment=7,    XSThermalisation=8,
	     XSPositronium=9,   XSAnnihilation=10,
	     XSChargeTransfer=11
};

		       
class G4VLEPTSModel : public G4VEmModel
{

public: 
  
  G4VLEPTSModel(const G4String& processName);
  ~G4VLEPTSModel();
  
  void BuildPhysicsTable(const G4ParticleDefinition& aParticleType); //G4ParticleWithCuts::SetCut()
  G4double GetMeanFreePath(const G4Material* mate,
			   const G4ParticleDefinition* aParticle,
			   G4double kineticEnergy );
  
  G4ThreeVector SampleNewDirection(const G4Material* aMaterial, G4ThreeVector Dir, G4double e, G4double el);
  //  G4double SampleAngle(const G4Material* aMaterial, G4double e, G4double el);
  G4ThreeVector SampleNewDirection(G4ThreeVector Dir, G4double ang);
  
  G4VLEPTSModel& operator=(const G4VLEPTSModel &right); //hide assignment operator
  G4VLEPTSModel(const G4VLEPTSModel& );


  
protected:
  void Init();
  G4bool ReadMasterFile(G4String, G4String, const G4Material*,
			string&, string&, string&, string&, string&, string&, string&);
  bool GetValues(string, string, string,
		 string&, string&, string&, string&, string&, string&, string&,
		 double&, double&, double&);

  std::map<G4int,std::vector<G4double> > ReadIXS(G4String, const G4Material* );
  std::map<G4int,std::vector<G4double> > ReadIXS(G4String, const G4Material*, G4String );
  void BuildMeanFreePathTable( const G4Material* aMaterial, std::map< G4int, std::vector<G4double> >& integralXS );
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
  
  std::map<const G4Material*, G4LEPTSDiffXS*> theDiffXS;
  std::map<const G4Material*, G4LEPTSDiffXS*> theDiffXSROT; //new
  std::map<const G4Material*, G4LEPTSDiffXS*> theDiffXSSEC; //new
  std::map<const G4Material*, G4LEPTSDistribution*> theRMTDistr;

  std::map<const G4Material*, G4LEPTSElossDistr*> theElostDistr;
  std::map<const G4Material*, G4LEPTSElossDistr*> theElostDistrVib;

  std::map<const G4Material*, G4int> theNXSdat;
  std::map<const G4Material*, G4int> theNXSsub;
   


  G4bool isInitialised;
  XSType theXSType;
  G4String theType;
  G4int verboseLevel;

};


#endif // G4VLEPTSModel_hh
