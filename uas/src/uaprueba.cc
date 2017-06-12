#include "uactions.hh"

GmAnalysisMgr* theAnaMgr;
G4double theEDepo;

TH1D * hgEdX1, * hgEdXax1;

//-----------------------------------------------------------------------
UserActionPrueba::UserActionPrueba() : GmUserRunAction(),
				     GmUserEventAction(),
				     GmUserSteppingAction() {
  G4cout << "UserActionPrueba Destructor" << G4endl;
}

UserActionPrueba::~UserActionPrueba() {
  G4cout << "UserActionPrueba Destructor" << G4endl;
}

//-----------------------------------------------------------------------
void UserActionPrueba::BeginOfRunAction( const G4Run* ) {
  G4cout << "UserActionPrueba BeginOfRunAction" << G4endl;
  G4double hisMax = GmParameterMgr::GetInstance()->GetNumericValue("UserActionPrueba:HisMax",10.);

  theAnaMgr = GmAnalysisMgr::GetInstance("Edep");
  //theAnaMgr->CreateHisto1D("Eddep",100,0,hisMax,1234);
  theAnaMgr->CreateHisto1D("Eddep",100,0,10.,1234);
  hgEdX1   = new TH1D("EdX",   "Edep(eV) - Depth(cm)",      200, -100, 100);
  hgEdXax1 = new TH1D("EdXax", "Edep(eV) - Depth(cm) Axis", 200, -100, 100);
} 
void UserActionPrueba::EndOfRunAction( const G4Run* )
{
  G4cout << "UserActionPrueba EndOfRunAction" << G4endl;
  TFile * f = new TFile("hprueba.root", "RECREATE");
  hgEdX1->Write();
  hgEdXax1->Write();
  f->Close();
} 

//-----------------------------------------------------------------------
void UserActionPrueba::BeginOfEventAction( const G4Event* )
{
  theEDepo = 0.;
}

//-----------------------------------------------------------------------
void UserActionPrueba::EndOfEventAction( const G4Event* evt )
{
  theAnaMgr->GetHisto1(1234)->Fill(theEDepo);
  //G4cout << "Event ID " << evt->GetEventID() << " EDepo = " << theEDepo << G4endl;

}


//-----------------------------------------------------------------------
void UserActionPrueba::UserSteppingAction(const G4Step* aStep)
{
  G4double      Ed       = aStep->GetTotalEnergyDeposit();
  G4StepPoint * prePoint = aStep->GetPreStepPoint();
  G4ThreeVector r        = prePoint->GetPosition();
  //G4String      matName  = prePoint->GetMaterial()->GetName;

  theEDepo += Ed/MeV;
  
  if(Ed>0) {
    hgEdX1->Fill(r[0]/cm, Ed/eV);

    static G4double h = 0.5*cm;
    static G4double l = -0.5*cm;
    if( r[1] > l && r[1] < h && r[2] > l && r[2] < h ) {
      hgEdXax1->Fill(r[0]/cm, Ed/eV);
      //G4cout << "xxxAction   " << r << " Ed " << Ed/eV << G4endl;
    }
  }

  //G4cout << "Histogram UserSteppingAction xx " << xx << " " << Ed << G4endl;

  // const G4VProcess* proc = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  // if( proc == 0 ) { 
  //   G4cerr << " proc 0 " << G4endl;
  //   return;
  // }
  // G4String procName = proc->GetProcessName();
  // if(procName.find("Ioni") != std::string::npos &&
  //    aStep->GetTrack()->GetVolume()->GetName() == "Crystal" ) {
  //   theEDepo += aStep->GetTotalEnergyDeposit();
  // }

}

