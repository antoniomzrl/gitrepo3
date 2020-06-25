#include "colv.hh"

extern DataObject * d;
extern vector <vec3> vertices, normals;
extern vector <vec4> posval;
extern vector <vec4> plt;

mat4 theMVP, theIMV, Rm, Id;


GLuint programID, VAO,
  mvpID, imvID, sfID, bfID, 
  pltID, pltSzID, pltDtzID,
  LightPosID, LightID, LightColorID, greenAttenID,
  vtxID, nmlID, pvlID, vtxVBO, nmlVBO, pvlVBO;


void InitShadersIcosahedrons() {
  printDbg(5, GLInfo() );

  programID = LoadShaders("shaders.glsl");
  glUseProgram(programID);
  
  // Get handle for uniforms
  mvpID        = glGetUniformLocation(programID, "MVP");
  imvID        = glGetUniformLocation(programID, "IMV");
  sfID         = glGetUniformLocation(programID, "SizeFactor");
  bfID         = glGetUniformLocation(programID, "BallFactor");
  pltID        = glGetUniformLocation(programID, "plt");
  pltSzID      = glGetUniformLocation(programID, "pltSz");
  pltDtzID     = glGetUniformLocation(programID, "pltDtz");
  LightPosID   = glGetUniformLocation(programID, "LightPos"); // position
  LightID      = glGetUniformLocation(programID, "Light"); //ambient diffuse specular intensit
  LightColorID = glGetUniformLocation(programID, "LightColor");
  greenAttenID = glGetUniformLocation(programID, "greenAttenuation");

  vtxID = glGetAttribLocation(programID, "theVertex"  );
  nmlID = glGetAttribLocation(programID, "theNormal"  );
  pvlID = glGetAttribLocation(programID, "thePV");

  // VAO
  glGenVertexArrays(1, &VAO);
  // VBO's gpu-buffers handlers
  glGenBuffers(1, &vtxVBO);
  glGenBuffers(1, &nmlVBO);
  glGenBuffers(1, &pvlVBO);
  
  //glEnable(GL_CULL_FACE);  // Cull triangles which normal is not towards the camera
  //glEnable(GL_DEPTH_TEST); // Enable depth test (z-buffer)
  //glDepthFunc(GL_LESS);    // Accept fragment if it closer to camera than former
}


string AxisToMe() {
  vector <vec4> axis;
  vector <string> name;
  axis.push_back( vec4( 1, 0, 0,0) ) ; name.push_back("X");
  axis.push_back( vec4(-1, 0, 0,0) ) ; name.push_back("-X");
  axis.push_back( vec4( 0, 1, 0,0) ) ; name.push_back("Y");
  axis.push_back( vec4( 0,-1, 0,0) ) ; name.push_back("-Y");
  axis.push_back( vec4( 0, 0, 1,0) ) ; name.push_back("Z");
  axis.push_back( vec4( 0, 0,-1,0) ) ; name.push_back("-Z");

  vec4 eye(0,0,1,0);
  vec4 neweye = theMVP * eye;
  
  float maxsp = 0;
  string atm = "XYZ";
  
  for(unsigned int i=0; i < axis.size(); i++) {
    float sp = dot( axis[i], neweye);
    if( sp > maxsp) {
      maxsp = sp;
      atm = name[i];
    }
  }

  return( atm );
}


void UpdateTransformMatrices() {
  printDbg(3, "UpdateTransformMatrices\n", "BLUE");
  
  //mat4 Projection = perspective(45.0 * pi<float>()/180.0, 4.0/3.0, 0.1, 100.0);
  //mat4 Proj = ortho(-FoV,FoV, -FoV,FoV, FoV*2,-FoV)
  float FoV = d->FoV, AR = d->AR;
  mat4 Proj = ortho(-FoV*AR-AR+1, FoV*AR-AR+1, -FoV,FoV, -FoV*2,FoV*2);
  
  // Camera matrix       Eye          LookAt       Head up
  mat4 View = lookAt( vec3(0,0,1), vec3(0,0,0), vec3(0,1,0) );
  mat4 Id = mat4(1.0);
  mat4 S  = scale( Id, d->Scale * d->theZoom );
  mat4 Rh = rotate( Id, d->theHorizAngle, vec3(0,1,0) );
  mat4 Rv = rotate( Id, d->theVertAngle,  vec3(1,0,0) );
  mat4 Rf = rotate( Id, d->theFrontAngle, vec3(0,0,1) );
  mat4 T  = translate( Id, vec3(d->theHorizDisp, d->theVertDisp, d->theDepthDisp) );
  mat4 Model = T * Rh * Rv * Rf * S;
   
  theMVP = Proj * View * Model; // ModelViewProjection : mult 3 matrices
  theIMV = transpose(inverse(View * Model));
  Rm     = Rh * Rv;

  //cout << "IMV\n"; printgsl(IMV); //printgsl(ModelMat);
  //cout << "AxisToMe: " << AxisToMe() << endl;
}



