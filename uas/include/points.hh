#ifndef POINTS_HH
#define POINTS_HH
#include <iostream>
#include <string>
using namespace std;

typedef struct ColPoint {
  double x, y, z;
  float  f1, f2, f3, f4; // E, lost, deposit, dens
  char tag[64];          // partname & procname
} ColPoint;


typedef struct Point {
  double x, y, z;
  float  f[7];
  // 0                          1           2        3  4     5     6
  //Part (1 gamma, 2 e-, 3 e+), TypeIntert, TrackID, E, Lost, Depo, Dens
} Point;



void InitTpi(string);
int TPI(string, int, int);
int TPI(string, int);
int TPI(string);
int PN(string );



typedef struct Point6 {
  double x, y, z;
  float  f1, f2, f3, f4; // E, lost, deposit, dens
  int i1, i2, i3;        // Part (1 gamma, 2 e-, 3 e+), TypeInteract, TrackID
} Point6;

typedef struct Point5 {
  double x, y, z;
  float  f1, f2, f3, f4; // E, lost, deposit, dens
  int i1, i2, i3;        // Part (1 gamma, 2 e-, 3 e+), TypeInteract, TrackID
  char tag[4];           // Material
} Point5;

typedef struct Point4 {
  double x, y, z;
  float  E, El, Ed, Dens; // E, lost, deposit, dens
  short int Pn, Ti, tid;  // Part (1 gamma, 2 e-, 3 e+), TypeOfInteraction, trackid
  char Mat[4];            // Material
} Point4;


typedef struct Point3 {
  double x, y, z;
  float  E, El, Ed, Dens; // E, lost, deposit, dens
  short int Pn, Ti;       // Particle (1 gamma, 2 e-, 3 e+) & TypeOfInteraction
  char Mat[4];            // Material
} Point3;


typedef struct Point2 {
  double x, y, z,
    t, // Type int
    e, // Energy
    l, // Eloss
    d; // Edep
    //c // Color
} Point2;


typedef struct Point1 {
  float e, x, y, z, t;
} Point1;


#define NNN 23
typedef struct PointArce {
  double f[NNN];
} PointArce;

typedef struct PointArce2 {
  double x, y, z;
  float f1, f2, f3, f4;
  int   i1, i2, i3;
  char tag[40];        // volume_in - volume_out
} PointArce2;

typedef struct PointArce1 {
  double x, y, z;
  double distDiff; 
  int eventID, codeID;
  char tag[40];        // volume_in - volume_out
} PointArce1;


#endif //POINTS_HH
