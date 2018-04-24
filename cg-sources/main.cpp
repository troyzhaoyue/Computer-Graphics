/**
* \brief OpenGL sample selection application
* \file
*/
#include "sdlwrapper.h"                 // libSDL helper class to initialize library and OpenGL context
#include "streamredirector.h"           // Redirects standard output and standard error to files to help debugging on Windows
#include "examplescene1.h"
#include "examplescene2.h"
#include "examplescene3.h"
#include "examplescene4.h"
#include "objparser.h"

#include "Assignment1.h"
#include "Assignment2.h"
#include "Assignment3.h"

/**
* \brief Dump loaded object information to show how it can be accessed using ObjParser class
*
* This is an example function that iterates over different properties of the parsed object file
* and can be used as a base to convert that data into any format used in Vertex Buffer Objects and Vertex Array Objects.
*
* Material is organized to obj.matlib["materialName"].get("parameterName", ...)
*
* Different vertex positions, texture coordinates and normal vectors can be found at
* obj.objVertexSet["objectName"].pos[index], obj.objVertexSet["objectName"].texture and
* obj.objVertexSet["objectName"].normal[index].
*
* These have been organized into unique triplets already by ObjParser and they are referenced
* with face indexes in obj.objVertexSet["objectName"].groupMaterialFaces["groupName"]["materialName"][faceIndex][vertex_index];
*/

void dump_obj_info(const ObjParser &obj)
{
	// Dump object information
	std::cout << "Base path for loading textures etc: " << obj.basePath << std::endl;

	// Dump some material information
	// Information below can also be accessed using obj.matlib["materialName"].get("parameterName", ...)
	std::cout << "Provided materials: " << std::endl;
	std::map<std::string, ObjParser::Material>
		::const_iterator mat_it;
	for (mat_it = obj.matlib.begin(); mat_it != obj.matlib.end(); ++mat_it)
	{
		std::cout << " Name: \"" << mat_it->first << "\"" << std::endl;

		// Example information extraction from the material definition
		glm::vec3 diffuse_color;
		if (mat_it->second.get("Kd", diffuse_color))
			std::cout << "  Diffuse color: " << diffuse_color.r << ", " << diffuse_color.g << ", " << diffuse_color.b << std::endl;

		// If material has texture information..
		std::string diffuse_texture;
		if (mat_it->second.get("map_Kd", diffuse_texture))
			std::cout << "  Diffuse texture: " << obj.basePath << "/" << diffuse_texture << std::endl;

	}

	// Dump mesh information
	std::map<std::string, ObjParser::VertexSet>
		::const_iterator obj_it;
	for (obj_it = obj.objVertexSet.begin(); obj_it != obj.objVertexSet.end(); ++obj_it)
	{
		std::cout << "Object name: \"" << obj_it->first << "\"" << std::endl;
		std::cout << " Number of separate vertex buffers: " << obj_it->second.vertexBuffers.size() << std::endl;
		for (size_t vb = 0; vb < obj_it->second.vertexBuffers.size(); ++vb)
		{
			// To reduce typing, create a shortcut
			const ObjParser::VertexBuffer &vbuffer = obj_it->second.vertexBuffers[vb];

			std::cout << "  Vertex Buffer " << vb << ":" << std::endl;
			std::cout << "   Number of vertices: " << vbuffer.pos.size() << std::endl;
			std::cout << "   Number of indices: " << vbuffer.indices.size() << std::endl;
			std::cout << "   Has texture coordinates: " << vbuffer.hasTexture() << std::endl;
			std::cout << "   Has normal vectors: " << vbuffer.hasNormal() << std::endl;
		}

		// Dump groups inside object.
		const ObjParser::VertexSet::group_type &group = obj_it->second.groupMaterialFaces;
		ObjParser::VertexSet::group_type::const_iterator vertex_group_it;
		for (vertex_group_it = group.begin(); vertex_group_it != group.end(); ++vertex_group_it)
		{
			// Dump material groups within object groups
			std::cout << " Vertex group \"" << vertex_group_it->first << "\" materials: " << std::endl;
			ObjParser::VertexSet::material_type::const_iterator material_it;
			for (material_it = vertex_group_it->second.begin(); material_it != vertex_group_it->second.end(); ++material_it)
			{
				std::cout << "  Material \"" << material_it->first << "\":" << std::endl;
				// Dump sequential index ranges within material group
				std::cout << "   Number of separate face ranges: " << material_it->second.size() << std::endl;
				ObjParser::VertexSet::faces_type::const_iterator face_range_it;
				for (face_range_it = material_it->second.begin(); face_range_it != material_it->second.end(); ++face_range_it)
				{
					// To reduce typing, create a shortcut
					const ObjParser::VertexBuffer &vbuffer = obj_it->second.vertexBuffers[face_range_it->vbIndex];

					std::cout << "    Uses VertexBuffer " << face_range_it->vbIndex << " indices from: [" << face_range_it->startIndex << " .. " << face_range_it->startIndex + face_range_it->length - 1 << "]" << std::endl;
					glm::vec4 pos = vbuffer.pos[vbuffer.indices[face_range_it->startIndex]];
					std::cout << "     First vertex position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
				}
			}
		}
	}
}

