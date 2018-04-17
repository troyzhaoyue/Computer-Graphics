#version 330 core
layout (location=0) in vec3 in_Position;
layout (location=1) in vec3 in_Color;
layout (location=2) in vec3 in_Normal;

// mvpmatrix is the result of multiplying the model, view, and projection matrices
uniform mat4 mvpmatrix;

out vec3 ex_Color;

void main(void)
{
	// Multiply the mvp matrix by the vertex to obtain our final vertex position
	// gl_Position is an output variable defined by standards
	gl_Position = mvpmatrix * vec4(in_Position, 1.0);
    
	ex_Color = in_Color;
}
