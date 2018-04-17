#version 330 core
layout (location=0) in vec3 in_Position;
layout (location=3) in vec2 in_TexCoord0;

// mvpmatrix is the result of multiplying the model, view, and projection matrices
uniform mat4 mvpmatrix;

// Texture coordinate for the fragment shader
out vec2 f_TexCoord0; 

void main(void)
{
	// Multiply the mvp matrix by the vertex to obtain our final vertex position
	// gl_Position is an output variable defined by standards
	gl_Position = mvpmatrix * vec4(in_Position, 1.0);

	f_TexCoord0 = in_TexCoord0;
}
