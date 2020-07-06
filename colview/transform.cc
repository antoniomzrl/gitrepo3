
#include "colv.hh"
extern DataObject * d;

extern mat4 Rm;

void InitialPosition() {
  d->theHorizAngle = 0; //glm::pi<float>(); // Initial
  d->theVertAngle  = 0;
  d->theFrontAngle = 0;
  d->theHorizDisp  = 0;
  d->theVertDisp   = 0;
  d->theDepthDisp  = 0;
  d->theZoom       = 1;
}


void RotateScene(int dx, int dy, int dz) {
  static float inc = glm::pi<float>()/180.0 *0.1;
  d->theHorizAngle += dy * inc;
  d->theVertAngle  += dx * inc;
  d->theFrontAngle += dz * inc;

  //cout << "angs " << d->theHorizAngle*180.0/glm::pi<float>()
  //     << " " << d->theVertAngle*180.0/glm::pi<float>() << endl;
}


void ScaleScene(int dy) {
  d->theZoom += (float)dy / (d->PlotSize.y/2.0);
  if(d->theZoom < 0) d->theZoom = 0;
}


void TranslateScene(int dx, int dy) {
  d->theHorizDisp += (float)dx / (d->PlotSize.y/2.0) * d->FoV;
  d->theVertDisp  -= (float)dy / (d->PlotSize.y/2.0) * d->FoV;
}




void ClipData(DataObject * d) {
  
  float xm = d->MouseSelection.x;
  float xM = d->MouseSelection.y;
  float ym = d->MouseSelection.z;
  float yM = d->MouseSelection.w;
  
  cout << "xcyc " << d->c.x << " " << d->c.y << endl;
  
  xm = xm * d->sw.w + d->c.x;
  xM = xM * d->sw.w + d->c.x;
  ym = ym * d->sw.w + d->c.y;
  yM = yM * d->sw.w + d->c.y;
  
  cout << "1 Clip ND " << (d->r).size()        << endl;

  int nd = 0;
  for(unsigned long i=0; i< (d->r).size(); i++) {
    if( d->r[i].x < xM && d->r[i].x > xm &&
	d->r[i].y < yM && d->r[i].y > ym ) {
      d->r[nd] = d->r[i];
      nd++;
    }
  }

  if( nd > 1) {
    (d->r).resize(nd);
    ComputeFrame(d);
  }
  cout << "2 Clip ND " << (d->r).size() << endl;

  ComputeHistogram(d);
  d->NeedUpdatePV = true;
}



void CommitRotation() {
  cout << "CommitRotation" << endl;
  
  for(unsigned long i=0; i< (d->r).size(); i++) {
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

  d->theHorizAngle = glm::round( d->theHorizAngle / pim) * pim;
  d->theVertAngle  = glm::round( d->theVertAngle  / pim) * pim;
  d->theFrontAngle = glm::round( d->theFrontAngle / pim) * pim;

  mat4 Id = mat4(1.0);
  mat4 Rh = rotate( Id, d->theHorizAngle, vec3(0,1,0) );
  mat4 Rv = rotate( Id, d->theVertAngle,  vec3(1,0,0) );
  mat4 Rf = rotate( Id, d->theFrontAngle, vec3(0,0,1) );
  Rm      = Rh * Rv * Rf;
  
  CommitRotation();
}
