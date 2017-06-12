#include "uactions.hh"
#include "points.hh"

extern vector <string> tpi, ntpi, pn;

#define NTPI 50
TH1D *hEl[NTPI], *hEd[NTPI];


UAEloss::UAEloss() : GmUserRunAction(), GmUserEventAction(),
				     GmUserSteppingAction() {
  G4cout << "UAEloss Constructor" << G4endl;
}


UAEloss::~UAEloss() {
  G4cout << "UAEloss Destructor" << G4endl;
}


void UAEloss::BeginOfRunAction( const G4Run* ) {
  G4cout << "UAEloss BeginOfRunAction" << G4endl;

  string TpiFn = GmParameterMgr::GetInstance()->GetStringValue(theName + ":TpiFile", "tpi_all.txt");
  string Title = GmParameterMgr::GetInstance()->GetStringValue(theName + ":Title", " MeV");
  double Nbins = GmParameterMgr::GetInstance()->GetNumericValue(theName + ":Nbins", 1000);
  double Emin  = GmParameterMgr::GetInstance()->GetNumericValue(theName + ":Emin", 0);
  double Emax  = GmParameterMgr::GetInstance()->GetNumericValue(theName + ":Emax", 5000);

  char * path = getenv("LCVPATH");
  if( path ) TpiFn = string(path) + string("/") + TpiFn;
  InitTpi(TpiFn);
  
  string name, title;
  for(int j=0; j<tpi.size(); j++) {
    name     = "EL_" + tpi[j];
    title    = name + " - E Lost - " + Title;
    hEl[j] = new TH1D(name.c_str(), title.c_str(), Nbins, Emin, Emax);    
    name     = "ED_" + tpi[j];
    title    = name + " - E Dep - " + Title;
    hEd[j] = new TH1D(name.c_str(), title.c_str(), Nbins, Emin, Emax);    
  }

  hEl[tpi.size()] = new TH1D("Elost Total", "E Lost Total", Nbins, Emin, Emax);
  hEd[tpi.size()] = new TH1D("Edep Total", "E Dep Total", Nbins, Emin, Emax);
} 


void UAEloss::EndOfRunAction( const G4Run* ) {
  G4cout << "UAEloss EndOfRunAction" << G4endl;

  G4String JobName = GmParameterMgr::GetInstance()->GetStringValue("JobName", "job");
  G4String fn = "hgeld_" + JobName + ".root";
  
  TFile * Fintsp = new TFile(fn.c_str(), "RECREATE");

  for(int j=0; j<=tpi.size(); j++) {
    if(hEl[j]->GetEntries() > 0) hEl[j]->Write();
  }
  for(int j=0; j<=tpi.size(); j++) {
    if(hEd[j]->GetEntries() > 0) hEd[j]->Write();
  }
  
  Fintsp->Close();
} 


void UAEloss::BeginOfEventAction( const G4Event* ) {
  //theEDepo1 = 0.;
}


void UAEloss::EndOfEventAction( const G4Event* evt ) {
  //G4cout << "Event ID " << evt->GetEventID()
  // << " EDepo = " << theEDepo << G4endl;
}



void UAEloss::UserSteppingAction(const G4Step* aStep) {
  G4StepPoint * Pt1 = aStep->GetPreStepPoint();
  G4StepPoint * Pt2 = aStep->GetPostStepPoint();
  double El = Pt1->GetKineticEnergy() - Pt2->GetKineticEnergy();
  double Ed = aStep->GetTotalEnergyDeposit();

  // process Edep / Depth
  const G4VProcess * proc = Pt2->GetProcessDefinedStep();
  if(proc) {
    G4String procName = proc->GetProcessName();
    int it = TPI(procName);
    hEl[it]->Fill(El/eV);
    hEl[tpi.size()]->Fill(El/eV);
    hEd[it]->Fill(Ed/eV);
    hEd[tpi.size()]->Fill(Ed/eV);
  }
 
}
