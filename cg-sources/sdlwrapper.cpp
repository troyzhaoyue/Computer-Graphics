/**
 * \brief SDL wrapper class implementation
 */
#include <iostream>
#include "sdlwrapper.h"
#include "debugmessagecallback.h"

SDL::SDL(bool doOpenGLDebug, int ogl_major_version, int ogl_minor_version, Uint32 flags, const std::string &window_name, Uint32 window_width, Uint32 window_height) :
	win(0),
	glcontext(0),
	ok(false)
{
	// Initialize SDL
	if (SDL_Init(flags) != 0)
	{
		std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
		return;
	}

	// Initialize SDL_image for texture loading
	{
		int flags = IMG_INIT_JPG | IMG_INIT_PNG;
		if (IMG_Init(flags) != flags)
		{
			std::cerr << "SDL::SDL(): Initializing SDL_Image failed: " << IMG_GetError() << std::endl;
			return;
		}
	}

	// No need to load OpenGL library - done by GLEW instead
//	if (SDL_GL_LoadLibrary(0))
//	{
//		std::cerr << "SDL::SDL(): Unable to load OpenGL library" << std::endl;
//		return;
//	}

	// Setup OpenGL state before creating a window
	if (doOpenGLDebug)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG); // VERY useful for debugging but might affect performance! https://www.opengl.org/wiki/Debug_Output
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ogl_major_version);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ogl_minor_version);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // Core functionality only, no compability for deprecated functionality
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY); // Allow deprecated functionality
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1); // Vertical Sync control. Swap every 1 vsync. 0 swaps immediately. -1 (in some systems) enables late swap tearing: https://wiki.libsdl.org/SDL_GL_SetSwapInterval

	// SDL_GL_DEPTH_SIZE (in bits)
	// SDL_GL_STENCIL_SIZE (in bits)

	// Create window
	// Pos can also be SDL_WINDOWPOS_CENTERED, or SDL_WINDOWPOS_UNDEFINED
	// See https://wiki.libsdl.org/SDL_CreateWindow for more options
	int posX = SDL_WINDOWPOS_CENTERED, posY = SDL_WINDOWPOS_CENTERED;

	win = SDL_CreateWindow(window_name.c_str(), posX, posY, window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (win == 0)
	{
		std::cerr << "SDL::SDL(): Error creating a window: " << SDL_GetError() << std::endl;
		return;
	}

	// Create an OpenGL context associated with the window.
	glcontext = SDL_GL_CreateContext(win);
	if (glcontext == 0)
	{
		std::cerr << "SDL::SDL(): Unable to create OpenGL context: " << SDL_GetError() << std::endl;
		return;
	}

	// Initialize GLEW now that we have a context
	// Setting glewExperimental allows OpenGL symbols to be found even when they are not listed in driver's extension list - Needed when using core profiles due to GLEW bugs
	// Depending on the drivers, this might be necessary to get even some of the basic functionality to work.
	glewExperimental = GL_TRUE; // This is a global variable provided by GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "SDL::SDL(): glewInit error: " << glewGetErrorString(err) << std::endl;
		return;
	}

	// Ignore GL errors caused by GLEW initialization (invalid ENUM)
	// Wait for all OpenGL functions to complete first and then cleate error state.
	glFinish();
	while (glGetError() != GL_NO_ERROR);

	std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

	// Initialize OpenGL debugging if available (See debugmessagecallback.cpp)
	// Requires SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG); above

	if (doOpenGLDebug)
		installDebugMessageCallback();

	{
		int v;
		bool version_ok = true;

		std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION: ";
		if (SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &v))
			std::cout << "error: " << SDL_GetError() << std::endl;
		else
			std::cout << v << std::endl;

		if (v != ogl_major_version)
			version_ok = false;

		std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: ";
		if (SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &v))
			std::cout << "error: " << SDL_GetError() << std::endl;
		else
			std::cout << v << std::endl;

		if (v != ogl_minor_version)
			version_ok = false;

		if (!version_ok)
		{
			std::cerr << "SDL::SDL(): Unable to select OpenGL version " << ogl_major_version << "." << ogl_minor_version << std::endl;
			return;
		}
	}

	// Set up global OpenGL state
	glPixelStorei(GL_PACK_ALIGNMENT, 1); // How data is stored to client memory - byte alignment for textures etc.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // How data is read from client memory - byte alignment for textures etc.

	// Everything is finally ok
	ok = true;
}

SDL::~SDL()
{
	// Release GL context
	if (glcontext)
		SDL_GL_DeleteContext(glcontext);

	// Release window
	if (win)
		SDL_DestroyWindow(win);

	// Release SDL_image resources
	IMG_Quit();

	SDL_Quit();
}

bool SDL::isOk() const
{
	return ok;
}
