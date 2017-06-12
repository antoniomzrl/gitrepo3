
// From loadshaders:
//#if defined(GLSL330)
//#version 330 core
//#elif defined(GLSL130)
//#version 130
//#endif

#if defined(VERTEXSHADER)

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 IMV;  // inverse-transpose MV
uniform vec3 LightPos;
uniform vec3 sizeFactor;


// Input vertex data, different for all executions of shader.
#if defined(GLSL330)
layout(location = 0) in vec3 theVertex;   // vertices Position modelspace
layout(location = 1) in vec3 theNormal;   // normals  modelspace
layout(location = 2) in vec3 thePosition; // positions;
layout(location = 3) in vec4 theColor;    // colors;
#elif defined(GLSL130)
in vec3 theVertex;
in vec3 theNormal;
in vec3 thePosition;
in vec4 theColor;
#endif


// Output data interpolated for each fragment. //out vec3 positionWS;
out vec3 normalCS;
out vec3 LightDirCS;
out vec3 eyeDirCS;
out vec3 fragmentColor;


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



#elif defined(FRAGMENTSHADER)

// Values that stay constant for the whole mesh.
//uniform vec4 LightPos;  // position
uniform vec4 LightProp; //ambient diffuse specular & intensity

// Interpolated values from the vertex shaders
//in vec3 positionWS;
in vec3 normalCS;
in vec3 LightDirCS;
in vec3 eyeDirCS;
in vec3 fragmentColor;


// Ouput data
out vec3 color;


void main() {
  float cosTheta = clamp( dot(normalCS, LightDirCS), 0,1 ); // Cos angle normal / light dir

  vec3 R = reflect(LightDirCS, normalCS);           // reflected light
  float cosAlpha = clamp( dot(eyeDirCS, R), 0, 1 ); // Cos angle Eye vector / Reflect vector

  vec3 LightColor = vec3(1,1,1);  // Light emission properties

  //float dist = length( LightPos.xyz - positionWS ); // Distance to light

  color = ( LightProp.x              * fragmentColor            + // Ambient-indirect
	    LightProp.y * LightColor * fragmentColor * cosTheta + // Diffuse-object ///(dist*dist)
	    LightProp.z * LightColor * pow(cosAlpha,5)            // Specular-reflect ///(dist*dist)
	    ) * LightProp.w ;

}

#endif //VERTEXSHADER // FRAGMENTSHADER
