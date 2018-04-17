/**
 * \brief Helper class for shader programs
 * \file 
 */
#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_

#include <string>
#include <GL/glew.h>

/**
 * \brief Class to load shaders from file and compile them into combined programs to run on GPU
 */
class ShaderProgram
{
	GLuint vertexshader;
	GLuint fragmentshader;
	GLuint shaderprogram;

	bool loadFile(const std::string &filename, std::string &contents) const;
	bool compile(GLenum shadertype, const std::string &source, GLuint &shaderhandle);
public:
	ShaderProgram();
	ShaderProgram(const std::string &vertexshader, const std::string &fragmentshader);
	~ShaderProgram();

	bool load(const std::string &vertexshader, const std::string &fragmentshader);

	GLuint getShaderProgram()
	{
		return shaderprogram;
	}

	GLuint getPositionAttribLocation() const { return 0; } 
	GLuint getColorAttribLocation() const { return 1; }
	GLuint getNormalAttribLocation() const { return 2; }
	GLuint getTexture0AttribLocation() const { return 3; }
	GLuint getTexture1AttribLocation() const { return 4; }
};

#endif
