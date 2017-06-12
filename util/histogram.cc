#include "histogram.hh"
//#include "jm_general.hh"
 #include <math.h>

//extern jm_gral_var * jmGlobals;



AmrHistogram::AmrHistogram(int nB, double xL, double xH, const char * name) {
  InitHistogram(nB, xL, xH, name, " ");
}

AmrHistogram::AmrHistogram(double bW, double xL, double xH, const char * name) {
  int nB = (int)( (xH -xL)/bW );
  InitHistogram(nB, xL, xH, name, " ");
}

AmrHistogram::AmrHistogram(int nB, double xL, double xH, const char * name, const char * com) {
  InitHistogram(nB, xL, xH, name, com);
}

AmrHistogram::AmrHistogram(double bW, double xL, double xH, const char * name, const char * com) {
  int nB = (int)( (xH -xL)/bW );
  InitHistogram(nB, xL, xH, name, com);
}


// 1D Histogram
void AmrHistogram::InitHistogram(int nB, double xL, double xH, const char * name, const char * com)
{
  xLow  = xL;
  xHigh = xH;
  nBins = nB;

  histName = name;
  histComment = com;
  FileName = (string)name + ".hg";

#ifdef VERBOSE
  DataFileName = (string)name + ".d";
#endif
  nData      = 0;
  for(int i=0; i<6; i++) sum[i]=0;

  binWidth   = (xHigh -xLow)/double(nBins);
  foundMin   = xH;
  foundMax   = xL;
  nLostLeft  = 0;
  nLostRight = 0;

  //cout << "uuuuuuu " << binWidth << " " << xL << " " << xH << " " << nB << endl;
  cout << "InitHistogram " << this->histName << " [" << xL << " , " << xH << "] "
       << binWidth << " " << nB << " " << histComment << endl;

  wx = new double[nBins+2];  // 0 - nBins+1
  nx = new long[nBins+2];

  for( long bin=0; bin<(nBins+2); bin++) {    // After debug session ...
    wx[bin] = 0;
    nx[bin] = 0;
  }
  
  //string dfn = (string)jmGlobals->DirOut + "/" + FileName;
  string dfn = FileName;
  file.open(dfn.c_str()); 

  if( ! file.is_open() ) {
    cout << "Error opening file - PrintHistogram:" << dfn << endl;
  }
  
#ifdef VERBOSE 
  //if( strcmp(DataFileName, "FALSE") ) {
  //string dfn2 = (string)jmGlobals->DirOut + "/" + DataFileName;
  string dfn2 = DataFileName;
  dfile.open(dfn2.c_str()); 
  if( ! dfile.is_open() ) {
    cout << "Error opening file - PrintHistogram:" << dfn2 << endl;
  }

  char dxFileName[80];
  strcpy(dxFileName, name);
  strcat(dxFileName, ".dx");

  //string dfn3 = (string)jmGlobals->DirOut + "/" + (string)dxFileName;
  string dfn3 = (string)dxFileName;
  dxfile.open(dfn3.c_str()); 
  if( ! dxfile.is_open() ) {
    cout << "Error opening file - PrintHistogram:" << dxFileName << endl;
  }

  cout << "AmrHistogram init - " << this->histName << "\tNo bins: "
	 << nBins << "\tInterval:  [" << xLow << ", " << xHigh << "]" << endl;
#endif

}



AmrHistogram::~AmrHistogram()
{
  //cout << "destr " << this->histName << endl;

  Print();
  file.close();

#ifdef VERBOSE
  if( dfile.is_open() ) { //colour data file
    dfile.close();
  }

  if( dxfile.is_open() ) { // OpenDX header file
    dxfile << "file = "    << DataFileName     << endl
	   << "points = " << nData            << endl
	   << "format = ascii"                << endl
	   << "interleaving = field"          << endl
	   << "field = locations, Energy, Edep, TypeInterac"  << endl
	   << "structure = 3-vector, scalar, scalar, scalar"  << endl
	   << "type = float, float, float, float"             << endl
	   << "end"                                           << endl;
    dxfile.close();
  }
#endif

  delete wx;
  delete nx;
}



