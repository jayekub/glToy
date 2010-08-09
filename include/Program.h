#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <unordered_map>
#include <string>
#include <vector>

#include <boost/foreach.hpp>

#include "glToy.h"
#include "Listener.h"
#include "Vec.h"
#include "Mat.h"

// This sentinel value is returned as the location of uniforms and attributes
// that don't exist or aren't active
#define PROGRAM_NONEXISTENT_LOCATION 9999

// XXX disallow copying

class Program : public Listener
{
public:
    class Shader
    {
    public:
        Shader(GLuint type);
        Shader(const std::string &filename, GLuint type);
        ~Shader();

        Shader &compile() const;
        void reload() const;

        Shader &addFile(const std::string &filename);

        GLuint getId() const { return _shader; }
        std::string getInfoLog() const;

    private:
        std::vector<std::string> _filenames;
        GLuint _type;

        GLuint _shader;

        void _compile();
    };

    Program(bool debug = false);
    Program(Shader *shader, bool debug = false);
    Program(const std::vector<Shader *> &shaders, bool debug = false);
    virtual ~Program();

    void reload();

    Program &use() const;
    Program &link();

    Program &addShader(Shader *shader);

    bool hasUniform(const std::string &name) const;
    bool hasAttribute(const std::string &name) const;

    GLuint uniform(const std::string &name) const;
    GLuint attribute(const std::string &name) const;

    GLuint getId() const { return _program; }
    std::string getInfoLog() const;

    Program &setDebug(bool debug) { _debug = debug; return *this; }
    Program &setParameter(GLenum param, GLint value) const;

#define UNIFORM_SETTER_DECL(type) \
    Program &setUniform(const std::string &name, type value) const

    UNIFORM_SETTER_DECL(int);
    UNIFORM_SETTER_DECL(float);
    UNIFORM_SETTER_DECL(const Vec3 &);
    UNIFORM_SETTER_DECL(const Mat4 &);

#undef UNIFORM_SETTER_DECL

private:
    std::vector<Shader *> _shaders;
    bool _debug;

    GLuint _program;

    // unordered_map is a hash map
    typedef std::unordered_map<std::string, GLuint> _ParamMap;

    _ParamMap _uniforms;
    _ParamMap _attributes;

    void _rebuildParamMaps();
};

#endif /* PROGRAM_H_ */
