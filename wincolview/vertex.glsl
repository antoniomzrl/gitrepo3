#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 theVertex;   // vertices Position modelspace
layout(location = 1) in vec3 theNormal;   // normals  modelspace
layout(location = 2) in vec3 thePosition; // positions;
layout(location = 3) in vec4 theColor;    // colors;

// Output data ; will be interpolated for each fragment.
//out vec3 positionWS;
out vec3 normalCS;
out vec3 LightDirCS;
out vec3 eyeDirCS;
out vec3 fragmentColor;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 IMV;  // inverse-transpose MV
uniform vec3 LightPos;
uniform vec3 sizeFactor;

void main() {
  vec3 vertexWS = thePosition + theVertex * theColor.w * sizeFactor.x; //scale & translate
  gl_Position =  MVP * vec4(vertexWS,1); // Out pos of vertex in clip space

  normalCS = normalize( ( IMV * vec4(theNormal, 0)).xyz ); // Normal in camera space
  
  // Vector from vertex to  camera, in camera space.
  // In camera space, the camera is at (0,0,0).
  //vec3 vertexPositionCS = ( V * M * vec4(vertexWS,1)).xyz;
  //vec3 vertexPositionCS = ( IMV * vec4(theVertex,1)).xyz;
  //eyeDirCS = normalize( vec3(0,0,10) - vertexPositionCS ) / (-1.0f);

  eyeDirCS = normalize( vec3(0,0,-10) - theVertex ); // * (-1.0f);

  LightDirCS = normalize(LightPos  + eyeDirCS );

  fragmentColor = theColor.xyz; //color of vertices will be interpolated to produce color of fragment
}
