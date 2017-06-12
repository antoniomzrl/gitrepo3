#include "visu.hh"


extern DataObject * d;
extern vector <string> tpi, ntpi;
extern string pn[];

extern GdkRectangle * windowSz;

float BestNear(float x) {

  //int N = log10(x);
  //if(N < 1) N = 1;
  float v = x;
  if( v < 10) v=10;
  
  int N = log10(v);
  N = pow(10,N-1);

  float y = (float)( N*lrint(x/N) );

  return y;
}
	 
void PlotColorBarText(vec3 l, vec3 u) {
  //glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glClear(GL_COLOR_BUFFER_BIT);
  
  printDbg(3, "PlotcolorBarText\n", "BLUE");
  
  //ortho(0.0f, (float)wWidth, 0.0f, (float)wHeight);

  float dy = (u.y-l.y)/1000;

  float min = d->m.f[d->vType];
  float max = d->M.f[d->vType];

  if(min <= max) {
    min *= 0.999;
    max *= 1.001;
  }

  vec3 color = vec3(1,1,1);
  if(d->Background > 0.5) color = vec3(0,0,0);
  else                    color = vec3(1,1,1);
  
  // labels
  float dt = (max - min)/10;
  if( d->vType == 1 ) dt =1;

  char label[80];
  for(int i=0;i<(int)tpi.size(); i++) {

    float t = min+i*dt;
    t = BestNear(t);

    float y = l.y + (u.y-l.y)*(t-min)/(max-min);  // Interpolate

    char Kilo[4], Mega[4];
    if(d->vType == 3 || d->vType == 4 || d->vType == 5) {
      strcpy(Kilo, "KeV");
      strcpy(Mega, "MeV");
    }
    else {
      strcpy(Kilo, "K");
      strcpy(Mega, "M");
    }

    if(d->vType == 0) sprintf(label, "%s", pn[(int)t].c_str() );
    else if(t < 1e3)  sprintf(label, "%.3g", t);
    else if(t < 1e6)  sprintf(label, "%.3g %s", t/1e3, Kilo);
    else              sprintf(label, "%.2g %s", t/1e6, Mega);

    if( d->vType == 1 && (unsigned int)t < tpi.size() ) {
      if( d->Hist[(int)t] > 0 ) {
	sprintf(label, "%s %ld", ntpi[(int)t].c_str(), d->Hist[(int)t]);
	if(d->Background > 0.5) color = vec3(0,0,0);
	else                    color = vec3(1,1,1);

      }
      else if( (unsigned int)t < tpi.size() ) {
	sprintf(label, "%s", ntpi[(int)t].c_str());
	color = vec3(0.5,0.5,0.5);
      }
    }

    if( y < u.y*1.01) {
      RenderText(label, u.x+dy*20, y-dy*20, d->FontSize, color);
    }
  }


  char lab[9][128];

  sprintf(lab[0], "Plot: %s", d->Label[d->vType].c_str() );
 
  if(d->Background < 0.5) glColor3f(1.0, 1.0, 1.0);
  else                    glColor3f(0.0, 0.0, 0.0);

  float rho = 0.0;  // (g/cm3)
  for(unsigned long i=0; i< d->NumDat; i++) {
    rho += d->r[i].f[6];
  }
  rho /= d->NumDat;

  float mass = rho * d->Volume/pow(d->SpatialFactor,3)/1e12; // (Kg)
  float Dose = d->EdepTotal * 1.60217733e-19 / mass;
    
  sprintf(lab[1], "Density: %g g/cm3", rho);
  sprintf(lab[2], "Mass:    %g g",     mass);
  sprintf(lab[3], "Edep:    %g eV",    d->EdepTotal);
  sprintf(lab[4], "         %g Joule", d->EdepTotal*1.60217733e-19);
  sprintf(lab[5], "Dose:    %g Gy",    Dose);
  sprintf(lab[6], "width:   %g %s", d->xn*2, d->SpatialUnits.c_str() );
  sprintf(lab[7], "height:  %g %s", d->yn*2, d->SpatialUnits.c_str() );
  sprintf(lab[8], "depth:   %g %s", d->zn*2, d->SpatialUnits.c_str() );

  for(int i=0; i<9; i++) {
    float y = windowSz->height/2 -windowSz->height/15 - (i+1)*windowSz->height/35;
    RenderText(lab[i], l.x, y, d->FontSize, vec3(1,1,1) );
  }
  
  glDisable(GL_BLEND);
}


void PlotColorBarText() {
  vec3 l = vec3(windowSz->width*0.021, windowSz->height*0.507, 0.0);
  vec3 u = vec3(windowSz->width*0.045, windowSz->height*0.985, 0.0);

  PlotColorBarText(l, u);
}
