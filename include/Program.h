#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <map>
#include <string>
#include <vector>

#include <boost/foreach.hpp>

#include "glToy.h"
#include "Reloadable.h"

class Program : public Reloadable
{
public:
    typedef std::pair<std::string, GLuint> ShaderSpec;

    Program();
    Program(const std::vector<ShaderSpec> &shaderSpecs);
    virtual ~Program();

    void reload();
    void use();

    GLuint uniform(const std::string &name) { return _uniforms[name]; };
    GLuint attribute(const std::string &name) { return _attributes[name]; };

    void addShader(const ShaderSpec &shaderSpec);
    //void setShaders(const std::vector<ShaderSpec> &shaderSpecs,
    //                bool doReload = true);

    static GLuint makeShader(const std::string &filename, GLenum shaderType);
    static GLuint makeProgram(const std::vector<GLuint> &shaders);

    static std::string getShaderInfoLog(GLuint shader);
    static std::string getProgramInfoLog(GLuint program);

private:
    std::map<std::string, GLuint> _uniforms;
    std::map<std::string, GLuint> _attributes;

    std::vector<ShaderSpec> _shaderSpecs;
    std::vector<GLuint> _shaders;

    GLuint _program;

    void _rebuildParamMaps();
};

#endif /* PROGRAM_H_ */