void BindAndAttrib(GLuint vao, GLuint bufID, GLuint bufGPU, GLuint size, GLuint divisor) {
  glBindVertexArray(vao);
  glEnableVertexAttribArray(bufID);
  glBindBuffer(GL_ARRAY_BUFFER, bufGPU);
  glVertexAttribPointer(bufID, size, GL_FLOAT, GL_FALSE, 0, (void*)0 );        
  // layout in shader, size, type, normalized?, stride, array buffer offset
  glVertexAttribDivisor(bufID, divisor); 
  // for glDrawArrays*Instanced*. 1st param is attribute buffer
  // 2nd "rate at which generic vertex attributes advance when rendering multiple instances"
}


// update gpu-buffer
void UpdateGpuBuffer(GLuint bufGPU, unsigned int SZ, void * ptr) {
  glBindBuffer(GL_ARRAY_BUFFER, bufGPU);
  glBufferData(GL_ARRAY_BUFFER, SZ, ptr, GL_STATIC_DRAW);
}

void UpdateGpuStreamBuffer(GLuint bufGPU, unsigned int SZ, void * ptr) {
  glBindBuffer(GL_ARRAY_BUFFER, bufGPU);
  glBufferData(GL_ARRAY_BUFFER, SZ, NULL, GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, SZ, ptr);
  // buffer orphaning, common way to improve streaming performance
  // http://www.opengl.org/wiki/Buffer_Object_Streaming
}



void PlotIcosahedrons() {
  printDbg(3, "PlotIcosahedrons\n", "BLUE");

  double IcoLevel = d->IcosahedronLevel;
  if(d->vEconomic && (d->r).size() > 100000) {
    IcoLevel = std::min(IcoLevel, 1.0);
  }
  
  static double IcoLevelAnt = -1;
  if(IcoLevel != IcoLevelAnt) {
    ComputeIcosahedronVertex(IcoLevel);
    IcoLevelAnt = IcoLevel;
  }

  
  vec4 Light;
  vec3 LightPos, LightColor;
  GLuint primitive;
  LightColor = vec3(1,1,1);

  if(vertices.size() == 1) {  // points
    primitive = GL_POINTS;
    Light     = vec4(0.4, 1.0, 0.0, 10.0);
    LightPos  = vec3(0,0,-3);
    glPointSize(d->BallSize *10.0);
  }
  else {                      // icosahedrons
    primitive = GL_TRIANGLES;
    Light     = d->Light;
    LightPos  = d->LightPos;
  }
  
  UpdateTransformMatrices();

  if( d->NeedUpdatePalette) {
    UpdatePalette();
    d->NeedUpdatePalette = false;
  }
  
  if( d->NeedUpdatePV) {
    d->NeedUpdatePV = false;
    UpdatePV(d);
    UpdateGpuBuffer(pvlVBO, posval.size()*sizeof(vec4), &posval[0]);
  }

  static unsigned int Nvertices = 0;
  if( vertices.size() != Nvertices ) {
    Nvertices = vertices.size();
    UpdateGpuBuffer(vtxVBO, Nvertices*sizeof(vec3), &vertices[0]);
    UpdateGpuBuffer(nmlVBO, Nvertices*sizeof(vec3),  &normals[0]);
  }
  
  glUseProgram(programID);

  //cout << "------- "; printglm(LightPos);
  
  // Send our transformation to the currently bound shader, in the "MVP" uniform
  glUniformMatrix4fv( mvpID, 1, GL_FALSE, &theMVP[0][0] );
  glUniformMatrix4fv( imvID, 1, GL_FALSE, &theIMV[0][0] );
  glUniform4fv( pltID,    plt.size(), &plt[0][0] );
  glUniform1i(  pltSzID,  plt.size() );
  glUniform1f(  pltDtzID, d->DiscretizePalette );
  glUniform4fv( LightID,  1, &Light[0] );
  glUniform3fv( LightPosID, 1, &LightPos[0] );
  glUniform3fv( LightColorID, 1, &LightColor[0] );
  glUniform1f(  greenAttenID, d->greenAttenuation );
  glUniform1f(  sfID, d->BallSize*0.05 );
  glUniform1f(  bfID, d->BallFactor );

  BindAndAttrib(VAO, vtxID, vtxVBO, 3, 0); //1st attrib buf: vtx, always reuse -> 0
  BindAndAttrib(VAO, nmlID, nmlVBO, 3, 0); //2nd attribute buffer : normals
  BindAndAttrib(VAO, pvlID, pvlVBO, 4, 1); //3rd : pos+value, one per instance -> 1
 
  glDrawArraysInstanced(primitive, 0, vertices.size(), posval.size() );
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glDisableVertexAttribArray(vtxID);
  // glDisableVertexAttribArray(nmlID);
  // glDisableVertexAttribArray(pvlID);

  // Cleanup VBO and shader
  // glDeleteBuffers(1, &(shd->vtxGPU));
  // glDeleteBuffers(1, &(shd->nmlGPU));
  // glDeleteBuffers(1, &(shd->pvlGPU));
  // glDeleteProgram(shd->programID);

  printDbg(1, ShowGPUResources(), "BLUE");
}
