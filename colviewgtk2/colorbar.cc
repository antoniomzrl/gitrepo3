#include "visu.hh"


extern DataObject * d;
extern vector <string> tpi, ntpi;
extern string pn[];
extern float verboseFlag;




void PlotColorBar(float t1, float t2, float x1, float y1, float x2, float y2,
		  string title) {
  printDbg(3, "PlotcolorBar\n", "BLUE");

  float Dx = (y2-y1)/10;
  float dx = Dx/10;
  float dy = Dx/10;

  //UpdateCanvasVector();

  // color rect
  glLineWidth(5);
  glBegin(GL_LINES);
  for(float y=y1; y<=y2; y+=dy*0.5) {
    // SetColor(y, y1, y2);
    vec3 c = ComputeColor((y-y1)/(y2-y1)) *1.5f;
    glColor3f(c.x, c.y, c.z);
    glVertex2f(x1, y);
    glVertex2f(x2, y);
  }
  glEnd();
  
  // glBegin(GL_QUADS);
  // for(float y=y1; y<=y2; y+=dy) {
  //   vec3 cb = ComputeColor((y-y1)/(y2-y1));
  //   vec3 ct = ComputeColor((y+dy-y1)/(y2-y1));
  //   glColor3f(cb.x, cb.y, cb.z);
  //   glVertex2f(x1, y);
  //   glVertex2f(x2, y);
  //   glColor3f(ct.x, ct.y, ct.z);
  //   glVertex2f(x2, y+dy);
  //   glVertex2f(x1, y+dy);
  //   //x1+=0.01;
  //   //x2+=0.01;
  // }
  // glEnd();
 
  if(d->Background < 0.5) glColor3f(1.0, 1.0, 1.0);
  else                    glColor3f(0.0, 0.0, 0.0);

  glLineWidth(1);
  glBegin(GL_LINES);
  glVertex2f(x1, y1);
  glVertex2f(x2, y1);
  glVertex2f(x1, y1);
  glVertex2f(x1, y2);
  glVertex2f(x2, y2);
  glVertex2f(x2, y1);
  glVertex2f(x2, y2);
  glVertex2f(x1, y2);
  glEnd();


  // labels
  float dt = (t2 - t1)/10;
  if( d->vType == 1 ) dt =1;

  char label[80];
  for(int i=0;i<40; i++) {

    float t = t1+i*dt;
    t = BestNear(t);

    float y = y1 + (y2-y1)*(t-t1)/(t2-t1);  // Interpolate
      
    char Kilo[4], Mega[4];
    if(d->vType == 3 || d->vType == 4 || d->vType == 5) {
      strcpy(Kilo, "KeV");
      strcpy(Mega, "MeV");
    }
    else {
      strcpy(Kilo, "K");
      strcpy(Mega, "M");
    }
    //sprintf(label, "%g", t);
    int tt = (int)t;

    if(d->vType == 0) sprintf(label, "%s", pn[tt].c_str() );
    else if(t < 1e3)  sprintf(label, "%.3g", t);
    else if(t < 1e6)  sprintf(label, "%.3g %s", t/1e3, Kilo);
    else              sprintf(label, "%.2g %s", t/1e6, Mega);

    if( d->vType == 1 && t1 < tpi.size() ) {
      unsigned int t1 = (int)t;

      if( d->Hist[t1] > 0 ) {
	sprintf(label, " %s %ld", ntpi[t1].c_str(), d->Hist[t1]);
	//glColor3f(1.0, 1.0, 1.0);
	if(d->Background < 0.5) glColor3f(1.0, 1.0, 1.0);
	else                    glColor3f(0.0, 0.0, 0.0);
      }
      else if(t1 < tpi.size() ) {
	sprintf(label, " %s", ntpi[t1].c_str());
	glColor3f(0.5, 0.5, 0.5);
      }
    }

    //cout << label << " -----> " << y1 << " , " << y << " , " << y2 << endl;
    if( y < y2*1.01) {
      glBegin(GL_LINES);    // tics
      glVertex2f(x2,    y);
      glVertex2f(x2+dx*2, y);
      glEnd();
      PlotText(x2+dx*2, y-dx*1.5, 0.0, label);
    }

  }

  sprintf(label, "%s", title.c_str());
  //glColor3f(1.0, 1.0, 1.0);
  PlotText(x1, y2+Dx, 0.0, label);
}



void PlotColorBar(float t1, float t2, float R, int D, string title) {
  
  float Dx, x1, y1, x2, y2;
  Dx = R/5;

  y1 = -R + Dx;
  y2 =  R - Dx;
  Dx =  (y2-y1)/10;
  x1 = -R + (D+1)*Dx;
  x2 = -R + (D+2)*Dx;

  PlotColorBar(t1, t2, x1, y1, x2, y2, title);
}


void PlotColorBars(DataObject * d) {

  ComputeHistogram(d);

  glPushMatrix();
  glLoadIdentity(); // Avoid rotation

  double R = d->Rv;
  glOrtho(-R,R, -R,R, -R,R);
  //glOrtho(d->xLeft, d->xRight, d->yBottom, d->yTop, d->zNear, d->zFar); 

  PlotColorBar(d->m.f[3], d->M.f[3], d->rMax, 0, "ENERGY");
  PlotColorBar(d->m.f[4], d->M.f[4], d->rMax, 4, "ENERGY LOST");
  PlotColorBar(d->m.f[1], d->M.f[1], d->rMax, 8, "INTERACTION TYPE");

  glPopMatrix();
}


void PlotColorBar(DataObject * d) {
  string label;
  float min =0, max =0;

  if(d->Background < 0.5) glColor3f(1.0, 1.0, 1.0);
  else                    glColor3f(0.0, 0.0, 0.0);

  glPushMatrix();
  glLoadIdentity();

  label = d->Label[d->vType];
  min = d->m.f[d->vType];
  max = d->M.f[d->vType];

  if(min <= max) {
    min *= 0.999;
    max *= 1.001;
  }

  PlotColorBar(min, max, -0.9, 0.0, -0.7, 0.95, d->Label[d->vType]);
  //PlotColorBar(min, max, 0.7, 0.35, 0.78, 0.95, label);

  glPopMatrix();
}
