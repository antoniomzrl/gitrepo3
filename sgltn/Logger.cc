
#include "Logger.hh"


Logger * Logger::theInstance = NULL;


Logger::Logger() {
  cout << "Logger Constructor" << endl;
}


static Logger * Logger::getInstance() {
  if( !theInstance) {
    theInstance = new Logger;
    cout << "Logger FIRST instance" << endl;
  }
  else cout << "Logger other instance" << endl;

  return theInstance;
}


void Logger::Write( string str) {

  for(unsigned int i=0; i<lines.size(); i++) {
    if( str == lines[i]) {
      //cout << str << " ddd dup !!!" << endl;
      return;
    }
  }
  
  lines.push_back(str);
  cout << str << " xxx " << lines.size() << endl;
}
