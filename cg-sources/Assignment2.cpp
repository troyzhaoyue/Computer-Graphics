/**
* \brief Assignment1 Scene implementation
* \file
*/
#include <cassert>
#include "Assignment2.h"


Assignment2::Assignment2() 
{
	assert(glBindBuffer != 0);
	assert(glBindVertexArray != 0);
	assert(glBufferData != 0);
	assert(glClear != 0);
	assert(glClearColor != 0);
	assert(glCullFace != 0);
	assert(glDepthFunc != 0);
	assert(glDeleteBuffers != 0);
	assert(glDeleteVertexArrays != 0);
	assert(glDisableVertexAttribArray != 0);
	assert(glDrawArrays != 0);
	assert(glEnable != 0);
	assert(glGenVertexArrays != 0);
	assert(glGenBuffers != 0);
	assert(glUseProgram != 0);
	assert(glUniformMatrix4fv != 0);
	assert(glVertexAttribPointer != 0);
	assert(glViewport != 0);
}

Assignment2::~Assignment2() 
{
	glUseProgram(0); // Shader state
	
	glDeleteBuffers(1, &ebo_flag);
	glDeleteBuffers(1, &ebo_pole);
	
	glDeleteBuffers(1, &vbo_flag);
	glDeleteBuffers(1, &vbo_pole);

	glDeleteProgram(flag_shader_ID);
	glDeleteProgram(pole_shader_ID);

	glDeleteVertexArrays(1, &vao);

	if (flagTexture)
		delete flagTexture;

}

void Assignment2::createFlag(GLfloat flagHeight, GLfloat flagWidth, GLfloat poleHeight) 
{
	flag.clear();
	flagIndices.clear();
	GLfloat TriangleHeight = flagHeight / 6.0f;
	GLfloat TriangleWidth = flagWidth / 9.0f;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			//No. (10 * i + j) Vertex
			flag.push_back(Vertex(glm::vec3(j * TriangleWidth, poleHeight - i * TriangleHeight, 0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec2(j * 1.0f/9,1.0 - i * 1.0f/6)));
		}
	}

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			flagIndices.push_back(10 * i + j); flagIndices.push_back(10 * (i + 1) + j); flagIndices.push_back(10 * i + j + 1);
			flagIndices.push_back(10 * (i + 1) + j); flagIndices.push_back(10 * i + j + 1); flagIndices.push_back(10 * (i + 1) + j + 1);
		}
	} 

	return;
}

void Assignment2::createPole(GLfloat poleHeight, GLfloat poleWidth) {   
	//                         v7      v6
	//                           *------*     y
	//                          /|     /|     ^
	//                       v3/ |  v2/ |     |
	//                        *--*---*--*v5   +--->x
	//                        | /v4  | /     /
	//                        |/     |/     z
	//                        *------*
	//                      v0      v1
	
	pole.clear();
	poleIndices.clear();
	//8 vertex
	pole.push_back(Vertex(glm::vec3(-poleWidth, -poleHeight, poleWidth/2.0), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec2(0.0f, 0.0f)));		//0
	pole.push_back(Vertex(glm::vec3(0.0, -poleHeight, poleWidth/2.0), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec2(0.0f, 0.0f)));				//1
	pole.push_back(Vertex(glm::vec3(0.0, poleHeight, poleWidth/2.0), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec2(0.0f, 0.0f)));				//2
	pole.push_back(Vertex(glm::vec3(-poleWidth, poleHeight, poleWidth/2.0), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec2(0.0f, 0.0f)));		//3
	pole.push_back(Vertex(glm::vec3(-poleWidth, -poleHeight, -poleWidth/2.0), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec2(0.0f, 0.0f)));		//4
	pole.push_back(Vertex(glm::vec3(0.0, -poleHeight, -poleWidth/2.0), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec2(0.0f, 0.0f)));			//5
	pole.push_back(Vertex(glm::vec3(0.0, poleHeight, -poleWidth/2.0), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec2(0.0f, 0.0f)));				//6
	pole.push_back(Vertex(glm::vec3(-poleWidth, poleHeight, -poleWidth/2.0), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec2(0.0f, 0.0f)));		//7
	
	//front
	poleIndices.push_back(0); poleIndices.push_back(1); poleIndices.push_back(2);
	poleIndices.push_back(2); poleIndices.push_back(3); poleIndices.push_back(0);

	//right
	poleIndices.push_back(1); poleIndices.push_back(5); poleIndices.push_back(6);
	poleIndices.push_back(6); poleIndices.push_back(2); poleIndices.push_back(1);

	//bottom
	poleIndices.push_back(1); poleIndices.push_back(0); poleIndices.push_back(4);
	poleIndices.push_back(4); poleIndices.push_back(5); poleIndices.push_back(1);

	//left
	poleIndices.push_back(4); poleIndices.push_back(0); poleIndices.push_back(3);
	poleIndices.push_back(3); poleIndices.push_back(7); poleIndices.push_back(4);

	//top
	poleIndices.push_back(3); poleIndices.push_back(2); poleIndices.push_back(6);
	poleIndices.push_back(6); poleIndices.push_back(7); poleIndices.push_back(3);

	//back
	poleIndices.push_back(5); poleIndices.push_back(4); poleIndices.push_back(7);
	poleIndices.push_back(7); poleIndices.push_back(6); poleIndices.push_back(5);

	return;
}

