
extern bool Debug;

#include "visu.hh"


#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#endif


//#include <iostream>
//using namespace std;


double myclock() {
  double clk;

#ifdef _OPENMP
  clk = omp_get_wtime();
  //cout << "myclock omp_wtime()" << endl;
#else
  clk = clock()/ (double)CLOCKS_PER_SEC;
  //cout << "myclock clock()" << endl;
#endif

  return clk;
}


#include <sys/stat.h>
unsigned long GetBinaryFileSize(const char *pathname) {
  struct stat sb;
  if(stat(pathname, &sb) < 0) {
    cout << "getFileSize stat\n";
    exit(-1);
  }
  return sb.st_size;
}


unsigned long GetFileSize(char * Fn) {
  if(Debug) cout << "GetFileSize" << endl;

  string str;
  unsigned long c(0);

  ifstream fi(Fn);

  while( getline(fi, str) ) {
    c++;
    //if( c%1000 == 0 ) cout << "line: " << c << endl;
  }

  fi.close();
  return c;
}



void FlagProc(vector <ColPoint> s, vector <Point> r) {
  cout << "FlagProc sz1 " << s.size() << " sz2 " << r.size() << endl;
  for(unsigned long i=0; i< s.size(); i++) {
    r[i].x = s[i].x;
    r[i].y = s[i].y;
    r[i].z = s[i].z;
    r[i].f[3] = s[i].f1;
    r[i].f[4] = s[i].f2;
    r[i].f[5] = s[i].f3;
    r[i].f[6] = s[i].f4;
    r[i].f[0] = i%3;
    r[i].f[1] = i%3;
    r[i].f[2] = i%3;
  }
  cout << "fin FlagProc " << endl;
  float f; 
  cin >> f;
}


bool FlagPoint(Point *p, ColPoint *q) {
  p->x = q->x;
  p->y = q->y;
  p->z = q->z;
  p->f[3] = q->f1;
  p->f[4] = q->f2;
  p->f[5] = q->f3;
  p->f[6] = q->f4;

  string s = (string)(q->tag);

  p->f[0] = PN(s);
  p->f[1] = TPI(s);
  
  // if( s.find("e-") != NPOS) {
  //   //p->f[0] = 1;
  //    p->f[1] = TPI(s);
  //    //if( s.find("Ioni") !=NPOS && q->f[4] > 250.0) p->f[1] = TPI((string)"Auger");
  // }
  // else if(s.find("e+") != NPOS) {
  //   //p->f[0] = 2;
  //    p->f[1] = TPI(s);
  // }
  // else if( s.find("proton") != NPOS || s.find("hydrogen") != NPOS) {
  //   //p->f[0] = 3;
  //   if(      s.find("Elastic")    != NPOS) p->f[1] = TPI((string)"pElastic");
  //   else if( s.find("Excitation") != NPOS) p->f[1] = TPI((string)"pExcitation");
  //   //else if( s.find("ChargeInc")  != NPOS) p->f[1] = TPI((string)"pCharge+");
  //   //else if( s.find("ChargeDec")  != NPOS) p->f[1] = TPI((string)"pCharge-");
  //   else if( s.find("Ioni")       != NPOS) p->f[1] = TPI((string)"pIoni");
  //   else                                   p->f[1] = TPI(s);
  
  // }
  // else {    // gamma
  //   //p->f[0] = 0;
  //   p->f[1] = TPI(s);
  // }

  return true;
    
  // if(p->f[1] < 21) return true;
  // else           return false;
}


