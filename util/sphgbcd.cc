#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <math.h>
using namespace std;


typedef struct Point {
  double x, y, z;
  float  E, El, Ed, Dens; // E, lost, deposit, dens
  int Pn, Ti, tid;  // Part (1 gamma, 2 e-, 3 e+), TypeOfInteraction, trackid
  char Mat[4];            // Material
} Point;



class SpatialHg {
  int    Nx, Ny, Nz, Nxyz, Nyz;
  double xL, xH, yL, yH, zL, zH; // Low High
  double dx, dy, dz;             // binwidth
  double Dx, Dy, Dz;             // width

  long   * N;
  double * E;

public:
  SpatialHg(int, int, int, double, double, double, double, double, double);
  SpatialHg(string);
  ~SpatialHg();
  void Init(int, int, int, double, double, double, double, double, double);
  void AddData(double, double, double, double, int);
  void SaveHg(string fn);
  void SaveHg();
  void WriteHeaderFile(string, const char *, int);
};


void SpatialHg::Init(int nx, int ny, int nz, double xl, double xh,
		     double yl, double yh, double zl, double zh) {

  cout << "SpatialHg: " << nx << " " << ny << " " << nz << " "
       << xl << " " << xh << " " << yl << " " << yh << " " << zl << " " << zh
       << endl;

  Nx = nx;  xL = xl;  xH = xh;
  Ny = ny;  yL = yl;  yH = yh;
  Nz = nz;  zL = zl;  zH = zh;
  Nxyz = Nx*Ny*Nz;
  Nyz  = Ny*Nz;

  Dx = (xH - xL);
  Dy = (yH - yL);
  Dz = (zH - zL);

  dx = Dx/Nx;
  dy = Dy/Ny;
  dz = Dz/Nz;

  try {
    E = new double[Nxyz];
    N = new   long[Nxyz];
    cout << "SpatialHg Nxyz: " << Nxyz << endl;
  }
  catch( bad_alloc) {
    cerr << "Memory exhausted SpatialHg: " << Nxyz << endl;
    exit(1);
  }


  for(int i=0; i<Nxyz; i++) {
    E[i] = 0;
    N[i] = 0;
  }
}

SpatialHg::SpatialHg(int nx, int ny, int nz, double xl, double xh,
		     double yl, double yh, double zl, double zh) {
  Init(nx, ny, nz, xl, xh, yl, yh, zl, zh);
}

SpatialHg::SpatialHg(string strhg) {
  int nx, ny, nz;
  double xl, xh, yl, yh, zl, zh;

  cout << "SpatialHg(str): " << strhg << endl;

  for(int i=0;i<strhg.length();i++)
    if( strhg[i] == ',' || strhg[i] == '_' || strhg[i] == '/')
      strhg[i] = ' ';

  istringstream iss(strhg);
  iss >> nx >> xl >> xh >> ny >> yl >> yh >> nz >> zl >> zh;
  Init(nx, ny, nz, xl, xh, yl, yh, zl, zh);
}



SpatialHg::~SpatialHg() {
  cout << "SpatialHg destructor" << endl;

  SaveHg();
}



void SpatialHg::AddData(double x, double y, double z, double Ed, int Ni) {
  //  int i =  (int)( (x-xL)/dx );
  //  int j =  (int)( (y-yL)/dy );
  //  int k =  (int)( (z-zL)/dz );
  int i = lrint( (x-xL)/dx -0.5);
  int j = lrint( (y-yL)/dy -0.5);
  int k = lrint( (z-zL)/dz -0.5);

  if( i>=0 && j>=0 && k>=0 && i<Nx && j<Ny && k<Nz) {
    E[Nyz*i +Nz*j +k] += Ed; //[i][j][k]
    N[Nyz*i +Nz*j +k] += Ni;
    //if(y< -5.5) 
    //cout << "sphg-add: " << z << " " << y << " dif " << y-yL << " j " << j << endl;
  }
  else {
    cout << "Error SpHg AddData (" << x << " , "  << y << " , "  << z << ") (" 
	 << i << " , "  << j << " , "  << k << ")" << endl;
  }

}


