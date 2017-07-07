#include "uactions.hh"

TH1D *hgA, *hgE, *hgEfw, *hgEfwItg, *hgEbwItg, *hgEbw, *hgEfwph, *hgEfwel,
  *hgEel20, *hgEph20, *hgEel80, *hgEph80;

UAExit::UAExit() : GmUserRunAction(), GmUserEventAction(),
			 GmUserSteppingAction() {
  cout << "UAExit Constructor" << endl;
}


UAExit::~UAExit() {
  cout << "UAExit Destructor" << endl;
}


void UAExit::BeginOfRunAction( const G4Run* ) {
  cout << "UAExit BeginOfRun" << endl;
  G4double Nb   = GmParameterMgr::GetInstance()->GetNumericValue("UAExit:EnergyBins", 1000);
  G4double EMax = GmParameterMgr::GetInstance()->GetNumericValue("UAExit:EnergyMax", 1*MeV);

  hgA     = new TH1D("Angular",        "Ang(deg)",180,  0.0, 180);
  hgE     = new TH1D("E",              "E(eV)",   Nb, 0.0, EMax);
  hgEfw   = new TH1D("Eforward",       "E(eV)",   Nb, 0.0, EMax);
  hgEfwItg= new TH1D("EforwardInteg",  "E(eV)",   Nb, 0.0, EMax);
  hgEfwph = new TH1D("Eforward_ph",    "E(eV)",   Nb, 0.0, EMax);
  hgEfwel = new TH1D("Eforward_el",    "E(eV)",   Nb, 0.0, EMax);
  hgEbw   = new TH1D("Ebackward",      "E(eV)",   Nb, 0.0, EMax);
  hgEbwItg= new TH1D("EbackwardInteg", "E(eV)",   Nb, 0.0, EMax);
  hgEel20  = new TH1D("Eel20deg",      "E(eV)",   Nb, 0.0, EMax);
  hgEph20  = new TH1D("Eph20deg",      "E(eV)",   Nb, 0.0, EMax);
  hgEel80  = new TH1D("Eel80deg",      "E(eV)",   Nb, 0.0, EMax);
  hgEph80  = new TH1D("Eph80deg",      "E(eV)",   Nb, 0.0, EMax);
} 


void UAExit::EndOfRunAction( const G4Run* ) {
  cout << "UAExit EndOfRun" << endl;
  
  // Integrado desde la derecha:
  G4double Sum=0, Sam=0;
  for (int i=hgEfw->GetNbinsX(); i >=0; i--) {
    Sum += hgEfw->GetBinContent(i);
    Sam += hgEbw->GetBinContent(i);
    hgEfwItg->SetBinContent(i, Sum);
    hgEbwItg->SetBinContent(i, Sam);
  }

  G4String JobName = GmParameterMgr::GetInstance()->GetStringValue("JobName", "job");
  G4String fn = "hexit" + JobName + ".root";
  TFile * fexit = new TFile(fn.c_str(), "RECREATE");

  hgA->Write();
  hgE->Write();
  hgEfw->Write();
  hgEfwItg->Write();
  hgEfwph->Write();
  hgEfwel->Write();
  hgEbw->Write();
  hgEbwItg->Write();
  hgEel20->Write();
  hgEph20->Write();
  hgEel80->Write();
  hgEph80->Write();
  fexit->Close();
} 


void UAExit::BeginOfEventAction( const G4Event* evt) {
}


void UAExit::EndOfEventAction( const G4Event* evt) {
}


void UAExit::UserSteppingAction(const G4Step* aStep) {
  G4String partName = aStep->GetTrack()->GetDefinition()->GetParticleName();
  G4double Edep     = aStep->GetTotalEnergyDeposit();

  G4StepPoint * Pt   = aStep->GetPreStepPoint();
  G4StepPoint * Pt2  = aStep->GetPostStepPoint();

  G4VPhysicalVolume * pv  = Pt->GetPhysicalVolume();
  G4VPhysicalVolume * pv2 = Pt2->GetPhysicalVolume();

  G4String phvlName  = "null";
  G4String phvlName2 = "null";

  if(pv  )  phvlName  = pv->GetName();
  if(pv2 )  phvlName2 = pv2->GetName();

  //if(phvlName != phvlName2) cout << "UAExit phvl change" << endl;

  if(phvlName2 != "null") return;

  //cout << "UAExit phvl2 null" << endl;

  G4double     KinE2 = Pt2->GetKineticEnergy();
  G4ThreeVector Pos2 = Pt2->GetPosition();
  G4double         y = sqrt( pow(Pos2[1],2) + pow(Pos2[2],2) );
  G4double       ang = atan2(y, Pos2[0]) * 360/twopi;
  G4String        pn = aStep->GetTrack()->GetDefinition()->GetParticleName();
  
  //cout << "exit " << KinE2/eV << " xeV " << ang << endl;

  hgE->Fill(KinE2);
  hgA->Fill(ang);

  if(ang < 90.0) {
    hgEfw->Fill(KinE2);
    if( pn == "gamma")
      hgEfwph->Fill(KinE2);
    else if( pn == "e-")
      hgEfwel->Fill(KinE2);
  }
  else
    hgEbw->Fill(KinE2);


  if( pn == "gamma") {
    if(ang < 20)
      hgEph20->Fill(KinE2);
    else if(ang > 80 && ang < 100)
      hgEph80->Fill(KinE2);
  }
  else if( pn == "e-") {
    if(ang < 20)
      hgEel20->Fill(KinE2);
    else if(ang > 80 && ang < 100)
      hgEel80->Fill(KinE2);
  }
}

