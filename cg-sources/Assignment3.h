	/**
	* \brief Assignment 3 Scene Interface
	*/
#ifndef ASSIGNMENT_3
#define ASSIGNMENT_3

#include <vector>                       // Standard vector template
#include <GL/glew.h>                    // OpenGL extension wrangler library
#include <SDL.h>                        // libSDL functionality
#include <glm/glm.hpp>                  // Matrix library
#include <glm/gtc/matrix_transform.hpp> // Needed for glm::perspective() and friends.
#include <glm/gtc/type_ptr.hpp>         // Needed for glm::value_ptr(x). You can use &x[0] instead of that.
#include "scene.h"                      // Abstract scene class
#include "shaderprogram.h"              // For shader management
#include "texture.h"

class Assignment3 : public Scene
{
private:	
	struct Vertex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 uv_cord;
		glm::vec3 normal;

		Vertex(	glm::vec3 pos, 
				glm::vec3 col, 
				glm::vec2 uv,
				glm::vec3 norm) 
		{
			position = pos;
			color = col;
			uv_cord = uv;
			normal = norm;
		};

	};

	ShaderProgram shaderProgram;
	
	
	glm::mat4 mvpMat;
	glm::mat4 modelMat;
	glm::mat4 viewMat;	
	glm::mat4 projectionMat;
	glm::mat4 modelViewMat;

	GLuint vao;

	//flag
	std::vector<Vertex> flag;
	std::vector<GLuint> flagIndices;
	GLuint vbo_flag;
	GLuint ebo_flag;
	GLuint flag_shader_ID;
	void createFlag(GLfloat flagHeight, GLfloat flagWidth, GLfloat poleHeight);
	void render_flag();
	GLfloat gtime = 0;
	Texture *flagTexture;

	//pole
	std::vector<Vertex> pole;
	std::vector<GLuint> poleIndices;
	GLuint vbo_pole;
	GLuint ebo_pole;
	GLuint pole_shader_ID;
	void createPole(GLfloat poleHeight, GLfloat poleWidth);
	void render_pole();

	//land
	std::vector<Vertex> land;
	std::vector<GLuint> landIndices;
	GLuint vbo_land;
	GLuint ebo_land;
	GLuint land_shader_ID;
	void createLand(GLfloat x_len, GLfloat z_len, GLfloat y_offset);
	void render_land();
	Texture *landTexture;


	//camera
	float camera_distance = 3.0f;
	glm::vec3 position = glm::vec3(0.0, 0.0, camera_distance);
	glm::vec3 direction = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	float fovy = 45.0f;
	float aspectRatio = 4.0f / 3.0f;
	int h_rotation = 0; 
	int v_rotation = 0;

	//texture 
	GLuint usedTextureUnit;


	//private computation functions
	void computemvpMat();


public:
	//cannot be changed!   Used in main interface

	Assignment3();
	virtual ~Assignment3();

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