#version 330 core

smooth in vec4 vertexColor;

out vec4 outputColor;

void main() {
  outputColor = vertexColor;
}
