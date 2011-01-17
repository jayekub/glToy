#include "glToy.h"
#include "Noise.h"
#include "Camera.h"
#include "Particle.h"

#include "SprayPrim.h"

SprayPrim::SprayPrim(
    const char *name,
    ParticleSystem *particleSystem) :
    ParticleSystemPrim(name, particleSystem, true)
{
    //_sprayProgram.setDebug(true);

    _sprayProgram.addShader(
        (new Program::Shader(GL_VERTEX_SHADER))
             ->addFile("shaders/particlequad.vs").compile());

    _sprayProgram.addShader(
        (new Program::Shader(GL_GEOMETRY_SHADER))
            ->addFile("shaders/common.inc")
             .addFile("shaders/particlequad.gs").compile());

    _sprayProgram.addShader(
        ((new Program::Shader(GL_FRAGMENT_SHADER))
        ->addFile("shaders/common.inc")
         .addFile("shaders/spray.fs").compile()));

    _sprayProgram.link();
}

/*
// sort according to viewplane distance
ParticleSystemPrim::_ParticleLt *
SprayPrim::_getParticleLtImpl(const RenderState &state) const
{
    struct _ParticleLtImpl : public _ParticleLt {

        _ParticleLtImpl(const RenderState &state,
                        const ParticleSystem *particleSystem) :
            _ParticleLt(NULL), _particleSystem(particleSystem) {

            _modelViewMat = state.getTransformMat() * state.viewMat;
        }

        bool operator()(const Particle *a, const Particle *b) const {
            vec_t aDist = _modelViewMat.ptransform(a->position).z;
            vec_t bDist = _modelViewMat.ptransform(b->position).z;

            return aDist > bDist;
        }

    private:
        Mat4 _modelViewMat;
        const ParticleSystem *_particleSystem;
    };

    static _ParticleLtImpl *particleLtImpl = NULL;

    delete particleLtImpl;
    return new _ParticleLtImpl(state, _particleSystem);
}
*/

void SprayPrim::_preRender(RenderState &state)
{
    _sprayProgram.use();
    _sprayProgram.setUniform("modelMat", state.getTransformMat())
                 .setUniform("viewMat", state.viewMat)
                 .setUniform("projMat", state.projectionMat)
                 .setUniform("cameraPos", state.camera->position)
                 .setUniform("meanRadius", 0.5f)
                 .setUniform("radiusSpread", 0.25f);

    const GLuint centerInLoc = _sprayProgram.attribute("centerIn");
    const GLuint velocityInLoc = _sprayProgram.attribute("velocityIn");
    const GLuint radiusInLoc = _sprayProgram.attribute("radiusIn");

    glBindBuffer(GL_ARRAY_BUFFER, _particleBuffer);

    glEnableVertexAttribArray(centerInLoc);

    glVertexAttribPointer(centerInLoc, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Particle), 0);

    glEnableVertexAttribArray(velocityInLoc);

    glVertexAttribPointer(velocityInLoc, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Particle),
                          BUFFER_OFFSET(sizeof(Vec3)));

    glEnableVertexAttribArray(radiusInLoc);

    glVertexAttribPointer(radiusInLoc, 1, GL_FLOAT, GL_FALSE,
                          sizeof(Particle),
                          BUFFER_OFFSET(2 * sizeof(Vec3)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_UPPER_LEFT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SprayPrim::_postRender(RenderState &state)
{
    glDisable(GL_BLEND);
    //glDisable(GL_POINT_SPRITE);
    glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
}
