//orthoendrons and colorbar lines

#include "visu.hh"


extern DataObject * d;
extern vector <vec4> plt;
extern mat4 theMVP;
extern GdkRectangle * windowSz;

GLuint proglin, VAOlin,
  mvpIDl,
  pltIDl, pltSzIDl, pltDtzIDl,
  vtxIDl, nmlIDl, pvlIDl, vtxVBOl, nmlVBOl, pvlVBOl;


void InitShadersLines() {

  proglin = LoadShaders("shaderslin.glsl");
  glUseProgram(proglin);
  
  mvpIDl    = glGetUniformLocation(proglin, "MVP");
  pltIDl    = glGetUniformLocation(proglin, "plt");
  pltSzIDl  = glGetUniformLocation(proglin, "pltSz");
  pltDtzIDl = glGetUniformLocation(proglin, "pltDtz");
  
  vtxIDl = glGetAttribLocation(proglin, "theVertex" );
  nmlIDl = glGetAttribLocation(proglin, "theNormal" );
  pvlIDl = glGetAttribLocation(proglin, "thePV" );

  glGenBuffers(1, &vtxVBOl);
  glGenBuffers(1, &nmlVBOl);
  glGenBuffers(1, &pvlVBOl);

  glGenVertexArrays(1, &VAOlin);
}



void AddOrthoLines(vec3 l, vec3 u, vector <vec4> &pvl, vector <vec3> &dir) {
 
  vec3 len = u - l;
  vec3 m = (u + l) *0.5f;
  float val = -1;
  if( d->Background > 0.5 ) val = -2;

  pvl.push_back( vec4(m.x, l.y, l.z, val) );
  pvl.push_back( vec4(m.x, l.y, u.z, val) );
  pvl.push_back( vec4(m.x, u.y, l.z, val) );
  pvl.push_back( vec4(m.x, u.y, u.z, val) );
  
  pvl.push_back( vec4(l.x, m.y, l.z, val) );
  pvl.push_back( vec4(l.x, m.y, u.z, val) );
  pvl.push_back( vec4(u.x, m.y, l.z, val) );
  pvl.push_back( vec4(u.x, m.y, u.z, val) );

  pvl.push_back( vec4(l.x, l.y, m.z, val) );
  pvl.push_back( vec4(l.x, u.y, m.z, val) );
  pvl.push_back( vec4(u.x, l.y, m.z, val) );
  pvl.push_back( vec4(u.x, u.y, m.z, val) );

  // normal/dir x:0, y:1, z:2, segment lenght
  dir.push_back( vec3(0, len.x, 0) );
  dir.push_back( vec3(0, len.x, 0) );
  dir.push_back( vec3(0, len.x, 0) );
  dir.push_back( vec3(0, len.x, 0) );
                                      
  dir.push_back( vec3(1, len.y, 0) );
  dir.push_back( vec3(1, len.y, 0) );
  dir.push_back( vec3(1, len.y, 0) );
  dir.push_back( vec3(1, len.y, 0) );
                                      
  dir.push_back( vec3(2, len.z, 0) );
  dir.push_back( vec3(2, len.z, 0) );
  dir.push_back( vec3(2, len.z, 0) );
  dir.push_back( vec3(2, len.z, 0) );  
}




void AddColorBarLines(vec3 l, vec3 u, vector <vec4> &pvl, vector <vec3> &dir) {
  
  AddOrthoLines(l, u, pvl, dir);

  vec3 len = u - l;
  vec3   m = (u + l) *0.5f;
  float dy = (u.y-l.y)/1000;
  float dx = (u.x-l.x)/6;

  for(float y=l.y; y<=u.y; y+=dy) {
    float val = (y - l.y) / len.y;
    pvl.push_back( vec4(m.x, y, m.z, val) );
    pvl.push_back( vec4(m.x, y, m.z, val) );
    dir.push_back( vec3(0, len.x, 0) );
    dir.push_back( vec3(0, len.x, 0) );
  }
  
  float min = d->m.f[d->vType];
  float max = d->M.f[d->vType];

  if(min <= max) {
    min *= 0.999;
    max *= 1.001;
  }

  float dt = (max - min)/10;
  if( d->vType == 1 ) dt =1;
  
  for(int i=0;i<40; i++) {
    float t = min+i*dt;
    t = BestNear(t);

    float y = l.y + (u.y-l.y)*(t-min)/(max-min);  // Interpolate
    
    if( y < u.y*1.01) {
      pvl.push_back( vec4(u.x+dx/2, y, 0.0, -1) );
      pvl.push_back( vec4(u.x+dx/2, y, 0.0, -1) );
      dir.push_back( vec3(0, dx, 0) );
      dir.push_back( vec3(0, dx, 0) );
    }
  }
}




