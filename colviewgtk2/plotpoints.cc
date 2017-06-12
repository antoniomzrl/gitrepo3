#include "visu.hh"

extern DataObject * d;



bool InClip(Point r) {
  if(r.x < d->c.x || r.x > d->C.x ||
     r.y < d->c.y || r.y > d->C.y ||
     r.z < d->c.z || r.z > d->C.z
     // || r.f[3] < d->c.f[3] || r.f[3] > d->C.f[3]
     // || r.f[4] < d->c.f[4] || r.f[4] > d->C.f[4]
     // || r.f[5] < d->c.f[5] || r.f[5] > d->C.f[5]
     // || r.f[6] < d->c.f[6] || r.f[6] > d->C.f[6]
     // || r.f[0] < d->c.f[0] || r.f[0] > d->C.f[0]
     || r.f[1] < d->c.f[1] || r.f[1] > d->C.f[1]
     // || r.f[2] < d->c.f[2] || r.f[2] > d->C.f[2]
     )
    return(false);
  else
    return(true);
}

void PlotPoint(GLfloat v, GLfloat vm, GLfloat vM, int i) {
  SetColor(v, vm, vM);
  glVertex3f(d->r[i].x, d->r[i].y, d->r[i].z);
}



// void PlotPoints(DataObject * d) {
//   glPointSize(d->BallSize*10);
//   glBegin(GL_POINTS);

//   int theStride = d->Stride;
//   if(d->vEconomic && d->NumDat > 10000) theStride = d->NumDat/10000;
//   //cout << "theStride " << d->NumDat << " " << theStride << " " << d->NumDat/theStride << endl;
  
//   int n1=0, n2=0, n3=0, n4=0;
  
//   for(unsigned long i=d->MinDat; i<d->MaxDat; i+=theStride) {
//     if( !InClip(d->r[i]) ) continue;

//     if(     d->vType == 2) {     //Elastic
//       if(d->r[i].f[1] == d->m.f[1]) {
// 	n1++;
// 	if(n1 == d->Stride1) n1 = 0;
// 	else                 continue;
//       }
//       else if(d->r[i].f[1] == (d->m.f[1] +1) ) { //Rotat
// 	n2++;
// 	if(n2 == d->Stride2) n2 = 0;
// 	else                 continue;
//       }
//       else if(d->r[i].f[1] == (d->m.f[1] +2) ) { //Rotat
// 	n3++;
// 	if(n3 == d->Stride3) n3 = 0;
// 	else                 continue;
//       }
//       else if(d->r[i].f[1] == (d->m.f[1] +3) ) { //Rotat
// 	n4++;
// 	if(n4 == d->Stride3) n4 = 0;
// 	else                 continue;
//       }

//       PlotPoint(d->r[i].f[1], d->m.f[1], d->M.f[1], i);
//     }
    
//     else if(d->vType == 1 ) PlotPoint(d->r[i].f[0], d->m.f[0], d->M.f[0], i);
//     else if(d->vType == 10) PlotPoint(d->r[i].f[3], d->m.f[3], d->M.f[3], i);
//     else if(d->vType == 20) PlotPoint(d->r[i].f[4], d->m.f[4], d->M.f[4], i);
//     else if(d->vType == 30) PlotPoint(d->r[i].f[5], d->m.f[5], d->M.f[5], i);
//     else if(d->vType == 40) PlotPoint(d->r[i].f[3], d->m.f[3], d->M.f[3], i);
//     else if(d->vType == 50) PlotPoint(d->r[i].f[4], d->m.f[4], d->M.f[4], i);
//   }

//   glEnd();
// }


void PlotLorLines(DataObject * d) {
  glBegin(GL_LINES);
  glColor3f(0.0, 1.0, 1.0); //cyan
  for(unsigned long i=d->MinDat; i<(d->MaxDat-1); i++) {  // LOR lines
    glVertex3f(d->r[i].x,   d->r[i].y,   d->r[i].z);
  }
  glEnd();
}


void PlotTracks(DataObject * d) {
  glColor3f(0.0, 1.0, 1.0); //cyan

  for(unsigned long i=1; i<(d->NumDat-1); i++) {  // LOR lines
    if(d->r[i].f[2] == d->r[i-1].f[2] ) //&& d->r[i-1].f[1] != 11 )
      glBegin(GL_LINES);
    glVertex3f(d->r[i-1].x, d->r[i-1].y, d->r[i-1].z);
    glVertex3f(d->r[i].x, d->r[i].y, d->r[i].z);
    glEnd();
  }
}

