
#include "visu.hh"
extern DataObject * d;
extern GtkWidget * glw;

extern float FoV, AR,
  theHorizAngle, theVertAngle, theZoom,
  theHorizDisp, theVertDisp, theDepthDisp;
extern mat4 Rm;

void InitialPosition() {
  theHorizAngle = 0; //glm::pi<float>(); // Initial
  theVertAngle  = 0;
  theHorizDisp  = 0;
  theVertDisp   = 0;
  theDepthDisp  = 0;
  theZoom       = 1;
}


void RotateScene(int dx, int dy, int dz) {
  static float inc = glm::pi<float>()/180.0 *0.1;
  theHorizAngle -= dy * inc;
  theVertAngle  -= dx * inc;
}


void ScaleScene(int dy) {
  //cout << "zoom " << theZoom << endl;
  //theZoom += exp((double)dy*0.01);
  theZoom += dy / (float)d->Wsize;
  
  if(theZoom < 0) theZoom = 0;
}


void TranslateScene(int dx, int dy) {
  theHorizDisp += dx / (float)d->Wsize * FoV;
  theVertDisp  -= dy / (float)d->Wsize * FoV;
}




void ClipData(DataObject * d) {
  
  float xm = std::min(d->xBegin, d->xEnd);
  float xM = std::max(d->xBegin, d->xEnd);
  float ym = std::min(d->yBegin, d->yEnd);
  float yM = std::max(d->yBegin, d->yEnd);

  cout << "xcyc " << d->xc << " " << d->yc << endl;
  
  xm = xm * d->wn + d->xc;
  xM = xM * d->wn + d->xc;
  ym = ym * d->wn + d->yc;
  yM = yM * d->wn + d->yc;
  
  cout << "1 Clip ND " << d->NumDat        << endl;

  int j = 0;
  for(unsigned long i=0; i< d->NumDat; i++) {
    if( d->r[i].x < xM && d->r[i].x > xm &&
	d->r[i].y < yM && d->r[i].y > ym ) {
      d->r[j] = d->r[i];
      j++;
    }
  }

  if( j > 1) {
    d->NumDat = j;
    d->MaxDat = j;
    ComputeFrame(d);
  }
  cout << "2 Clip ND " << d->NumDat << " " << d->xBegin << " " <<  d->xEnd
       << " " << d->yBegin << " " << d->yEnd
       << endl;

  //glLoadIdentity();
  ComputeHistogram(d);
  //InitialPosition();
  d->NeedUpdatePV = true;
}



void CommitRotation() {
  cout << "CommitRotation" << endl;
  
  for(unsigned long i=0; i< d->NumDat; i++) {
    vec4 q = Rm * vec4(d->r[i].x, d->r[i].y, d->r[i].z, 0);
    d->r[i].x = q.x;
    d->r[i].y = q.y;
    d->r[i].z = q.z;
  }
  
  ComputeFrame(d);
  InitialPosition();
  d->NeedUpdatePV = true;
}



void CommitRotationHalfPi() {
  cout << "CommiRotation" << endl;
  float pim = glm::pi<float>()/2.0;

  theHorizAngle = glm::round( theHorizAngle / pim) * pim;
  theVertAngle  = glm::round( theVertAngle  / pim) * pim;

  mat4 Id = mat4(1.0);
  mat4 Rh = rotate( Id, theHorizAngle, vec3(0,1,0) );
  mat4 Rv = rotate( Id, theVertAngle,  vec3(1,0,0) );
  Rm      = Rh * Rv;
  
  CommitRotation();
}
