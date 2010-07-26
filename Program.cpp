#include <stdio.h>

#include "utils.h"
#include "Program.h"

Program::Program() {
    _program = glCreateProgram();
}

Program::Program(const std::vector<ShaderSpec> &shaderSpecs)
{
    Program::Program();

    //setShaders(shaderSpecs);
    _shaderSpecs = shaderSpecs;
    reload();
}

Program::~Program()
{
    glDeleteProgram(_program);
}

void Program::reload()
{
    BOOST_FOREACH(GLuint shader, _shaders) {
        glDeleteShader(shader);
    }

    _shaders.clear();
    glDeleteProgram(_program);

    BOOST_FOREACH(ShaderSpec spec, _shaderSpecs) {
        GLuint shader = Program::makeShader(spec.first, spec.second);
        //printf("%s: %s\n", spec.first.c_str(),
        //       Program::getShaderInfoLog(shader).c_str());

        _shaders.push_back(shader);
    }

    _program = Program::makeProgram(_shaders);

    //puts(Program::getProgramInfoLog(_program).c_str());

    _rebuildParamMaps();
}

void Program::use()
{
    glUseProgram(_program);
}

void Program::addShader(const ShaderSpec &shaderSpec)
{
    _shaderSpecs.push_back(shaderSpec);
}

/*
void Program::setShaders(
        const std::vector<ShaderSpec> &shaderSpecs, bool doReload)
{
    _shaderSpecs = shaderSpecs;
    if (doReload) reload();
}*/

// static
GLuint Program::makeShader(const std::string &filename, GLuint type)
{
    GLuint shader = glCreateShader(type);
    std::string sourceStr = readFile(filename);

    GLint sourceLen = (GLint) sourceStr.size();
    const GLchar *source = sourceStr.c_str();

    glShaderSource(shader, 1, &source, &sourceLen);
    glCompileShader(shader);

    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

    //if (compileStatus != GL_TRUE) {
        printf("%s: %s\n", filename.c_str(), getShaderInfoLog(shader).c_str());
    //}

    return shader;
}

// static
GLuint Program::makeProgram(const std::vector<GLuint> &shaders)
{
    GLuint program = glCreateProgram();

    BOOST_FOREACH(GLuint shader, shaders) {
        glAttachShader(program, shader);
    }

    glLinkProgram(program);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    //if (linkStatus != GL_TRUE) {
        printf("%s\n", getProgramInfoLog(program).c_str());
    //}

    return program;
}

// static
std::string Program::getShaderInfoLog(GLuint shader)
{
    int infoLogLen;
    std::string infoLogStr;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

    if (infoLogLen > 0) {
        int lenWritten;
        char *infoLog = new char[infoLogLen];

        glGetShaderInfoLog(shader, infoLogLen, &lenWritten, infoLog);
        infoLogStr = infoLog;

        delete infoLog;
    }

    return infoLogStr;
}

// static
std::string Program::getProgramInfoLog(GLuint program)
{
    int infoLogLen;
    std::string infoLogStr;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);

    if (infoLogLen > 0) {
        int lenWritten;
        char *infoLog = new char[infoLogLen];

        glGetProgramInfoLog(program, infoLogLen, &lenWritten, infoLog);
        infoLogStr = infoLog;

        delete infoLog;
    }

    return infoLogStr;
}

void Program::_rebuildParamMaps()
{
    _uniforms.clear();
    _attributes.clear();

    int numUniforms, uniformNameLength;

    glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &numUniforms);
    glGetProgramiv(_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformNameLength);

    GLchar *name = new char[uniformNameLength];

    for (int i = 0; i < numUniforms; ++i) {
        GLint size;
        GLenum type;

        glGetActiveUniform(_program, i, uniformNameLength, NULL, &size, &type,
                           name);

        _uniforms[name] = glGetUniformLocation(_program, name);
    }

    delete name;

    int numAttribs, attribNameLength;

    glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTES, &numAttribs);
    glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attribNameLength);

    name = new char[attribNameLength];

    for (int i = 0; i < numAttribs; ++i) {
        GLint size;
        GLenum type;

        glGetActiveAttrib(_program, i, attribNameLength, NULL, &size, &type,
                          name);

        _attributes[name] = glGetAttribLocation(_program, name);
    }

    delete name;
}
