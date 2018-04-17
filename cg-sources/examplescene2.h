/**
* \brief Example Scene 2 interface
*/
#ifndef EXAMPLE_SCENE_2_H_
#define EXAMPLE_SCENE_2_H_

#include <vector>                       // Standard vector template
#include <GL/glew.h>                    // OpenGL extension wrangler library
#include <SDL.h>                        // libSDL functionality
#include <glm/glm.hpp>                  // Matrix library
#include <glm/gtc/matrix_transform.hpp> // Needed for glm::perspective() and friends.
#include <glm/gtc/type_ptr.hpp>         // Needed for glm::value_ptr(x). You can use &x[0] instead of that.
#include "scene.h"                      // Abstract scene class
#include "shaderprogram.h"              // For shader management
#include "texture.h"

/**
 * \brief Draws a texture mapped cube
 */
class ExampleScene2 : public Scene
{
	struct Vertex
	{
		GLfloat position[3];
		GLfloat uv[2]; // Texture coordinates

		Vertex(GLfloat x = 0.0, GLfloat y = 0.0, GLfloat z = 0.0, GLfloat u = 0.0f, GLfloat v = 0.0f)
		{
			position[0] = x;
			position[1] = y;
			position[2] = z;
			uv[0] = u;
			uv[1] = v;
		}
	};

	ShaderProgram shaderProgram;
	glm::mat4 mvpMat;

	glm::mat4 projectionMat;
	glm::mat4 viewMat;
	glm::mat4 modelMat;

	// Vertex Array Object, Vertex Buffer Object and Index Buffer Object handlers
	GLuint vao, vbo, ibo;

	float rotation; // Current rotation position
	std::vector<Vertex> cube; // Source data for our model
	std::vector<GLushort> cubeIndices; // Index values for cube
	Texture *cubeTexture;
	GLuint usedTextureUnit;

	void createCube();
public:
	ExampleScene2();
	virtual ~ExampleScene2();

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
