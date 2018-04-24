#version 330 core
layout(location = 0) in vec3 in_Position;
layout(location = 2) in vec2 in_TexCoord0;
layout(location = 3) in vec3 in_Normal;

// mvpmatrix is the result of multiplying the model, view, and projection matrices
uniform float gtime;
//uniform mat4 mvpmatrix;


// model-view and projection matrices
uniform mat4 mvmatrix;
uniform mat4 pmatrix;
uniform vec4 lightPosition; // In view coordinate frame!


const vec4 ambientProduct = vec4(0.1, 0.1, 0.1, 1.0);
const vec4 diffuseProduct = vec4(0.9, 0.0, 0.0, 1.0);
const vec4 specularProduct = vec4(0.7, 0.7, 0.7, 1.0);
const float shininess = 80.0;



out vec2 f_TexCoord0;
out vec3 ex_Color;      //result of shading
void main()
{
    float pi = 3.14;
    
    float Z_newpos = 0.8 * in_Position.x * sin(pi * in_Position.x - pi * gtime);

    gl_Position = pmatrix * mvmatrix * vec4(in_Position.x, in_Position.y, Z_newpos, 1.0);




    vec3 pos = (mvmatrix * vec4(in_Position.x, in_Position.y, Z_newpos, 1.0)).xyz;
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
    //ex_Color = vec3(0.0f,0.0f,0.5f);
    f_TexCoord0 = in_TexCoord0;
    
}
