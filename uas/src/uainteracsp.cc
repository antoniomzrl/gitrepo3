#include "uactions.hh"
#include "points.hh"

double sdx, sdy;
extern vector <string> tpi, ntpi, pn;

#define NTPI 50
TH1D *hgP, *hgNI, *hgEdxT, *hgEdxAxis, *hgEx[6], *hgEx2[6], *hgENx[6],
  *hgEdx[NTPI], *hgNx[NTPI], *hgNsx[NTPI], *hgEsx[NTPI];


UAInteractionSp::UAInteractionSp() : GmUserRunAction(), GmUserEventAction(),
				     GmUserSteppingAction() {
  G4cout << "UAInteractionSp Constructor" << G4endl;
}


UAInteractionSp::~UAInteractionSp() {
  G4cout << "UAInteractionSp Destructor" << G4endl;
}


void UAInteractionSp::BeginOfRunAction( const G4Run* ) {
  G4cout << "UAInteractionSp BeginOfRunAction" << G4endl;

  G4String Title = GmParameterMgr::GetInstance()->GetStringValue(theName + ":Title", "mm");
  string TpiFn = GmParameterMgr::GetInstance()->GetStringValue(theName + ":TpiFile", "tpi_all.txt");
  vector<double> v(3,1);
  vector<double> x = GmParameterMgr::GetInstance()->GetVNumericValue(theName + ":x", v);
  sdy = GmParameterMgr::GetInstance()->GetNumericValue(theName + ":Width", 10*m);
  sdx = (x[2]-x[1]) / x[0];
  
  cout << "UAInteractionSp dx " << G4BestUnit(2*sdx, "Length")
       << " dy " << G4BestUnit(2*sdy, "Length")
       << " volume " << G4BestUnit(8*sdx*sdy*sdy, "Volume") << endl;

  char * path = getenv("LCVPATH");
  if( path ) TpiFn = string(path) + string("/") + TpiFn;
  InitTpi(TpiFn);
  
  hgNI = new TH1D("NoInt", "Number of interactions", 3, 0, 3);
  hgP  = new TH1D("NoPrimaries", "Number of primaries", 3, 0, 3);

  string name, title;
  for(int j=0; j<tpi.size(); j++) {
    name    = "No_" + tpi[j];
    title   = name + " - Depth - " + Title;
    hgNx[j] = new TH1D(name.c_str(), title.c_str(), x[0], x[1], x[2]);
    
    name     = "Ed_" + tpi[j];
    title    = name + " - Depth - " + Title;
    hgEdx[j] = new TH1D(name.c_str(), title.c_str(), x[0], x[1], x[2]);    

    name    = "Seco_N_" + tpi[j];
    title   = name + " - Depth - " + Title;
    hgNsx[j] = new TH1D(name.c_str(), title.c_str(), x[0], x[1], x[2]);
    
    name     = "Seco_E_" + tpi[j];
    title    = name + " - Depth - " + Title;
    hgEsx[j] = new TH1D(name.c_str(), title.c_str(), x[0], x[1], x[2]);    
  }
  
  hgEdxT = new TH1D("EdepTotal", "EdepTotal", x[0], x[1], x[2]);
  hgEdxAxis = new TH1D("EdepAxis", "EdepAxis", x[0], x[1], x[2]);
 
  for(int j=0; j<6; j++) {
    string name = "E_" + pn[j];
    string title = "E" + name + " - Depth - " + Title;
    hgEx[j] = new TH1D(name.c_str(), title.c_str(), x[0], x[1], x[2]);
    name = "E2_" + pn[j];
    hgEx2[j] = new TH1D(name.c_str(), title.c_str(), x[0], x[1], x[2]);
    name = "E_aux_" + pn[j];
    title = "E_aux_" + pn[j];
    hgENx[j] = new TH1D(name.c_str(), title.c_str(), x[0], x[1], x[2]);
  }

} 


