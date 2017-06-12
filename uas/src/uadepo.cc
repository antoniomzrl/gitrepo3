#include "uactions.hh"

TH1D * hgEd, * hgEl;
double unit2 = 1*mm;

int    NoEv = 0, NoEvI;
double Edep = 0, Elost = 0;


UADepo::UADepo() : GmUserRunAction(), GmUserEventAction(), GmUserSteppingAction() {
  G4cout << "UADepo Constructor" << G4endl;
}

UADepo::~UADepo() {
  G4cout << "UADepo Destructor" << G4endl;
}


void UADepo::BeginOfRunAction( const G4Run* ) {
  G4cout << "UADepo BeginOfRunAction" << G4endl;

  vector<double> v(3,1);
  vector<double> x = GmParameterMgr::GetInstance()->GetVNumericValue("SPhg:x", v);
  vector<double> y = GmParameterMgr::GetInstance()->GetVNumericValue("SPhg:y", v);
  vector<double> z = GmParameterMgr::GetInstance()->GetVNumericValue("SPhg:z", v);

  G4String Units = GmParameterMgr::GetInstance()->GetStringValue("SPhg:Units", "mm");
  if(Units == (G4String)"um") unit2 = 1*um;

  cout << "x " << x[0] << " " << x[1] << " " << x[2] << endl
       << "y " << y[0] << " " << y[1] << " " << y[2] << endl
       << "z " << z[0] << " " << z[1] << " " << z[2] << endl;


  string ti1 = "Edep(eV) - Depth(" + Units + ")";
  string ti2 = "Edep(eV) - Depth(" + Units + ") Axis";
  string ti3 = "Edep(eV) - xyz(" + Units + ")";

  hgEd = new TH1D("Edepo",  "Edep(eV)",  1000, 0.0, 1000);
  hgEl = new TH1D("Eloss", "Eloss(eV)", 1000, 0.0, 1000);
} 


void UADepo::EndOfRunAction( const G4Run* ) {

  double D = GmParameterMgr::GetInstance()->GetNumericValue("STPW:D",1.0e-6*g/cm3);
  double L = GmParameterMgr::GetInstance()->GetNumericValue("STPW:L",100.0*um);

  double avgEdep  = Edep/NoEv;
  double avgElost = Elost/NoEv;
  double avgEdepI = Edep/NoEvI;
  double SP  = avgEdep/D/L;
  double SP2 = avgElost/D/L;

  cout << "UADepo EndOfRunAction"   << endl
       << "\tNoEv:     " << NoEv    << endl
       << "\tNoEvI:    " << NoEvI   << endl
       << "\tEdep(eV): " << Edep/eV << endl
       << "\tAvgEdep:  " << avgEdep/eV     << endl
       << "\tAvgEdepI: " << avgEdepI/eV    << endl
       << "\tSP(Ed):   " << SP/(MeV*cm2/g) << " MeV*cm2/g SP(El): " << SP2/(MeV*cm2/g)
       << endl;

  G4String JobName = GmParameterMgr::GetInstance()->GetStringValue("JobName", "job");
  G4String fn = "hdepo" + JobName + ".root";

  TFile * f2 = new TFile(fn.c_str(), "RECREATE");
  hgEd->Write();
  hgEl->Write();
  f2->Close();
} 


void UADepo::BeginOfEventAction( const G4Event* ) {
  NoEv++;
}


void UADepo::EndOfEventAction( const G4Event* evt ) {
  //G4cout << "Event ID " << evt->GetEventID()
  // << " EDepo = " << theEDepo << G4endl;
}



void UADepo::UserSteppingAction(const G4Step* aStep) {

  G4StepPoint * Pt1 = aStep->GetPreStepPoint();
  G4StepPoint * Pt2 = aStep->GetPostStepPoint();
  double El = Pt1->GetKineticEnergy() - Pt2->GetKineticEnergy();
  Elost += El;

  G4double Ed = aStep->GetTotalEnergyDeposit();
  if(Ed == 0) return;
  NoEvI++;
  Edep += Ed;

  //G4ThreeVector r = Pt1->GetPosition()/unit2;
  G4ThreeVector r = Pt2->GetPosition()/unit2;

  //cout << "Fill " << r[0] << " " << r[1] << " " << r[2] << " " << Ed << endl;

  hgEd->Fill(Ed/eV);
  hgEl->Fill(El/eV);
}

