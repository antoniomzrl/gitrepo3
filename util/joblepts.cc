
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <string>
using namespace std;


string ToUpper(string str) {
  for(unsigned int i=0;i<str.length();i++)
    str[i] = toupper(str[i]);
  return str;
}

string ToBlank(string str) {
  for(unsigned int i=0;i<str.length();i++)
    if( str[i] == ',' ) // || str[i] == '_' )
      str[i] = ' ';
  return str;
}

void command( string cmd) {
  cout << "executing: " << cmd << endl;
  system( cmd.c_str() );
}



class KeyValue {
  int NK;
  int nk;

  string key[100];
  string alias[100];
  string val[100];

public:

  void AddKeyVal(string k, string a, string v) {
    key[nk] = k;
    alias[nk] = a;
    val[nk] = v;
    nk++;
  }

  void AddKeyVal(string k, string v) {
    AddKeyVal(k, k, v);
  }


  void ChangeVal(string k, string v) {
    string K = ToUpper(k);
    for(int i=0; i<nk; i++) {
      if( K == key[i] || K == alias[i]) {
	//val[i] = ToBlank(v);
	val[i] = v;
	//cout << "ChangeVal (" << key[i] << ")\t-> " << v << endl;
      }
    }
  }

  void ShowKeyVals() {
    cout << "Key Vals:" << endl;
    for(int i=0; i<nk; i++) {
      cout << "(" << i << ") " << right << setw(20) << key[i]
	   << " -> " << val[i] << endl;
    }
  }

  string TheValueOf(string k) {
    string v;
    for(int i=0; i<nk; i++) {
      if( k == key[i] || k == alias[i]) {
	v = val[i];
      }
    }
    return v;
  }

  void SetDefaultKeyVals() {
    AddKeyVal( "CHAMBER",       "GEOMETRY", "cilindro");
    AddKeyVal( "RADIUS"   ,                 "10*cm"   );
    AddKeyVal( "HALFLENGTH",                "5*cm"    );
    AddKeyVal( "HALFLENGTHWATER",           "none"    );
    AddKeyVal( "WORLDBOX",                  "30*cm"   );
    AddKeyVal( "MATERIAL",                  "water"   );
    AddKeyVal( "DENSITY",                   "1*g/cm3" );
    AddKeyVal( "TEMPERATURE",               "273.15*kelvin" );
    AddKeyVal( "WRAPPER",                   "none" );
    AddKeyVal( "MAGNETICFIELD",      "MAG", "none" );
    AddKeyVal( "LOCALMAGNETICFIELD", "LOCMAG", "none" );
    AddKeyVal( "SPATIALHISTOGRAM",   "SPHG", "none");

    AddKeyVal( "GENERATOR",                 "point"   );
    AddKeyVal( "GPOSITION",                 "0 0 0"   );
    AddKeyVal( "GDIRECTION",                "1 0 0"   );
    AddKeyVal( "GPARTICLE",                 "e-"      );
    AddKeyVal( "GENERGY",            "E",   "10*keV"  );
    AddKeyVal( "GSPECTRUM",          "GSP", "none"    );
    AddKeyVal( "GHISTOGRAMS",        "GHG", "none"    );
    AddKeyVal( "CONTFILE",            "CF", "none"    );
    AddKeyVal( "BEAMON",                    "none"    );

    AddKeyVal( "DETECTOR",                  "none"    );
    AddKeyVal( "DETECTORBCK", "DETECTOR2",  "none"    );
    AddKeyVal( "DIAFRAGM",                  "none"    );


    AddKeyVal( "BATCH",                     "no"      );
    AddKeyVal( "SEED",                      "none"    );

    AddKeyVal( "PHYSICS",          "LeptsPhysicsList");
    AddKeyVal( "XSDAT",              "XS",  "water"   );
    AddKeyVal( "ELECTRONMODEL",      "EM",  "lepts"   );
    AddKeyVal( "PHOTONMODEL",        "PM",  "penelope");
    AddKeyVal( "DIRJOB"  ,                  "jobdir"  );
    AddKeyVal( "XSECTIONDIR",        "DIR", "xx"      );
    AddKeyVal( "STOPENERGYELECTRON", "STPE","20*eV"   );
    AddKeyVal( "PDE",                       "0.9"     );

    AddKeyVal( "VISUALIZE",          "VIS", "none"    );
    AddKeyVal( "VERBOSE",            "V",   "none"    );
  }

