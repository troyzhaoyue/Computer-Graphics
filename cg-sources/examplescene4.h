/**
 * \brief Example Scene 4 interface
 */
#ifndef EXAMPLE_SCENE_4_H_
#define EXAMPLE_SCENE_4_H_

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
class ExampleScene4 : public Scene
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec3 normal;

		Vertex(const glm::vec3 &position_ = glm::vec3(0.0f, 0.0f, 0.0f),
		       const glm::vec3 &color_ = glm::vec3(0.0f, 0.0f, 0.0f),
			   const glm::vec3 &normal_ = glm::vec3(0.0f, 0.0f, 0.0f)) :
			   position(position_),
			   color(color_),
			   normal(normal_)
		{
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
	std::vector<Vertex> sphere;
	std::vector<GLushort> sphereIndices;

	void createIcosahedron(std::vector<Vertex> &tetrahedron, std::vector<GLushort> &tetrahedronIndices) const;
	void createSphere(std::vector<Vertex> &sphere, std::vector<GLushort> &sphereIndices, int numTesselations) const;

	void updateShading(std::vector<Vertex> &mesh, const glm::mat4 &modelMat, const glm::vec4 lightPos) const;
public:
	ExampleScene4();
	virtual ~ExampleScene4();

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
