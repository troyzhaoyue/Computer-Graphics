/**
 * \brief SDL wrapper class interface
 * \file
 */
#ifndef SDLWRAPPER_H_
#define SDLWRAPPER_H_

#include <string>
#include <GL/glew.h>   // OpenGL extension wrangler library
#include <SDL.h>       // libSDL functionality
#include <SDL_image.h> // Texture loading support

/**
 * \brief SDL library helper class
 *
 * This class handles initialization and management of SDL library utilities and
 * creation of OpenGL context
 */
class SDL
{
	SDL_Window *win;
	SDL_GLContext glcontext;
	bool ok;
public:
	SDL(bool doOpenGLDebug, int ogl_major_version, int ogl_minor_version, Uint32 flags, const std::string &window_name, Uint32 window_width, Uint32 window_height);

	~SDL();

	bool isOk() const;

	/**
	 * \brief Get application window
	 * \return Pointer to window information.
	 */
	SDL_Window *getWindow() { return win; }
};

#endif