  KeyValue() {
    //cout << "#!/bin/sh (help emacs)" << endl;
    //cout << "constructor KeyValue" << endl;
    nk = 0;
    SetDefaultKeyVals();
    //ShowKeyVals();
  }

};


KeyValue theKV;

string tvo(string key) {
  return theKV.TheValueOf(key);
}



int main (int argc, char **argv) {

  string str, keyw, keyv;

  //cout << "argc " << argc << endl;

  if( argc == 1 ) {      // stdin
    while( getline(cin, str) ) {
      if(str[0] != '#' && str[0] != '!' && str.length() > 0 ) {
	istringstream iss(str);
	iss >> keyw >> keyv;
	while( iss >> str) keyv = keyv + " " + str;
	theKV.ChangeVal(keyw, keyv);
      }
    }
  }
  else if (argc == 2 ) { // Input file

  }
  else {                 // Flags
    for(int i=0; i<argc; i++) {
      keyw = (string) argv[i];
      if((i+1) < argc ) {
	keyv = (string) argv[i+1];
	theKV.ChangeVal(keyw, keyv);
      }
    }
  }

  string vb = tvo("VERBOSE");
  bool VerboseParser     = vb.find("parser")   != string::npos;
  bool VerboseGamos      = vb.find("gamos")    != string::npos;
  bool VerboseLepts      = vb.find("lepts")    != string::npos;
  bool VerboseHeaders    = vb.find("headers")      != string::npos;
  bool VerboseDetector   = vb.find("detectorhits") != string::npos;
  bool VerboseHistograms = vb.find("histograms")   != string::npos;

  if(VerboseParser) theKV.ShowKeyVals();



  string DirJob  = tvo("DIRJOB");

  string fni = DirJob + "/" + DirJob + ".inp";
  string fng = DirJob + "/" + DirJob + ".geom";
  string fnj = DirJob + "/" + DirJob + ".sh";

  command("mkdir -p " + DirJob);

  ofstream fi(fni.c_str());
  ofstream fg(fng.c_str());

  string ChName, ChMat, ChR, ChDx,ChX; // Chamber
  string GenName="none", GenMat, GenR, GenDx,GenX; //Generator
  string DetName,  DetMat,  DetR,  DetDx,  DetX; //Detector
  string DetName2, DetMat2, DetR2, DetDx2, DetX2; //Detector back
  string DiaName, DiaMat, DiaR, DiaDx,DiaX; //Diafragm det

  istringstream iss( ToBlank(tvo("CHAMBER")) );
  iss >> ChName >> ChMat >> ChR >> ChDx >> ChX;
  istringstream issd( ToBlank(tvo("DETECTOR")) );
  issd >> DetName >> DetMat >> DetR >> DetDx >> DetX;
  istringstream issf( ToBlank(tvo("DETECTOR2")) );
  issf >> DetName2 >> DetMat2 >> DetR2 >> DetDx2 >> DetX2;
  istringstream isse( ToBlank(tvo("DIAFRAGM")) );
  isse >> DiaName >> DiaMat >> DiaR >> DiaDx >> DiaX;


  fi << "#!/bin/sh (help emacs)" << endl
     << endl;

  if( VerboseGamos ) {
    fi << "# VERBOSITY"          << endl
       << "/control/verbose 2"   << endl
       << "/run/verbose 2"       << endl
       << "#/event/verbose 4"     << endl
       << "#/tracking/verbose 4"  << endl;

    fi << "# VERBOSITY"          << endl
       << "/gamos/verbosity GmBaseVerbosity -1"      << endl
       << "/gamos/verbosity GmBaseVerbosity -1"      << endl
       << "/gamos/verbosity GmPhysicsVerbosity -1"   << endl
       << "/gamos/verbosity GmSDVerbosity -1"        << endl
       << "/gamos/verbosity GmUAVerbosity -1"        << endl
       << "/gamos/verbosity GmScoringVerbosity -1"   << endl
       << "/gamos/verbosity GmDataVerbosity -1"      << endl
       << "/gamos/verbosity GmUAVerbosity -1"        << endl
       << endl;
  }

  fi << "# GEOMETRY" << endl
     << "/gamos/setParam GmGeometryFromText:FileName " << DirJob << ".geom" << endl
     << "/gamos/geometry GmGeometryFromText" << endl
     << endl

     << "# PHYSICS" << endl
     << "/gamos/physicsList " << tvo("PHYSICS") << endl;


  fi << "# OUTPUT: ANALYSIS, USERACTIONS"     << endl
     << "/gamos/setParam XsectionDir "        << tvo("XSECTIONDIR")   << endl
     << "/gamos/setParam ElectronModel "      << tvo("ELECTRONMODEL") << endl
     << "/gamos/setParam PhotonModel "        << tvo("PHOTONMODEL")   << endl
     << "/gamos/setParam XsDat "              << tvo("XSDAT")         << endl
     << "/gamos/setParam CutEnergyIoniz   10" << endl
     << "/gamos/setParam StopEnergyElectron " << tvo("STOPENERGYELECTRON") << endl
     << "/gamos/setParam probOfDeathElastic " << tvo("PDE")              << endl
     << "/gamos/setParam VerboseLepts "       << VerboseLepts            << endl
     << "/gamos/setParam VerboseHeaders "     << VerboseHeaders          << endl
     << "/gamos/setParam VerboseDetector "    << VerboseDetector         << endl
     << "/gamos/setParam VerboseHistograms "  << VerboseHistograms       << endl
     << "/gamos/setParam ChamberHalfLength "  << ChDx << endl
     << "/gamos/setParam SpatialHistogram "   << tvo("SPATIALHISTOGRAM") << endl
     << "/gamos/setParam ContinuationFile "   << tvo("CONTFILE") << endl
     << endl

     << "/gamos/userAction Run"   << endl
     << "/gamos/userAction Event" << endl
     << endl

     << "#/gamos/setParam GmCountTracksUA:EachNEvent 1000" << endl
     << "#/gamos/userAction GmCountTracksUA"    << endl
     << "#/gamos/userAction GmCountProcessesUA" << endl
     << "#/gamos/setParam track:hEMax 10*keV"   << endl
     << "#/gamos/setParam track:hENbins 1000"   << endl
     << "#/gamos/userAction GmTrackHistosUA"    << endl
     << endl;
    
  if(tvo("GHISTOGRAMS") != "none") {
    fi << "/gamos/analysis/histo1Max *Ener* " << tvo("GHISTOGRAMS") << endl
       << "/gamos/userAction GmGenerHistosUA" << endl;

    fi << "/gamos/userAction UserActionPrueba" << endl;
  }

  if(tvo("SEED") != "none") {
    fi << "# SEEDS:" << endl
       << "/gamos/random/setSeeds " << tvo("SEED") << " " << tvo("SEED")
       << endl << endl;
  }


  fi <<"# INIT" << endl
     << "/run/initialize" << endl << endl
      
     << "#filtro generador track" << endl
     << "#/gamos/setParam track.text_GmPrimaryFilter:PosInitialY 1" << endl
     << "#/gamos/setParam track.text_GmPrimaryFilter:PosInitialX 1" << endl
     << "#/gamos/setParam track.text_GmPrimaryFilter:PosInitialZ 1"  << endl
     << "#/gamos/setParam track.text_GmPrimaryFilter:EnergyInitial 1" << endl
     << "#/gamos/setParam track.text_GmPrimaryFilter:EDepo 1" << endl
     << "#/gamos/setParam track.text_GmPrimaryFilter:ELost 1" << endl
     << "#/gamos/setParam track.text_GmPrimaryFilter:NSeco 1" << endl
     << "#/gamos/setParam track.text_GmPrimaryFilter:NSteps 1" << endl
     << "#/gamos/setParam track.text_GmPrimaryFilter:CreatorProcess 1" << endl
     << "#/gamos/userAction GmTrackTextFileUA GmPrimaryFilter" << endl
     << endl;

  if(tvo("HALFLENGTHWATER") != "none") {
    fi << "#DETECTOR3" << endl
       << "/gamos/SD/assocSD2LogVol Detector detector3 cilindrow"  << endl;
  }

  if(tvo("WRAPPER") != "none") {
    fi << "#DETECTOR2" << endl
       << "/gamos/SD/assocSD2LogVol Detector detector2 wrapper"  << endl;
  }

  
  if(tvo("CHAMBER") == "brachy") {
    fi << "/gamos/SD/assocSD2LogVol Detector detect1 ojo" << endl
       << "/gamos/SD/assocSD2LogVol Detector detect2 capaRu" << endl << endl

       << "#GENERATOR" << endl
       << "/gamos/generator GmGenerator" << endl
       << "/gamos/generator/addSingleParticleSource LaFuente e- 2*MeV" << endl
       << "/gamos/generator/energyDist LaFuente GmGenerDistEnergyFromFile "
       << tvo("XSECTIONDIR") << "/Ru106-Spectrum-e.dat fixed 1*eV"  << endl
       << "/gamos/generator/positionDist LaFuente GmGenerDistPositionInG4Volumes capaRu" << endl
       << "#/gamos/generator/listSources" << endl << endl;
  }
  else if(tvo("CHAMBER") == "brachygamma") {
    fi << "/gamos/SD/assocSD2LogVol Detector detect1 ojo" << endl
       << "/gamos/SD/assocSD2LogVol Detector detect2 capaRu" << endl << endl

       << "#GENERATOR" << endl
       << "/gamos/generator GmGenerator" << endl
       << "/gamos/generator/addSingleParticleSource LaFuente gamma 2*MeV" << endl
       << "/gamos/generator/energyDist LaFuente GmGenerDistEnergyFromFile "
       << tvo("XSECTIONDIR") << "/Ru106-Spectrum-gamma.dat fixed 1*eV"  << endl
       << "/gamos/generator/positionDist LaFuente GmGenerDistPositionInG4Volumes capaRu" << endl
       << "#/gamos/generator/listSources" << endl << endl;
  }

  else  {
    fi << "#DETECTOR" << endl
       << "/gamos/SD/assocSD2LogVol Detector det  " << ChName << endl;

    if(DetName != "none")
      fi  << "/gamos/SD/assocSD2LogVol Detector det1 " << DetName << endl;
    if(DetName2 != "none")
      fi  << "/gamos/SD/assocSD2LogVol Detector det2 " << DetName2 << endl;
    if(DiaName != "none")
      fi << "/gamos/SD/assocSD2LogVol Detector det3 " << DiaName << endl << endl;


    istringstream iss( ToBlank(tvo("GENERATOR")) );
    iss >> GenName >> GenMat >> GenR >> GenDx >> GenX;
    cout << "xxxxx  " << tvo("GENERATOR")<< endl;
    cout << "xxxxx2 " << GenName << endl;



    if(GenName == "file" ) {
      string GenFileName = GenMat;
      cout << "xxxxx3 " << GenFileName << endl;
      fi << "/gamos/setParam GmGeneratorFromTextFile:FileName " << GenFileName << endl
	 << "/gamos/generator GmGeneratorFromTextFile" << endl;
    }
    else {
      fi << "#GENERATOR" << endl
	 << "/gamos/generator GmGenerator" << endl;
      fi << "/gamos/generator/addSingleParticleSource miFuente "
	 << tvo("GPARTICLE") << " " << tvo("GENERGY") << endl;
      fi << "/gamos/generator/positionDist miFuente GmGenerDistPositionInG4Volumes "
	 << GenName << endl;
      fi << "/gamos/generator/directionDist miFuente GmGenerDistDirectionConst "
	 << tvo("GDIRECTION") << endl;

    if(tvo("GSPECTRUM") != "none")
      fi << "/gamos/generator/energyDist miFuente GmGenerDistEnergyFromFile "
	 << ToBlank(tvo("GSPECTRUM")) << endl;
    }



    if(tvo("GENERATOR") == "point" ) {
      fi << "/gamos/generator/positionDist miFuente GmGenerDistPositionPoint "
	 << tvo("GPOSITION") << endl;
    }

    fi << "#/gamos/generator/listSources" << endl << endl;
  }


  if(tvo("VISUALIZE") == "vrml" || tvo("VISUALIZE") == "wrl") {
    fi << "#VISU" << endl 
       << "/vis/scene/create"   << endl
       << "/vis/open VRML2FILE" << endl
       << "/vis/viewer/flush"   << endl
       << "/tracking/storeTrajectory 1"  << endl
       << "/vis/scene/add/trajectories"  << endl
       << "/vis/scene/endOfEventAction accumulate"  << endl
       << "/vis/viewer/update" << endl
       << "/vis/viewer/flush"  << endl << endl;
  }
  else if(tvo("VISUALIZE") == "ogl") {
    fi << "#VISU"             << endl
       << "/vis/scene/create" << endl
       << "/vis/open OGLSX"   << endl
       << "/vis/viewer/flush" << endl
       << "/vis/viewer/set/viewpointThetaPhi 10 120 deg" << endl
       << "/vis/viewer/zoom 1" << endl
       << "#/vis/viewer/pan -10  40 cm" << endl
       << "/tracking/storeTrajectory 1" << endl
       << "/vis/scene/add/trajectories" << endl
       << "/vis/scene/endOfEventAction accumulate" << endl
       << "/vis/viewer/update" << endl
       << "/vis/viewer/flush"  << endl
       << "/vis/ogl/printEPS"  << endl;
  }
  else if(tvo("VISUALIZE") != "none") {
    fi << "/control/execute " << tvo("VISUALIZE") << endl << endl;
  }

  if(tvo("MAGNETICFIELD") != "none") {
    fi << "#Magetic Field" << endl
       << "/gamos/magneticField/setField " << tvo("MAGNETICFIELD")
       << endl << endl;
  }
  if(tvo("LOCALMAGNETICFIELD") != "none") {
    fi << "#Magetic Field" << endl
       << "/gamos/magneticField/setLocalField " << tvo("LOCALMAGNETICFIELD")
       << " " << ChName
       << endl << endl;
  }

  fi << "# RUN" << endl
     << "/run/setCut 1e-9 um" << endl
     << "/run/beamOn " << tvo("BEAMON") << endl << endl;



  fg << "// Matrs"          << endl
     << ":ROTM rm0 0 0. 0." << endl
     << ":ROTM rmz 0 90 0" << endl << endl

     << ":ELEM Hydrogen H 1. 1." << endl
     << ":ELEM Oxygen   O 8 16."  << endl
     << ":ELEM Carbon   C 6 12."  << endl
     << ":MIXT Water " << tvo("DENSITY") << " 2" << endl
     << "   Hydrogen 2./18."  << endl
     << "   Oxygen  16./18." << endl
     << ":MIXT Ethylene " << tvo("DENSITY") << " 2" << endl
     << "   Hydrogen 4./28."  << endl
     << "   Carbon  24./28." << endl
     << ":MIXT TetraHF " << tvo("DENSITY") << " 3"  << endl
     << "   Hydrogen 8./72."  << endl
     << "   Carbon  48./72." << endl
     << "   Oxygen  16./72." << endl << endl

     << ":MATE vacio 1.0   1.008*g/mole   1.e-25*g/cm3"   << endl
     << ":MATE gold        79. 196.967*g/mole 19.3*g/cm3" << endl
     << ":MATE goldwrapper 80 160*g/mole  1e9*g/cm3"      << endl
     << ":MATE water       10. 18*g/mole "     << tvo("DENSITY") << endl
     << ":MATE waterB      10. 18*g/mole "     << tvo("DENSITY") << endl
     << ":MATE ethylene    16. 28*g/mole "     << tvo("DENSITY") << endl
     << ":MATE argon       18. 39.948*g/mole " << tvo("DENSITY") << endl
     << ":MATE THF         40. 72.11*g/mole "  << tvo("DENSITY") << endl
     << ":MATE_TEMPERATURE " << ChMat << " " << tvo("TEMPERATURE")
     << endl

     << ":VOLU   world BOX " << tvo("WORLDBOX") << " "<< tvo("WORLDBOX")
     << " " << tvo("WORLDBOX") << " vacio" << endl
     << ":COLOUR world 1 1 1" << endl
     << endl;


  if(tvo("CHAMBER") == "brachy" ||
     tvo("CHAMBER") == "brachygamma") {
    fg << ":VOLU   ojo ORB 11.5*mm " << tvo("MATERIAL") << endl
       << ":PLACE  ojo 1 world rm0 0. 0. 0." << endl
       << ":COLOUR ojo 0.2 0.2 1"  << endl << endl

       << ":VOLU   tumor CONE 0 0.1*mm 0 3*mm 2.5*mm " << tvo("MATERIAL") << endl
       << ":PLACE  tumor 1 world rmz 0. 0. 0." << endl
       << ":COLOUR tumor 1 0 0"  << endl << endl

       << ":VOLU   capaRu SPHERE 12*mm 12.3*mm 0 360*deg 0 40*deg vacio" << endl
       << ":PLACE  capaRu 1 world rmz 0. 0. 0." << endl
       << ":COLOUR capaRu 1 0 0"  << endl << endl

       << ":VOLU   chapa SPHERE 12.3 13.3 0 360*deg 0 40*deg gold" << endl
       << ":PLACE  chapa 1 world rmz 0. 0. 0." << endl
       << ":COLOUR chapa 0 1 1" << endl;
  }
  else {
    //capa agua izda:
    if(tvo("HALFLENGTHWATER") != "none") {
      string displ = "-" + tvo("HALFLENGTHWATER");
      fg << ":VOLU   cilindrow TUBE 0 " << tvo("RADIUS") << " "
	 << tvo("HALFLENGTHWATER") << " water" << endl
	 << ":PLACE  cilindrow 1 world rmz " << displ << " 0 0" << endl
	 << ":COLOUR cilindrow 0.2 1 0.2" << endl << endl;
    }

    istringstream iss( ToBlank(tvo("DETECTOR")) );
    iss >> DetName >> DetMat >> DetR >> DetDx >> DetX;
    istringstream issb( ToBlank(tvo("DETECTOR2")) );
    issb >> DetName2 >> DetMat2 >> DetR2 >> DetDx2 >> DetX2;
    istringstream iss2( ToBlank(tvo("DIAFRAGM")) );
    iss2 >> DiaName >> DiaMat >> DiaR >> DiaDx >> DiaX;

    if(ChName == "ch_box")
      fg << ":VOLU " << ChName << " BOX " << ChR <<" "<< ChR <<" "<< ChDx <<" "<< ChMat << endl;
    else
      fg << ":VOLU " << ChName << " TUBE 0 " << ChR << " " << ChDx << " " << ChMat << endl;

    fg << ":PLACE  " << ChName << " 1 world rmz " << ChX << " 0 0" << endl
       << ":COLOUR " << ChName << " 0.6 0.6 0.0" << endl << endl;


    if(DetName != "none")
      fg << "//Detector" << endl
	 << ":VOLU   " << DetName << " TUBE 0 " << DetR << " " << DetDx << " " << DetMat << endl
	 << ":PLACE  " << DetName << " 1 world rmz " << DetX << " 0 0" << endl
	 << ":COLOUR " << DetName << " 1 1 0" << endl << endl;

    if(DetName2 != "none")
      fg  << "//BckDetector" << endl
	  << ":VOLU   " << DetName2 << " TUBE 0 " << DetR2 <<" "<< DetDx2 <<" "<< DetMat2 << endl
	  << ":PLACE  " << DetName2 << " 1 world rmz " << DetX2 << " 0 0" << endl
	  << ":COLOUR " << DetName2 << " 1 1 0" << endl << endl;

    if(DiaName != "none")
      fg << "//Diafragm det" << endl
	 << ":VOLU   " << DiaName << " TUBE " << DiaR << " 0.9*" << ChR << " " << DiaDx
	 << " " << DiaMat << endl
	 << ":PLACE  " << DiaName << " 1 world rmz " << DiaX << " 0 0" << endl
	 << ":COLOUR " << DiaName << " 1 1 0" << endl << endl;

    if(GenName != "file" && GenName != "point") {
      fg << "//Fuentes" << endl;
      if(GenName == "g_box")
	fg << ":VOLU " << GenName << " BOX " << GenR <<" "<< GenR <<" "<< GenDx
	   << " " << GenMat << endl;
      
      fg << ":PLACE  " << GenName << " 1 world rmz " << GenX << " 0 0" << endl
	 << ":COLOUR " << GenName << " 1 0 0" << endl << endl;
    }
  }
  
  // else fg << ":VOLU " << GenName << " TUBE 0 "<< GenR <<" " << GenDx << " " << GenMat << endl;


  if(tvo("WRAPPER") != "none") {
    string ird = "0.89*" + tvo("WORLDBOX");
    string ord = "0.9*" + tvo("WORLDBOX");
    fg << ":VOLU wrapper SPHERE " << ird << " " << ord << " 0 360*deg 0 180*deg gold" << endl
       << ":PLACE  wrapper 1 world rmz 0. 0. 0." << endl
       << ":COLOUR wrapper 1 1 1" << endl;
    if(tvo("WRAPPER") == "invisible" )
      fg << ":VIS wrapper OFF" << endl;
  }

  fi.close();
  fg.close();

  if( tvo("BATCH") == "no" ) {
    command("cd " + tvo("DIRJOB") + "; gamos " + DirJob +
	    ".inp ; #find . -empty -exec rm {} \\;");
  }
  else if( tvo("BATCH") == "norun" ) {
    command("echo norun: only input files");
  }
  else {
    ofstream fj(fnj.c_str());
    fj << "###PBS -m e -M roldan -joe" << endl
       << "#PBS -l nodes=1,walltime=" << tvo("BATCH") << endl
       << "source $HOME/lepts/conflepts.sh" << endl
       << "cd $PBS_O_WORKDIR"               << endl
      //<< "mv * $TMPDIR ; cd $TMPDIR"       << endl
       << "echo $LEPTSDIR" << endl
       << "time gamos " << DirJob << ".inp" << endl
       << "#find . -empty -exec rm {} \\;"   << endl
      //<< "mv * $PBS_O_WORKDIR"              << endl
       << "#ssh euler mail -s leptsjob roldan@ciemat.es < gamos.log" << endl
       << endl;
    fj.close();
    command("scp -r " + tvo("DIRJOB") + " euler:batch");
    command("ssh euler 'cd batch/" + tvo("DIRJOB") + ";qsub " + DirJob + ".sh'");
  }

}