DataObject * ReadBinaryFile(string fn, string type) {
  //if(Debug)
  cout << "ReadBinaryFile (" << fn << ") " << type << endl;
  cout << " double " << sizeof(double)
       << " float " << sizeof(float)
       << " int " << sizeof(int)
       << " sh int " << sizeof(short int)
       << " Point " << sizeof(Point) << endl;

  double T1 = myclock();
  int SP = sizeof(struct Point);
  int SCP = sizeof(struct ColPoint);
  DataObject * d = new DataObject();
  unsigned long ND=0;
  int fr;
    
  if( type == string("BCDGZ") ) { //compressed
    Point pt;
    int fd = open(fn.c_str(), O_RDONLY);
    if(fd == -1) cout << "error open (" << fn << ")\n";
    gzFile gzfi = gzdopen(fd, "rb");
  
    while( (fr=gzread(gzfi, &pt, SP) ) == SP )
      (d->r).push_back(pt);
    gzclose(gzfi);
    ND = (d->r).size();
    cout << "rbfgz ";
  }
  else if( type == string("BCD") ) { //binary 
    FILE * fi = fopen(fn.c_str(), "rb");

    //Point pt;
    //while( (fr=fread(&pt, SP, 1, fi) ) == 1 )
    //(d->r).push_back(pt); //slooow in MS win
    //ND = (d->r).size();

    ND = GetBinaryFileSize(fn.c_str() ) / SP;
    (d->r).reserve(ND);
    (d->r).resize(ND);
    fr = fread(&d->r[0], SP, ND, fi);
    fclose(fi);
    cout << "rbf bcd " << fr << " " << SP << endl;
  }
  else if( type == string("BIN") ) { //binary
    Point p;
    ColPoint q;
    int fd = open(fn.c_str(), O_RDONLY);
    if(fd == -1) cout << "error opening (" << fn << ")\n";
    gzFile gzfi = gzdopen(fd, "rb");

    while( (fr=gzread(gzfi, &q, SCP) ) == SCP ) {
      if( FlagPoint(&p, &q) == true )
	(d->r).push_back(p);
    }

    gzclose(gzfi);
    ND = (d->r).size();

    cout << "rbf bin " << fr << " " << SCP << endl;
  }
  else if( type == string("ARCE") ) {
    Point pt;
    PointArce pta;
    int SPA = sizeof(struct PointArce);
    int fd = open(fn.c_str(), O_RDONLY);
    if(fd == -1) cout << "error open (" << fn << ")\n";
    gzFile gzfi = gzdopen(fd, "rb");
  
    //bool First = true;

    while( (fr=gzread(gzfi, &pta, SPA) ) == SPA ) {
      for(int i=0; i<NNN; i++)
	cout << pta.f[i] << " ";
      cout << endl;

      pt.x = pta.f[0];
      pt.y = pta.f[1];
      pt.z = pta.f[2];
      pt.f[1] = rand();
      (d->r).push_back(pt);
    }

    gzclose(gzfi);
    ND = (d->r).size();
    cout << "rbfArce size " << SPA << endl;
  }

  d->NumDat = ND;
  d->MaxDat = ND;
  d->MinDat = 0;
  d->FileName = fn;
  d->FileType = type;

  cout << "ReadBinFile ND: " << d->NumDat << " (" << myclock()-T1 << " sec)" << endl;
  //cout << "xyz " << d->r[0].x << " " << d->r[0].y << " " << d->r[0].z << endl;

  ComputeHistogram(d);
  ComputeFrame(d);
  d->NeedUpdatePV = true;
  return(d);
}


Point StringToPoint(string str) {
  istringstream iss(str);
  Point r;

  iss >> r.x >> r.y >> r.z >> r.f[3] >> r.f[4] >> r.f[5] >> r.f[6]
      >> r.f[0] >> r.f[1] >> r.f[2];

  return(r);
}


DataObject * ReadInputFile(char * Fn) {
  if(Debug) cout << "ReadInputFile" << endl;

  ifstream fi(Fn, ios::in);
  if(! fi.is_open()) {
    cout << "Unable to open file " << Fn << endl; 
    exit(-1);
  }

  DataObject * d = new DataObject();
  string line;

  while( fi.good() ) {
    getline(fi, line);
    // cout << line << endl;
    Point pt = StringToPoint(line);
    (d->r).push_back(pt);
  }

  fi.close();

  d->NumDat = (d->r).size();
  d->MaxDat = d->NumDat;
  d->MinDat = 0;
  ComputeFrame(d);
  d->NeedUpdatePV = true;
  return(d);
}