void UAInteractionSp::EndOfRunAction( const G4Run* ) {
  G4cout << "UAInteractionSp EndOfRunAction" << G4endl;

  G4String JobName = GmParameterMgr::GetInstance()->GetStringValue("JobName", "job");
  G4String fn = "hgsp_" + JobName + ".root";
  
  TFile * Fintsp = new TFile(fn.c_str(), "RECREATE");

  hgNI->LabelsOption("a", "X");
  hgNI->Write();
  hgP->LabelsOption("a", "X");
  hgP->Write();

  if(hgEdxT->GetEntries() > 0) hgEdxT->Write();
  if(hgEdxAxis->GetEntries() > 0) hgEdxAxis->Write();


  for(int j=0; j<6; j++) {
    if(hgEx[j]->GetEntries() > 0) {
      for(int i=0; i <= hgEx[j]->GetNbinsX()+1; i++) {
       	G4double vm = 0.0;
       	if(hgENx[j]->GetBinContent(i) != 0)
       	  vm = hgEx[j]->GetBinContent(i) / hgENx[j]->GetBinContent(i);
       	hgEx2[j]->SetBinContent(i, vm);
	// cout << "xxx" << pn[j]
	//      << "\t" << hgENx[j]->GetBinContent(i)/MeV
	//      << "\t" << hgEx[j]->GetBinContent(i)/MeV
	//      << "\t" << hgEx2[j]->GetBinContent(i)/MeV
	//      << endl;
      }
      //hgEx[j]->Write();
      //hgENx[j]->Write();
      hgEx2[j]->Write();
    }
  }


  for(int j=0; j<tpi.size(); j++) {
    for(int i=0; i <= hgNsx[j]->GetNbinsX()+1; i++) {
      if(hgNsx[j]->GetBinContent(i) != 0) {
	G4double vm = hgEsx[j]->GetBinContent(i) / hgNsx[j]->GetBinContent(i);
	hgEsx[j]->SetBinContent(i, vm);
      }
    }
    if(hgNsx[j]->GetEntries() > 0) hgNsx[j]->Write();
    if(hgEsx[j]->GetEntries() > 0) hgEsx[j]->Write();
  }

  for(int j=0; j<tpi.size(); j++) {
    if(hgNx[j]->GetEntries() > 0)  hgNx[j]->Write();
    if(hgEdx[j]->GetEntries() > 0) hgEdx[j]->Write();
  }

  Fintsp->Close();
} 


void UAInteractionSp::BeginOfEventAction( const G4Event* ) {
  //theEDepo1 = 0.;
  hgP->Fill( (G4String)"Primary", 1);
}


void UAInteractionSp::EndOfEventAction( const G4Event* evt ) {
  //G4cout << "Event ID " << evt->GetEventID()
  // << " EDepo = " << theEDepo << G4endl;
}



void UAInteractionSp::UserSteppingAction(const G4Step* aStep) {
 
  G4StepPoint * P2 = aStep->GetPostStepPoint();
  //G4StepPoint * P1 = aStep->GetPreStepPoint();
  //G4ThreeVector  r = (P1->GetPosition() + P2->GetPosition())*0.5;
  G4ThreeVector  r = P2->GetPosition();

  if( abs(r[1]) > sdy || abs(r[2]) > sdy) return;

  G4double      Ke = P2->GetKineticEnergy();
  G4double      Ed = aStep->GetTotalEnergyDeposit();
  

  // Particle energy
  G4String partName = aStep->GetTrack()->GetDefinition()->GetParticleName();
  int ip = PN(partName);
  hgEx[ip]->Fill(r[0], Ke);
  hgENx[ip]->Fill(r[0], 1);

  // process Edep / Depth
  const G4VProcess * proc = P2->GetProcessDefinedStep();
  if(proc) {
    G4String procName = proc->GetProcessName();
    //int it = TPI(procName, 0, 29);
    int it = TPI(procName);
    hgNx[it]->Fill(r[0]);
    hgEdx[it]->Fill(r[0], Ed);
    hgEdxT->Fill(r[0], Ed);
    hgNI->Fill(procName, 1);

    //if( abs(r[1]) < sdy && abs(r[2]) < sdy)
    //hgEdxAxis->Fill(r[0], Ed);
  }
}
