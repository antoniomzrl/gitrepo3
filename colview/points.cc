
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>

#include "points.hh"

#ifndef DEFAULTPATH
#define DEFAULTPATH "/usr/local/colview"
#endif

vector <string> tpi, ntpi, pn;


string GetAbsoluteFile(string Fn) {
  {
    string fn = "./" + Fn;
    ifstream fi(fn.c_str(), ios::in);
    if(fi.good()) return(fn);
  }
  
  char * path = getenv("COLVIEWPATH");
  if( path ) {
    string fn = string(path) + "/" + Fn;
    ifstream fi(fn.c_str(), ios::in);
    if(fi.good()) return(fn);
  }

  {
    string fn = string(DEFAULTPATH) + string("/") + string(Fn);
    ifstream fi(fn.c_str(), ios::in);
    if(fi.good()) return(fn);
  }
  
  cout << "GetAbsoluteFile " << Fn << " not found" << endl;
  return( string(""));
}



void InitTpi(string Fn) {

  tpi.clear();
  string fn = GetAbsoluteFile(Fn);
 
  if( fn != string("") ) {
    ifstream fi(fn.c_str(), ios::in);
  
    string line;
    while( fi.good() ) {
      getline(fi, line);
      if( ! line.empty() && line.find("#") == std::string::npos )
	tpi.push_back(line);
    }
  
    fi.close();
  }

  tpi.push_back("-----"); // 0-index

  //Reverse order
  for(unsigned int i=0; i<tpi.size()/2; i++) {
    string tmp = tpi[i];
    tpi[i] = tpi[tpi.size()-1-i];
    tpi[tpi.size()-1-i] = tmp;
  }

  
  //Alias replacement
  int Nalias=12;
  string alias[] = {
    "proton"        , "p "         ,
    "hydrogen"      , "h "         ,
    "e-"            , "e-"         ,
    "e+"            , "e+"         ,
    "_G4DNA"        , ""           , //5
    "_Lepts"        , ""           ,
    "crease"        , "c"          ,
    "ation"         , "at"      ,
    "Electronic"    , "Electr"     ,
    "phot"          , "gamma Phot ",
    "compt"         , "gamma Compt", //10
    "Rayl"          , "gamma Rayl ",
    
    "_G4LEPTS"      , ""           ,
    "VibExcitation" , "Vibrat     " ,
    "ExcitVibrat"   , "Vibrat     " ,
    "Excitation"    , "Excitat    " ,
    "ExcitRotat"    , "Rotat      " ,
    "Attachment"    , "Attach     " ,
    "Elastic"       , "Elast      " ,
    "Ionisation"    , "Ionis      " ,
    "Thermalisation", "Thermalis  " ,

  };

  ntpi.clear();
  
  for(unsigned int i=0; i<tpi.size(); i++) {
    ntpi.push_back(tpi[i]);

    for(int j=0; j<Nalias; j++) {
      size_t pos = ntpi[i].find(alias[2*j]);
      if( pos != std::string::npos )
    	ntpi[i].replace(pos, alias[2*j].size(), alias[2*j+1]);
    }
    //ntpi[i].resize(20);
    if(ntpi[i].size() < 13)
      ntpi[i].insert(ntpi[i].size(), 13-ntpi[i].size(), ' ');
  }

  pn.clear();
  
  string p[] = { "UnkNown", "gamma", "e-", "e+", "proton", "hydrogen"};
  for(int i=0; i<6; i++)
    pn.push_back(p[i]);
  
  cout << "TPI File: " << fn << " " << tpi.size() << endl;


}



int TPI(string it, int Nmin, int Nmax) {
  //cout << "TPI " << it << " " << Nmin  << " " << Nmax << endl;
  for(int i=Nmin; i<=Nmax; i++) {
    //cout << "TPI for " << " " << it << " " << i << " " << tpi[i] << endl;
    if( it.find(tpi[i]) != std::string::npos ) {
      //cout << "find!!\n";
      return(i);
    }
  }

  cout << "Warning TPI unknown " << it << " added "
       << tpi.size() << " " << Nmin << " " << Nmax << endl;
  tpi.push_back(it);
  ntpi.push_back(it);
  return( tpi.size()-1 );
}


int TPI(string it) {
  return( TPI(it, 0, tpi.size()-1) );
}




int PN(string p) {
  for(int i=0; i<6; i++)
    if(p.find(pn[i]) != std::string::npos ) 
      return(i);

  //cout << "PN unknown: " << p << endl;
  return(0);
}



// string tpi[100] = {
//   "reserv0",
//   "reserv1",
//   "reserv2",
//   "Transportation",
//   "e-_G4LEPTSElastic",
//   "e-_G4LEPTSExcitRotat",       //5
//   "e-_G4LEPTSExcitVibrat",
//   "e-_G4LEPTSExcitation",
//   "e-_G4LEPTSDissocNeutr",
//   "e-_G4LEPTSIonisation",
//   "e-_G4LEPTSThermalisation",   //10
//   "e-_G4LEPTSAttachment",
//   "e+_G4LEPTSElastic",
//   "e+_G4LEPTSExcitRotat",
//   "e+_G4LEPTSExcitVibrat",
//   "e+_G4LEPTSExcitation",       //15
//   "e+_G4LEPTSDissocNeutr",
//   "e+_G4LEPTSIonisation",
//   "e+_G4LEPTSAnnihilation",
//   "e+_G4LEPTSPositronium",
//   "hydrogen_G4LEPTSElastic",    //20
//   "hydrogen_G4LEPTSExcitation",
//   "hydrogen_G4LEPTSChargeIncrease",
//   "hydrogen_G4LEPTSIonisation",
//   "proton_G4LEPTSElastic",
//   "proton_G4LEPTSExcitation",   //25
//   "proton_G4LEPTSChargeDecrease",
//   "proton_G4LEPTSIonisation",
//   "reserv28",
//   "UserSpecialCut",
//   "reserv30",        //30
//   "Elastic",
//   "ExcitRotat", 
//   "ExcitVibrat",
//   "Excitation",
//   "DissocNeutr",           //35
//   "Ioni",   //"Auger",
//   "Thermalisation",
//   "Attachment",
//   "Annihilation",
//   "Positronium",         //40
//   "ChargeInc",
//   "ChargeDec",
//   "Rayl",
//   "compt",
//   "phot",               //45
//   "conv",
//   "reserv",
//     "reserv",
//     "reserv",
//     "reserv",         //50
// };
