/**
 * \brief Class to load shader programs
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include "shaderprogram.h"

bool ShaderProgram::loadFile(const std::string &filename, std::string &contents) const
{
	std::ifstream is(filename.c_str(), std::ifstream::binary);

	if (!is.is_open())
	{
		std::cerr << "ShaderProgram::loadFile(): Unable to open '" << filename << "' for reading." << std::endl;
		return false;
	}

	is.seekg(0, std::ios::end);
	contents.resize(is.tellg());
	is.seekg(0, std::ios::beg);
	is.read(&contents[0], contents.size()); // Must work with C++11 forward. Earlier do not need to have continuous space but no system actually uses that option..

	return is.good();
}

/**
 * Compile a single shader.
 * \param shadertype GL_FRAGMENT_SHADER, GL_VERTEX_SHADER etc.
 * \param source Shader source
 * \param[out] shaderhandle Allocated handle for the new shader if successful
 */
bool ShaderProgram::compile(GLenum shadertype, const std::string &source, GLuint &shaderhandle)
{
	// Allocate handle for new shader
	shaderhandle = glCreateShader(shadertype);

	// Set shader source to come from a single string. glShaderSource() supports multiple strings that are concatenated together.
	const GLchar *src = source.c_str();
	glShaderSource(shaderhandle, 1, &src, 0);

	// Try to compile it
	GLint isCompiled;
	glCompileShader(shaderhandle);
	glGetShaderiv(shaderhandle, GL_COMPILE_STATUS, &isCompiled);

	if (!isCompiled)
	{
		GLint maxlen;
		// Something was wrong. Query for any error/information string lengths. It includes \0 at the end.
		glGetShaderiv(shaderhandle, GL_INFO_LOG_LENGTH, &maxlen);
		std::string errstr;
		errstr.resize(maxlen);
		glGetShaderInfoLog(shaderhandle, maxlen, &maxlen, &errstr[0]);

		std::cerr << "Unable to compile shader: " << std::endl << errstr << std::endl;

		// Release shader
		glDeleteShader(shaderhandle);
		shaderhandle = 0;
		return false;
	}

	// Everything should be ok now.
	return true;
}

ShaderProgram::ShaderProgram() :
	vertexshader(0),
	fragmentshader(0),
	shaderprogram(0)
{
}

ShaderProgram::ShaderProgram(const std::string &vertexshaderfile, const std::string &fragmentshaderfile) :
	vertexshader(0),
	fragmentshader(0),
	shaderprogram(0)
{
	load(vertexshaderfile, fragmentshaderfile);
}

ShaderProgram::~ShaderProgram()
{
	// In this implementation, independent shaders have already been released except the final program but
	// we need to make sure that it also happens in case we encounter some problem during shader compilation and linking.
	if (vertexshader)
		glDeleteShader(vertexshader);
	if (fragmentshader)
		glDeleteShader(fragmentshader);
	if (shaderprogram)
		glDeleteProgram(shaderprogram);
}

bool ShaderProgram::load(const std::string &vertexshaderfile, const std::string &fragmentshaderfile)
{
	std::string vs;
	std::string fs;

	// Load shader strings
	if ((!loadFile(vertexshaderfile, vs)) ||
		(!loadFile(fragmentshaderfile, fs)))
	{
		// Unable to load one of the shader sources
		return false;
	}

	// Compile vertex shader
	if (!compile(GL_VERTEX_SHADER, vs, vertexshader))
	{
		std::cerr << "Unable to compile '" << vertexshaderfile << std::endl;
		return false;
	}

	// Compile fragment shader
	if (!compile(GL_FRAGMENT_SHADER, fs, fragmentshader))
	{
		std::cerr << "Unable to compile '" << fragmentshaderfile << std::endl;
		return false;
	}

	// Create shader program from above fragment shaders
	shaderprogram = glCreateProgram();

	if (shaderprogram == 0)
	{
		std::cerr << "Unable to create shader program using " << vertexshaderfile << " and " << fragmentshaderfile << std::endl;
		return false;
	}

	// Attach different shader passes to shader program
	glAttachShader(shaderprogram, vertexshader);
	glAttachShader(shaderprogram, fragmentshader);

	// Link shaders into an executable
	glLinkProgram(shaderprogram);

	// Now that we have linked shaders to a program, we can detach and delete individual shaders.
	// This frees up memory (shader sources) that are no longer needed. The rest of the resources are released
	// automatically when the program is deleted (assuming non-buggy drivers..).
	glDetachShader(shaderprogram, vertexshader);
	glDeleteShader(vertexshader);
	vertexshader = 0;
	glDetachShader(shaderprogram, fragmentshader);
	glDeleteShader(fragmentshader);
	fragmentshader = 0;

	return true;
}
