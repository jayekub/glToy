#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "glToy.h"
#include "utils.h"

#include "ParticleSystem.h"
#include "TextureRenderPass.h"
#include "ScreenRenderPass.h"
#include "CellNoiseRenderer.h"
#include "FractalRenderer.h"
#include "ofxMSAFluidSolver.h"

int windowWidth = 640;
int windowHeight = 480;
double invWidth, invHeight;

ParticleSystem *particles0, *particles1;
TextureRenderPass *cellNoisePass0, *cellNoisePass1;
ScreenRenderPass *screenPass;

CellNoiseRenderer *cellNoiseRenderer;
FractalRenderer *fractalRenderer;

ofxMSAFluidSolver fluidSolver;

Vec2d mouse, lastMouse;

bool reset = false;

void resize(int w, int h) {
	windowWidth = w;
	windowHeight = h;
	invWidth = 1.0 / (double) w;
	invHeight = 1.0 / (double) h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);
	glOrtho(0, w, h, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	fluidSolver.setSize(w / 3.0, h / 3.0);
	cellNoisePass0->setSize(w, h);
	cellNoisePass1->setSize(w, h);
	screenPass->setSize(w, h);
}

void handleKey(unsigned char key, int x, int y)
{
	switch(key) {
		case 'r':
            reset = true;
			break;
		case 'q':
			exit(0);
			break;
	}
}

void handleMouseMotion(int x, int y)
{
    //printf("Mouse moved at (%d, %d)\n", x, y);
    lastMouse.x = mouse.x;
    lastMouse.y = mouse.y;
    mouse.x = x;
    mouse.y = y;

    Vec2d vel = mouse - lastMouse;

    vel = vel.normalize().mult(10.0);

    fluidSolver.addForceAtPos((double) x * invWidth,
                              1.0 - (double) y * invHeight, vel.x, -vel.y);
}

void handleMouse(int button, int state, int x, int y)
{

}

void draw() {
    if (reset) {
        particles0->reset();
        particles1->reset();
        fluidSolver.reset();
        cellNoiseRenderer->reload();
        fractalRenderer->reload();

        reset = false;
    }

    fluidSolver.update();

    //printf("Fluid average speed is %g\n", fluidSolver.getAvgSpeed());

    BOOST_FOREACH(Particle *p, particles0->getParticles()) {
        ofPoint velocity;

        fluidSolver.getInfoAtPos(p->position.x, p->position.y, &velocity, NULL);

        p->velocity.x += velocity.x;
        p->velocity.y += velocity.y;

        //printf("Velocity at (%g, %g) is (%g, %g)\n",
        //        p->position.x, p->position.y,
        //        p->velocity.x, p->velocity.y);
    }

    BOOST_FOREACH(Particle *p, particles1->getParticles()) {
        ofPoint velocity;

        fluidSolver.getInfoAtPos(p->position.x, p->position.y, &velocity, NULL);

        p->velocity.x += velocity.x;
        p->velocity.y += velocity.y;

        //printf("Velocity at (%g, %g) is (%g, %g)\n",
        //        p->position.x, p->position.y,
        //        p->velocity.x, p->velocity.y);
    }

	particles0->update(0.5);
	particles1->update(0.5);

	cellNoiseRenderer->render(cellNoisePass0, particles0, 200.0);
    cellNoiseRenderer->render(cellNoisePass1, particles1, 100.0);

	fractalRenderer->render(screenPass);

	glutSwapBuffers();
}


int main(int argc, char **argv) {
    srandom((unsigned int) time(NULL));

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight);

	glutCreateWindow("GL Toy");

	glewInit();

	fluidSolver.setup(windowWidth, windowHeight);
	fluidSolver.enableRGB(false).setFadeSpeed(0).setDeltaT(0.1).setVisc(0.00015)
	           .setWrap(true, true).setColorDiffusion(0);

	particles0 = new ParticleSystem(200, 0.01);
	particles1 = new ParticleSystem(1000, 0.01);

	cellNoisePass0 = new TextureRenderPass(windowWidth, windowHeight);
	cellNoisePass1 = new TextureRenderPass(windowWidth, windowHeight);

	cellNoiseRenderer = new CellNoiseRenderer(particles0, 200.0);

	std::vector<TextureRenderPass *> passes;

	passes.push_back(cellNoisePass0);
    passes.push_back(cellNoisePass1);

    fractalRenderer = new FractalRenderer(passes);
	screenPass = new ScreenRenderPass(windowWidth, windowHeight);

    resize(windowWidth, windowHeight);

    glutDisplayFunc(draw);
    glutIdleFunc(draw);
    glutReshapeFunc(resize);
    glutKeyboardFunc(handleKey);

    glutMotionFunc(handleMouseMotion);
    glutMouseFunc(handleMouse);

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}