void AmrHistogram::AddDatum(double datum, double weight, const char * property)
{

  long bin = 1 + (long)( (datum-xLow)/(xHigh-xLow)*nBins );  // which bin in hist

  if( bin<0) {
    nx[0]     += 1;
    nLostLeft += 1;

#ifdef VERBOSE
    cout << "Error - AddDatum (out of range! oink!): "  << this->histName
	   << " bin: " << bin << " datum: " << datum  << " " << property << endl;
#endif
  }
  else if( bin>nBins) {
    nx[nBins+1] += 1;
    nLostRight  += 1;

#ifdef VERBOSE
    cout << "Error - AddDatum (out of range! oink!): " << this->histName
	   << " bin: " << bin << " datum: " << datum << " " << property << endl;
#endif
  }
  else {
    nx[bin] += 1;
    nData   += 1;
    wx[bin] += weight;
    for(int i=0; i<6; i++) sum[i] += weight*pow(datum,i);
  }

  if( datum < foundMin) foundMin = datum;
  if( datum > foundMax) foundMax = datum;

//   if(! strcmp( this->histName, "gasz.hist")) {
//     cout << "AmrHistogram - AddDatum " << this->histName
// 	   << " Datum: " << datum << " bin: " << bin
// 	   << " w: " << weight << " property: " << property << endl;
//   }
    
#ifdef VERBOSE
  cout << "AmrHistogram - AddDatum " << this->histName << " Datum: " << datum
	 << " bin: " << bin << " w: " << weight << " property: " << property << endl;

  if( nData<100000 && dfile.is_open() ) {
    //cout << std::right << std::setw(16) << datum << property << "bin" << bin << "kxkx1\n";
    //dfile << std::right << std::setw(16) << datum << property << "bin" << bin << endl;
    //dfile << std::right << std::setw(16) << datum << property << endl;
    dfile  << property << " " << datum << endl;
  }
#endif
}


void AmrHistogram::AddDatum(double datum, double weight)
{
  AddDatum(datum, weight, " ");
}

void AmrHistogram::AddDatum(double datum, const char * property)
{
  AddDatum(datum, 1.0, property);
}


void AmrHistogram::AddDatum(double datum)            // Add 1 datum
{
  AddDatum(datum, 1.0, "");                          // asume weight = 1
}



