#version 330 core

// Interpolated values from the vertex shaders
//in vec3 positionWS;
in vec3 normalCS;
in vec3 LightDirCS;
in vec3 eyeDirCS;
in vec3 fragmentColor;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
//uniform vec4 LightPos;  // position
uniform vec4 LightProp; //ambient diffuse specular & intensity

void main() {
  float cosTheta = clamp( dot(normalCS, LightDirCS), 0,1 ); // Cos angle normal / light dir

  vec3 R = reflect(LightDirCS, normalCS);          // reflected light
  float cosAlpha = clamp( dot(eyeDirCS, R), 0,1 ); // Cos angle Eye vector / Reflect vector

  vec3 LightColor = vec3(1,1,1);  // Light emission properties

  //float dist = length( LightPos.xyz - positionWS ); // Distance to light

  color = ( LightProp.x              * fragmentColor            + // Ambient-indirect
	    LightProp.y * LightColor * fragmentColor * cosTheta + // Diffuse-object ///(dist*dist)
	    LightProp.z * LightColor * pow(cosAlpha,5)            // Specular-reflect ///(dist*dist)
	    ) * LightProp.w ;
}
