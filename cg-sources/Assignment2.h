	/**
	* \brief Assignment 2 Scene Interface
	*/
#ifndef ASSIGNMENT_2
#define ASSIGNMENT_2

#include <vector>                       // Standard vector template
#include <GL/glew.h>                    // OpenGL extension wrangler library
#include <SDL.h>                        // libSDL functionality
#include <glm/glm.hpp>                  // Matrix library
#include <glm/gtc/matrix_transform.hpp> // Needed for glm::perspective() and friends.
#include <glm/gtc/type_ptr.hpp>         // Needed for glm::value_ptr(x). You can use &x[0] instead of that.
#include "scene.h"                      // Abstract scene class
#include "shaderprogram.h"              // For shader management
#include "texture.h"

class Assignment2 : public Scene
{
	struct Vertex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 uv_cord;

		Vertex(glm::vec3 pos, glm::vec3 col, glm::vec2 uv) 
		{
			position = pos;
			color = col;
			uv_cord = uv;
		};

	};

	ShaderProgram shaderProgram;
	glm::mat4 mvpMat;
	
	glm::mat4 modelMat;
	glm::mat4 viewMat;	
	glm::mat4 projectionMat;

	GLuint vao;
	GLuint vbo_flag;
	GLuint ebo_flag;


	GLuint vbo_pole;
	GLuint ebo_pole;

	GLuint flag_shader_ID;
	GLuint pole_shader_ID;


	float camera_distance = 3.0f;
	glm::vec3 position = glm::vec3(0.0, 0.0, camera_distance);
	glm::vec3 direction = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);


	float fovy = 45.0f;
	float aspectRatio = 4.0f / 3.0f;



	std::vector<Vertex> flag;
	std::vector<GLuint> flagIndices;

	std::vector<Vertex> pole;
	std::vector<GLuint> poleIndices;




	GLfloat gtime = 0;

	//initial view angle of camera
	int h_rotation = 0; 
	int v_rotation = 0;

	//texture 
	Texture *flagTexture;
	GLuint usedTextureUnit;

	void createFlag(GLfloat flagHeight, GLfloat flagWidth, GLfloat poleHeight);
	void createPole(GLfloat poleHeight, GLfloat poleWidth);
	void render_flag();
	void render_pole();
	void computemvpMat();


public:
	//cannot be changed!   Used in main interface

	Assignment2();
	virtual ~Assignment2();

	// Initialize scene
	virtual bool init();

	// Called on window resize
	virtual void resize(GLsizei width, GLsizei height);

	// Update scene
	virtual void update(float timestep);

	// Render view
	virtual void render();

	// Handle SDL event
	virtual bool handleEvent(const SDL_Event &e);
};


#endif