void SpatialHg::SaveHg( string Fn) {
  cout << "SaveHg: " << endl;

  string fn = Fn + (string)".bcd";
  string fna = Fn + (string)".txt";

  FILE * fi = fopen(fn.c_str(), "wb");

  std::ofstream fa(fna.c_str(), std::ios::out);
  if(!fa) {
    cerr << "Error open file (" << fna << ")" << endl;
    exit(-1);
  }

  Point r;
  for(int i=0; i<Nx; i++) {
    for(int j=0; j<Ny; j++) {
      for(int k=0; k<Nz; k++) {
	r.x = xL + dx*(i+0.5);
	r.y = yL + dy*(j+0.5);
	r.z = zL + dz*(k+0.5);
	r.Ed = E[Nyz*i +Nz*j +k];
	r.Ti = N[Nyz*i +Nz*j +k];
	if( r.Ed>0 || r.Ti>0 ) {
	  fwrite(&r, sizeof(Point), 1, fi);
	  // fa << r.x << " " << r.y << " " << r.z << " "
	  //    << r.Ed << " " << r.Ed << " " << r.Ed << " " << r.Ed << " "
	  //    << r.Ti << " " << r.Ti << " " << r.Ti << " " << "xxx" << endl;
	  fa << r.x << " " << r.Ed << endl;
	}
      }
    }
  }
  fclose(fi);
  fa.close();

  string fn2 = (string)"d3hg" + (string)"-E.bin";
  FILE * fi2 = fopen(fn2.c_str(), "wb");
  fwrite(E, sizeof(double), (Nxyz), fi2);
  fclose(fi2);
  WriteHeaderFile(fn2, "float", sizeof(double));

  string fn3 = (string)"d3hg" + (string)"-N.bin";
  FILE * fi3 = fopen(fn3.c_str(), "wb");
  fwrite(N, sizeof(long), (Nxyz), fi3);
  fclose(fi3);
  WriteHeaderFile(fn2, "integer", sizeof(long));
}



void SpatialHg::SaveHg() {
  SaveHg( (string)"sphg");
}



void SpatialHg::WriteHeaderFile(string fn, const char * datatype, int datasize) {
  string fn2 = fn + ".hv";
  FILE * fi = fopen(fn2.c_str(), "w");

  fprintf(fi, "!INTERFILE  := \n");
  fprintf(fi, "name of data file := %s\n", fn.c_str());
  fprintf(fi, "!GENERAL DATA :=\n");
  fprintf(fi, "!GENERAL IMAGE DATA :=\n");
  fprintf(fi, "!type of data := PET\n");
  fprintf(fi, "imagedata byte order := LITTLEENDIAN\n");
  fprintf(fi, "!PET STUDY (General) :=\n");
  fprintf(fi, "!PET data type := Image\n");
  fprintf(fi, "process status := Reconstructed\n");
  fprintf(fi, "!number format := %s\n", datatype);

  //"!number format" Value must be one of ASCII, short float, signed integer or unsigned integer.
  //"!number of bytes per pixel" Value must be 1 or 2 for integer types, 4 or 8 for float types, ignored for ASCII type. 
  fprintf(fi, "!number of bytes per pixel := %d\n", datasize);
  fprintf(fi, "number of dimensions := 3\n");
  fprintf(fi, "matrix axis label [1] := z\n");
  fprintf(fi, "!matrix size [1] := %d\n", Nz);
  fprintf(fi, "scaling factor (mm/pixel) [1] := %g\n", dz);
  fprintf(fi, "matrix axis label [2] := y\n");
  fprintf(fi, "!matrix size [2] := %d\n", Ny);
  fprintf(fi, "scaling factor (mm/pixel) [2] := %g\n", dy);
  fprintf(fi, "matrix axis label [3] := x\n");
  fprintf(fi, "!matrix size [3] := %d\n", Nx);
  fprintf(fi, "scaling factor (mm/pixel) [3] := %g\n", dx);
  fprintf(fi, "number of time frames := 1\n");
  fprintf(fi, "image scaling factor[1] := 1\n");
  fprintf(fi, "data offset in bytes[1] := 0\n");
  fprintf(fi, "quantification units := 1\n");
  fprintf(fi, "!END OF INTERFILE := \n");
  fclose(fi);
}






int main (int argc, char **argv) {
  string hgstr = "100,-500,500,100,-500,500,100,-500,500";
  string fno = "jhg";

  for(int i=0; i<argc; i++) {
    if( (string)argv[i] == (string)"-hg" )
      hgstr = argv[i+1];
    if( (string)argv[i] == (string)"-o" )
      fno = (string) argv[i+1];
  }

  SpatialHg * sphg = new SpatialHg(hgstr);

  int fr;
  Point pt;
  int SP = sizeof(struct Point);

  for(int i=0; i<argc; i++) {
    if( strstr(argv[i], ".bcd") ) {
      string fn = (string)argv[i];
      cout << "fn: " << fn << endl;
      FILE * fi = fopen(fn.c_str(), "rb");
      while( (fr=fread(&pt, SP, 1, fi) ) == 1 )
	sphg->AddData(pt.x, pt.y, pt.z, pt.Ed, 1);
      fclose(fi);
    }
  }

  sphg->SaveHg( fno );
}