void AmrHistogram::Print()
{
  //std::ofstream file ( );
  //if( ! file.is_open() ) {
  //  cout << "Error opening file - PrintHistogram:" << FileName << endl;
  //}

  double M[6], m[6], k[6];
  for (int i=0; i<6; i++) M[i] = sum[i]/sum[0];

  m[0] = 1;
  m[1] = 0;
  m[2] = M[2] -pow(M[1],2);
  m[3] = M[3] -3.0*M[2]*M[1] +2.0*pow(M[1],3);
  m[4] = M[4] -4.0*M[3]*M[1] +6.0*M[2]*pow(M[1],2) -3.0*pow(M[1],4);
  m[5] = 0; //ya veremos

  k[0] = 0;
  k[1] = M[1];
  k[2] = m[2];
  k[3] = m[3];
  k[4] = m[4] - 3.0*pow(m[2],2);

  file << "# Histogram:           "  << this->histName << " (" << histComment << ")"
       << "\n# Interval selected:   [" << xLow          << " , " << xHigh    << "]"
       << "\n# Interval found:      [" << foundMin      << " , " << foundMax << "]"
       << "\n# Width:               " << foundMax-foundMin
       << "\n# Number of bins:      " << nBins
       << "\n# Bin width:           " << binWidth
       << "\n# Number of Data:      " << nData
       << "\n# Agregate Weight Sum: " << sum[0]
       << "\n# Agregate Data Sum:   " << sum[1]
       << "\n# Average:             " << M[1]
       << "\n# Agregate Square Sum: " << sum[2]
       << "\n#        Cuadr. Media: " << M[2]
       << "\n#                 Var: " << m[2]
       << "\n#               Sigma: " << sqrt(m[2])
       << "\n#             3 Sigma: " << 3.0*sqrt(m[2])
       << "\n#   Skewness  (m3/s3): " << m[3]/pow(sqrt(m[2]), 3)
       << "\n#   Kurtosis  (m4/s4): " << m[4]/pow(m[2], 2)
       << "\n\n";
  


  file << "\n# Moments         (Mi): ";
  for (int i=1; i<5; i++) file << std::setw(16) << M[i];

  file << "\n# Central Moments (mi): ";
  for (int i=1; i<5; i++) file << std::setw(16) << m[i];

  file << "\n# Cumulants       (ki): ";
  for (int i=1; i<5; i++) file << std::setw(16) << k[i];

  file << "\n# Cumulants/Avrg  (Fi): ";
  for (int i=1; i<5; i++) file << std::setw(16) << k[i]/M[1];

  file << "\n# Cumulants/(Av+1)    : ";
  for (int i=1; i<5; i++) file << std::setw(16) << k[i]/(M[1]+1);

  file << "\n# M1F2F3F4            : ";
  file << std::setw(16) << M[1];
  for (int i=2; i<5; i++) file << std::setw(16) << k[i]/M[1];

  if( (nLostLeft + nLostRight) > 0) {
    file << "\n# Warning!\n"
	 << "# Data lost left:  "  << nLostLeft  << "\n"
	 << "# Data lost right: "  << nLostRight << "\n\n\n";
  }
  else
    file << "\n#\n#\n#\n\n\n";

  file << std::right << std::setw(8)  << "#    bin"
       << std::right << std::setw(16) << "Xi"
       << std::right << std::setw(16) << "Ni"
       << std::right << std::setw(16) << "fi"
       << std::right << std::setw(16) << "fi/bw"
       << std::right << std::setw(16) << "Fi"
       << std::right << std::setw(16) << "Wi"
       << std::right << std::setw(16) << "Wi/bw"
       << endl;
  file << std::right << std::setw(8)  << "#    (1)"
       << std::right << std::setw(16) << "(2)"
       << std::right << std::setw(16) << "(3)"
       << std::right << std::setw(16) << "(4)"
       << std::right << std::setw(16) << "(5)"
       << std::right << std::setw(16) << "(6)"
       << std::right << std::setw(16) << "(7)"
       << std::right << std::setw(16) << "(8)"
       << endl << endl;

  //file.fill('#');
  //file.setf(std::ios_base::right, std::ios_base::adjustfield);

  if( nData > 0) {
    double Fi = 0;
    for( long i=1; i <= nBins; i++) {
      //if( nx[i]>0 || nx[i-1]>0 || nx[i+1]>0 || i==1 || i==nBins) {
      if( nx[i]>0 || nx[i-1]>0 || nx[i+1]>0) {
	double xi = xLow + binWidth/2.0 + (i-1)*binWidth;
	double fi = wx[i]/sum[0];

	Fi += fi;
	file << std::right << std::setw(8)  << i
	     << std::right << std::setw(16) << xi
	     << std::right << std::setw(16) << nx[i]
	     << std::right << std::setw(16) << fi
	     << std::right << std::setw(16) << fi/binWidth
	     << std::right << std::setw(16) << Fi
	     << std::right << std::setw(16) << wx[i]
	     << std::right << std::setw(16) << wx[i]/binWidth
	     << endl;
      }
    }
  }

#ifdef VERBOSE
  cout << "AmrHistogram - Print "<< this->histName  << endl;
#endif

  file.close();

}


int AmrHistogram::NumberOfData()
{
  return(nData);
}


double AmrHistogram::WeightSum()
{
  return(sum[0]);
}


double AmrHistogram::DataSum()
{
  return(sum[1]);
}

double AmrHistogram::Data2Sum()
{
  return(sum[2]);
}

double AmrHistogram::Average()
{
  return(sum[1]/sum[0]);
}




void InteractionCounter::AddDatum(int i, int j, int k)
{
  C[i][j][k]++;
  //cout << "InteractionCounter::AddDatum " << i << " " << j << " " << k << " " << C[i][j][k] << endl;
}

