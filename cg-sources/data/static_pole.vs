#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Color;

uniform mat4 mvpmatrix;

out vec3 ex_Color;

void main(){

gl_Position = mvpmatrix * vec4(in_Position , 1.0);
ex_Color = in_Color;

}