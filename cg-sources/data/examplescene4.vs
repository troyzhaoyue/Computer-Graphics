#version 330 core
layout (location=0) in vec3 in_Position;
layout (location=1) in vec3 in_Color; // Ignored if shading..
layout (location=2) in vec3 in_Normal;

// model-view and projection matrices
uniform mat4 mvmatrix;
uniform mat4 pmatrix;
uniform vec4 lightPosition; // In view coordinate frame!

out vec3 ex_Color;
//out vec4 gl_Position;

const vec4 ambientProduct = vec4(0.1, 0.1, 0.1, 1.0);
const vec4 diffuseProduct = vec4(0.5, 0.3, 0.2, 1.0);
const vec4 specularProduct = vec4(0.9, 0.9, 0.9, 1.0);
const float shininess = 100.0;

void main(void)
{
	vec3 pos = (mvmatrix * vec4(in_Position, 1.0)).xyz;
	vec3 light = lightPosition.xyz;
	vec3 L = normalize(light - pos);
	vec3 E = normalize(-pos);
	vec3 H = normalize(L + E);

	vec3 N = normalize((mvmatrix * vec4(in_Normal, 0.0)).xyz);

	vec4 ambient = ambientProduct;

	float Kd = max(dot(L, N), 0.0);
	vec4 diffuse = Kd * diffuseProduct;

	float Ks = pow(max(dot(N, H), 0.0), shininess);
	vec4 specular = Ks * specularProduct;

	if (dot(L, N) < 0.0) {
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	}

	ex_Color = (ambient + diffuse + specular).rgb;

	// Multiply the mvp matrix by the vertex to obtain our final vertex position
	// gl_Position is an output variable defined by GLSL standards
	gl_Position = pmatrix * mvmatrix * vec4(in_Position, 1.0);
    
//	ex_Color = in_Color;
}
