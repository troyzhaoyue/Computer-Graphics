#version 330 core
layout(location = 0) in vec3 in_Position;
layout(location = 2) in vec2 in_TexCoord0;

// mvpmatrix is the result of multiplying the model, view, and projection matrices
uniform float gtime;
uniform mat4 mvpmatrix;

out vec2 f_TexCoord0;
out vec3 ex_Color;      //result of shading
void main()
{
    
    
    float pi = 3.14;
    
    float Z_newpos = 0.8 * in_Position.x * sin(pi * in_Position.x - pi * gtime);

    gl_Position = mvpmatrix * vec4(in_Position.x, in_Position.y, Z_newpos, 1.0);
    
    f_TexCoord0 = in_TexCoord0;
    
}
