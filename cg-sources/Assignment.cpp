/**
* \brief Example Scene 1 implementation
* \file
*/
#include <cassert>
#include <iostream>
#include "Assignment.h"
#include <stdlib.h> 
#include <time.h>  

void Assignment::createTetrahedron(float y_offset, bool unique_color, float R, float G, float B)
{
	// Define our object. Note that after it has been stored in GPU memory, source buffers could be freed as long as we remember how many
	// vertices we want to to render from it

	if (unique_color)
	{
		tetrahedron_backend.push_back(Vertex(1.0f, 1.0f + y_offset, 1.0f, R, G, B));
		tetrahedron_backend.push_back(Vertex(-1.0f, 1.0f + y_offset, -1.0f, R, G, B));
		tetrahedron_backend.push_back(Vertex(-1.0f, -1.0f + y_offset, 1.0f, R, G, B));

		tetrahedron_backend.push_back(Vertex(1.0f, 1.0f + y_offset, 1.0f, R, G, B));
		tetrahedron_backend.push_back(Vertex(-1.0f, -1.0f + y_offset, 1.0f, R, G, B));
		tetrahedron_backend.push_back(Vertex(1.0f, -1.0f + y_offset, -1.0f, R, G, B));

		tetrahedron_backend.push_back(Vertex(1.0f, 1.0f + y_offset, 1.0f, R, G, B));
		tetrahedron_backend.push_back(Vertex(1.0f, -1.0f + y_offset, -1.0f, R, G, B));
		tetrahedron_backend.push_back(Vertex(-1.0f, 1.0f + y_offset, -1.0f, R, G, B));

		tetrahedron_backend.push_back(Vertex(-1.0f, -1.0f + y_offset, 1.0f, R, G, B));
		tetrahedron_backend.push_back(Vertex(-1.0f, 1.0f + y_offset, -1.0f, R, G, B));
		tetrahedron_backend.push_back(Vertex(1.0f, -1.0f + y_offset, -1.0f, R, G, B));
	}
	else 
	{
		tetrahedron.push_back(Vertex(1.0f, 1.0f + y_offset, 1.0f, R, G, B));
		tetrahedron.push_back(Vertex(-1.0f, 1.0f + y_offset, -1.0f, R, G, B));
		tetrahedron.push_back(Vertex(-1.0f, -1.0f + y_offset, 1.0f, R, G, B));

		tetrahedron.push_back(Vertex(1.0f, 1.0f + y_offset, 1.0f, R, G, B));
		tetrahedron.push_back(Vertex(-1.0f, -1.0f + y_offset, 1.0f, R, G, B));
		tetrahedron.push_back(Vertex(1.0f, -1.0f + y_offset, -1.0f, R, G, B));

		tetrahedron.push_back(Vertex(1.0f, 1.0f + y_offset, 1.0f, R, G, B));
		tetrahedron.push_back(Vertex(1.0f, -1.0f + y_offset, -1.0f, R, G, B));
		tetrahedron.push_back(Vertex(-1.0f, 1.0f + y_offset, -1.0f, R, G, B));

		tetrahedron.push_back(Vertex(-1.0f, -1.0f + y_offset, 1.0f, R, G, B));
		tetrahedron.push_back(Vertex(-1.0f, 1.0f + y_offset, -1.0f, R, G, B));
		tetrahedron.push_back(Vertex(1.0f, -1.0f + y_offset, -1.0f, R, G, B));
		obj_number++;
	}
	
}


Assignment::Assignment()
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

Assignment::~Assignment()
{
	// Clean up everything
	glUseProgram(0); // Shader state
	glDisableVertexAttribArray(shaderProgram.getPositionAttribLocation()); // VBO state
	glDisableVertexAttribArray(shaderProgram.getColorAttribLocation());
	glDeleteBuffers(1, &vbo); // Allocated vertex data
	glDeleteVertexArrays(1, &vao); // Allocated object data
}

bool Assignment::init()
{
	// Load shader program used in this example
	if (!shaderProgram.load("data/Assignment.vs", "data/Assignment.fs"))
		return false;

	// Create 3 tetrahedron along y axis
	tetrahedron.clear();
	createTetrahedron(0.0f, false, 1.0f, 0.0f, 0.0f);
	createTetrahedron(0.0f, false, 1.0f, 0.0f, 0.0f);
	createTetrahedron(0.0f, false, 1.0f, 0.0f, 0.0f);

	tetrahedron_backend.clear();
	for (int i = 0; i < obj_number; i++) 
	{
		createTetrahedron(0.0f, true, i * 0.1, 0.0, 0.0);
	}


	// Use shader program to render everything
	glUseProgram(shaderProgram.getShaderProgram());

	//Initialize clear color for glClear()
	glClearColor(0.2f, 0.3f, 0.1f, 1.f);

	// Back-face culling will improve how the object looks at the borders and improve performance. Check it out!
	//	glCullFace(GL_BACK);
	//	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

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
	glBufferData(GL_ARRAY_BUFFER, tetrahedron.size() * sizeof(struct Vertex), &tetrahedron[0], GL_STATIC_DRAW);



	glBindBuffer(GL_ARRAY_BUFFER, vbo);



	// Note: The following attribute indexes must match what is defined in shader (in shaderprogram.cpp) for glBindAttribLocation() calls!

	// Specify that our coordinate data is going into attribute index 0 (shaderProgram.getPositionAttribLocation()), and contains three floats per vertex
	// Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) 0
	glVertexAttribPointer(shaderProgram.getPositionAttribLocation(), 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*)offsetof(struct Vertex, position));

	// Enable attribute index 0 as being used
	glEnableVertexAttribArray(shaderProgram.getPositionAttribLocation());

	// Specify that our color data is going into attribute index 1 (shaderProgram.getColorAttribLocation()), and contains three floats per vertex
	// Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) ( 3 * sizeof ( GLdouble ) ) i.e. the size (in bytes)
	// occupied by the first attribute (position)
	glVertexAttribPointer(shaderProgram.getColorAttribLocation(), 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*)offsetof(struct Vertex, color));

	// Enable attribute index 1 as being used
	glEnableVertexAttribArray(shaderProgram.getColorAttribLocation()); // Bind our second VBO as being the active buffer and storing vertex attributes (colors)

	// Unbind buffer (not strictly necessary but it is a state in context instead of vbo
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Reset object rotation
	rotation = 0;

	return true;
}

