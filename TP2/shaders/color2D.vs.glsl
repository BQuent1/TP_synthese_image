#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragColor;

mat3 translate(float tx, float ty) {
  return mat3(
    1, 0, 0,
    0, 1, 0,
    tx, ty, 1
  );
}

mat3 scale(float sx, float sy) {
  return mat3(
    sx, 0, 0,
    0, sy, 0,
    0, 0, 1
  );
}

mat3 rotate(float angle) {
  float c = cos(angle);
  float s = sin(angle);
  return mat3(
    c, -s, 0,
    s, c, 0,
    0, 0, 1
  );
}

void main() {
  vFragColor = aVertexColor;
  //déplacer le triangle selon (0.5,0.5) et le réduire de moitié
  gl_Position = vec4(translate(0.5,0.5) * scale(0.5,0.5) * vec3(aVertexPosition, 1.0), 1.0);
};