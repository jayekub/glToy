#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <boost/foreach.hpp>

#include "utils.h"

std::string readFile(const std::string &filename) {
	std::ifstream file(filename.c_str());
	std::string contents;

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			contents += line + '\n';
		}
	} else {
		throw "Unable to open file";
	}

	return contents;
}

GLuint makeShader(const  std::string &filename, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);
	std::string sourceStr = readFile(filename);

	GLint sourceLen = (GLint) sourceStr.size();
	const GLchar *source = sourceStr.c_str();

	glShaderSource(shader, 1, &source, &sourceLen);
	glCompileShader(shader);

	int infoLogLen;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

	if (infoLogLen > 0) {
		int lenWritten;
		char *infoLog = new char[infoLogLen];

		glGetShaderInfoLog(shader, infoLogLen, &lenWritten, infoLog);
		puts(infoLog);

		delete infoLog;
	}

	return shader;
}

GLuint makeProgram(const std::vector<GLuint> &shaders) {
	GLuint program = glCreateProgram();

	BOOST_FOREACH(GLuint shader, shaders) {
		glAttachShader(program, shader);
	}

	glLinkProgram(program);

	int infoLogLen;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);

	if (infoLogLen > 0) {
		int lenWritten;
		char *infoLog = new char[infoLogLen];

		glGetProgramInfoLog(program, infoLogLen, &lenWritten, infoLog);
		puts(infoLog);

		delete infoLog;
	}

	return program;
}
