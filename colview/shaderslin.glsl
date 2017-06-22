
#if defined(VERTEXSHADER)

uniform mat4 MVP;
uniform vec4 plt[10];
uniform int  pltSz;
uniform float pltDtz;

// For colorbar must match glsl shader
vec3 ComputeColor(float v) {
  float x = clamp( round(pltDtz *v) / pltDtz, plt[0].w, plt[pltSz-1].w );

  vec3 cl;
  
  for( int j=0; j< pltSz-1; j++) {
    if( x <= plt[j+1].w  && x >= plt[j].w ) {
      cl = plt[j].xyz + (plt[j+1].xyz - plt[j].xyz)
  	* (x - plt[j].w) / (plt[j+1].w - plt[j].w);
    }
  }

  return (cl);
}


#if defined(GLSL330)
layout(location = 0) in vec3 theVertex; // vertices Position modelspace
layout(location = 1) in vec3 theNormal; // line length, orientation: 0 x, 1 y, 2 z
layout(location = 2) in vec4 thePV;     // position + value in (0,1)
#elif defined(GLSL130)
in vec3 theVertex;
in vec3 theNormal;
in vec4 thePV;
#endif


out vec3 fragmentColor;

void main() {

  vec3 vtx;
  if(      theNormal.x == 0) vtx = theVertex;
  else if( theNormal.x == 1) vtx = vec3(0, theVertex.x, 0);
  else if( theNormal.x == 2) vtx = vec3(0, 0, theVertex.x);

  vec3 vertexWS = thePV.xyz + vtx * theNormal.y; //scale & translate
  gl_Position =  MVP * vec4(vertexWS,1);

  if(    thePV.w > -0.1) fragmentColor = ComputeColor(thePV.w) *1.5f;
  else if(thePV.w == -1) fragmentColor = vec3(1,1,1);
  else if(thePV.w == -2) fragmentColor = vec3(0,0,0);
  else if(thePV.w == -3) fragmentColor = vec3(1,1,0);
  else if(thePV.w == -4) fragmentColor = vec3(0,0,1);
}

#elif defined(FRAGMENTSHADER)

in vec3 fragmentColor;
out vec3 color;

void main() {
  color = fragmentColor;
}

#endif
