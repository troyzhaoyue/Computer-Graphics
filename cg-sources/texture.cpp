/**
 * \brief Texture class implementation
 * \file
 */
#include "texture.h"

/**
 * \brief Static method to load GL texture without creating a software surface to back it.
 *
 * Use this if you just want to load a texture and use some other way to manage OpenGL texture object ids
 * instead of Texture object. As this releases any software buffers to hold the texture, it also uses less RAM.
 *
 * \param filename Image file to load
 * \return Texture id or 0 if something failed.
 */
GLuint Texture::loadGLTexture(const std::string &filename)
{
	GLuint oid = 0;

	// Load texture if possible
	SDL_Surface *orig_surface = IMG_Load(filename.c_str());
	SDL_Surface *surface = 0;

	// Texture loading failed..
	if (!orig_surface)
	{
		std::cerr << "Texture::loadGLTexture(): Unable to load image " << filename << std::endl;
		return 0;
	}

	// Convert texture into RGBA anyway because GPUs like that
	// Alpha channel defined. Convert to RGBA texture
	SDL_PixelFormat *target_format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	surface = SDL_ConvertSurface(orig_surface, target_format, 0);
	SDL_FreeFormat(target_format);

	if (surface == 0)
		std::cerr << "Texture::loadGLTexture(): Unable to convert surface from " << filename << ": " << SDL_GetError() << std::endl;

	// Release original surface
	SDL_FreeSurface(orig_surface);

	if (surface)
	{
		// Generate texture id
		glGenTextures(1, &oid);

		// Bind texture as 2-D texture
		glBindTexture(GL_TEXTURE_2D, oid);

		if (SDL_MUSTLOCK(surface))
			SDL_LockSurface(surface);

//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, surface->pixels);

		if (SDL_MUSTLOCK(surface))
			SDL_UnlockSurface(surface);
	}

	// Return allocated object id
	return oid;
}

/**
* \brief Create empty texture surface that can be modified
* \param width Image width in pixels
* \param height Image height in pixels
*/
Texture::Texture(int width, int height) :
	oid(0),
	dirty(true),
	surface(0)
{
	Uint32 rmask, gmask, bmask, amask = 0;

	// SDL pixel masks are handled as 32-bit numbers. Make sure that endianess is correct
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif

	surface = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);

	// Create OpenGL texture with this
	updateGLTexture();
}

/**
 * \brief Load texture from file
 */
Texture::Texture(const std::string &filename) :
	oid(0),
	dirty(true),
	surface(0)
{
	// Load texture if possible
	SDL_Surface *orig_surface = IMG_Load(filename.c_str());

	// Texture loading failed..
	if (!orig_surface)
	{
		std::cerr << "Texture::Texture(): Unable to load image " << filename << std::endl;
		return;
	}

	// Convert texture into either RGBA surface as GPUs prefer those
	// LibSDL accesses these images as Uint32 and that affects underlying byte order depending on endianess.
	SDL_PixelFormat *target_format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	surface = SDL_ConvertSurface(orig_surface, target_format, 0);
	SDL_FreeFormat(target_format);

	if (surface == 0)
		std::cerr << "Texture::Texture(): Unable to convert surface from " << filename << ": " << SDL_GetError() << std::endl;

	// Release original surface
	SDL_FreeSurface(orig_surface);

	// Flip loaded image vertically
	// Might not be the most efficient implementation.. memcpy() would probably be faster with a separate row buffer
	for (int y = 0; y < surface->h / 2; ++y)
	{
		Uint32 *ptr1 = reinterpret_cast<Uint32 *>(static_cast<Uint8 *>(surface->pixels) + ((y) * surface->pitch));
		Uint32 *ptr2 = reinterpret_cast<Uint32 *>(static_cast<Uint8 *>(surface->pixels) + ((surface->h - 1 - y) * surface->pitch));

		// Swap elements of these rows. 
		for (int x = 0; x < surface->w; ++x)
			std::swap(*ptr1++, *ptr2++);
	}

	// Create OpenGL texture of this
	updateGLTexture();
}

/**
 * \brief Set pixel value for texture
 *
 * This function sets a single pixel to a new value in the texture. This is a really slow but simple
 * way for generating or modifying an existing texture image
 * \param x x-coordinate of the pixel to modify
 * \param y y-coordinate of the pixel to modify
 * \param r Red color [0..255]
 * \param g Green color [0..255]
 * \param b Blue color [0..255]
 * \param a Alpha channel [0..255] (0 = transparent, 255 = opaque), needs a DEPTH_RGBA image to work
 */
