/**
* \brief Example Scene 2 implementation
* \file
*/
#include <cassert>
#include "examplescene2.h"

/**
 * \brief Create cube with texture coordinates and face indices.
 */
void ExampleScene2::createCube()
{
	// Define our object (Texture-mapped 2x2x2 cube centered at origin).
	// Note that after it has been stored in GPU memory, source buffers could be freed as long as we remember how many
	// vertices we want to to render from it.
	// 
	// Because we need different texture coordinates (and normal coordinates for lighting calculations), we need to define different vertices
	// for all the different parameter combinations even when position coordinates for the same vertex are the same. If _all_ the used vertex
	// properties (position, normal, texture coordinates, vertex color, ...) are the same, we can reuse the same index value! This happens
	// in this example when we define triangles that form each side of the cube.
	//
	// After we have defined each side, we also define vertex index values that are used for actual triangles that are drawn. All the triangles
	// are defined in counter-clockwise orientation.
	//
	// Texture coordinates with labeled sides      Vertex names for our cube
	//                                             v8      v7
	//  3/3+---+---+---+                           *------*     y
	//     | 1 | 2 | 3 |                          /|     /|     ^
	//  2/3+---+---+---+                       v4/ |  v3/ |     |
	//     | 4 | 5 | 6 |                        *--*---*--*v6   +--->x
	//  1/3+---+---+---+                        | /v5  | /     /
	//     |   |   |   |                        |/     |/     z
	//    0+---+---+---+                        *------*
	//     0  1/3 2/3 3/3                      v1      v2
	//
	// Bottom area of the texture will be unused in this example so that when a square texture is used, the source texture is not stretched
	cube.clear();
	cubeIndices.clear();

	/*
	// Debug texture for side 1 that shows it fully
	cube.push_back(Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f)); // 0: Vertex 1 of side 1
	cube.push_back(Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f)); // 1: Vertex 2 of side 1
	cube.push_back(Vertex(1.0f, 1.0f, 1.0f, 1.0f, 1.0f)); // 2: Vertex 3 of side 1
	cube.push_back(Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f)); // 3: Vertex 4 of side 1
	*/

	// Side 1: v1, v2, v3, v4 (front)
	cube.push_back(Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 2.0f / 3.0f)); // 0: Vertex 1 of side 1
	cube.push_back(Vertex(1.0f, -1.0f, 1.0f, 1.0f / 3.0f, 2.0f / 3.0f)); // 1: Vertex 2 of side 1
	cube.push_back(Vertex(1.0f, 1.0f, 1.0f, 1.0f / 3.0f, 3.0f / 3.0f)); // 2: Vertex 3 of side 1
	cube.push_back(Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 3.0f / 3.0f)); // 3: Vertex 4 of side 1
	// Triangle 1 of side 1: v1, v2, v3
	cubeIndices.push_back(0); cubeIndices.push_back(1); cubeIndices.push_back(2);

	// Triangle 2 of side 1: v3, v4, v1
	cubeIndices.push_back(2); cubeIndices.push_back(3); cubeIndices.push_back(0);

	// Side 2: v2, v6, v7, v3 (right side)
	cube.push_back(Vertex(1.0f, -1.0f, 1.0f, 1.0f / 3.0f, 2.0f / 3.0f)); // 4: Vertex 2 of side 2
	cube.push_back(Vertex(1.0f, -1.0f, -1.0f, 2.0f / 3.0f, 2.0f / 3.0f)); // 5: Vertex 6 of side 2
	cube.push_back(Vertex(1.0f, 1.0f, -1.0f, 2.0f / 3.0f, 3.0f / 3.0f)); // 6: Vertex 7 of side 2
	cube.push_back(Vertex(1.0f, 1.0f, 1.0f, 1.0f / 3.0f, 3.0f / 3.0f)); // 7: Vertex 3 of side 2

	// Triangle 1 of side 2: v2, v6, v7
	cubeIndices.push_back(4); cubeIndices.push_back(5); cubeIndices.push_back(6);
	// Triangle 2 of side 2: v7, v3, v2
	cubeIndices.push_back(6); cubeIndices.push_back(7); cubeIndices.push_back(4);

	// Side 3: v2, v1, v5, v6 (bottom)
	cube.push_back(Vertex(1.0f, -1.0f, 1.0f, 2.0f / 3.0f, 2.0f / 3.0f)); // 8: Vertex 2 of side 3
	cube.push_back(Vertex(-1.0f, -1.0f, 1.0f, 3.0f / 3.0f, 2.0f / 3.0f)); // 9: Vertex 1 of side 3
	cube.push_back(Vertex(-1.0f, -1.0f, -1.0f, 3.0f / 3.0f, 3.0f / 3.0f)); // 10: Vertex 5 of side 3
	cube.push_back(Vertex(1.0f, -1.0f, -1.0f, 2.0f / 3.0f, 3.0f / 3.0f)); // 11: Vertex 6 of side 3

	// Triangle 1 of side 3: v1, v2, v6
	cubeIndices.push_back(8); cubeIndices.push_back(9); cubeIndices.push_back(10);
	// Triangle 2 of side 3: v6, v5, v1
	cubeIndices.push_back(10); cubeIndices.push_back(11); cubeIndices.push_back(8);

	// Side 4: v4, v3, v7, v8 (top)
	cube.push_back(Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f / 3.0f)); // 12: Vertex 4 of side 4
	cube.push_back(Vertex(1.0f, 1.0f, 1.0f, 1.0f / 3.0f, 1.0f / 3.0f)); // 13: Vertex 3 of side 4
	cube.push_back(Vertex(1.0f, 1.0f, -1.0f, 1.0f / 3.0f, 2.0f / 3.0f)); // 14: Vertex 7 of side 4
	cube.push_back(Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 2.0f / 3.0f)); // 15: Vertex 8 of side 4

	// Triangle 1 of side 4: v4, v3, v7
	cubeIndices.push_back(12); cubeIndices.push_back(13); cubeIndices.push_back(14);
	// Triangle 2 of side 4: v7, v8, v4
	cubeIndices.push_back(14); cubeIndices.push_back(15); cubeIndices.push_back(12);

	// Side 5: v5, v1, v4, v8 (left)
	cube.push_back(Vertex(-1.0f, -1.0f, -1.0f, 1.0f / 3.0f, 1.0f / 3.0f)); // 16: Vertex 5 of side 5
	cube.push_back(Vertex(-1.0f, -1.0f, 1.0f, 2.0f / 3.0f, 1.0f / 3.0f)); // 17: Vertex 1 of side 5
	cube.push_back(Vertex(-1.0f, 1.0f, 1.0f, 2.0f / 3.0f, 2.0f / 3.0f)); // 18: Vertex 4 of side 5
	cube.push_back(Vertex(-1.0f, 1.0f, -1.0f, 1.0f / 3.0f, 2.0f / 3.0f)); // 19: Vertex 8 of side 5

	// Triangle 1 of side 5: v5, v1, v4
	cubeIndices.push_back(16); cubeIndices.push_back(17); cubeIndices.push_back(18);
	// Triangle 2 of side 5: v4, v8, v5
	cubeIndices.push_back(18); cubeIndices.push_back(19); cubeIndices.push_back(16);

	// Side 6: v6, v5, v8, v7 (back)
	cube.push_back(Vertex(1.0f, -1.0f, -1.0f, 2.0f / 3.0f, 1.0f / 3.0f)); // 20: Vertex 6 of side 6
	cube.push_back(Vertex(-1.0f, -1.0f, -1.0f, 3.0f / 3.0f, 1.0f / 3.0f)); // 21: Vertex 5 of side 6
	cube.push_back(Vertex(-1.0f, 1.0f, -1.0f, 3.0f / 3.0f, 2.0f / 3.0f)); // 22: Vertex 8 of side 6
	cube.push_back(Vertex(1.0f, 1.0f, -1.0f, 2.0f / 3.0f, 2.0f / 3.0f)); // 23: Vertex 7 of side 6

	// Triangle 1 of side 6: v6, v5, v8
	cubeIndices.push_back(20); cubeIndices.push_back(21); cubeIndices.push_back(22);
	// Triangle 2 of side 6: v8, v7, v6
	cubeIndices.push_back(22); cubeIndices.push_back(23); cubeIndices.push_back(20);
}

