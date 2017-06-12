
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Logger{

private:
  static Logger * theInstance;
  Logger();                             // Private so that it can  not be called
  //Logger(Logger const&){};            // copy constructor is private
  //Logger& operator=(Logger const&){}; // assignment operator is private
  vector <string> lines;
  
public:
  static Logger * getInstance();
  void Write( string);
};
