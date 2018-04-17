/**
 * \brief Example Scene 3 implementation
 * \file
 */
#include <cassert>
#include <iostream>
#include "examplescene4.h"

void ExampleScene4::createIcosahedron(std::vector<Vertex> &icosahedron, std::vector<GLushort> &icosahedronIndices) const
{
	// Define our object. Note that after it has been stored in GPU memory, source buffers could be freed as long as we remember how many
	// vertices we want to to render from it
	icosahedron.clear();

	icosahedron.push_back(Vertex(glm::normalize(glm::vec3(-0.26286500f, 0.0000000f, 0.42532500f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::normalize(glm::vec3(-0.26286500f, 0.0000000f, 0.42532500f))));
	icosahedron.push_back(Vertex(glm::normalize(glm::vec3(0.26286500f, 0.0000000f, 0.42532500f)), glm::vec3(1.0f, 0.0f, 0.0f), glm::normalize(glm::vec3(0.26286500f, 0.0000000f, 0.42532500f))));
	icosahedron.push_back(Vertex(glm::normalize(glm::vec3(-0.26286500f, 0.0000000f, -0.42532500f)), glm::vec3(0.0f, 1.0f, 0.0f), glm::normalize(glm::vec3(-0.26286500f, 0.0000000f, -0.42532500f))));
	icosahedron.push_back(Vertex(glm::normalize(glm::vec3(0.26286500f, 0.0000000f, -0.42532500f)), glm::vec3(0.0f, 0.0f, 1.0f), glm::normalize(glm::vec3(0.26286500f, 0.0000000f, -0.42532500f))));
	icosahedron.push_back(Vertex(glm::normalize(glm::vec3(0.0000000f, 0.42532500f, 0.26286500f)), glm::vec3(1.0f, 1.0f, 0.0f), glm::normalize(glm::vec3(0.0000000f, 0.42532500f, 0.26286500f))));
	icosahedron.push_back(Vertex(glm::normalize(glm::vec3(0.0000000f, 0.42532500f, -0.26286500f)), glm::vec3(1.0f, 0.0f, 1.0f), glm::normalize(glm::vec3(0.0000000f, 0.42532500f, -0.26286500f))));
	icosahedron.push_back(Vertex(glm::normalize(glm::vec3(0.0000000f, -0.42532500f, 0.26286500f)), glm::vec3(0.0f, 1.0f, 1.0f), glm::normalize(glm::vec3(0.0000000f, -0.42532500f, 0.26286500f))));
	icosahedron.push_back(Vertex(glm::normalize(glm::vec3(0.0000000f, -0.42532500f, -0.26286500f)), glm::vec3(1.0f, 1.0f, 1.0f), glm::normalize(glm::vec3(0.0000000f, -0.42532500f, -0.26286500f))));
	icosahedron.push_back(Vertex(glm::normalize(glm::vec3(0.42532500f, 0.26286500f, 0.0000000f)), glm::vec3(1.0f, 0.5f, 0.0f), glm::normalize(glm::vec3(0.42532500f, 0.26286500f, 0.0000000f))));
	icosahedron.push_back(Vertex(glm::normalize(glm::vec3(-0.42532500f, 0.26286500f, 0.0000000f)), glm::vec3(1.0f, 0.0f, 0.5f), glm::normalize(glm::vec3(-0.42532500f, 0.26286500f, 0.0000000f))));
	icosahedron.push_back(Vertex(glm::normalize(glm::vec3(0.42532500f, -0.26286500f, 0.0000000f)), glm::vec3(0.5f, 1.0f, 0.0f), glm::normalize(glm::vec3(0.42532500f, -0.26286500f, 0.0000000f))));
	icosahedron.push_back(Vertex(glm::normalize(glm::vec3(-0.42532500f, -0.26286500f, 0.0000000f)), glm::vec3(0.5f, 0.0f, 1.0f), glm::normalize(glm::vec3(-0.42532500f, -0.26286500f, 0.0000000f))));

	// Create indices
	icosahedronIndices.push_back(0); icosahedronIndices.push_back(6);  icosahedronIndices.push_back(1);
	icosahedronIndices.push_back(0); icosahedronIndices.push_back(11); icosahedronIndices.push_back(6);
	icosahedronIndices.push_back(1); icosahedronIndices.push_back(4);  icosahedronIndices.push_back(0);
	icosahedronIndices.push_back(1); icosahedronIndices.push_back(8);  icosahedronIndices.push_back(4);
	icosahedronIndices.push_back(1); icosahedronIndices.push_back(10); icosahedronIndices.push_back(8);
	icosahedronIndices.push_back(2); icosahedronIndices.push_back(5);  icosahedronIndices.push_back(3);
	icosahedronIndices.push_back(2); icosahedronIndices.push_back(9);  icosahedronIndices.push_back(5);
	icosahedronIndices.push_back(2); icosahedronIndices.push_back(11); icosahedronIndices.push_back(9);
	icosahedronIndices.push_back(3); icosahedronIndices.push_back(7);  icosahedronIndices.push_back(2);
	icosahedronIndices.push_back(3); icosahedronIndices.push_back(10); icosahedronIndices.push_back(7);
	icosahedronIndices.push_back(4); icosahedronIndices.push_back(8);  icosahedronIndices.push_back(5);
	icosahedronIndices.push_back(4); icosahedronIndices.push_back(9);  icosahedronIndices.push_back(0);
	icosahedronIndices.push_back(5); icosahedronIndices.push_back(8);  icosahedronIndices.push_back(3);
	icosahedronIndices.push_back(5); icosahedronIndices.push_back(9);  icosahedronIndices.push_back(4);
	icosahedronIndices.push_back(6); icosahedronIndices.push_back(10); icosahedronIndices.push_back(1);
	icosahedronIndices.push_back(6); icosahedronIndices.push_back(11); icosahedronIndices.push_back(7);
	icosahedronIndices.push_back(7); icosahedronIndices.push_back(10); icosahedronIndices.push_back(6);
	icosahedronIndices.push_back(7); icosahedronIndices.push_back(11); icosahedronIndices.push_back(2);
	icosahedronIndices.push_back(8); icosahedronIndices.push_back(10); icosahedronIndices.push_back(3);
	icosahedronIndices.push_back(9); icosahedronIndices.push_back(11); icosahedronIndices.push_back(0);
}

void ExampleScene4::createSphere(std::vector<Vertex> &sphere, std::vector<GLushort> &sphereIndices, int numTesselations) const
{
	// Start with a tetrahedron
//	createTetrahedron(sphere, sphereIndices);
	createIcosahedron(sphere, sphereIndices);

	// Tesselate object numTesselations times until we have a round enough sphere
	for (int tesselation = 0; tesselation < numTesselations; ++tesselation)
	{
		// Get number of triangles at previous tesselation level
		size_t origTriangles = sphereIndices.size() / 3;

		std::cout << "Tesselation " << tesselation + 1 << " with " << origTriangles << " triangles" << std::endl;

		// Split each existing triangle v0, v1, v2 into four parts using a new vertices v3, v4 and v5
		//         *v2       //
		//        / \        //
		//       /   \       //
		//    v5*.....*v4    //
		//     / .   . \     //
		//    /   . .   \    //
		// v0*-----*-----*v1 //
		//         v3        //
		for (size_t t = 0; t < origTriangles; ++t)
		{
			// Get a new center point of the vertices
			size_t tpos = t * 3;

			GLushort v0Index = sphereIndices[tpos];
			GLushort v1Index = sphereIndices[tpos + 1];
			GLushort v2Index = sphereIndices[tpos + 2];
			const Vertex &v0 = sphere[v0Index];
			const Vertex &v1 = sphere[v1Index];
			const Vertex &v2 = sphere[v2Index];

//			std::cout << "Splitting triangle (v0, v1, v2): " << v0Index << ", " << v1Index << ", " << v2Index << std::endl;
//			std::cout << "v0 coordinates: " << v0.position.x << ", " << v0.position.y << ", " << v0.position.z << std::endl;
//			std::cout << "v1 coordinates: " << v1.position.x << ", " << v1.position.y << ", " << v1.position.z << std::endl;
//			std::cout << "v2 coordinates: " << v2.position.x << ", " << v2.position.y << ", " << v2.position.z << std::endl;

			// Create new vertices
			Vertex v3(glm::normalize(v0.position + v1.position), 0.5f * (v0.color + v1.color), glm::normalize(v0.position + v1.position));
			Vertex v4(glm::normalize(v1.position + v2.position), 0.5f * (v1.color + v2.color), glm::normalize(v1.position + v2.position));
			Vertex v5(glm::normalize(v2.position + v0.position), 0.5f * (v2.color + v0.color), glm::normalize(v2.position + v0.position));

//			std::cout << "v3 coordinates: " << v3.position.x << ", " << v3.position.y << ", " << v3.position.z << std::endl;
//			std::cout << "v4 coordinates: " << v4.position.x << ", " << v4.position.y << ", " << v4.position.z << std::endl;
//			std::cout << "v5 coordinates: " << v5.position.x << ", " << v5.position.y << ", " << v5.position.z << std::endl;

			// Add a new vertex index for the center point
			GLuint v3Index = static_cast<GLuint>(sphere.size());
			sphere.push_back(v3);
			GLuint v4Index = static_cast<GLuint>(sphere.size());
			sphere.push_back(v4);
			GLuint v5Index = static_cast<GLuint>(sphere.size());
			sphere.push_back(v5);

//			std::cout << "v3 index: " << v3Index << std::endl;
//			std::cout << "v4 index: " << v4Index << std::endl;
//			std::cout << "v5 index: " << v5Index << std::endl;

			// Replace original triangle v0, v1, v2 with v0, v3, v5
			sphereIndices[tpos + 1] = v3Index;
			sphereIndices[tpos + 2] = v5Index;

			// Add triangle v3, v1, v4 to the model
			sphereIndices.push_back(v3Index); sphereIndices.push_back(v1Index); sphereIndices.push_back(v4Index);

			// Add triangle v3, v4, v5 to the model
			sphereIndices.push_back(v3Index); sphereIndices.push_back(v4Index); sphereIndices.push_back(v5Index);

			// Add triangle v5, v4, v2 to the model
			sphereIndices.push_back(v5Index); sphereIndices.push_back(v4Index); sphereIndices.push_back(v2Index);
		}
	}

	// Note that some (half) of new vertices are duplicates after tesselation as edges are shared with another triangle!
	std::cout << "Created sphere with " << sphere.size() << " vertices and " << sphereIndices.size() / 3 << " faces" << std::endl;

	// Verify that we don't have too many vertices (over 65535)
	assert(sphere.size() <= (GLushort)-1);
}

/**
 * \brief Update mesh shading using Blinn-Phong model.
 * We can create Gouraud-shading (vertex-based shading) this way with a point light source
 * This ignores set vertex colors and overwrites them with given material colors
 */
void ExampleScene4::updateShading(std::vector<Vertex> &mesh, const glm::mat4 &modelMat, const glm::vec4 lightPos) const
{
	// Viewer parameters
	glm::vec4 viewPos(0.0f, 1.0f, 5.0f, 1.0f);

	// Different light properties
	glm::vec4 lightAmbient(0.2, 0.2, 0.2, 1.0);
	glm::vec4 lightDiffuse(1.0, 1.0, 1.0, 1.0);
	glm::vec4 lightSpecular(1.0, 1.0, 1.0, 1.0);
	glm::float32 attenuationConstant(1.0f);
	glm::float32 attenuationLinear(0.0f);
	glm::float32 attenuationQuadratic(0.0f);

	// Different material properties
	glm::vec4 materialAmbient(1.0, 0.0, 1.0, 1.0);
	glm::vec4 materialDiffuse(1.0, 0.8, 0.0, 1.0);
	glm::vec4 materialSpecular(1.0, 0.8, 0.0, 1.0);
	glm::float32 materialShininess = 100.0f;
	glm::vec4 materialEmission(0.0, 0.3, 0.3, 1.0);

	// Effect of both light and material properties is multiplication
	glm::vec4 ambientProduct = glm::matrixCompMult(lightAmbient, materialAmbient);
	glm::vec4 diffuseProduct = glm::matrixCompMult(lightDiffuse, materialDiffuse);
	glm::vec4 specularProduct = glm::matrixCompMult(lightSpecular, materialSpecular);

	// For each vertex..
	for (unsigned int v = 0; v < mesh.size(); ++v)
	{
		// Diffuse component. Depends on an angle between light source and surface normal.
		// If normal points to opposite direction, it receives no light.
		glm::vec4 lightVector = lightPos - glm::vec4(mesh[v].position, 1.0f);
		glm::float32 lightDistance = glm::length(lightVector);
		lightVector /= lightDistance;
		glm::float32 diffuseCoef = glm::max(glm::dot(glm::vec4(mesh[v].normal, 0.0f), lightVector), 0.0f);

		glm::vec4 diffuse = diffuseCoef * diffuseProduct;

		// Specular component.
		glm::vec4 specular(0.0f, 0.0f, 0.0f, 1.0f);

		if (diffuseCoef > 0.0f)
		{
			glm::vec4 viewDirection = glm::normalize(viewPos - glm::vec4(mesh[v].position, 1.0f));
			glm::vec4 half = glm::normalize(lightVector + viewDirection);

			glm::float32 specularCoef = glm::dot(glm::vec3(half), mesh[v].normal);
			if (specularCoef > 0.0f)
				specular = glm::pow(specularCoef, materialShininess) * specularProduct;
		}

		// Calculate light attenuation based on distance
		glm::float32 lightAttenuation = 1.0f / ((attenuationQuadratic * lightDistance + attenuationLinear) * lightDistance + attenuationConstant);

		// Combine all the components to final colors
		mesh[v].color = (glm::vec3(diffuse) + glm::vec3(specular) + glm::vec3(ambientProduct)) * lightAttenuation + glm::vec3(materialEmission);
	}
}

ExampleScene4::ExampleScene4()
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

ExampleScene4::~ExampleScene4()
{
	// Clean up everything
	glUseProgram(0); // Shader state
	glDisableVertexAttribArray(shaderProgram.getPositionAttribLocation()); // VBO state
	glDisableVertexAttribArray(shaderProgram.getColorAttribLocation());
	glDeleteBuffers(1, &ibo); // Allocated index data
	glDeleteBuffers(1, &vbo); // Allocated vertex data
	glDeleteVertexArrays(1, &vao); // Allocated object data
}

bool ExampleScene4::init()
{
	// Reset object rotation
	rotation = 0;

	// Load shader program used in this example
	if (!shaderProgram.load("data/examplescene4.vs", "data/examplescene4.fs"))
		return false;

	// Create object geometry
	int numTesselations = 3;
	createSphere(sphere, sphereIndices, numTesselations);

	// Calculate model transformation
	modelMat = glm::rotate(glm::mat4(), rotation, glm::vec3(0.0, 1.0, 0.0)); // Rotate object around y-axis

	// Update our model's shading with some orientation in the world
	glm::vec4 lightPos(2.0f, 4.0f, 3.0f, 1.0f);
	updateShading(sphere, modelMat, lightPos);

	// Use shader program to render everything
	glUseProgram(shaderProgram.getShaderProgram());

	//Initialize clear color for glClear()
	glClearColor(0.2f, 0.3f, 0.1f, 1.f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);

	// Allocate and assign a Vertex Array Object to our handle
	glGenVertexArrays(1, &vao);

	// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vao);

	// Create Index Buffer Object
	glGenBuffers(1, &ibo);

	// Bind our Index Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	// Allocate and assign One Vertex Buffer Object to our handle
	glGenBuffers(1, &vbo);

	// Bind our VBO as being the active buffer and storing vertex attributes (coordinates + colors)
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Store our index values in Index Buffer Object
	// GL_STATIC_DRAW means that we don't expect this buffer to change (It's a hint that it can be stored on GPU)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(GLushort), &sphereIndices[0], GL_STATIC_DRAW);

	// Copy the vertex data from tetrahedron to our buffer
	// GL_STATIC_DRAW means that we don't expect this buffer to change (It's a hint that it can be stored on GPU)
	glBufferData(GL_ARRAY_BUFFER, sphere.size() * sizeof (struct Vertex), &sphere[0], GL_STATIC_DRAW);

	// Note: The following attribute indexes must match what is defined in shader (in shaderprogram.cpp) for glBindAttribLocation() calls!

	// Specify that our coordinate data is going into attribute index 0 (shaderProgram.getPositionAttribLocation()), and contains three floats per vertex
	glVertexAttribPointer(shaderProgram.getPositionAttribLocation(), 3, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (const GLvoid*)offsetof(struct Vertex, position));

	// Enable attribute index 0 as being used
	glEnableVertexAttribArray(shaderProgram.getPositionAttribLocation());

	// Specify that our color data is going into attribute index 1 (shaderProgram.getColorAttribLocation()), and contains three floats per vertex
	glVertexAttribPointer(shaderProgram.getColorAttribLocation(), 3, GL_FLOAT, GL_FALSE, sizeof (struct Vertex), (const GLvoid*)offsetof(struct Vertex, color));

	// Enable attribute index 1 as being used
	glEnableVertexAttribArray(shaderProgram.getColorAttribLocation()); // Bind our second VBO as being the active buffer and storing vertex attributes (colors)

	// Specify that our vertex normals are going into attribute index 2 (shaderProgram.getNormalAttribLocation()), and contains three floats per vertex
	glVertexAttribPointer(shaderProgram.getNormalAttribLocation(), 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*)offsetof(struct Vertex, normal));

	// Enable attribute index 2 as being used
	glEnableVertexAttribArray(shaderProgram.getNormalAttribLocation()); // Bind our second VBO as being the active buffer and storing vertex attributes (colors)

	// Unbind buffer (not strictly necessary but it is a state in context instead of vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Set up our view matrix that determines camera position in the scene
	// glm::lookAt replaces old GLU library functionality for creating a projection matrix
	viewMat = glm::lookAt(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	return true;
}

void ExampleScene4::resize(GLsizei width, GLsizei height)
{
	// Update OpenGL viewport to match window system's window size
	glViewport(0, 0, width, height);

	// Set up projection matrix and model matrix etc.
	float fovy = 45.0f;
	projectionMat = glm::perspective(fovy, static_cast<float>(width) / static_cast<float>(height), 0.1f, 1000.0f);
}

void ExampleScene4::update(float timestep)
{
	// Rotate object
	rotation += glm::two_pi<float>() * 0.1f * timestep;
}

// Render view
void ExampleScene4::render()
{
	// Clear background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Calculate model transformation
	modelMat = glm::rotate(glm::mat4(), rotation, glm::vec3(0.0, 1.0, 0.0)); // Rotate object around y-axis
	/*
	std::cout << "model matrix: " << std::endl;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
			std::cout << ' ' << modelMat[j][i];
		std::cout << std::endl;
	}

	std::cout << "view matrix: " << std::endl;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
			std::cout << ' ' << viewMat[j][i];
		std::cout << std::endl;
	}

	std::cout << "projection matrix: " << std::endl;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
			std::cout << ' ' << projectionMat[j][i];
		std::cout << std::endl;
	}
	*/
	// Update our model's shading with new orientation with the camera
	glm::vec4 lightPos(0.0f, 0.0f, 3.0f, 1.0f);
//	updateShading(sphere, modelMat, lightPos);

	// Precalculate transformation matrix for the shader and use it
	glm::vec4 lightInViewPos = viewMat * modelMat * lightPos; // Light tied to object transformation

//	std::cout << "Light in view pos: " << lightInViewPos.x << ", " << lightInViewPos.y << ", " << lightInViewPos.z << std::endl;

	glm::mat4 modelViewMat = viewMat * modelMat;
	/*
	glm::vec4 testVertexPos(0.0f, 0.0f, 0.1f, 1.0f);
	glm::vec4 testVertexNormal = testVertexPos;
	testVertexNormal.w = 0.0f;

	// Vertex in view
	std::cout << "vertex: " << testVertexPos.x << ", " << testVertexPos.y << ", " << testVertexPos.z << std::endl;
	std::cout << "normal: " << testVertexNormal.x << ", " << testVertexNormal.y << ", " << testVertexNormal.z << std::endl;
	glm::vec4 pos = modelViewMat * testVertexPos;
	std::cout << "in view: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
	glm::vec4 normal4 = modelViewMat * testVertexNormal;
	glm::vec3 normal(normal4.x, normal4.y, normal4.z);
	normal = glm::normalize(normal);
	std::cout << "normal in view: " << normal.x << ", " << normal.y << ", " << normal.z << std::endl;

	glm::vec4 L = glm::normalize(lightInViewPos - pos);
	glm::vec4 E = glm::normalize(-pos);
	glm::vec4 H = glm::normalize(L + E);

	std::cout << "L in view: " << L.x << ", " << L.y << ", " << L.z << std::endl;
	std::cout << "E in view: " << E.x << ", " << E.y << ", " << E.z << std::endl;
	std::cout << "H in view: " << H.x << ", " << H.y << ", " << H.z << std::endl;

	float dK = glm::dot(glm::vec3(L.x, L.y, L.z), glm::vec3(normal.x, normal.y, normal.z));
	std::cout << "dK: " << dK << std::endl;
	*/
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getShaderProgram(), "mvmatrix"), 1, GL_FALSE, glm::value_ptr(modelViewMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getShaderProgram(), "pmatrix"), 1, GL_FALSE, glm::value_ptr(projectionMat));
	glUniform4fv(glGetUniformLocation(shaderProgram.getShaderProgram(), "lightPosition"), 1, glm::value_ptr(lightInViewPos));

	// Count is the number of elements in the array that will form triangles. It is not the number of triangles defined by the array.
	// When VBOs are in use and GL_ELEMENT_ARRAY_BUFFER is bound, the last parameter (pointer to data) is interpreted as an offset within IBO instead of
	// actual program memory address.
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(sphereIndices.size()), GL_UNSIGNED_SHORT, 0);
}

bool ExampleScene4::handleEvent(const SDL_Event &e)
{
        // Put any event handling code here.
        // Window-resizing is handled in event loop already.

	// Return false if you want to stop the program
	return true;
}

