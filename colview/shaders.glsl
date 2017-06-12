
// From loadshaders:
//#if defined(GLSL330)
//#version 330 core
//#elif defined(GLSL130)
//#version 130
//#endif

#if defined(VERTEXSHADER) ////////////////////////////////////////////////

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 IMV;     // inverse-transpose MV
uniform vec4 plt[10];
uniform int  pltSz;
uniform float pltDtz;
uniform float SizeFactor;
uniform float BallFactor;
uniform vec3 LightPos;


// For colorbar must match glsl shader
// uniform : plt
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


// Input vertex data, different for all executions of shader.
#if defined(GLSL330)
layout(location = 0) in vec3 theVertex; // vertices Position modelspace
layout(location = 1) in vec3 theNormal; // normals  modelspace
layout(location = 2) in vec4 thePV;     // position + value in (0,1)
#elif defined(GLSL130)
in vec3 theVertex;
in vec3 theNormal;
in vec4 thePV;
#endif


// Output data interpolated for each fragment. //out vec3 positionWS;
out vec3 normalCS;
out vec3 LightDirCS;
out vec3 eyeDirCS;
out vec3 fragmentColor;


void main() {
  float sz = BallFactor + (1 - BallFactor) * thePV.w;
  vec3 vertexWS = thePV.xyz + theVertex * sz * SizeFactor; //scale & translate
  gl_Position =  MVP * vec4(vertexWS,1); // Out pos of vertex in clip space

  normalCS = normalize( ( IMV * vec4(theNormal, 0)).xyz ); // Normal in camera space
  
  // Vector from vertex to  camera, in camera space.
  // In camera space, the camera is at (0,0,0).
  //vec3 vertexPositionCS = ( V * M * vec4(vertexWS,1)).xyz;
  //vec3 vertexPositionCS = ( IMV * vec4(theVertex,1)).xyz;
  //eyeDirCS = normalize( vec3(0,0,10) - vertexPositionCS ) / (-1.0f);
  eyeDirCS = normalize( vec3(0,0,-10) - theVertex ); // * (-1.0f);

  LightDirCS = normalize(LightPos  + eyeDirCS );

  //color of vertices will be interpolated to produce color of fragment
  fragmentColor = ComputeColor(thePV.w);
}



#elif defined(FRAGMENTSHADER) ////////////////////////////////////////////////

uniform vec4 Light;       //ambient diffuse specular & intensity
uniform vec3 LightColor;  // Light emission properties
uniform float greenAttenuation;

// Interpolated values from the vertex shaders
//in vec3 positionWS;
in vec3 normalCS;
in vec3 LightDirCS;
in vec3 eyeDirCS;
in vec3 fragmentColor;


// Ouput data
out vec3 color;


void main() {
  float cosTheta = clamp( dot(normalCS, LightDirCS), 0,1 ); // angle normal / light dir

  vec3 R = reflect(LightDirCS, normalCS);           // reflected light
  float cosAlpha = clamp( dot(eyeDirCS, R), 0, 1 ); // angle Eye vector / Reflect vector
  
  //float dist = length( LightPos.xyz - positionWS ); // Distance to light
  // color = ( Light.x              * fragmentColor            + // Ambient-indirect
  //           Light.y * LightColor * fragmentColor * cosTheta + // Diffuse-object ///(dist*dist)
  // 	       Light.z * LightColor * pow(cosAlpha,5)            // Specular-reflect ///(dist*dist)
  // 	       ) * Light.w ;

  color = ( Light.x * fragmentColor            +    // Ambient-indirect
	    Light.y * fragmentColor * cosTheta +    // Diffuse-object
	    Light.z * LightColor * pow(cosAlpha,9)  // Specular-reflect
	    ) * Light.w * (1.0f - greenAttenuation * fragmentColor.g);

}

#endif //VERTEXSHADER // FRAGMENTSHADER