void Assignment::resize(GLsizei width, GLsizei height)
{
	windowWidth = width;
	windowHeight = height;
	
	// Update OpenGL viewport to match window system's window size
	glViewport(0, 0, windowWidth, windowHeight);

	// Set up projection matrix and model matrix etc.
	float fovy = 45.0f;

	projectionMat = glm::perspective(fovy,															//zoom
									static_cast<float>(windowWidth) / static_cast<float>(windowHeight),			//Aspect Ratio. Depends on the size of your window.
									0.1f,															//Near clipping plane.
									1000.0f);														//Far clipping plane.
	viewMat = glm::lookAt(glm::vec3((-2.0f * obj_number) / 2, 0.0f, 5.0f),				//position of camera
						  glm::vec3(0.0f, 0.0f, 0.0f),				//where to look at (world space)
		                  glm::vec3(0.0f, 1.0f, 0.0f));				// up-side/down   

}

void Assignment::update(float timestep)
{
	// Rotate object (one rotation / 10 seconds)
	rotation += glm::two_pi<float>() * 0.1f * timestep;
}

// Render view
void Assignment::render()
{
	//std::cout << "still render!" << std::endl;
	// Clear background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	for (int i = 0; i < obj_number; i++) 
	{
		// Calculate model transformation
		modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, glm::vec3(0.0 + i * 2.0, 0.0, 0.0)); //  no translate object 
		modelMat = glm::rotate(modelMat, rotation, glm::vec3(0.0, 1.0, 0.0)); // Rotate object around y-axis
		modelMat = glm::scale(modelMat, glm::vec3(0.6f - i * 0.1, 0.6f - i * 0.1, 0.6f - i * 0.1));

		// Precalculate transformation matrix for the shader and use it
		mvpMat = projectionMat * viewMat * modelMat;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getShaderProgram(), "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(mvpMat));

		// Draw individual triangles when we have correct VBO in use
		// We are not using tetrahedron vector data here at all. We need to just know the number of vertices to draw!
		glDrawArrays(GL_TRIANGLES, i * static_cast<GLsizei>(tetrahedron.size() / obj_number), static_cast<GLsizei>(tetrahedron.size() / obj_number));
	}

}

int Assignment::findobject(int x, int y)
{
	frameBufferName = 0;
	glGenFramebuffers(1, &frameBufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferName);

	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)		// check that our framebuffer is ok
		return -1;
	


	glBufferData(GL_ARRAY_BUFFER, tetrahedron.size() * sizeof(struct Vertex), &tetrahedron_backend[0], GL_STATIC_DRAW);
	render();
	glBufferData(GL_ARRAY_BUFFER, tetrahedron.size() * sizeof(struct Vertex), &tetrahedron[0], GL_STATIC_DRAW);

	GLfloat pixels[3];
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);								//pixel-color-read
	glDeleteTextures(1, &renderedTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	int obj_index = -1;
	for (int i = 0; i < obj_number; i++) {
		if ((pixels[0]- i * 0.1) <0.03 && pixels[1] == 0.0 && pixels[2] == 0.0)		//find it!
		{
			obj_index = i;
			break;
		}
	}
	std::cout << "Unique color:" << pixels[0] << "," << pixels[1] << "," << pixels[2] << std::endl;
	std::cout << obj_index << std::endl;
	return obj_index;
}

void Assignment::updatecolor(int index)
{
	srand(time(0));
	std::cout << "seed: " << time(0) << std::endl;
	for (int i = 0; i < 12; i++)
	{
		tetrahedron[index * 12 + i].color[0] = rand() / float(RAND_MAX);
		tetrahedron[index * 12 + i].color[1] = rand() / float(RAND_MAX);
		tetrahedron[index * 12 + i].color[2] = rand() / float(RAND_MAX);
	}
	glBufferData(GL_ARRAY_BUFFER, tetrahedron.size() * sizeof(struct Vertex), &tetrahedron[0], GL_STATIC_DRAW);
	render();			//render immediately
}

bool Assignment::handleEvent(const SDL_Event &e)
{
	// Put any event handling code here.
	// Window-resizing is handled in event loop already.
	switch (e.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		int obj_index = findobject(e.button.x, e.button.y);
		if (obj_index != -1)
		{
			updatecolor(obj_index);
		}
		break;
	}

	// Return false if you want to stop the program
	return true;
}
