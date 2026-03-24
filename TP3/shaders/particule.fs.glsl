#version 330 core

in vec3 vFragColor;

out vec3 fFragColor;

void main() {

    vec4 v = gl_FragCoord;
    float a = 4.0;
    float b = 0.05;

    vec2 center = vec2(400.0, 300.0);

    float dist = distance(v.xy, center);
    float x = a * exp(-b * dist);

    fFragColor = vec3(vFragColor * x);
};