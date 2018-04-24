#version 330 core
in  vec3 ex_Color;
layout (location=0) out vec4 fragColor;

void main(void) {
    fragColor = vec4(ex_Color + vec3(1.0,0.0,0.0),1.0);
}
