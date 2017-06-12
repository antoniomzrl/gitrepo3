
#include "visu.hh"
#include <map>

extern float verboseFlag;


void printDbg(int vb, string text, string color) {
  if( vb > verboseFlag ) return;
  
  static map<string,string> cl;
  cl["RED"]    = "\033[1;31m";
  cl["GREEN"]  = "\033[1;32m";
  cl["BLUE"]   = "\033[1;34m";
  cl["YELLOW"] = "\033[1;33m";
  cl["PURPLE"] = "\033[1;35m";
  cl["NC"]     = "\033[0m";
  
  cout << cl[color] << text << cl["NC"];
}

void printDbg(int vb, string t, double x, string t2, string color) {
  ostringstream oss;
  oss << t << x << t2;
  printDbg(vb, oss.str(), color);
}

void printDbg(int vb, string text) {
  printDbg(vb, text, "NC");
}


void printgsl(mat4 m) {
  for( int i = 0; i < 4; i++) {
    for( int j = 0; j < 4; j++)
      cout << left << setw(12) << m[i][j];    
    cout << endl;
  }
}


void printgsl(vec4 v) {
  for( int i = 0; i < 4; i++)
    cout << left << setw(12) << v[i];
  cout << endl;
}


void printgsl(vec3 v) {
  for( int i = 0; i < 3; i++)
    cout << left << setw(12) << v[i];
  cout << endl;
}

