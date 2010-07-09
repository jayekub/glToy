#include <stdio.h>
#include <stdlib.h>

#include <boost/foreach.hpp>

#include <GL/glew.h>
#include <GL/glut.h>

#include "utils.h"
#include "Particle.h"

GLuint cellNoiseProgram;

std::vector<Particle *> particles;

int numParticles = 5000;
int maxParticleVelocity = 10;
double particleSize = 100.0;

int windowWidth = 1024;
int windowHeight = 768;

void resize(int w, int h) {
	windowWidth = w;
	windowHeight = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);
	glOrtho(0, w, h, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	//	glLoadIdentity();
}

void initParticles() {
	for (int i = 0; i < numParticles; ++i) {
		Particle *p = new Particle();

		p->position.x = randFloat() * windowWidth;
		p->position.y = randFloat() * windowHeight;

		p->velocity = Vec2d(2.0 * randFloat() - 1.0, 2.0 * randFloat() - 1.0);
		p->velocity = p->velocity.normalize().mult(maxParticleVelocity);

		particles.push_back(p);
	}
}

void moveParticles(double dt) {
	BOOST_FOREACH(Particle *p, particles) {
		p->position.x += p->velocity.x * dt;
		p->position.y += p->velocity.y * dt;

		if (p->position.x < -particleSize)
			p->position.x = windowWidth + particleSize;
		else if (p->position.x > windowWidth + particleSize)
			p->position.x = -particleSize;

		if (p->position.y < -particleSize)
			p->position.y = windowHeight + particleSize;
		else if (p->position.y > windowHeight + particleSize)
			p->position.y = -particleSize;
	}
}

void drawParticles() {
	glUseProgram(cellNoiseProgram);

	BOOST_FOREACH(Particle *p, particles) {
		glPushMatrix();

		glTranslated(p->position.x, p->position.y, 0.0);
		glScaled(particleSize, particleSize, 1.0);

		glColor3f(randFloat(), randFloat(), randFloat());

		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2d(0.0, 0.0);
			glVertex2d(-0.5, -0.5);

			glTexCoord2d(1.0, 0.0);
			glVertex2d(0.5, -0.5);

			glTexCoord2d(0.0, 1.0);
			glVertex2d(-0.5, 0.5);

			glTexCoord2d(1.0, 1.0);
			glVertex2d(0.5, 0.5);
		glEnd();

		glPopMatrix();
	}
}

float angle = 0.0;

void draw() {
	// notice that we're now clearing the depth buffer
	// as well this is required, otherwise the depth buffer
	// gets filled and nothing gets rendered.
	// Try it out, remove the depth buffer part.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	moveParticles(1.0);
	drawParticles();

	// swapping the buffers causes the rendering above to be
	// shown
	glutSwapBuffers();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight);

	glutCreateWindow("GL Toy");

	glewInit();

	glutDisplayFunc(draw);
	glutIdleFunc(draw);
	glutReshapeFunc(resize);

	std::vector<GLuint> shaders;
	shaders.push_back(makeShader("cellNoise.fp", GL_FRAGMENT_SHADER));

	cellNoiseProgram = makeProgram(shaders);

	initParticles();

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}
