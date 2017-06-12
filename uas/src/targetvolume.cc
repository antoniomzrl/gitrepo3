#include "points.hh"
#include "uactions.hh"
#include "G4Gamma.hh"
#include "G4RunManager.hh"
#include "G4TransportationManager.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

UATargetVolume::~UATargetVolume() {
  G4cout << "UATargetVolume Destructor" << G4endl;
}


UATargetVolume::UATargetVolume() {
  G4cout << "UATargetVolume Constructor" << G4endl;
}


void UATargetVolume::BeginOfRunAction( const G4Run* ) {
  G4cout << theName + " BeginOfRunAction" << G4endl;

  PhotCount=0;
  
  targetName = GmParameterMgr::GetInstance()->GetVStringValue(theName+":TargetName", targetName);
  NoPhotInside = GmParameterMgr::GetInstance()->GetNumericValue(theName+":NoPhotInside", 10);
  
  if( targetName.size() == 0 ) {
    G4Exception("SelectVolume ", " xxx", FatalException,
		("No touchable selected in " + theName).c_str());
  }

  G4cout << "UATargetVolume " << NoPhotInside << endl;
  
  std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( "*" );
  for( size_t it = 0; it < touchs.size(); it++) {
    G4cout << "  Touchable: " << touchs[it]->GetName() << "\t" << touchs[it]->GetLongName();
    if( touchs[it]->GetName() == targetName[0] ) {
      theTarget = (G4VSolid *)touchs[it]->GetSolid();
      double targetVolume    = theTarget->GetCubicVolume();
      theTargetPosition = touchs[it]->GetGlobalPosition();
      G4cout << "\t<-- target at  " << theTargetPosition << " " << targetVolume << " (mm3)";
    }
    G4cout << G4endl;
  }

  theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
}



void UATargetVolume::EndOfRunAction( const G4Run* ) {
  G4cout << theName + " EndOfRunAction" << G4endl;
}


// Max Reach (mm) vs Energy (MeV)
double MaxReach(double E )  {
  static double e[10] = {  0   , 0.1e-3,   1e-3,   3e-3,  10e-3, 30e-3, 0.1   , 0.3, 1  , 10 };
  static double r[10] = { 20e-6,  20e-6, 130e-6, 0.6e-3, 4.2e-3, 26e-3, 180e-3, 1  , 4.6, 55 };

  double mreach = DBL_MAX;
  
  for(int i=0; i<9; i++) {
    if( E >= e[i] && E <= e[i+1] ) {
      mreach = r[i] + (r[i+1] - r[i]) * (E-e[i]) / (e[i+1] - e[i]);
    }
  }

  return mreach*1.1;
  
  // cout << "mmmm " << i << " " << r[i]<< " " << r[i+1]<< " " << e[i]<< " " << e[i+1]<< endl;
  // for(double e=0; e<6*MeV; e+=1*keV)
  //   cout << "eeaa " << e/MeV << " " << MaxReach(e)/mm << endl;
  // exit(-1);
}

     

G4ClassificationOfNewTrack UATargetVolume::ClassifyNewTrack(const G4Track* aTrack ) {
  static G4ParticleDefinition * gammaDef = G4Gamma::GammaDefinition();
  //G4String partName = aTrack->GetDefinition()->GetParticleName();

  if( PhotCount > NoPhotInside) {
    PhotCount--;
    cout << "ClassifyNewTrack: NoPhotInside (" << PhotCount << ") reached" << endl;
    G4RunManager::GetRunManager()->AbortRun();
  }

  if( aTrack->GetDefinition() == gammaDef) {
    return fUrgent;
  }

  double dist = theTarget->DistanceToIn(aTrack->GetPosition() - theTargetPosition);
  
  if( dist > MaxReach(aTrack->GetKineticEnergy() ) ) {
    //cout << "classifyfKill " << aTrack->GetKineticEnergy()/eV << " " << dist << " "
    //	 << MaxReach(aTrack->GetKineticEnergy()) << " " << aTrack->GetPosition() << endl;
    return fKill;
  }
  
  if( dist < 1e-9 ) {
    const G4VProcess * pro = aTrack->GetCreatorProcess();;
    G4String procName = "null";
    if(pro ) procName = pro->GetProcessName();
    if(procName == "phot")
      PhotCount++;
  }

  //cout << "classifyfUrgent " << aTrack->GetKineticEnergy()/eV << " " << dist << " "
  //   << MaxReach(aTrack->GetKineticEnergy()) << " " << aTrack->GetPosition() << endl;
  return fUrgent;
}



// G4ClassificationOfNewTrack UATargetVolume::ClassifyNewTrack(const G4Track* aTrack ) {
//   static G4ParticleDefinition * gammaDef = G4Gamma::GammaDefinition();

//   double d = theTarget->DistanceToIn(aTrack->GetPosition() - theTargetPosition);
//   //G4cout << "classify111\t" << d << " " << aTrack->GetPosition() << G4endl;

//   if( d == 0 )
//     G4cout << "classifyiii\t" << d << " " << aTrack->GetPosition() << G4endl;
//   else if( d > 10 && d < 15)
//     G4cout << "classifynnn\t" << d << " " << aTrack->GetPosition() << G4endl;
    
//   return fUrgent;
// }
