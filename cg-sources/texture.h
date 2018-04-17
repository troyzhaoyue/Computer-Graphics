/**
 * \brief Texture loading functionality using SDL_image library
 * \file
 */
#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

/**
 * \brief Class to generate OpenGL textures
 *
 * For simplicity, this always creates either RGB or RGBA surfaces and keeps original texture map
 * in SDL_Surface object for editing. For regular loaded textures this is not in general necessary
 * but it enables easy creation of procedural textures using setPixel() functionality.
 *
 * If texture has been modified using setPixel(), updateGLTexture() must be called to refresh OpenGL texture for drawing
 * for the changes to be visible.
 */
class Texture
{
	GLuint oid; // Texture object id
	bool dirty; // True if texture has been modified after it has been converted into a texture object
	SDL_Surface *surface; // Software texture buffer for generated textures
public:

	static GLuint loadGLTexture(const std::string &filename);

	Texture(int width, int height);
	Texture(const std::string &filename);

	void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

	GLuint updateGLTexture();

	/**
	 * \brief Get OpenGL texture id
	 *
	 * \return Created OpenGL texture object id or 0 if it has not been created
	 */
	GLuint getTextureId() const
	{
		return oid;
	}

	GLuint getWidth() const
	{
		return surface->w;
	}

	GLuint getHeight() const
	{
		return surface->h;
	}


	~Texture();

};

#endif