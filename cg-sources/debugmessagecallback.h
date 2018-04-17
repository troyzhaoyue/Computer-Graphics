/**
 * \brief Callback functio to be used with glDebugMessageCallback() or glDebugMessageCallbackARB();
 *
 * This functionality is available as standard starting from OpenGL 4.3 but may be available as an extension in older OpenGL contexts as well if
 * hardware and drivers can support that.
 * \file
 */
#ifndef DEBUGMESSAGECALLBACK_H_
#define DEBUGMESSAGECALLBACK_H_

bool installDebugMessageCallback();

#endif