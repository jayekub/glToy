#include <stdio.h>

#include <boost/foreach.hpp>

#include "Particle.h"
#include "FluidParticleSystem.h"

void FluidParticleSystem::update(double dt, ofxMSAFluidSolver *fluid)
{
    BOOST_FOREACH(Particle *p, _particles) {
        ofPoint velocity;

        fluid->getInfoAtPos(p->position.x, p->position.y, &velocity, NULL);

        //p->position.x += velocity.x * dt;
        //p->position.y += velocity.y * dt;

        p->velocity.x += velocity.x;
        p->velocity.y += velocity.y;

        p->position.x += p->velocity.x * dt;
        p->position.y += p->velocity.y * dt;

        WRAP_PARTICLE(p);
    }
}
