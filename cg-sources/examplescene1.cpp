/**
 * \brief Example Scene 1 implementation
 * \file
 */
#include <cassert>
#include <iostream>
#include "examplescene1.h"

void ExampleScene1::createTetrahedron()
{
	// Define our object. Note that after it has been stored in GPU memory, source buffers could be freed as long as we remember how many
	// vertices we want to to render from it
	tetrahedron.clear();

	tetrahedron.push_back(Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f));
	tetrahedron.push_back(Vertex(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f));
	tetrahedron.push_back(Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f));

	tetrahedron.push_back(Vertex(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f));
	tetrahedron.push_back(Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f));
	tetrahedron.push_back(Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f));

	tetrahedron.push_back(Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f));
	tetrahedron.push_back(Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f));
	tetrahedron.push_back(Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f));

	tetrahedron.push_back(Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f));
	tetrahedron.push_back(Vertex(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f));
	tetrahedron.push_back(Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f));
}

ExampleScene1::ExampleScene1()
{
	// These OpenGL functions must be defined by the OpenGL (or through GLEW) for this example to work..
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

ExampleScene1::~ExampleScene1()
{
	// Clean up everything
	glUseProgram(0); // Shader state
	glDisableVertexAttribArray(shaderProgram.getPositionAttribLocation()); // VBO state
	glDisableVertexAttribArray(shaderProgram.getColorAttribLocation());
	glDeleteBuffers(1, &vbo); // Allocated vertex data
	glDeleteVertexArrays(1, &vao); // Allocated object data
}

bool ExampleScene1::init()
{
	// Load shader program used in this example
	if (!shaderProgram.load("data/examplescene1.vs", "data/examplescene1.fs"))
		return false;

	// Create object geometry
	createTetrahedron();

	// Use shader program to render everything
	glUseProgram(shaderProgram.getShaderProgram());

	//Initialize clear color for glClear()
	glClearColor(0.2f, 0.3f, 0.1f, 1.f);

	// Back-face culling will improve how the object looks at the borders and improve performance. Check it out!
//	glCullFace(GL_BACK);
//	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	// Allocate and assign a Vertex Array Object to our handle
	glGenVertexArrays(1, &vao);

	// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vao);

	// Allocate and assign One Vertex Buffer Object to our handle
	glGenBuffers(1, &vbo);

	// Bind our VBO as being the active buffer and storing vertex attributes (coordinates + colors)
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Copy the vertex data from tetrahedron to our buffer
	// 12 * sizeof(GLfloat) is the size of the tetrahedrom array, since it contains 12 Vertex values
	// GL_STATIC_DRAW means that we don't expect this buffer to change (It's a hint that it can be stored on GPU)
	glBufferData(GL_ARRAY_BUFFER, tetrahedron.size() * sizeof (struct Vertex), &tetrahedron[0], GL_STATIC_DRAW);

	// Note: The following attribute indexes must match what is defined in shader (in shaderprogram.cpp) for glBindAttribLocation() calls!

	// Specify that our coordinate data is going into attribute index 0 (shaderProgram.getPositionAttribLocation()), and contains three floats per vertex
	// Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) 0
	glVertexAttribPointer(shaderProgram.getPositionAttribLocation(), 3, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (const GLvoid*)offsetof(struct Vertex, position));

	// Enable attribute index 0 as being used
	glEnableVertexAttribArray(shaderProgram.getPositionAttribLocation());

	// Specify that our color data is going into attribute index 1 (shaderProgram.getColorAttribLocation()), and contains three floats per vertex
	// Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) ( 3 * sizeof ( GLdouble ) ) i.e. the size (in bytes)
	// occupied by the first attribute (position)
	glVertexAttribPointer(shaderProgram.getColorAttribLocation(), 3, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (const GLvoid*)offsetof(struct Vertex, color));

	// Enable attribute index 1 as being used
	glEnableVertexAttribArray(shaderProgram.getColorAttribLocation()); // Bind our second VBO as being the active buffer and storing vertex attributes (colors)

	// Unbind buffer (not strictly necessary but it is a state in context instead of vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Reset object rotation
	rotation = 0;

	return true;
}

void ExampleScene1::resize(GLsizei width, GLsizei height)
{
	// Update OpenGL viewport to match window system's window size
	glViewport(0, 0, width, height);

	// Set up projection matrix and model matrix etc.
	float fovy = 45.0f;

	projectionMat = glm::perspective(fovy, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
	viewMat = glm::lookAt(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

}

void ExampleScene1::update(float timestep)
{
	// Rotate object (one rotation / 10 seconds)
	rotation += glm::two_pi<float>() * 0.1f * timestep;
}

// Render view
void ExampleScene1::render()
{
	// Clear background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Calculate model transformation
	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, glm::vec3(1.0, 0.0, 0.0)); // Translate object +1 on x-axis after rotation
	modelMat = glm::rotate(modelMat, rotation, glm::vec3(0.0, 1.0, 0.0)); // Rotate object around y-axis
	modelMat = glm::scale(modelMat, glm::vec3(0.5f, 0.5f, 0.5f));

	// Precalculate transformation matrix for the shader and use it
	mvpMat = projectionMat * viewMat * modelMat;
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getShaderProgram(), "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(mvpMat));

	// Draw individual triangles when we have correct VBO in use
	// We are not using tetrahedron vector data here at all. We need to just know the number of vertices to draw!
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(tetrahedron.size())); 
}

bool ExampleScene1::handleEvent(const SDL_Event &e)
{
        // Put any event handling code here.
        // Window-resizing is handled in event loop already.

	// Return false if you want to stop the program
	return true;
}
