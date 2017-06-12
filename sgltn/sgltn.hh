#include <iostream>
using namespace std;

class MySingleton {
private:
  static MySingleton * theInstance;
  MySingleton();

public:
  static MySingleton * getInstance();
  void method();
  ~MySingleton();
};



class FileMgr {
private:
  static FileMgr * theInstance;
  FileMgr();
  vector <string> file;
  
public:
  static FileMgr * getInstance();
  void SeeFile(string);
  ~FileMgr();
};