/**
* \brief Check for any OpenGL errors that might have happened
*/
bool checkOpenGLErrors()
{
	GLenum e;
	bool allOk = true;
	while ((e = glGetError()) != GL_NO_ERROR)
	{
		allOk = false;

		switch (e)
		{
#ifdef GL_INVALID_ENUM
		case GL_INVALID_ENUM:
			std::cerr << "OpenGL error GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument." << std::endl;
			break;
#endif
#ifdef GL_INVALID_VALUE
		case GL_INVALID_VALUE:
			std::cerr << "OpenGL error GL_INVALID_VALUE: A numeric argument is out of range." << std::endl;
			break;
#endif
#ifdef GL_INVALID_OPERATION
		case GL_INVALID_OPERATION:
			std::cerr << "OpenGL error GL_INVALID_OPERATION: The specified operation is not allowed in the current state." << std::endl;
			break;
#endif
#ifdef GL_INVALID_FRAMEBUFFER_OPERATION
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cerr << "OpenGL error GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete." << std::endl;
			break;
#endif
#ifdef GL_OUT_OF_MEMORY
		case GL_OUT_OF_MEMORY:
			std::cerr << "OpenGL error GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined now!" << std::endl;
			break;
#endif
#ifdef GL_STACK_UNDERFLOW
		case GL_STACK_UNDERFLOW:
			std::cerr << "OpenGL error GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow." << std::endl;
			break;
#endif
#ifdef GL_STACK_OVERFLOW
		case GL_STACK_OVERFLOW:
			std::cerr << "OpenGL error GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow." << std::endl;
			break;
#endif
		}
	}

	return allOk;
}

