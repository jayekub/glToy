#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string>
#include <sstream>

#include "glToy.h"
#include "utils.h"

#include "FluidParticleSystem.h"
#include "TextureRenderPass.h"
#include "ScreenRenderPass.h"
#include "CellNoiseRenderer.h"
#include "FractalRenderer.h"
#include "ofxMSAFluidSolver.h"

int windowWidth = 640;
int windowHeight = 480;

const int fluidSize = 100;

double invWidth, invHeight;

bool reset = false;
int lastDrawTime = -1;
int lastFPSTime = -1;
int frames = 0;

Vec2d mouse, lastMouse;

////

FluidParticleSystem *particles0, *particles1;
TextureRenderPass *cellNoisePass0, *cellNoisePass1;
ScreenRenderPass *screenPass;

CellNoiseRenderer *cellNoiseRenderer;
FractalRenderer *fractalRenderer;

ofxMSAFluidSolver fluidSolver;

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

	fluidSolver.setSize(fluidSize, (float) fluidSize * (float) h / (float) w);
	cellNoisePass0->setSize(w, h);
	cellNoisePass1->setSize(w, h);
	screenPass->setSize(w, h);
}

void handleKey(unsigned char key, int x, int y)
{
	switch(key) {;
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
    // Prevent popping when the button is first pressed
    if (mouse.x < 0) {
        mouse.x = x;
        mouse.y = y;
        return;
    }

    lastMouse.x = mouse.x;
    lastMouse.y = mouse.y;
    mouse.x = x;
    mouse.y = y;

    Vec2d vel = mouse - lastMouse;

    vel.x *= invWidth;
    vel.y *= invHeight;

    vel = vel.mult(1000.0);

    fluidSolver.addForceAtPos((double) x * invWidth,
                              1.0 - (double) y * invHeight, vel.x, -vel.y);
}

void handleMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Signal that the mouse coords aren't valid
        mouse.x = -1;
        mouse.y = -1;
        lastMouse.x = -1;
        lastMouse.y = -1;
    }
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

    int drawTime = glutGet(GLUT_ELAPSED_TIME);
    double dt = (drawTime - lastDrawTime) / 1000.0;

    lastDrawTime = drawTime;

    fluidSolver.setDeltaT(dt);
    fluidSolver.update();

	particles0->update(dt, &fluidSolver);
	particles1->update(dt, &fluidSolver);

	cellNoiseRenderer->render(cellNoisePass0, particles0, 0.3);
    cellNoiseRenderer->render(cellNoisePass1, particles1, 0.15);

	fractalRenderer->render(screenPass);

	glutSwapBuffers();
	++frames;
}

void updateFramerate(int /* ignored */)
{
    int time = glutGet(GLUT_ELAPSED_TIME);
    int elapsed = time - lastFPSTime;
    float framerate = 1000.0 * (float) frames / (float) elapsed;

    std::stringstream framerateSS;

    framerateSS << "GL Toy " << framerate << " fps";

    glutSetWindowTitle(framerateSS.str().c_str());

    lastFPSTime = time;
    frames = 0;

    glutTimerFunc(1000, updateFramerate, 0);
}

int main(int argc, char **argv) {
    srandom((unsigned int) time(NULL));

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	if (argc > 1 && strcmp(argv[1], "-f") == 0) {
        int fullWidth = glutGet(GLUT_SCREEN_WIDTH);
        int fullHeight = glutGet(GLUT_SCREEN_HEIGHT);

        std::stringstream gameModeSS;

        gameModeSS << fullWidth << "x" << fullHeight;

        glutGameModeString(gameModeSS.str().c_str());
        glutEnterGameMode();
	} else {
	    glutInitWindowPosition(0, 0);
	    glutInitWindowSize(windowWidth, windowHeight);

	    glutCreateWindow("GL Toy");
	}

	glewInit();

	fluidSolver.setup();
	fluidSolver.enableRGB(false).setFadeSpeed(0).setDeltaT(0.1).setVisc(0.002)
               .setWrap(true, true).setColorDiffusion(0);

	particles0 = new FluidParticleSystem(200, 0.01, 0.1);
	particles1 = new FluidParticleSystem(1000, 0.01, 0.1);

	cellNoisePass0 = new TextureRenderPass(windowWidth, windowHeight);
	cellNoisePass1 = new TextureRenderPass(windowWidth, windowHeight);

	cellNoiseRenderer = new CellNoiseRenderer(particles0, 0.3);

	std::vector<TextureRenderPass *> passes;

	passes.push_back(cellNoisePass0);
    passes.push_back(cellNoisePass1);

    fractalRenderer = new FractalRenderer(passes);
	screenPass = new ScreenRenderPass(windowWidth, windowHeight);

    resize(windowWidth, windowHeight);

    glutDisplayFunc(draw);
    glutIdleFunc(draw);
    glutTimerFunc(1000, updateFramerate, 0);

    glutReshapeFunc(resize);
    glutKeyboardFunc(handleKey);

    glutMotionFunc(handleMouseMotion);
    glutMouseFunc(handleMouse);

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}
