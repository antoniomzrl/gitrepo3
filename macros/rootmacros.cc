
# root -b -p -q .x compareAll.C++\(\"MYFILE.root\"\)
#                  printAll.C++


//EdX->Print("all");

hadd wldepo.root  owl_*/hdepo.root
hadd wlint.root   owl_*/hinteract.root
hadd wlintsp.root owl_*/hintsp.root
hadd pldepo.root  opl_*/hdepo.root
hadd plint.root   opl_*/hinteract.root
hadd plintsp.root opl_*/hintsp.root

hadd wgdepo.root  owg_*/hdepo.root
hadd wgint.root   owg_*/hinteract.root
hadd wgintsp.root owg_*/hintsp.root
hadd pgdepo.root  opg_*/hdepo.root
hadd pgint.root   opg_*/hinteract.root
hadd pgintsp.root opg_*/hintsp.root

hadd g.root        ogs_*/hexit.root
hadd g01p.root ogs01nm_*/hexit.root
hadd g03p.root ogs03nm_*/hexit.root
hadd g05p.root ogs05nm_*/hexit.root
hadd g10p.root ogs10nm_*/hexit.root
hadd g30p.root ogs30nm_*/hexit.root

hadd ig.root        ogs_*/hinteract.root
hadd ig01p.root ogs01nm_*/hinteract.root
hadd ig03p.root ogs03nm_*/hinteract.root
hadd ig05p.root ogs05nm_*/hinteract.root
hadd ig10p.root ogs10nm_*/hinteract.root
hadd ig30p.root ogs30nm_*/hinteract.root



cout << "Process     No Ints Edep    Avg Edep" << endl;
for (int i=0; i <= Nint->GetNbinsX()+1; i++) {
  cout << setw(40) << Edint->GetXaxis()->GetBinLabel(i)
       << setw(16) << Nint->GetBinContent(i)
       << setw(16) << Edint->GetBinContent(i)
       << setw(16) << Emint->GetBinContent(i)/100
       << endl;
 }




for (int i=0; i <= Ang->GetNbinsX()+1; i++) {
  cout << setw(8)  << Ang->GetBinCenter(i)
       << setw(16) << Ang->GetBinContent(i) << endl;
}
for (int i=0; i <= E->GetNbinsX()+1; i++) {
  cout << setw(8)  << E->GetBinCenter(i)
       << setw(16) << E->GetBinContent(i) << endl;
}


for (int i=0; i <= E32deg->GetNbinsX()+1; i++) {
  cout << setw(8)  << E32deg->GetBinCenter(i)
       << setw(16) << E32deg->GetBinContent(i) << endl;
}


cout << "Edep(eV) - x(mm)" << endl;
for (int i=0; i <= EdX->GetNbinsX()+1; i++) {
  cout << setw(8)  << EdX->GetBinCenter(i)
       << setw(16) << EdX->GetBinContent(i)
       << endl;
}


cout << "x(mm) / Elastic ExcitRot ExcitVibr ExcitElectr DisNeutr Ioniz Attach Auger " << endl;
for (int i=0; i <= Elastic->GetNbinsX()+1; i++) {
  cout << setw(10)  << Elastic->GetBinCenter(i)
       << setw(14) << Elastic->GetBinContent(i)
       << setw(14) << ExcitRotat->GetBinContent(i)
       << setw(14) << ExcitVibrat->GetBinContent(i)
       << setw(10) << ExcitElectr->GetBinContent(i)
       << setw(10) << DissocNeutr->GetBinContent(i)
       << setw(10) << Ionization->GetBinContent(i)
       << setw(10) << Attachment->GetBinContent(i)
       << setw(10) << IonizAuger->GetBinContent(i)
       << endl;
}
