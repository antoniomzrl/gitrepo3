#include "points.hh"
#include "uactions.hh"
#include "G4Gamma.hh"
#include "G4RunManager.hh"
#include "G4TransportationManager.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

void UAVolume::Init() {
  targetName = GmParameterMgr::GetInstance()->GetVStringValue(theName+":TargetName", targetName);
  targetReach = GmParameterMgr::GetInstance()->GetNumericValue(theName+":TargetReach", 0.0);

  if( targetName.size() == 0 ) {
    G4Exception("UAVolume::Init", " xxx",FatalException,
		("No touchable selected, check command: /gamos/setParam "
		 + theName + ":Name ... ").c_str());
  }
  
  std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( "*" );
  for( size_t it = 0; it < touchs.size(); it++) {
    G4cout << "Touchable: " << touchs[it]->GetName() << "\t" << touchs[it]->GetLongName();
    if( touchs[it]->GetName() == targetName[0] ) {
      theTarget = (G4VSolid *)touchs[it]->GetSolid();
      Volume    = theTarget->GetCubicVolume();
      theTargetPosition = touchs[it]->GetGlobalPosition();
      G4cout << "\t<----- select at " << theTargetPosition;
    }
    G4cout << G4endl;
  }

  theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
}



bool UAVolume::IsInsideVoxel(const G4Step* aStep) {
  //G4VPhysicalVolume * pv = theNavigator->LocateGlobalPointAndSetup(r, (const G4ThreeVector*)0, false, true);
  G4VPhysicalVolume * pv = aStep->GetPostStepPoint()->GetPhysicalVolume();
  G4String pvName;
  if( pv ) pvName = pv->GetName();
  else     pvName = "null";

  if( pvName == targetName[0] )  return true;
  else                           return false;
}



bool UAVolume::IsInsideBigVoxel(const G4Track* aTrack) {
  // G4ThreeVector r = aTrack->GetPosition();
  // G4VPhysicalVolume * pv = theNavigator->LocateGlobalPointAndSetup(r, (const G4ThreeVector*)0, false, true);
  // G4String pvName = pv->GetName();
  // if( pvName == targetName[0] || pvName == targetName[1])  return true;
  // else                                                     return false;
  
  double dist = theTarget->DistanceToIn(aTrack->GetPosition() -theTargetPosition);

  if( dist > targetReach) return false;
  else                    return true;
}
