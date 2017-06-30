//orthoendrons and colorbar lines

#include "colv.hh"


extern DataObject * d;
extern vector <vec4> plt;
extern mat4 theMVP;

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



void AddOrthoLines(vec3 l, vec3 u, float color, vector <vec4> &pvl, vector <vec3> &dir) {
 
  vec3 len = u - l;
  vec3 m = (u + l) * 0.5f;

  pvl.push_back( vec4(m.x, l.y, l.z, color) );
  pvl.push_back( vec4(m.x, l.y, u.z, color) );
  pvl.push_back( vec4(m.x, u.y, l.z, color) );
  pvl.push_back( vec4(m.x, u.y, u.z, color) );
  
  pvl.push_back( vec4(l.x, m.y, l.z, color) );
  pvl.push_back( vec4(l.x, m.y, u.z, color) );
  pvl.push_back( vec4(u.x, m.y, l.z, color) );
  pvl.push_back( vec4(u.x, m.y, u.z, color) );

  pvl.push_back( vec4(l.x, l.y, m.z, color) );
  pvl.push_back( vec4(l.x, u.y, m.z, color) );
  pvl.push_back( vec4(u.x, l.y, m.z, color) );
  pvl.push_back( vec4(u.x, u.y, m.z, color) );

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

  float color = -1;
  if( d->Background > 0.5 ) color = -2;

  AddOrthoLines(l, u, color, pvl, dir);

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
  
  float min = d->m.f[d->ac];
  float max = d->M.f[d->ac];

  if(min <= max) {
    min *= 0.999;
    max *= 1.001;
  }

  float dt = (max - min)/10;
  if( d->ac == 1 ) dt =1;
  
  for(int i=0;i<40; i++) {
    float t = min+i*dt;
    t = BestNear(t);

    float y = l.y + (u.y-l.y)*(t-min)/(max-min);  // Interpolate
    
    if( y < u.y*1.01) {
      float color = -1;
      if( d->Background > 0.5 ) color = -2;
      
      pvl.push_back( vec4(u.x+dx/2, y, 0.0, color) );
      pvl.push_back( vec4(u.x+dx/2, y, 0.0, color) );
      dir.push_back( vec3(0, dx, 0) );
      dir.push_back( vec3(0, dx, 0) );
    }
  }
}




void RenderLines(vector <vec3> lin, vector <vec4> pvl, vector <vec3> dir, mat4 MVP) {
  glUseProgram(proglin);

  glUniform4fv( pltIDl,    plt.size(), &plt[0][0] );
  glUniform1i(  pltSzIDl,  plt.size() );
  glUniform1f(  pltDtzIDl, d->DiscretizePalette );
  glUniformMatrix4fv( mvpIDl, 1, GL_FALSE, &MVP[0][0] );

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
    float color = -3;
    if( d->Background > 0.5 ) color = -4;
    vec3 l = vec3(d->MouseSelection.x, d->MouseSelection.z,  d->sw.z/d->sw.w);
    vec3 u = vec3(d->MouseSelection.y, d->MouseSelection.w, -d->sw.z/d->sw.w);
     AddOrthoLines(l, u, color, pvl, dir);
  }

  if(d->vFrame) {
    float color = -1;
    if( d->Background > 0.5 ) color = -2;
    vec3 u = vec3(d->sw.x, d->sw.y, d->sw.z) / d->sw.w;
    AddOrthoLines(-u, u, color, pvl, dir);
  }

  RenderLines(lin, pvl, dir, theMVP);
}



void PlotColorBarLines() {
  vector <vec3> lin, dir;
  vector <vec4> pvl;
 
  lin.push_back( vec3(-0.5,  0,  0) );
  lin.push_back( vec3( 0.5,  0,  0) );

  vec3 l = vec3(d->PlotSize.x*0.02,  d->PlotSize.y/2,    0.0);
  vec3 u = vec3(d->PlotSize.x*0.045, d->PlotSize.y*0.98, 0.0);
  AddColorBarLines(l, u, pvl, dir);
  
  mat4 Proj = ortho(0.0f, d->PlotSize.x, 0.0f, d->PlotSize.y);
  
  RenderLines(lin, pvl, dir, Proj);
}
