#include <stdio.h>

#include <sstream>

#include <boost/format.hpp>

#include "utils.h"
#include "Program.h"

Program::Shader::Shader(GLuint type) : _type(type)
{
    _shader = glCreateShader(_type);
}

Program::Shader::Shader(const std::string &filename, GLuint type) : _type(type)
{
    // XXX why can't i call Shader(type)?
    _shader = glCreateShader(_type);

    addFile(filename);
    compile();
}

Program::Shader::~Shader()
{
    glDeleteShader(_shader);
}

Program::Shader &Program::Shader::compile() const
{
    std::string sourceStr;

    // XXX make this configurable?
    sourceStr += "#version 150\n";

    BOOST_FOREACH(std::string filename, _filenames) {
        sourceStr += "#line 0\n";
        sourceStr += readFile(filename);
    }

    GLint sourceLen = (GLint) sourceStr.size();
    const GLchar *source = sourceStr.c_str();

    glShaderSource(_shader, 1, &source, &sourceLen);
    glCompileShader(_shader);

    GLint compileStatus;
    glGetShaderiv(_shader, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus != GL_TRUE) {
        fprintf(stderr, "%s\n", getInfoLog().c_str());
    }

    return (Shader &) *this;
}

void Program::Shader::reload() const
{
    compile();
}

Program::Shader &Program::Shader::addFile(const std::string &filename)
{
    _filenames.push_back(filename);
    return *this;
}

std::string Program::Shader::getInfoLog() const
{
    int infoLogLen;
    std::string infoLogStr;

    glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &infoLogLen);

    if (infoLogLen > 0) {
        int lenWritten;
        char *infoLog = new char[infoLogLen];

        glGetShaderInfoLog(_shader, infoLogLen, &lenWritten, infoLog);
        infoLogStr = infoLog;

        delete infoLog;
    }

    return infoLogStr;
}

Program::Program(bool debug) : _debug(debug), _numSamplers(0)
{
    _program = glCreateProgram();
}

Program::Program(Shader *shader, bool debug) : _debug(debug), _numSamplers(0)
{
    _program = glCreateProgram();
    _shaders.push_back(shader);
    link();
}

Program::Program(const std::vector<Shader *> &shaders, bool debug) :
        _debug(debug), _numSamplers(0)
{
    _program = glCreateProgram();
    _shaders = shaders;
    link();
}

Program::~Program()
{
    // this is what prevents Program from being copyable.
    // is there a better way?
    glDeleteProgram(_program);

    // XXX this won't work if ShaderS are shared
    BOOST_FOREACH(Shader *shader, _shaders) {
        delete shader;
    }
}

void Program::reload()
{
    // call reload here rather than making Shader listen to ensure
    // the order of reloads is correct
    BOOST_FOREACH(Shader *shader, _shaders) {
        glDetachShader(_program, shader->getId());
        shader->reload();
    }

    link();
}

Program &Program::use() const
{
    glUseProgram(_program);
    return (Program &) *this;
}

Program &Program::link()
{
    BOOST_FOREACH(Shader *shader, _shaders) {
        glAttachShader(_program, shader->getId());
    }

    glLinkProgram(_program);

    GLint linkStatus;
    glGetProgramiv(_program, GL_LINK_STATUS, &linkStatus);

    if (linkStatus != GL_TRUE || _debug) {
        fprintf(stderr, "%s\n", getInfoLog().c_str());
    }

    _rebuildParamMaps();

    return *this;
}

bool Program::hasUniform(const std::string &name) const
{
    return _uniforms.count(name) > 0;
}

bool Program::hasAttribute(const std::string &name) const
{
    return _attributes.count(name) > 0;
}

GLuint Program::uniform(const std::string &name) const
{
    _ParamMap::const_iterator uniformIt = _uniforms.find(name);
    if (uniformIt != _uniforms.end()) {
        return (*uniformIt).second;
    } else {
        if (_debug)
            fprintf(stderr, "The uniform %s doesn't exist\n", name.c_str());
        return PROGRAM_NONEXISTENT_LOCATION;
    }
}

GLuint Program::attribute(const std::string &name) const
{
    _ParamMap::const_iterator attributeIt = _attributes.find(name);
    if (attributeIt != _attributes.end()) {
        return (*attributeIt).second;
    } else {
        if (_debug)
            fprintf(stderr, "The attribute %s doesn't exist\n", name.c_str());
        return PROGRAM_NONEXISTENT_LOCATION;
    }
}

Program &Program::addShader(Shader *shader)
{
    _shaders.push_back(shader);
    return *this;
}

Program &Program::addShader(Shader &shader)
{
    _shaders.push_back(&shader);
    return *this;
}

// static
std::string Program::getInfoLog() const
{
    int infoLogLen;
    std::string infoLogStr;

    glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLen);

    if (infoLogLen > 0) {
        int lenWritten;
        char *infoLog = new char[infoLogLen];

        glGetProgramInfoLog(_program, infoLogLen, &lenWritten, infoLog);
        infoLogStr = infoLog;

        delete infoLog;
    }

    return infoLogStr;
}

Program &Program::setParameter(GLenum param, GLint value) const
{
    // XXX gl3
    //glProgramParameteriARB(_program, param, value);
    return (Program &) *this;
}

Program &Program::resetSamplers()
{
    // XXX unecessary?
    for (int s = 0; s < _numSamplers; ++s) {
        glActiveTexture(GL_TEXTURE0 + s);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    _numSamplers = 0;

    return *this;
}

Program &Program::setSampler(const std::string &name, GLenum target,
                             GLuint texture)
{
    glActiveTexture(GL_TEXTURE0 + _numSamplers);
    glBindTexture(target, texture);
    setUniform(name, _numSamplers);

    ++_numSamplers;

    return *this;
}

#define UNIFORM_SETTER_IMPL(type, glSetExp) \
    Program &Program::setUniform(const std::string &name, type value) const \
	{ \
        int uniformLoc = uniform(name); \
        if (uniformLoc != PROGRAM_NONEXISTENT_LOCATION) \
            glSetExp; \
        return (Program &) *this; \
    }

UNIFORM_SETTER_IMPL(int, glUniform1i(uniformLoc, value))
UNIFORM_SETTER_IMPL(float, glUniform1f(uniformLoc, value))
UNIFORM_SETTER_IMPL(const Vec3 &, glUniform3fv(uniformLoc, 1, value.v))
UNIFORM_SETTER_IMPL(const Mat4 &,
                    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, value.v))

#undef UNIFORM_SETTER_IMPL

void Program::_rebuildParamMaps()
{
    _uniforms.clear();
    _attributes.clear();

    int numUniforms, uniformNameLength;
    GLchar *name;

    glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &numUniforms);

    if (numUniforms > 0) {
        glGetProgramiv(_program, GL_ACTIVE_UNIFORM_MAX_LENGTH,
                       &uniformNameLength);

        name = new char[uniformNameLength];

        for (int i = 0; i < numUniforms; ++i) {
            GLint size;
            GLenum type;

            glGetActiveUniform(_program, i, uniformNameLength, NULL, &size,
                               &type, name);

            _uniforms[name] = glGetUniformLocation(_program, name);
        }

        delete name;
    }

    int numAttribs, attribNameLength;

    glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTES, &numAttribs);

    if (numAttribs > 0) {
        glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
                       &attribNameLength);

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
}
