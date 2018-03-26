#include "uactions.hh"


UAAnalyser::UAAnalyser() : GmUserRunAction(), GmUserEventAction(),
			 GmUserSteppingAction() {
  cout << "UAAnalyser Constructor" << endl;
}


UAAnalyser::~UAAnalyser() {
  cout << "UAAnalyser Destructor" << endl;
}


void UAAnalyser::BeginOfRunAction( const G4Run* ) {
  cout << "UAAnalyser BeginOfRun" << endl;

  G4double Nb   = GmParameterMgr::GetInstance()->GetNumericValue("UAAnalyser:EnergyBins", 1000);
  G4double EMax = GmParameterMgr::GetInstance()->GetNumericValue("UAAnalyser:EnergyMax", 1*MeV);
  anapar = GmParameterMgr::GetInstance()->GetStringValue("UAAnalyser:AnalizerParallel", "NO");
  nameOut = GmParameterMgr::GetInstance()->GetStringValue("UAAnalyser:VolumeOut", "world");
  nameIn  = GmParameterMgr::GetInstance()->GetStringValue("UAAnalyser:VolumeIn", "null");

  hgAnaA     = new TH1D("Angular",        "Ang(deg)",180,  0.0, 180);
  hgAnaE     = new TH1D("E",              "E(eV)",   Nb, 0.0, EMax);
  hgAnaEfw   = new TH1D("Eforward",       "E(eV)",   Nb, 0.0, EMax);
  hgAnaEfwItg= new TH1D("EforwardInteg",  "E(eV)",   Nb, 0.0, EMax);
  hgAnaEfwph = new TH1D("Eforward_ph",    "E(eV)",   Nb, 0.0, EMax);
  hgAnaEfwel = new TH1D("Eforward_el",    "E(eV)",   Nb, 0.0, EMax);
  hgAnaEbw   = new TH1D("Ebackward",      "E(eV)",   Nb, 0.0, EMax);
  hgAnaEbwItg= new TH1D("EbackwardInteg", "E(eV)",   Nb, 0.0, EMax);
  hgAnaEel20  = new TH1D("Eel20deg",      "E(eV)",   Nb, 0.0, EMax);
  hgAnaEph20  = new TH1D("Eph20deg",      "E(eV)",   Nb, 0.0, EMax);
  hgAnaEel80  = new TH1D("Eel80deg",      "E(eV)",   Nb, 0.0, EMax);
  hgAnaEph80  = new TH1D("Eph80deg",      "E(eV)",   Nb, 0.0, EMax);
} 


void UAAnalyser::EndOfRunAction( const G4Run* ) {
  cout << "UAAnalyser EndOfRun" << endl;
  
  // Integrado desde la derecha:
  G4double Sum=0, Sam=0;
  for (int i=hgAnaEfw->GetNbinsX(); i >=0; i--) {
    Sum += hgAnaEfw->GetBinContent(i);
    Sam += hgAnaEbw->GetBinContent(i);
    hgAnaEfwItg->SetBinContent(i, Sum);
    hgAnaEbwItg->SetBinContent(i, Sam);
  }

  G4String JobName = GmParameterMgr::GetInstance()->GetStringValue("JobName", "job");
  G4String fn = "analyser" + JobName + ".root";
  TFile * fexit = new TFile(fn.c_str(), "RECREATE");

  hgAnaA->Write();
  hgAnaE->Write();
  hgAnaEfw->Write();
  hgAnaEfwItg->Write();
  hgAnaEfwph->Write();
  hgAnaEfwel->Write();
  hgAnaEbw->Write();
  hgAnaEbwItg->Write();
  hgAnaEel20->Write();
  hgAnaEph20->Write();
  hgAnaEel80->Write();
  hgAnaEph80->Write();
  fexit->Close();
} 


void UAAnalyser::BeginOfEventAction( const G4Event* evt) {
}


void UAAnalyser::EndOfEventAction( const G4Event* evt) {
}


void UAAnalyser::UserSteppingAction(const G4Step* aStep) {
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

  //if(phvlName != phvlName2) cout << "UAAnalyser phvl change" << endl;

  if(phvlName != nameOut || phvlName2 != nameIn) return;

  //cout << "UAAnalyser phvl2 null" << endl;

  G4double     KinE = Pt2->GetKineticEnergy();
  G4ThreeVector Pos = Pt2->GetPosition();
  G4double        y = sqrt( pow(Pos[1],2) + pow(Pos[2],2) );
  G4double      ang = atan2(y, Pos[0]) * 360/twopi;
  G4String       pn = aStep->GetTrack()->GetDefinition()->GetParticleName();

  
  if( anapar == "yes") {
    G4ThreeVector d = Pt2->GetMomentumDirection();
    KinE *= d.x()*d.x();
    //cout << "uaexit dirModE: " << d << " " << KinE/eV << endl;
  }
  
  //cout << "exit " << KinE/eV << " xeV " << ang << endl;

  hgAnaE->Fill(KinE);
  hgAnaA->Fill(ang);

  if(ang < 90.0) {
    hgAnaEfw->Fill(KinE);
    if( pn == "gamma")
      hgAnaEfwph->Fill(KinE);
    else if( pn == "e-")
      hgAnaEfwel->Fill(KinE);
  }
  else
    hgAnaEbw->Fill(KinE);


  if( pn == "gamma") {
    if(ang < 20)
      hgAnaEph20->Fill(KinE);
    else if(ang > 80 && ang < 100)
      hgAnaEph80->Fill(KinE);
  }
  else if( pn == "e-") {
    if(ang < 20)
      hgAnaEel20->Fill(KinE);
    else if(ang > 80 && ang < 100)
      hgAnaEel80->Fill(KinE);
  }
}

