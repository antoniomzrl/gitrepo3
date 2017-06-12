
#include "visu.hh"


extern DataObject * d;


int main (int argc, char **argv) {

  char iFn[160], oFn[160];
  
  for(int i=0; i<argc; i++) {
    if( (string)argv[i]==(string)"-i")
      strcpy(iFn, argv[i+1]);
    if( (string)argv[i]==(string)"-o")
      strcpy(oFn, argv[i+1]);
  }

  int fr;
  Point1 pi;
  Point  po;

  FILE * fi = fopen(iFn, "rb");
  FILE * fo = fopen(oFn, "wb");

  while( (fr=fread(&pi,sizeof(Point1) ,1,fi) ) == 1 ) {
    // cout << pi.x << " " << pi.y << " " << pi.z << " "
    // 	 << pi.t << " " << pi.e << endl;
    po.x = pi.x;
    po.y = pi.y;
    po.z = pi.z;
    po.Ti = pi.t;
    po.E  = pi.e;
    // po.El = pi.l;
    //po.Ed = pi.d;
    fwrite(&po, sizeof(Point), 1, fo);
  }
}
