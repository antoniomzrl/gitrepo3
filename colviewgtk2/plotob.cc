#include "visu.hh"

extern DataObject * d;
extern vector <vec3> vertices, normals;
extern vector <vec4> posval;
extern vector <vec4> plt;

mat4 theMVP, theIMV, Rm, Id;
float FoV = 1.2, AR = 4.0/3.0,
  theHorizAngle, theVertAngle, theZoom,
  theHorizDisp, theVertDisp, theDepthDisp;


ShaderID * GetShaderIDs( const char * shfn) {
  printDbg(3, "InitShaderIDs " + string(shfn) + "\n", "RED");
  
  ShaderID * b = new ShaderID;
  string theFileName = GetAbsoluteFile( string(shfn) );

  b->programID = LoadShaders( theFileName.c_str() );
  glUseProgram(b->programID);
  
  // Get a handle for our uniforms
  b->mvpID        = glGetUniformLocation(b->programID, "MVP");
  b->imvID        = glGetUniformLocation(b->programID, "IMV");
  b->sfID         = glGetUniformLocation(b->programID, "SizeFactor");
  b->bfID         = glGetUniformLocation(b->programID, "BallFactor");
  b->pltID        = glGetUniformLocation(b->programID, "plt");
  b->pltSzID      = glGetUniformLocation(b->programID, "pltSz");
  b->pltDtzID     = glGetUniformLocation(b->programID, "pltDtz");
  b->LightPosID   = glGetUniformLocation(b->programID, "LightPos"); // position
  b->LightID      = glGetUniformLocation(b->programID, "Light"); //ambient diffuse specular intensit
  b->LightColorID = glGetUniformLocation(b->programID, "LightColor");
  b->greenAttenID = glGetUniformLocation(b->programID, "greenAttenuation");
  b->LineColorID  = glGetUniformLocation(b->programID, "LineColor");

  b->vtxID = glGetAttribLocation(b->programID, "theVertex"  );
  b->nmlID = glGetAttribLocation(b->programID, "theNormal"  );
  b->pvlID = glGetAttribLocation(b->programID, "thePV");

  // VBO's gpu-buffers handlers
  glGenBuffers(1, &(b->vtxGPU) );
  glGenBuffers(1, &(b->nmlGPU) );
  glGenBuffers(1, &(b->pvlGPU) );
  
  glEnable(GL_DEPTH_TEST); // Enable depth test (z-buffer)
  glDepthFunc(GL_LESS);    // Accept fragment if it closer to camera than former
  glEnable(GL_CULL_FACE);  // Cull triangles which normal is not towards the camera

  return b;
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
  //mat4 Proj = ortho(-FoV,FoV, -FoV,FoV, FoV*2,-FoV);
  mat4 Proj = ortho(-FoV*AR-AR+1, FoV*AR-AR+1, -FoV,FoV, FoV*2,-FoV);
  
  // Camera matrix       Eye          LookAt       Head up
  mat4 View = lookAt( vec3(0,0,1), vec3(0,0,0), vec3(0,1,0) );
 
  mat4 Id = mat4(1.0);
  mat4 S  = scale( Id, vec3(theZoom, theZoom, theZoom) );
  mat4 Rh = rotate( Id, theHorizAngle, vec3(0,1,0) );
  mat4 Rv = rotate( Id, theVertAngle,  vec3(1,0,0) );
  mat4 T  = translate( Id, vec3(theHorizDisp, theVertDisp, theDepthDisp) );
  mat4 Model = T * Rh * Rv * S;
   
  theMVP = Proj * View * Model; // ModelViewProjection : mult 3 matrices
  theIMV = transpose(inverse(View * Model));
  Rm     = Rh * Rv;
  //cout << "IMV\n"; printgsl(IMV); //printgsl(ModelMat);
  cout << "AxisToMe: " << AxisToMe() << endl;

}




void SetAttrib(GLint bufID, GLint bufGPU, GLint size, GLuint divisor) {
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




ShaderID * shd; //buffer ID's icosaedron


void InitShadersIcosahedrons() {
  printDbg(5, GLInfo() );
  shd = GetShaderIDs("shaders.glsl");
  InitialPosition();
}


void PlotIcosahedrons() {
  printDbg(3, "PlotIcosahedrons\n", "BLUE");

  glUseProgram(shd->programID);

  
  vec4 Light;
  vec3 LightPos, LightColor;
  GLuint primitive;
  LightColor = vec3(1,1,1);

  if(vertices.size() == 1) {        // points
    primitive = GL_POINTS;
    Light     = vec4(0.4, 1.0, 0.0, 10.0);
    LightPos  = vec3(0,0,3);
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
    UpdateGpuBuffer(shd->pvlGPU, posval.size()*sizeof(vec4), &posval[0]);
  }

  static unsigned int Nvertices = 0;
  if( vertices.size() != Nvertices ) {
    Nvertices = vertices.size();
    UpdateGpuBuffer(shd->vtxGPU, Nvertices*sizeof(vec3), &vertices[0]);
    UpdateGpuBuffer(shd->nmlGPU, Nvertices*sizeof(vec3),  &normals[0]);
  }
  
  // Send our transformation to the currently bound shader, in the "MVP" uniform
  glUniformMatrix4fv( shd->mvpID, 1, GL_FALSE, &theMVP[0][0] );
  glUniformMatrix4fv( shd->imvID, 1, GL_FALSE, &theIMV[0][0] );
  glUniform4fv( shd->pltID,    plt.size(), &plt[0][0] );
  glUniform1i(  shd->pltSzID,  plt.size() );
  glUniform1f(  shd->pltDtzID, d->DiscretizePalette );
  glUniform4fv( shd->LightID,  1, &Light[0] );
  glUniform3fv( shd->LightPosID, 1, &LightPos[0] );
  glUniform3fv( shd->LightColorID, 1, &LightColor[0] );
  glUniform1f(  shd->greenAttenID, d->greenAttenuation );
  glUniform1f(  shd->sfID, d->BallSize*0.05 );
  glUniform1f(  shd->bfID, d->BallFactor );

  SetAttrib(shd->vtxID, shd->vtxGPU, 3, 0); // 1st attribute buffer : vertices, always reuse -> 0
  SetAttrib(shd->nmlID, shd->nmlGPU, 3, 0); // 2nd attribute buffer : normals
  SetAttrib(shd->pvlID, shd->pvlGPU, 4, 1); // 3rd : position+value, one per instance -> 1
 
  glDrawArraysInstanced(primitive, 0, vertices.size(), posval.size() );

  // glDisableVertexAttribArray(vtxID);
  // glDisableVertexAttribArray(nmlID);
  // glDisableVertexAttribArray(pvlID);

  // Cleanup VBO and shader
  // glDeleteBuffers(1, &vtxGPU);
  // glDeleteBuffers(1, &nmlGPU);
  // glDeleteBuffers(1, &pvlGPU);
  // glDeleteProgram(programID);

  printDbg(1, ShowGPUResources(), "BLUE");
}