int main(int argc, char **argv)
{
	// Redirect standard output and standard error streams to files.
	// This is most useful on Windows as we don't have a console available
#ifdef _WIN32
	StreamRedirector streamRedirector("stdout.txt", "stderr.txt");
#endif

	// Initialize libSDL, create an application window and initialize it with OpenGL context
	// See sdlwrapper.* for implementation details
	bool enableOpenGLDebugging = true;                        // Enable OpenGL debug context and install debug message callback if available
	int ogl_major_version = 3;                                // OpenGL major version to request. See https://www.opengl.org/wiki/History_of_OpenGL
	int ogl_minor_version = 3;                                // OpenGL minor version to request.
	Uint32 sdl_init_flags = SDL_INIT_TIMER | SDL_INIT_VIDEO;  // What SDL subsystems to initialize. See https://wiki.libsdl.org/SDL_Init
	std::string window_name = "CG 2018 example";              // Created window name - should be UTF-8 string for libSDL
	Uint32 window_width = 640;                                // Initial window width
	Uint32 window_height = 480;                               // Initial window height
	SDL sdl(enableOpenGLDebugging, ogl_major_version, ogl_minor_version, sdl_init_flags, window_name, window_width, window_height);

	if (!sdl.isOk())
	{
		std::cerr << "Unable to initialize SDL" << std::endl;
		return -1;
	}

	if (!checkOpenGLErrors())
	{
		std::cerr << "OpenGL Errors detected during SDL initialization" << std::endl;
		return -1;
	}

	//	ExampleScene1 scene; // A tetrahedron with vertex colors
	//	ExampleScene2 scene; // A texturemapped cube
	//	ExampleScene3 scene; // A shaded sphere (shading calculated to vertex colors)
	//	ExampleScene4 scene; // Gouraud-shaded sphere (shading calculated in vertex shader)

	//	Assignment1 scene;
	//	Assignment2 scene;
		Assignment3 scene;

	// Test object loading
	{
		ObjParser obj("data/cubescene.obj");

		dump_obj_info(obj);
	}

	if (!scene.init())
	{
		std::cerr << "Unable to init scene." << std::endl;
		return -1;
	}

	if (!checkOpenGLErrors())
	{
		std::cerr << "OpenGL Errors detected during scene.init()" << std::endl;
		return -1;
	}

	scene.resize(window_width, window_height);
	std::cout << "Initial width and height: " << window_width << " x " << window_height << std::endl;

	bool runRenderLoop = true;
	Uint32 prevTicks = SDL_GetTicks();
	while (runRenderLoop)
	{
		// Update the scene
		Uint32 curTicks = SDL_GetTicks();
		scene.update(0.001f * (curTicks - prevTicks)); // Parameter in seconds
		prevTicks = curTicks;

		// Render the scene
		scene.render();

		// Check for any errors that might have happened inside render call.
		// Stop the loop if there has been an error.
		runRenderLoop &= checkOpenGLErrors();

		// Display window
		SDL_GL_SwapWindow(sdl.getWindow());

		// Process events
		SDL_Event e;

		// https://wiki.libsdl.org/SDL_PollEvent
		// See https://wiki.libsdl.org/SDL_Event for events
		while (SDL_PollEvent(&e))
		{
			// Sample event handling code. Some of this could be useful in your own scene's handleEvent() function
			switch (e.type)
			{
				// Program window closed etc.
			case SDL_QUIT:
				runRenderLoop = false;
				break;
				// Keyboard key pressed down (scancode is the physical key on keyboard, keycode is the symbolic key meaning)
			case SDL_KEYDOWN:
				std::cout << "Key " << e.key.keysym.scancode << " (" << SDL_GetKeyName(e.key.keysym.sym) << ") pressed" << std::endl;
				break;
				// Keyboard key released
			case SDL_KEYUP:
				std::cout << "Key " << e.key.keysym.scancode << " (" << SDL_GetKeyName(e.key.keysym.sym) << ") released" << std::endl;
				break;
				// Mouse moved
			case SDL_MOUSEMOTION:
				std::cout << "Mouse motion: " << e.motion.x << ", " << e.motion.y << std::endl;
				break;
				// Mouse button pressed
			case SDL_MOUSEBUTTONDOWN:
				// See https://wiki.libsdl.org/SDL_MouseButtonEvent
				// Note: Mouse wheel has its own event
				std::cout << "Mouse button down at : " << e.button.x << ", " << e.button.y << " button: ";
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					std::cout << "Left";
					break;
				case SDL_BUTTON_RIGHT:
					std::cout << "Right";
					break;
				case SDL_BUTTON_MIDDLE:
					std::cout << "Middle";
					break;
				case SDL_BUTTON_X1:
					std::cout << "X1";
					break;
				case SDL_BUTTON_X2:
					std::cout << "X2";
					break;
				default:
					std::cout << "Unknown (" << e.button.button << ")";
				}
				std::cout << " clicks: " << static_cast<int>
					(e.button.clicks) << std::endl;
				break;
				// Mouse button released
			case SDL_MOUSEBUTTONUP:
				// See https://wiki.libsdl.org/SDL_MouseButtonEvent
				break;
				// Window-system event
			case SDL_WINDOWEVENT:
				// See https://wiki.libsdl.org/SDL_WindowEvent
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					window_width = e.window.data1;
					window_height = e.window.data2;
					std::cout << "Window Resized to : " << window_width << " x " << window_height << std::endl;
					scene.resize(window_width, window_height);
					break;
				}
				break;
			}

			// Tell running scene what just happened and stop the loop if handler returns false.
			// AND operation is done so that SDL_QUIT handler above won't be ignored either.
			runRenderLoop &= scene.handleEvent(e);
		}
	}

	return 0;
}
