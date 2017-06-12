#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <math.h>

using namespace std;


string ToUpper(string str) {
  for(unsigned int i=0;i<str.length();i++)
    str[i] = toupper(str[i]);
  return str;
}


string Pad(int i, int N) {
  string si;
  if( N < 10)
    si = to_string(i);
  else
    si = string( 1+log10(N-1) -to_string(i).length(), '0') + to_string(i);
  return si;
}


string FindKeyWord( string s) {
  string other  = "other";
  string geom   = "geom";
  string input  = "input";
  string option = "option";

  if(  0 == s.find("/versi")   // back compatib
    || 0 == s.find("/dir")  
    || 0 == s.find("/jn")   
    || 0 == s.find("/btime")
    || 0 == s.find("--")     ) return option;
  
  if(     0 == s.find("///") ) return other; ///home/...
  else if(0 == s.find(":")   ) return geom;
  else if(0 == s.find("//")  ) return geom;
  else if(0 == s.find("#")   ) return input;
  else if(0 == s.find("/")   ) return input;
  else                         return other;
}


int main (int argc, char **argv) {
  string GamosVersion = "";
  string JobDir = "out";
  string JobName = "j";
  string BatchTime  = "00:01:00";

  string HostName = "local";
  string NoJobs = "1";
  string PPN = "8";
  string JPN = "8";
  string seed = "1", SEED = "1";
  bool Run = true;

  ostringstream gs, is, js;

  string ckey = "other";

  for(int i=1; i<argc; i++) {
    string key = FindKeyWord( string(argv[i]) );

    if(     key == "input") is << endl;
    else if(key == "geom")  gs << endl;

    if(key != "other") ckey = key;
    
    if( ckey == "geom")
      gs << argv[i] << " " ;
    else if( ckey == "input")
      is << argv[i] << " ";
    else if( ckey == "option") {
      string  s = string(argv[i]);
      string ss = string(argv[i+1]);
      
      if(      s == "--version" || s == "/version") GamosVersion = ss;
      else if( s == "--dir"     || s == "/dir"    ) { JobDir = ss; JobName = ss; }
      else if( s == "--jn"      || s == "/jn"     ) JobName = ss;
      else if( s == "--btime"   || s == "/btime"  ) BatchTime = ss;
      else if( s == "--host"    ) HostName = ss;
      else if( s == "--ppn"     ) { PPN = ss; JPN = ss; }
      else if( s == "--jpn"     ) JPN = ss;
      else if( s == "--jobs"    ) NoJobs = ss;
      else if( s == "--seed"    ) seed = ss;
      else if( s == "--SEED"    ) SEED = ss;
      else if( s == "--norun"   ) Run = false;
    }
  }


  string cmd = "mkdir -p " + JobDir;
  system( cmd.c_str() );
  
  // geom file
  string fng = JobName + ".geom";
  ofstream gFile( (JobDir + "/" + fng).c_str() );
  gFile << gs.str() << endl;
  gFile.close();

  int iNoJobs = stoi(NoJobs);
  int nlj = 0;
  string fnj, jfiles;
    
  for(int i=0; i<iNoJobs; i++) {
    int iseed = stoi(seed)+i;
    int ISEED = stoi(SEED)+i;
    string jn = JobName + "_" + Pad(iseed,iNoJobs) + "_" + Pad(ISEED,iNoJobs);

    // input file
    string fni = JobDir + "/" + jn + ".inp";
    ofstream iFile(fni.c_str());
    iFile << "#!/bin/sh" << endl
	  << "/gamos/setParam GmAnalysisMgr:FileNameSuffix " << JobName + "_" << endl
	  << "# GEOMETRY" << endl
	  << "/gamos/setParam GmGeometryFromText:FileName " << fng << endl
	  << "/gamos/geometry GmGeometryFromText" << endl
	  << "/gamos/setParam JobName " << jn << endl
	  << "#/gamos/random/setSeeds " << endl
	  << "/gamos/random/restoreSeeds " << iseed << " " << ISEED << endl
	  << is.str() << endl;
    iFile.close();

    // job file
    js << "time gamos " << jn << ".inp >& " << jn << ".log &" << endl;
    nlj++;
    
    if( (i+1)%stoi(JPN) == 0 || (i+1) == iNoJobs) { //last
      fnj = JobDir + "/" + jn + ".sh";
      jfiles += jn + ".sh ";
      js << "wait" << endl;

      ofstream jFile(fnj.c_str());
      jFile << "#!/bin/bash" << endl
	    << "#PBS        -l nodes=1:ppn=" << min(nlj, stoi(PPN)) << endl
	    << "#SBATCH --nodes=1 --ntasks=" << min(nlj, stoi(PPN)) << endl
	    << "#PBS           -l walltime=" << BatchTime << endl
	    << "#SBATCH             --time=" << BatchTime << endl
	    << "test -z $PBS_O_WORKDIR || cd $PBS_O_WORKDIR" << endl
	    << "source $HOME/lepts/bin/conflepts.sh " << GamosVersion << endl;
      jFile << js.str() << endl;
      jFile.close();
      js.str("");
      nlj = 0;
    }
  }

  if( HostName == "local" )
    cmd = "cd " + JobDir + "; for j in " + jfiles + " ; do bash $j ; done";
  else if( HostName == "euler" || HostName == "dirac" )
    cmd = "scp -r " + JobDir + " " + HostName + ":batch ; "
      + "ssh " + HostName + " 'cd batch/" + JobDir
      + "; for j in " + jfiles + " ; do qsub $j ; done'";
  else // ceta
    cmd = "scp -r " + JobDir + " " + HostName + ":batch ; "
      + "ssh " + HostName + " 'cd batch/" + JobDir
      + "; for j in " + jfiles + " ; do sbatch $j ; done'";
 
  cout << "executing " << cmd << endl;
  if( Run) {
    system( cmd.c_str() );
  }
}
