#include <GL/glew.h>
#include <iostream>
#include <string>
#include "debugmessagecallback.h"

// Anonymous namespace for local callback function
namespace
{
	// Callback function to be called with various messages
	// Should match both GLDEBUGPROCARB and GLDEBUGPROC definitions

	// Template trickery is black magic here. :(
	//
	// The last parameter type depends on GLEW version that we would need to check at compile time to
	// provide a matching function definition. However, GLEW does not provide any macro variable to check
	// used version with a preprocessor directive!
	//
	// With a template type, the last parameter of this function is whatever happens to be defined
	// in the currently used GLEW version...

	template <typename T>
	void GLAPIENTRY debugCallback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar *message,
		T userParam) // void *userParam for older GLEW versions (At least 1.10.0). const void *userParam for new GLEW version (At least 1.13.0).
	{
		std::string sourcestr;
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:
			sourcestr = "API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			sourcestr = "Window system";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			sourcestr = "3rd party";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			sourcestr = "Application";
			break;
		default:
			sourcestr = "Other";
			break;
		}

		std::string typestr;
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:
			typestr = "Error";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			typestr = "Deprecated behavior";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			typestr = "Undefined behavior";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			typestr = "Portability";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			typestr = "Performance";
			break;
		case GL_DEBUG_TYPE_MARKER:
			typestr = "Marker";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			typestr = "Push group";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			typestr = "Pop group";
			break;
		default:
			typestr = "Other";
			break;
		}

		std::string severitystr;
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			severitystr = "HIGH!";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			severitystr = "Medium";
			break;
		case GL_DEBUG_SEVERITY_LOW:
			severitystr = "Low";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			severitystr = "Note";
			break;
		default:
			severitystr = "Unknown";
			break;
		}

		std::cout << "GL_Debug: " << sourcestr << " " << typestr << " (" << severitystr << "): " << id << " " << message << std::endl;
	}
}

/**
 * \brief Install debug message callback that prints out any messages to stdout
 *
 * Make sure that OpenGL context has been created with DEBUG features before calling this function to get maximum debug information!
 * \see https://www.opengl.org/wiki/Debug_Output
 * \return true on success
 */
bool installDebugMessageCallback()
{
	// Enable OpenGL debug output - enabled by default for OpenGL debug contexts
	glEnable(GL_DEBUG_OUTPUT);
	if (glGetError() != GL_NO_ERROR)
	{
		std::cout << "GL_DEBUG_OUTPUT not supported." << std::endl;
		return false;
	}

	// Make sure that debug output is synchronous. This has some performance penalty but makes sure that
	// debug message callback function is not called asynchronously from other threads.
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	if (glGetError() != GL_NO_ERROR)
		std::cout << "GL_DEBUG_OUTPUT_SYNCHRONOUS not supported." << std::endl;

	// Initialize debugging
	if (GLEW_KHR_debug)
	{
		// We could also check for GLEW_KHR_debug instead of function entry point directly
		std::cout << "GL_KHR_debug defined (OpenGL 4.3 feature)." << std::endl;
		glDebugMessageCallback(&debugCallback, 0);
		return true;
	} else
	if (GLEW_ARB_debug_output)
	{
		// We could also check for GLEW_ARB_debug_output instead of function entry points directly
		std::cout << "ARB_debug_output defined." << std::endl;
		glDebugMessageCallbackARB(&debugCallback, 0);
		return true;
	}

	std::cout << "No supported debug interfaces defined by OpenGL implementation." << std::endl; // Must use glGetError() to fetch information about invalid calls

	// You can filter messages using
	// void glDebugMessageControl​(GLenum source​, GLenum type​, GLenum severity​, GLsizei count​, const GLuint *ids​, GLboolean enabled​);
	return false;
}
