#include "uactions.hh"

TH1D * hEdSp;


UADepoSpheric::UADepoSpheric() : GmUserRunAction(), GmUserEventAction(), GmUserSteppingAction() {
  G4cout << "UADepoSpheric Constructor" << G4endl;
}

UADepoSpheric::~UADepoSpheric() {
  G4cout << "UADepoSpheric Destructor" << G4endl;
}


void UADepoSpheric::BeginOfRunAction( const G4Run* ) {
  G4cout << "UADepoSpheric BeginOfRunAction" << G4endl;

  hEdSp = new TH1D("Edep",  "Edep(eV)",  100, 0.0, 100);
} 


void UADepoSpheric::EndOfRunAction( const G4Run* ) {

  TH1D * hDs = (TH1D*)hEdSp->Clone("Dose");
  hDs->SetTitle("Dose spheric slab");
  
  for (int i=1; i <= hEdSp->GetNbinsX(); i++) {
    double r = (hEdSp->GetBinCenter(i) + hEdSp->GetBinCenter(i-1) )/2.0;
    double R = (hEdSp->GetBinCenter(i) + hEdSp->GetBinCenter(i+1) )/2.0;
    double mass = (1*g/cm3)*4.0/3.0*3.14159265359*(pow(R,3)-pow(r,3));
    double dose = hEdSp->GetBinContent(i)/mass;
    hDs->SetBinContent(i, dose);
    cout << "rreedd " << hEdSp->GetBinCenter(i) << " " << hEdSp->GetBinContent(i)/eV
	 << " " << dose/(joule/kg) << endl;
  }
    
  G4String JobName = GmParameterMgr::GetInstance()->GetStringValue("JobName", "job");
  G4String fn = "sph" + JobName + ".root";
  TFile * fsp = new TFile(fn.c_str(), "RECREATE");
  hEdSp->Write();
  hDs->Write();
  fsp->Close();
}

void UADepoSpheric::BeginOfEventAction( const G4Event* ) {

}


void UADepoSpheric::EndOfEventAction( const G4Event* evt ) {
  //G4cout << "Event ID " << evt->GetEventID()
  // << " EDepo = " << theEDepo << G4endl;
}



void UADepoSpheric::UserSteppingAction(const G4Step* aStep) {
  G4ThreeVector r = aStep->GetPostStepPoint()->GetPosition();
  G4double     Ed = aStep->GetTotalEnergyDeposit();
  
  hEdSp->Fill(r.mag(), Ed);
}

