#include "uactions.hh"

TH1D *hgNi,  *hgEdi, *hgEli, *hgEmi, *hgEmil, *hgElo;
TH1D *hgAngE, *hgAngEdeg, *hgAngR, *hgAngI, *hgAngIo;

int NoEvents=0, NoCols=0, NoColsEl=0, NoColsIo=0;
double Edepo=0, Elo=0;

string proclabel[10];
int NoIntThisEv[10];
int NoIntEv[10][100];


UAInteraction::UAInteraction() : GmUserRunAction(), GmUserEventAction(),
				 GmUserSteppingAction() {
  cout << "UAInteraction Constructor" << endl;
}

UAInteraction::~UAInteraction() {
  cout << "UAInteraction Destructor" << endl;
}

void UAInteraction::BeginOfRunAction( const G4Run* ) {
  cout << "UAInteraction BeginOfRunAction" << endl;

  proclabel[0] = "Total";
  proclabel[1] = "Elastic";
  proclabel[2] = "nicExcit";
  proclabel[3] = "Vibrat";
  proclabel[4] = "Ionis";
  proclabel[5] = "Attach";
  proclabel[6] = "xxx6";
  proclabel[7] = "xxx7";
  proclabel[8] = "xxx8";
  proclabel[9] = "xxx9";
  
  hgNi   = new TH1D("Nint",  "Number of ints",      3, 0, 3);
  hgEdi  = new TH1D("Edint", "Edep(eV) /int type",  3, 0, 3);
  hgEli  = new TH1D("Elint", "Eloss(eV) /int type",  3, 0, 3);
  hgElo  = new TH1D("Eloss", "Eloss(eV)",           2000, 0, 200);
  hgAngE = new TH1D("AngElastic", "Ang Disp (deg)", 2000, -10, 190);
  hgAngEdeg = new TH1D("AngElasticDeg", "Ang Disp (deg)", 200, -10, 190);
  hgAngR  = new TH1D("AngRotat",   "Ang Disp (deg)", 2000, -10, 190);
  hgAngI  = new TH1D("AngInelast", "Ang Disp (deg)", 2000, -10, 190);
  hgAngIo = new TH1D("AngIoni", "Ang Disp (deg)", 2000, -10, 190);

  for(int i=0; i<100; i++)
    for(int j=0; j<10; j++)
      NoIntEv[j][i]=0;
}

void UAInteraction::EndOfRunAction( const G4Run* ) {
  cout << "UAInteraction EndOfRunActnion" << endl;

  //hgEmi = new TH1D("Emint", "Edep(eV) average per int type",20, 0, 20);
  //hgEmi = hcopy(* hgEdi);

  //hgNi->gStyle->SetOptStat("nemrou")

  hgNi->LabelsOption("a", "X");
  hgEdi->LabelsOption("a", "X");
  hgEli->LabelsOption("a", "X");
  
  hgEmi = (TH1D*)hgEdi->Clone("Emint");
  hgEmi->SetTitle("Edep(eV) average per int type");
  hgEmil = (TH1D*)hgEdi->Clone("Emint");
  hgEmil->SetTitle("Elost(eV) average per int type");

  for (int i=0; i <= hgNi->GetNbinsX()+1; i++) {
    if(hgNi->GetBinContent(i) != 0) {
      G4double vm = hgEdi->GetBinContent(i) / hgNi->GetBinContent(i);
      G4double vm2= hgEli->GetBinContent(i) / hgNi->GetBinContent(i);
      hgEmi->SetBinContent(i, vm);
      hgEmil->SetBinContent(i, vm2);
    }
  }

  cout << right << setw(14) << "No Ints"
       << right << setw(14) << "Edep"
       << right << setw(14) << "Avg Edep"
       << right << setw(14) << "Elost"
       << right << setw(14) << "Avg Elost"
       << left  << "   Process" << endl;
    //<< right << setw(16) << "Edep/Tot"

  //  TAxis * xaxis = hgEdi->GetXaxis();

  for (int i=0; i <= hgNi->GetNbinsX()+1; i++) {
    if(hgNi->GetBinContent(i) != 0) {
      cout << right << setw(14) << hgNi->GetBinContent(i)
	   << right << setw(14) << hgEdi->GetBinContent(i)
	   << right << setw(14) << hgEmi->GetBinContent(i)
	   << right << setw(14) << hgEli->GetBinContent(i)
	   << right << setw(14) << hgEmil->GetBinContent(i)
	   << "   " << left  << hgEdi->GetXaxis()->GetBinLabel(i) << endl;
    }
  }
	//<< right << setw(16) << hgEdi->GetBinContent(i)/hgEdi->GetSum()

  cout << left  << "Total:" << endl
       << right << setw(12) << hgEdi->GetEntries()
       << right << setw(16) << hgEdi->GetSum()
       << endl;



  double Density = GmParameterMgr::GetInstance()->GetNumericValue("STPW:Density",  1*g/cm3);
  double Length  = GmParameterMgr::GetInstance()->GetNumericValue("STPW:Length",   0.01*nm);
  double MMass = GmParameterMgr::GetInstance()->GetNumericValue("STPW:MolecularMass", 18*g/mole);

  double MADL = MMass / (Avogadro*Density*Length);
  double XS   = -log(1.0 - NoCols / (double)NoEvents) * MADL;
  double XSel = -log(1.0 - NoColsEl / (double)NoEvents) * MADL;
  double XSio = -log(1.0 - NoColsIo / (double)NoEvents) * MADL;
  double SP  = Edepo/NoEvents/Density/Length;
  double SP2 =   Elo/NoEvents/Density/Length;
  double AvElo = Elo/NoEvents;
  
  cout << "\tNoEvents:      " << NoEvents << endl
       << "\tNo:            " << NoEvents  << endl
       << "\tNoCols:        " << NoCols   << endl
       << "\tNoColsEl:      " << NoColsEl  << endl
       << "\tNoColsIo:      " << NoColsIo  << endl
       << "\tEdepo(eV):     " << Edepo/eV << endl
       << "\tAvgEdepo:      " << Edepo/(double)NoEvents/eV << endl
       << "\tD(g/cm3):      " << Density/(g/cm3)  << endl
       << "\tL:             " << G4BestUnit(Length, "Length") << endl
       << "\tMM(g/mole):    " << MMass/(g/mole) << endl
       << "\tSP(MeV*cm2/g): " << SP/(MeV*cm2/g) << endl
       << "\tSP2(MeV*cm2/g): " << SP2/(MeV*cm2/g) << endl
       << "\tAvgEloss(eV):   " << AvElo/eV << endl
       << "\tTXS(10^-16cm2): " << XS/(1e-16*cm2) << endl
       << "\tTXSel:          " << XSel/(1e-16*cm2) << endl
       << "\tTXSio:          " << XSio/(1e-16*cm2) << endl;



  // for (int i=0; i <= hgNi->GetNbinsX()+1; i++) {
  //   if(hgNi->GetBinContent(i) != 0) {
  //     cout << left  << setw(40) << hgEdi->GetXaxis()->GetBinLabel(i)
  // 	   << right << setw(16) << (int)hgNi->GetBinContent(i);
  //     cout << right << setw(16) << fixed << hgEdi->GetBinContent(i)
  // 	   << right << setw(16) << fixed << hgEdi->GetBinContent(i)/hgEdi->GetSum()
  // 	   << right << setw(16) << fixed << hgEmi->GetBinContent(i) << endl;
  //   }
  // }

  G4String JobName = GmParameterMgr::GetInstance()->GetStringValue("JobName", "job");
  G4String fn = "hint" + JobName + ".root";

  TFile * fint = new TFile(fn.c_str(), "RECREATE");
  hgNi->Write();
  hgEdi->Write();
  hgEli->Write();
  hgEmi->Write();
  hgEmil->Write();
  hgAngE->Write();
  hgElo->Write();
  hgAngEdeg->Write();
  hgAngR->Write();
  hgAngI->Write();
  hgAngIo->Write();
  fint->Close();

  cout << "# NoIntPerEvent:" << endl
       << "# N";
  for(int j=0; j<10; j++)
    cout << " , " << proclabel[j];
  cout << endl;
  
  for(int i=0; i<100; i++) {
    if( NoIntEv[0][i] != 0 ) {
      cout << i;
      for(int j=0; j<10; j++)
	cout << ",\t" << NoIntEv[j][i];
      cout << endl;
    }
  } 
}

