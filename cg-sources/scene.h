/**
 * \brief Abstract Scene interface
 */
#ifndef EXAMPLE_SCENE_H_
#define EXAMPLE_SCENE_H_

#include <GL/glew.h>                    // OpenGL extension wrangler library
#include <SDL.h>                        // libSDL functionality

/**
 * \brief Draws a vertex-colored tetrahedron
 */
class Scene
{
public:
	/**
	 * \brief Virtual destructor
	 *
	 * Required for correctness when freeing Scene-typed objects.
	 */
	virtual ~Scene() {}

	/**
	 * \brief Initialize scene
	 *
	 * \return true if success
	 */
	virtual bool init() = 0;

	/**
	 * \brief Called on window resize
	 * \param width New width of the window
	 * \param height New height of the window
	 */
	virtual void resize(GLsizei width, GLsizei height) = 0;

	/**
	 * \brief Update scene
	 *
	 * Simulate a time step for your scene and things move.
 	 * \param timestep Time step in seconds
	 */
	virtual void update(float timestep) = 0;

	/**
	 * \brief Render view
	 *
	 * Render a view on the screen.
	 * Note: You can render also in handleEvent() for special uses.
	 */
	virtual void render() = 0;

	/**
	 * \brief Handle SDL event
	 *
	 * Do special things based on mouse movement, clicks, keyboard events etc.
	 * \param e Event information
	 * \return True if running the scene should continue. False if it should stop.
	 */
	virtual bool handleEvent(const SDL_Event &e) = 0;
};


#endif