ExampleScene2::ExampleScene2() :
	cubeTexture(0)
{
	// These must be defined by the OpenGL (or through GLEW) for this example to work..
	assert(glUseProgram != 0);
	assert(glClearColor != 0);
	assert(glEnable != 0);
	assert(glDepthFunc != 0);
	assert(glDisableVertexAttribArray != 0);
	assert(glDeleteBuffers != 0);
	assert(glDeleteVertexArrays != 0);
	assert(glGenVertexArrays != 0);
	assert(glBindVertexArray != 0);
	assert(glGenBuffers != 0);
	assert(glBindBuffer != 0);
	assert(glBufferData != 0);
	assert(glVertexAttribPointer != 0);
}

ExampleScene2::~ExampleScene2()
{
	// Clean up everything
	glUseProgram(0); // Shader state
	glDisableVertexAttribArray(shaderProgram.getPositionAttribLocation()); // VBO state
	glDisableVertexAttribArray(shaderProgram.getTexture0AttribLocation());
	glDeleteBuffers(1, &ibo); // Allocated index data
	glDeleteBuffers(1, &vbo); // Allocated vertex data
	glDeleteVertexArrays(1, &vao); // Allocated object data

	if (cubeTexture)
		delete cubeTexture;
}

bool ExampleScene2::init()
{
	// Load shader program used in this example
	if (!shaderProgram.load("data/examplescene2.vs", "data/examplescene2.fs"))
		return false;

	// Create cube geometry
	createCube();

	// Load cube texture
	cubeTexture = new Texture("data/cube-texture.png");
	/*
	// Add a transparent horizontal line
	for (unsigned int x = 0; x < cubeTexture->getWidth(); ++x)
		cubeTexture->setPixel(x, 150, 0, 255, 0, 0);
	cubeTexture->updateGLTexture();
	*/
	// Something failed?
	if (cubeTexture->getTextureId() == 0)
		return false;
	std::cout << "Loaded cube texture as texture " << cubeTexture->getTextureId() << std::endl;

	// Use shader program to render everything
	glUseProgram(shaderProgram.getShaderProgram());

	// Get uniform location for the shader's texture sampler
	GLint uniform_cubeShader_texture = glGetUniformLocation(shaderProgram.getShaderProgram(), "texture0");

	if (uniform_cubeShader_texture < 0)
	{
		std::cerr << "Unable to locate uniform variable texture0 from the shader" << std::endl;
		return false;
	}
	std::cout << "texture0 uniform id: " << uniform_cubeShader_texture << std::endl;

	// Welect what texture unit is used for
	usedTextureUnit = 0;
	glUniform1i(uniform_cubeShader_texture, usedTextureUnit);

	//Initialize clear color for glClear()
	glClearColor(0.2f, 0.3f, 0.1f, 1.f);

	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);

	// Enable blending (slower but necessary for partially transparent objects)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Allocate and assign a Vertex Array Object to our handle
	glGenVertexArrays(1, &vao);

	// Bind our Vertex Array Object as the currently used object
	// VAO will hold the state of our IBO and VBO below. Also any
	// settings selected for these buffer objects will be part of the VAO state as well.
	glBindVertexArray(vao);

	// Create Index Buffer Object
	glGenBuffers(1, &ibo);

	// Bind our Index Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	// Store our index values in Index Buffer Object
	// GL_STATIC_DRAW means that we don't expect this buffer to change (It's a hint that it can be stored on GPU)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(GLushort), &cubeIndices[0], GL_STATIC_DRAW);

	// Allocate and assign One Vertex Buffer Object to our handle
	glGenBuffers(1, &vbo);

	// Bind our VBO as being the active buffer and storing vertex attributes (coordinates + colors)
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Copy the vertex data from tetrahedron to our buffer
	// 12 * sizeof(GLfloat) is the size of the tetrahedrom array, since it contains 12 Vertex values
	// GL_STATIC_DRAW means that we don't expect this buffer to change (It's a hint that it can be stored on GPU)
	glBufferData(GL_ARRAY_BUFFER, cube.size() * sizeof (struct Vertex), &cube[0], GL_STATIC_DRAW);

	// Note: The following attribute indexes must match what is defined in shader (in shaderprogram.cpp) for glBindAttribLocation() calls!

	// Specify that our coordinate data is going into attribute index 0 (shaderProgram.getPositionAttribLocation()), and contains three doubles per vertex
	// Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) 0
	glVertexAttribPointer(shaderProgram.getPositionAttribLocation(), 3, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (const GLvoid*)offsetof(struct Vertex, position));

	// Enable attribute index 0 as being used
	glEnableVertexAttribArray(shaderProgram.getPositionAttribLocation());

	// Specify that our color data is going into attribute index 1 (shaderProgram.getColorAttribLocation()), and contains three floats per vertex
	// Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) ( 3 * sizeof ( GLdouble ) ) i.e. the size (in bytes)
	// occupied by the first attribute (position)
	glVertexAttribPointer(shaderProgram.getTexture0AttribLocation(), 2, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (const GLvoid*)offsetof(struct Vertex, uv));

	// Enable attribute index 1 as being used
	glEnableVertexAttribArray(shaderProgram.getTexture0AttribLocation()); // Bind our second VBO as being the active buffer and storing vertex attributes (colors)

	// Reset object rotation
	rotation = 0;

	// Our generated VAO is now ready and enabled for use
	// We could disable by calling glBindVertexArray(0) if we want to work with multiple objects and not use created VAO at the moment.

	// Set up our view matrix that determines camera position in the scene
	// glm::lookAt replaces old GLU library functionality for creating a projection matrix
	viewMat = glm::lookAt(glm::vec3(0.0f, 1.5f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	return true;
}

void ExampleScene2::resize(GLsizei width, GLsizei height)
{
	// Update OpenGL viewport to match window system's window size
	glViewport(0, 0, width, height);

	// Set up projection matrix and model matrix etc.
	float fovy = 45.0f;
	projectionMat = glm::perspective(fovy, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
}

void ExampleScene2::update(float timestep)
{
	// Rotate object
	rotation += glm::two_pi<float>() * 0.1f * timestep;
}

// Render view
void ExampleScene2::render()
{
	// Clear background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Calculate model transformation
	modelMat = glm::rotate(glm::mat4(), rotation, glm::vec3(0.0, 1.0, 0.0)); // Rotate object around y-axis

	// Select correct shader program for this object (we never selected anything else to replace that state after init())
	glUseProgram(shaderProgram.getShaderProgram());

	// Precalculate transformation matrix for the shader and use it
	mvpMat = projectionMat * viewMat * modelMat;
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getShaderProgram(), "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(mvpMat));

	// Turn on texture mapping on texture unit 0 and select our texture
	// It is redundant to set the same values all the time but texture settings are included here for clarity
	// glUniform1i(uniform_cubeShader_texture, usedTextureUnit); <- In init code
	glActiveTexture(GL_TEXTURE0 + usedTextureUnit); // Other textures are GL_TEXTURE0 + i (where i is the texture unit index up to GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1)
	glBindTexture(GL_TEXTURE_2D, cubeTexture->getTextureId());

	// For drawing the cube it needs to be bound using (we never bound anything else to replace that state after init())
	glBindVertexArray(vao);

	// Count is the number of elements in the array that will form triangles. It is not the number of triangles defined by the array.
	// When VBOs are in use and GL_ELEMENT_ARRAY_BUFFER is bound, the last parameter (pointer to data) is interpreted as an offset within IBO instead of
	// actual program memory address.
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(cubeIndices.size()), GL_UNSIGNED_SHORT, 0);
}

bool ExampleScene2::handleEvent(const SDL_Event &e)
{
        // Put any event handling code here.
        // Window-resizing is handled in event loop already.

	// Return false if you want to stop the program
	return true;
}

