
#if defined(VERTEXSHADER)

#if defined(GLSL330)
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
#elif defined(GLSL130)
in vec4  vertex;
#endif

out vec2 TexCoords;
uniform mat4 projection;

void main() {
  gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
  TexCoords = vertex.zw;
}

#elif defined(FRAGMENTSHADER)

in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main() {    
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
  color = vec4(textColor, 1.0) * sampled;
}

#endif
