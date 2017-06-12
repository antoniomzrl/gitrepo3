
//#include "G4ios.hh"
#include <fstream>
#include <iostream>
#include <iomanip>

// #include <iostream>
// #define double double
// #define int    int
// #define G4cout   std::cout
// #define G4endl   std::endl


#include <string>
using namespace std;

class AmrHistogram {

  double xLow, xHigh;
  long nBins;

  long   nData;
  double sum[6];             // sum wi, wi*xi, wi*xi2, wi*xi3, wi*xi4, wi*xi5;
  double binWidth;
  double foundMin, foundMax; // min & max values found
  long   nLostLeft, nLostRight;

  long   * nx;
  double * wx;

  const char * histName, * histComment;


  string FileName;
  string DataFileName;

  std::ofstream file, dfile, dxfile;      //output file's handles

  //void InitHistogram(int, double, double, const char *, const char* ="FALSE");
  void InitHistogram(int, double, double, const char *, const char *);

public:
  AmrHistogram();                                    // Empty const.
  AmrHistogram(int,    double, double, const char *);
  AmrHistogram(double, double, double, const char *);
  AmrHistogram(int,    double, double, const char *, const char *);
  AmrHistogram(double, double, double, const char *, const char *);
  ~AmrHistogram();

  void AddDatum(double, double, const char *);    // Add 1 datum & weight & property
  void AddDatum(double, double);                  // Add 1 datum & weight
  void AddDatum(double, const char *);            // Data + porperty
  void AddDatum(double);                          // Add 1 datum
  void Print();                                   // Somehow show hist.
  int NumberOfData();
  double WeightSum();
  double DataSum();
  double Data2Sum();
  double Average();

  void Test();
};



// Counters
class InteractionCounter {

public:
  int nElast, nInelast;

  int C[2][100][100]; // In/Out, NE, NI
  char inout[2], tmp1[9], tmp2[9], header[800], numbers[800];

  int Nd00;
  InteractionCounter();
  ~InteractionCounter() {};
  void AddDatum(int, int, int);
  void Print();
};