void Texture::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	// Don't try to modify pixels outside image
	if (x < 0 || y < 0 || x >= surface->w || y >= surface->h)
		return;

	// Locking surface can be an expensive operation so this is really inefficient way for manipulating lots of pixels!
	if (SDL_MUSTLOCK(surface))
		SDL_LockSurface(surface);

	// Convert parameters to a single 32-bit pixel value
	Uint32 col = (static_cast<Uint32>(r) << 24) +
		         (static_cast<Uint32>(g) << 16) +
				 (static_cast<Uint32>(b) << 8) +
				 a;

	// Process 32-bit RGBA pixel
	// As we work with a vertically flipped image to get correct OpenGL texture orientation, flip provided y-coordinate
	Uint32 *ptr = reinterpret_cast<Uint32 *>(static_cast<Uint8 *>(surface->pixels) + ((surface->h - 1 - y) * surface->pitch) + (x * 4));
	*ptr = col;

	dirty = true;

	if (SDL_MUSTLOCK(surface))
		SDL_UnlockSurface(surface);
}

/**
 * \brief Get OpenGL texture id for this texture
 *
 * On first call, allocates a new OpenGL texture id
 * If texture is modified between calls, updates OpenGL texture with a new copy
 * \return OpenGL texture id allocated for this texture
 */
GLuint Texture::updateGLTexture()
{
	// If texture has not been generated yet, do it now
	if (oid == 0)
	{
		glGenTextures(1, &oid);

		// Mark that this texture does not yet contain valid information
		dirty = 1;

		// Bind texture as 2-D texture
		glBindTexture(GL_TEXTURE_2D, oid);

		// We just allocate a new texture and not provide any actual texture for it
		// by providing null pointer as texture data as last parameter.
		// Texture data would be transferred there as well if the last parameter points to surface->pixels instead of having a null value
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, 0);
	}

	// If texture has been modified or has not been used yet, update it with a new copy
	if (dirty)
	{
		dirty = false;

		// Bind texture as 2-D texture
		glBindTexture(GL_TEXTURE_2D, oid);

		// Transfer image data to OpenGL
		if (SDL_MUSTLOCK(surface))
			SDL_LockSurface(surface);

		// This transfers modified pixel data to previously allocated texture. See https://www.opengl.org/wiki/Texture_Storage
		// We can also just replace a rectangular subsection of the image if necessary using glTexSubImage2D() but the following call replaces the whole image
		// As we request OpenGL to accesses memory using 32-bit integer operations (GL_UNSIGNED_INT_...), this line should match libSDL convention on both little and big endian systems
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, surface->pixels); // Does reallocation for image data
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, surface->w, surface->h, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, surface->pixels); // Just replaces previously allocated texture with specified one

		if (SDL_MUSTLOCK(surface))
			SDL_UnlockSurface(surface);

		// Example of how to set some texture parameters
		// Texture parameters can always be changed later by just binding the texture and updating the relevant parameters
/*		
		// Enable anisotropic filtering - requires mipmapping levels below to be available for the best results!
		// Maximum anisotropy level is determined by glGetFloatv(MAX_TEXTURE_MAX_ANISOTROPY_EX)
		if (GLEW_EXT_texture_filter_anisotropic)
		{
			GLfloat max_aniso;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
			std::cout << "Enabling anisotropic filtering for texture." << std::endl;
			
		}  else
		{
			std::cout << "Unable to enable anisotropic filtering for texture." << std::endl;
		}
*/	    
		// Use linear filtering for texture sampling
		// You can try GL_NEAREST and GL_LINEAR here
		/*
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		*/
		
		// Mipmapping example
		// Use linear filtering when zooming in after highest resolution level
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Use mipmapping with linearly interpolated layers when zooming in ("trilinear filtering")
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// Generate mipmaps from top level texture to reduce aliasing effects
		glGenerateMipmap(GL_TEXTURE_2D);
		
	}

	// Return allocated object id
	return oid;
}

/**
 * \brief Destructor
 */
Texture::~Texture()
{
	// Release software buffer if available
	if (surface)
		SDL_FreeSurface(surface);

	// Release allocated texture id
	if (oid)
		glDeleteTextures(1, &oid);
}