void WriteBinaryCompressFile(DataObject * d, char * Fn) {
  if(Debug) cout << "WriteBinaryCompressFile" << endl;

  char FnB[160];
  strcpy(FnB, "save.bcd.gz");
  cout << "WriteBinaryCompressFile (" << Fn << ") (" << FnB << ")" << endl;
  cout << "wbf " << sizeof(Point) << " | " <<sizeof(vector<Point>) << endl;

  int fdo = open(FnB, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if(fdo == -1) cout << "eror open (" << FnB << ")\n";

  gzFile gzfo = gzdopen(fdo, "wb");
  if (gzfo==NULL) printf("gzopen error\n");

  int Nd = sizeof(struct Point) * d->NumDat;
  int fr = gzwrite(gzfo, &d->r[0], Nd);
  if( fr != Nd) cout << "error writing " << FnB << endl;

  gzclose(gzfo);
}


void WriteBinaryFile(DataObject * d, char * fn) {
  FILE * fi = fopen(fn, "wb");
  //int fw = fwrite(r, sizeof(Point), N, fi);
  double sc = 1/d->SpatialFactor;
  ScaleData(d, sc, sc, sc);
  int fw = fwrite(&d->r[0], sizeof(Point), d->NumDat, fi);
  cout << "WriteBinaryFile(" << fn << ") (" << fn << ") " << d->NumDat << " " << fw << endl;
  cout << "wbf " << sizeof(Point) << " | " <<sizeof(vector<Point>) << endl;
  fclose(fi);
}


void WriteBinaryCompressFile(DataObject * d) {
  static int n=1;
  char fn[160];
  sprintf(fn, "save%02d.bcd.gz", n);
  n++;
  WriteBinaryCompressFile(d, fn);
}


void WriteBinaryFile(DataObject * d) {
  static int n=1;
  char fn[160];
  sprintf(fn, "save%02d.bcd", n);
  n++;
  WriteBinaryFile(d, fn);
}


//#include <sys/stat.h>
bool FileExists(char * fn) {
  struct stat st;
  int ist = stat(fn, &st);

  if(ist == 0)
    return true;
  else
    return false;
}


void SaveParamFile(DataObject * d) {
  static int n = 0;
  n++;

  char fn[160];
  sprintf(fn, "param%0d.pf", n);

  if(FileExists(fn)) {
    SaveParamFile(d);
    return;
  }

  cout << "SaveParamFile (" << fn << ")" << endl;

  std::ofstream fi(fn, std::ios::out);
  if(!fi) { // file couldn't be opened
    cerr << "Error open file (" << fn << ")" << endl;
    exit(-1);
  }

  fi //<< "FileName     " << d->FileName     << endl
     << "BallSize     " << d->BallSize     << endl
     << "BallFactor   " << d->BallFactor   << endl
     << "Background   " << d->Background   << endl  
     << "vAxis        " << d->vAxis        << endl
     << "vFrame       " << d->vFrame       << endl
     << "vFrameLabels " << d->vFrameLabels << endl
     << "vColorBar    " << d->vColorBar    << endl
     << "vSelection   " << d->vSelection   << endl  
     << "vColorScale  " << d->vColorScale  << endl
     << "vLorLines    " << d->vLorLines    << endl
     << "vBalls       " << d->vBalls       << endl
     << "vTracks      " << d->vTracks      << endl
     << "vElastic     " << d->vElastic     << endl
     << "vType        " << d->vType        << endl
     << "Stride       " << d->Stride       << endl
     << "IcosahedronLevel " << d->IcosahedronLevel   << endl  
     << "SpatialUnits " << d->SpatialUnits << endl;

  fi.close();
}



void ReadParamFile(char * fn, DataObject * d) {

  std::ifstream fi(fn, std::ios::in);
  if(!fi) { // file couldn't be opened
    cerr << "Error open file (" << fn << ")" << endl;
    exit(-1);
  }

  char bf[32]; //, fb[32];

  fi >> bf >> d->BallSize
     >> bf >> d->BallFactor
     >> bf >> d->Background
     >> bf >> d->vAxis
     >> bf >> d->vFrame
     >> bf >> d->vFrameLabels
     >> bf >> d->vColorBar
     >> bf >> d->vSelection
     >> bf >> d->vColorScale
     >> bf >> d->vLorLines
     >> bf >> d->vBalls
     >> bf >> d->vTracks
     >> bf >> d->vElastic
     >> bf >> d->vType
     >> bf >> d->Stride
     >> bf >> d->IcosahedronLevel
     >> bf >> d->SpatialUnits;

  fi.close();
}


void WriteASCIIFile(DataObject * d) {
  if(Debug) cout << "WriteASCIIFile" << endl;

  char FnA[160];
  strcpy(FnA, "save.txt");
  cout << "WriteASCIIFile(" << d->FileName << ") (" << FnA << ")" << endl;
  cout << "waf " << sizeof(Point) << " " << d->NumDat << endl;

  std::ofstream fi(FnA, std::ios::out);
  if(!fi) { // file couldn't be opened
    cerr << "Error open file (" << FnA << ")" << endl;
    exit(-1);
  }

  for(unsigned long i=0; i< d->NumDat; i++) {
    fi << d->r[i].x  << " " << d->r[i].y  << " " << d->r[i].z  << " "
       << d->r[i].f[3] << " " << d->r[i].f[4] << " " << d->r[i].f[5] << " "
       << d->r[i].f[6] << " " << d->r[i].f[0] << " " << d->r[i].f[1] << " "
       << d->r[i].f[2] << endl;
  }

  fi.close();
}

/*
DataObject * ReadInputFile1(char * Fn) {

  double T1 = myclock();
  cout << "ReadInputFile (" << Fn << ")" << endl;

  // no file
  if( strlen(Fn) == 0) {
    DataObject * d = new DataObject();
    d->NumDat = 0;
    return(d);
  }

  unsigned long ND = GetFileSize(Fn);
  cout << "ND: " << ND << endl;

  DataObject * d = new DataObject();
  //d->r = new Point[ND];

  strcpy(d->FileName, Fn);
  cout << "ReadInputFile fn (" << d->FileName << ")" << endl;

  //C++ 
  std::ifstream fi(Fn ,std::ios::in);
  if(!fi) { // file couldn't be opened
    cerr << "Error open file (" << Fn << ")" << endl;
    exit(-1);
  }

  int i=0;
  float t, E, Ed;
  while(fi >> d->r[i].x >> d->r[i].y >> d->r[i].z
    >> t >> d->r[i].E >> d->r[i].Ed >> d->r[i].El >> d->r[i].Ti ) {
    //cout << "kkk " << d->r[i].e << endl;
    i++;
    //if( i%1000 == 0 ) cout << "line: " << i << endl;
  }

  // string s;
  // float x4, x6, x7;
  // while( getline(fi, s) ) {
  //   if(i<6) cout << "(" << s << ")" << endl;

  //   sscanf(s.c_str(), "%g%g%g%g%g%g%g%g",&d->r[i].x, &d->r[i].y,
  //      &d->r[i].z, &x4, &d->r[i].e, &x6, &x7, &d->r[i].t);
  //   cout << "kkk " << x6 << " " << d->r[i].e << endl;
  //   i++;
  //   if( i%1000 == 0 ) cout << "line: " << i << endl;
  // }
  
  fi.close();

  //  cout << i << "fin "<< d->r[5].x << " " << d->r[5].e << " " << d->r[5].t << endl;

  // C 
  // FILE * fi = fopen(Fn, "r");
  // if( fi==NULL ) cout << "Error opening file (" << fi << ")" << endl;
  
  // cout << "kkkkk " << d->r[5].x << endl;

  // while( fscanf(fi, "%g %g %g %g %g %g %g %g", &d->r[i].x, &d->r[i].y,
  //     &d->r[i].z, &t, &d->r[i].e, &Ed, &El, &d->r[i].t) != EOF) {
  //   if(i<10)
  //     cout << d->r[i].x << " " << d->r[i].y << " " << t << " "
  //      << d->r[i].e << " " << d->r[i].t << endl;
  //   i++;
  //   //if( i%1000 == 0 ) cout << "line: " << i << endl;
  // }

  // fclose(fi);

  ND = i;
  cout << "ND2 " << ND << endl;

  d->NumDat = ND;
  d->MaxDat = ND;
  d->MinDat = 0;

  double T2 = myclock();
  cout << "\t(" << T2-T1 << " sec)" << endl;

  //WriteBinaryFile(d->r, ND, Fn);
  ComputeFrame(d);
  return(d);
}
*/
