#include "points.hh"
#include "uactions.hh"
#include "G4Gamma.hh"
#include "G4RunManager.hh"
#include "G4TransportationManager.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

Clock * tclk;

UASelectVolume::~UASelectVolume() {
  G4cout << "UASelectVolume Destructor" << G4endl;
}


UASelectVolume::UASelectVolume() {
  G4cout << "UASelectVolume Constructor" << G4endl;
}


void UASelectVolume::BeginOfRunAction( const G4Run* ) {
  G4cout << theName + " BeginOfRunAction" << G4endl;

  PhotCount=0;
  tclk = new Clock();
  
  targetName = GmParameterMgr::GetInstance()->GetVStringValue(theName+":TargetName", targetName);
  targetReach = GmParameterMgr::GetInstance()->GetNumericValue(theName+":TargetReach", 0.0);
  NoPhotInside = GmParameterMgr::GetInstance()->GetNumericValue(theName+":NoPhotInside", 10);
  TimeLimit = GmParameterMgr::GetInstance()->GetNumericValue(theName+":TimeLimit", 36000000);
  
  if( targetName.size() == 0 ) {
    G4Exception("SelectVolume ", " xxx", FatalException,
		("No touchable selected in " + theName).c_str());
  }

  G4cout << "UASelectVolume " << targetReach << " " << NoPhotInside
	 << " " << TimeLimit << " sec" << endl;
  
  std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( "*" );
  for( size_t it = 0; it < touchs.size(); it++) {
    G4cout << "  Touchable: " << touchs[it]->GetName() << "\t" << touchs[it]->GetLongName();
    if( touchs[it]->GetName() == targetName[0] ) {
      theTarget = (G4VSolid *)touchs[it]->GetSolid();
      //Volume    = theTarget->GetCubicVolume();
      theTargetPosition = touchs[it]->GetGlobalPosition();
      G4cout << "\t<-- select at  " << theTargetPosition;
    }
    G4cout << G4endl;
  }

  theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
}


void UASelectVolume::EndOfRunAction( const G4Run* ) {
  G4cout << theName + " EndOfRunAction" << G4endl;
}


G4ClassificationOfNewTrack UASelectVolume::ClassifyNewTrack(const G4Track* aTrack ) {
  static G4ParticleDefinition * gammaDef = G4Gamma::GammaDefinition();
  //G4String partName = aTrack->GetDefinition()->GetParticleName();

  if( PhotCount >= NoPhotInside || tclk->Elapsed() >= TimeLimit) {
    PhotCount--;
    cout << "(AMR) Time limit reached, killing RunManager" << endl;
    G4RunManager::GetRunManager()->AbortRun();
  }

  if( aTrack->GetDefinition() == gammaDef)
    return fUrgent;
    
  if( !IsNearVolume(aTrack, targetReach) )
    return fKill;
  else {
    if( IsNearVolume(aTrack, 1e-9) ) {
      const G4VProcess * pro = aTrack->GetCreatorProcess();;
      G4String procName = "null";
      if(pro ) procName = pro->GetProcessName();
      if(procName == "phot")
	PhotCount++;
    }
    
    return fUrgent;
  }
}



bool UASelectVolume::IsNearVolume(const G4Track* aTrack, double Dist) {
  // G4ThreeVector r = aTrack->GetPosition();
  // G4VPhysicalVolume * pv = theNavigator->LocateGlobalPointAndSetup(r,
  //                          (const G4ThreeVector*)0, false, true);
  // G4String pvName = pv->GetName();
  // if( pvName == targetName[0] || pvName == targetName[1])  return true;
  // else                                                     return false;
  
  double d = theTarget->DistanceToIn(aTrack->GetPosition() - theTargetPosition);

  //if(Dist < 0.19 && d < 1.0) cout << "isnearvol " << d << " " << Dist << endl;
  
  if( d > Dist) return false;
  else          return true;
}
