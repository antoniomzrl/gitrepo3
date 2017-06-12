#include "sgltn.hh"

MySingleton * MySingleton::theInstance = NULL;
MySingleton::MySingleton() { cout << "constructor" << endl; }
MySingleton::~MySingleton() { cout << "destructor" << endl; }
void MySingleton::method() { cout << "some method" << endl; }

MySingleton * MySingleton::getInstance() {
  if( theInstance) {
    cout << "other instance" << endl;
  }
  else {
    theInstance = new MySingleton();
    cout << "FIRST INSTANCE" << endl;
  }
  return theInstance;
}



FileMgr * FileMgr::theInstance = NULL;
FileMgr::FileMgr() { cout << "constructor" << endl; }
FileMgr::~FileMgr() { cout << "destructor" << endl; }
void FileMgr::SeeFile() { cout << "some method" << endl; }

FileMgr * FileMgr::getInstance() {
  if( !theInstance) {
    theInstance = new FileMgr();
    cout << "FIRST INSTANCE" << endl;
  }
  else {
    cout << "other instance" << endl;
  }
  return theInstance;
}
