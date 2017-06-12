#include "uactions.hh"
#include "G4RunManager.hh"

int RealNoPrimaries, NoEvent, NoReflected;

UAReflex::UAReflex() : GmUserRunAction(), GmUserEventAction(),  GmUserSteppingAction() {
  cout << "UAReflex Constructor" << endl;
}


UAReflex::~UAReflex() {
  cout << "UAReflex Destructor" << endl;
}


void UAReflex::BeginOfRunAction( const G4Run* ) {
  cout << "UAReflex BeginOfRun" << endl;
  NoEvent = 0;
  NoReflected = 0;
  RealNoPrimaries = GmParameterMgr::GetInstance()->GetNumericValue("UAReflex:RealNoPrimaries", 22);
}


void UAReflex::EndOfRunAction( const G4Run* ) {
  cout << "UAReflex EndOfRun RealPrim " << RealNoPrimaries
       << " Reflect " << NoReflected << " NoEv " << NoEvent << endl;
}


void UAReflex::BeginOfEventAction( const G4Event* evt) {
  NoEvent++;
}


void UAReflex::EndOfEventAction( const G4Event* evt) {
  
  if( NoEvent >= (RealNoPrimaries+NoReflected) ) {
    cout << "Ending Run" << endl;
    G4RunManager::GetRunManager()->AbortRun();
  }
}


void UAReflex::UserSteppingAction(const G4Step* aStep) {

  // G4StepPoint * Pt1  = aStep->GetPreStepPoint();
  // G4VPhysicalVolume * pv1 = Pt1->GetPhysicalVolume();
  // G4String phvlName1 = "null";
  // if(pv1 ) phvlName1 = pv1->GetName();

  G4StepPoint * Pt  = aStep->GetPostStepPoint();
  G4VPhysicalVolume * pv = Pt->GetPhysicalVolume();
  G4String phvlName = "null";
  if(pv ) phvlName = pv->GetName();

  // continue only if exit world
  if( phvlName != "null" ) return;

  
  G4ThreeVector Dir = Pt->GetMomentumDirection();

  //G4ThreeVector m2 = Pt2->GetMomentum();
  //G4ThreeVector r2 = Pt2->GetPosition();
  //G4cout << "Stepping: (" << NoEvent << ") " <<  phvlName1 << " -> " << phvlName2
  //	 << " Mom: " << m2 << " Pos: " << r2 << " Dir: " << Dir << endl;

  if( Dir.x() < 0 ) {
    NoReflected++;
  }
}