void RenderLines(vector <vec3> lin, vector <vec4> pvl, vector <vec3> dir) {
  UpdateGpuBuffer(vtxVBOl, lin.size()*sizeof(vec3), &lin[0]);
  UpdateGpuBuffer(nmlVBOl, dir.size()*sizeof(vec3), &dir[0]);
  UpdateGpuBuffer(pvlVBOl, pvl.size()*sizeof(vec4), &pvl[0]);
  
  BindAndAttrib(VAOlin, vtxIDl, vtxVBOl, 3, 0);
  BindAndAttrib(VAOlin, nmlIDl, nmlVBOl, 3, 1);
  BindAndAttrib(VAOlin, pvlIDl, pvlVBOl, 4, 1);

  glDrawArraysInstanced(GL_LINES, 0, lin.size(), pvl.size() );
}


void PlotOrthoLines() {
  vector <vec3> lin, dir;
  vector <vec4> pvl;

  lin.push_back( vec3(-0.5,  0,  0) );
  lin.push_back( vec3( 0.5,  0,  0) );
  
  if(d->vSelection) {
    vec3 l = vec3(std::min(d->xBegin, d->xEnd), std::min(d->yBegin, d->yEnd),  d->zn/d->wn);
    vec3 u = vec3(std::max(d->xBegin, d->xEnd), std::max(d->yBegin, d->yEnd), -d->zn/d->wn);
    AddOrthoLines(l, u, pvl, dir);
  }

  if(d->vFrame) {
    vec3 u(d->xn/d->wn, d->yn/d->wn, d->zn/d->wn);
    AddOrthoLines(-u, u, pvl, dir);
  }

  UpdateTransformMatrices();

  glUseProgram(proglin);

  glUniform4fv( pltIDl,    plt.size(), &plt[0][0] );
  glUniform1i(  pltSzIDl,  plt.size() );
  glUniform1f(  pltDtzIDl, d->DiscretizePalette );
  glUniformMatrix4fv( mvpIDl, 1, GL_FALSE, &theMVP[0][0] );

  RenderLines(lin, pvl, dir);
}

extern float FoV, AR;
void PlotColorBarLines() {
  cout << "PlotColorBarLines\n";
  
  vector <vec3> lin, dir, dircb;
  vector <vec4> pvl, pvlcb;

  glUseProgram(proglin);

  lin.push_back( vec3(-0.5,  0,  0) );
  lin.push_back( vec3( 0.5,  0,  0) );

  vec3 l = vec3(windowSz->width*0.02,  windowSz->height/2,    0.0);
  vec3 u = vec3(windowSz->width*0.045, windowSz->height*0.98, 0.0);
  AddColorBarLines(l, u, pvlcb, dircb);
  
  mat4 Proj = ortho(0.0f, (float)windowSz->width, 0.0f, (float)windowSz->height);
  theMVP = Proj;
  
  glUniformMatrix4fv( mvpIDl, 1, GL_FALSE, &Proj[0][0] );
  glUniform1i(  pltSzIDl,  plt.size() );
  glUniform1f(  pltDtzIDl, d->DiscretizePalette );
  glUniformMatrix4fv( mvpIDl, 1, GL_FALSE, &theMVP[0][0] );
  
  RenderLines(lin, pvlcb, dircb);
}