void UAInteraction::BeginOfEventAction( const G4Event* ) {
  NoEvents++;
  for(int j=0; j<10; j++)
    NoIntThisEv[j] = 0;
}


void UAInteraction::EndOfEventAction( const G4Event* evt ) {
  for(int j=0; j<10; j++)
    if( NoIntThisEv[j] <100)
      NoIntEv[j][NoIntThisEv[j]]++;
}


void UAInteraction::UserSteppingAction(const G4Step* aStep) {

  G4double Ed = aStep->GetTotalEnergyDeposit();
  Edepo += Ed;

  //if(Ed == 0) return; // Rayleigh!!

  G4StepPoint * Pt1  = aStep->GetPreStepPoint();
  G4StepPoint * Pt2  = aStep->GetPostStepPoint();

  double El = Pt1->GetKineticEnergy() - Pt2->GetKineticEnergy();
  Elo += El;

  G4String phvlName;
  G4VPhysicalVolume * pv = Pt2->GetPhysicalVolume();
  if(pv ) phvlName = pv->GetName();
  else    phvlName = "null";


  G4String procName;
  const G4VProcess * pro = Pt2->GetProcessDefinedStep();
  if(pro )  procName = pro->GetProcessName();
  else      procName = "null";

  G4String partName = aStep->GetTrack()->GetDefinition()->GetParticleName();

  G4String vp = phvlName + " " + partName + " " + procName;
  hgNi->Fill(vp, 1);
  hgEdi->Fill(vp, Ed/eV);
  hgEli->Fill(vp, El/eV);
  hgElo->Fill(El/eV);

  G4ThreeVector Mom1  = Pt1->GetMomentum();
  G4ThreeVector Mom2  = Pt2->GetMomentum();
  if(Mom2.mag() == 0) return;

  double ang = Mom1.angle(Mom2)*360/twopi;

  //cout << "ang " << procName << "\t" << ang << endl;
  
  if( procName.find("Elastic") != std::string::npos) {
    hgAngE->Fill(ang);
    hgAngEdeg->Fill(ang);
    NoColsEl++;
  }
  else if( procName.find("Ionis") != std::string::npos) {
    NoColsIo++;
    hgAngIo->Fill(ang);
  }
  else if( procName.find("Rotat") != std::string::npos)
    hgAngR->Fill(ang);
  else
    hgAngI->Fill(ang);


  
  // if( procName.find("Transportation") != std::string::npos) {
  //   NoIntThisEv++;
  //   NoCols++;
  // }

  for(int j=1; j<10; j++) {
    if( procName.find(proclabel[j]) != std::string::npos) {
      NoIntThisEv[j]++;
      NoIntThisEv[0]++; //Total
    }
  }
      
}
