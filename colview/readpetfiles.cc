
#include "colv.hh"

typedef struct PetRead {   //Arce & Oscar
  char name[8];
  float rA[3], r1[3], r2[3];
} PetOutRead;


DataObject * ReadPetFile(char * Fn) {

  cout << "ReadPetFile (" << Fn << ")" << endl;

  int SZ = sizeof(struct PetRead);
  int NumLin = GetBinaryFileSize(Fn)/SZ;
  unsigned long ND = NumLin*3;
  cout << "NumLin: " << NumLin << " ND: " << ND << endl;
  PetRead pr;

  DataObject * d = new DataObject();
  //d->r = new Point[ND];

  FILE * fpr = fopen(Fn, "rb");
  int fr, i=0;

  while( (fr=fread (&pr, SZ,1,fpr) ) == 1 ) {
    d->r[i].x = pr.rA[0];
    d->r[i].y = pr.rA[1];
    d->r[i].z = pr.rA[2];
    d->r[i].f[1] = 6;        //source
    d->r[i+1].x = pr.r1[0];
    d->r[i+1].y = pr.r1[1];
    d->r[i+1].z = pr.r1[2];
    d->r[i+1].f[1] = 0;        //lor
    d->r[i+2].x = pr.r2[0];
    d->r[i+2].y = pr.r2[1];
    d->r[i+2].z = pr.r2[2];
    d->r[i+2].f[1] = 0;        //lor
    i+=3;
  }

  cout << "ND2: " << i << endl;

  ComputeFrame(d);
  return(d);
}

