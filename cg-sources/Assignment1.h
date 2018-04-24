/**
* \brief Example Scene 1 interface
*/
#ifndef ASSIGNMENT_1
#define ASSIGNMENT_1

#include <vector>                       // Standard vector template
#include <GL/glew.h>                    // OpenGL extension wrangler library
#include <SDL.h>                        // libSDL functionality
#include <glm/glm.hpp>                  // Matrix library
#include <glm/gtc/matrix_transform.hpp> // Needed for glm::perspective() and friends.
#include <glm/gtc/type_ptr.hpp>         // Needed for glm::value_ptr(x). You can use &x[0] instead of that.
#include "scene.h"                      // Abstract scene class
#include "shaderprogram.h"              // For shader management

/**
* \brief Draws a vertex-colored tetrahedron
*/
class Assignment : public Scene
{
	int obj_number = 0;
	struct Vertex
	{
		GLfloat position[3];
		GLfloat color[3];

		Vertex(GLfloat x = 0.0, GLfloat y = 0.0, GLfloat z = 0.0, GLfloat r = 0.0f, GLfloat g = 0.0f, GLfloat b = 0.0f)
		{
			position[0] = x;
			position[1] = y;
			position[2] = z;
			color[0] = r;
			color[1] = g;
			color[2] = b;
		}
	};

	ShaderProgram shaderProgram;
	glm::mat4 mvpMat;

	glm::mat4 projectionMat;
	glm::mat4 viewMat;
	glm::mat4 modelMat;

	// Vertex Array Object and Vertex Buffer Object handlers
	GLuint vao, vbo, vbo_texture;

	GLsizei windowWidth;
	GLsizei windowHeight;


	float rotation; // Current rotation position
	std::vector<Vertex> tetrahedron; // Source data for our model
	std::vector<Vertex> tetrahedron_backend; // Source data for our model

	void createTetrahedron(float offset = 0.0f, bool unique_color = false, float R = 0.0f, float G = 0.0f, float B = 0.0f);
public:
	Assignment();
	virtual ~Assignment();

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

	GLuint frameBufferName;
	GLuint renderedTexture;
	void updatecolor(int index);
	int findobject(int x, int y);

};


#endif