bool Assignment2::init()
{
	// Vertex Shader for Animated Flag
	if (!shaderProgram.load("data/waving_flag.vs", "data/waving_flag.fs"))
		return false;

	flag_shader_ID = shaderProgram.getShaderProgram();

	// Vertex Shader for Static Pole
	if (!shaderProgram.load("data/static_pole.vs", "data/static_pole.fs"))
		return false;

	pole_shader_ID = shaderProgram.getShaderProgram();

	//create models
	createPole(0.9f, 0.1f);			//create a pole with fixed height and width
	createFlag(0.6f, 0.9f, 0.9f);

	// Load  texture image
	flagTexture = new Texture("data/flag-texture.png");
	if (flagTexture->getTextureId() == 0)
		return false;
	std::cout << "Loaded flag texture as texture " << flagTexture->getTextureId() << std::endl;

	
	//use flag shader here
	glUseProgram(flag_shader_ID);

	// Get uniform location for the shader's texture sampler
	GLint uniform_flagShader_texture = glGetUniformLocation(flag_shader_ID, "texture0");
	if (uniform_flagShader_texture < 0)
	{
		std::cerr << "Unable to locate uniform variable texture0 from the shader" << std::endl;
		return false;
	}
	std::cout << "texture0 uniform id: " << uniform_flagShader_texture << std::endl;

	// Welect what texture unit is used for
	usedTextureUnit = 0;
	glUniform1i(uniform_flagShader_texture, usedTextureUnit);

	//Initialize clear color for glClear()
	glClearColor(0.0f, 0.3f, 0.1f, 1.f);

	// Enable GP Depth Testing
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);


	// Allocate and assign a Vertex Array Object to our handle
	glGenVertexArrays(1, &vao);
	// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vao);
	
	
	// Allocate and assign a Vertex Buffer Object to our handle for flag
	glGenBuffers(1, &vbo_flag);
	// Bind our VBO as being the active buffer and storing vertex attributes (coordinates + colors)
	glBindBuffer(GL_ARRAY_BUFFER, vbo_flag);
	// Copy the vertex data from tetrahedron to our buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct Vertex) * flag.size(), &flag[0], GL_STATIC_DRAW);

	// The same thing for element of flag
	glGenBuffers(1, &ebo_flag);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_flag);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * flagIndices.size(), &flagIndices[0], GL_STATIC_DRAW);

	//the same thing for pole
	glGenBuffers(1, &vbo_pole);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pole);
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct Vertex) * pole.size(), &pole[0], GL_STATIC_DRAW);
	glGenBuffers(1, &ebo_pole);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_pole);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * poleIndices.size(), &poleIndices[0], GL_STATIC_DRAW);

	// Unbind used buffer 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	return true;
}

void Assignment2::resize(GLsizei width, GLsizei height) 
{

	glViewport(0, 0, width, height);
	projectionMat = glm::perspective(fovy, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
	viewMat = glm::lookAt(position, direction, up);
}

void Assignment2::update(float timestep) 
{

	gtime += timestep;
}

void Assignment2::render() 
{
	// Clear background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	computemvpMat();
	render_flag();
	render_pole();

}

void Assignment2::render_flag()
{
	glUseProgram(flag_shader_ID);
	
	glDisable(GL_CULL_FACE);			//both side of flag can be seen

	glBindBuffer(GL_ARRAY_BUFFER, vbo_flag);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_flag);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glUniform1f(glGetUniformLocation(flag_shader_ID, "gtime"), gtime);

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(flagIndices.size()), GL_UNSIGNED_INT, 0);
}

void Assignment2::render_pole()
{
	glUseProgram(pole_shader_ID);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_pole);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_pole);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(poleIndices.size()), GL_UNSIGNED_INT, 0);
}


void Assignment2::computemvpMat() 
{

	projectionMat = glm::perspective(fovy, aspectRatio, 0.1f, 100.0f);
	viewMat = glm::lookAt(position, direction, up);
	mvpMat = projectionMat * viewMat * modelMat;

	glUseProgram(flag_shader_ID);
	glUniformMatrix4fv(glGetUniformLocation(flag_shader_ID, "mvpmatrix"), 1, GL_FALSE, &mvpMat[0][0]);

	glUseProgram(pole_shader_ID);
	glUniformMatrix4fv(glGetUniformLocation(pole_shader_ID, "mvpmatrix"), 1, GL_FALSE, &mvpMat[0][0]);

}



bool Assignment2::handleEvent(const SDL_Event &e)
{

	switch (e.type) {
	case SDL_KEYDOWN:

		float h_rotation_radians;
		float v_rotation_radians;

		switch (e.key.keysym.scancode) {
		case SDL_SCANCODE_LEFT:  
			h_rotation -= 15;
			break;
		case SDL_SCANCODE_RIGHT:  
			h_rotation += 15;
			break;
		case SDL_SCANCODE_DOWN:  
			if (v_rotation > -90)
				v_rotation -= 15;
			break;
		case SDL_SCANCODE_UP:  
			if (v_rotation < 90)
				v_rotation += 15;
			break;
		}
		h_rotation_radians = (float)h_rotation * 3.14f / 180.0f;
		v_rotation_radians = (float)v_rotation * 3.14f / 180.0f;

		position = glm::vec3(camera_distance * cos(v_rotation_radians) * sin(h_rotation_radians),
							 camera_distance * sin(v_rotation_radians),
							 camera_distance * cos(v_rotation_radians) * cos(h_rotation_radians));

	}
	
	return true;
}
