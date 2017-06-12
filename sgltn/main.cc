
#include "sgltn.hh"

int main() {
  MySingleton * s1 = MySingleton::getInstance();
  MySingleton * s2 = MySingleton::getInstance();
  MySingleton * s3 = MySingleton::getInstance();
  s1->method();
  s2->method();
  s3->method();
}
