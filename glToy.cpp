#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string>
#include <sstream>

#include "glToy.h"
#include "utils.h"

#include "Anemone.h"
#include "ofxMSAFluidSolver.h"
#include "FluidParticleSystem.h"
#include "TextureRenderPass.h"
#include "ScreenRenderPass.h"
#include "CellNoiseRenderer.h"
#include "CombineRenderer.h"
#include "SceneRenderer.h"

int windowWidth = 640;
int windowHeight = 480;

const int fluidSize = 100;

float invWidth, invHeight;
//float fluidWidth, fluidHeight;

bool reset = false;
int lastDrawTime = -1;
int lastFPSTime = -1;
int frames = 0;
double avgDT = -1.0;

Vec2 mouse, lastMouse;

////

Anemone *anemone;

ofxMSAFluidSolver fluidSolver;
FluidParticleSystem *particles0, *particles1;

TextureRenderPass *cellNoisePass0 = NULL, *cellNoisePass1 = NULL;
ScreenRenderPass *screenPass;

CellNoiseRenderer *cellNoiseRenderer;
CombineRenderer *combineRenderer;
SceneRenderer *sceneRenderer;

void resize(int w, int h) {
	windowWidth = w;
	windowHeight = h;
	invWidth = 1.0 / (double) w;
	invHeight = 1.0 / (double) h;

	fluidSolver.setSize(fluidSize, (float) fluidSize * (float) h / (float) w);
	screenPass->setSize(w, h);

	if (cellNoisePass0) cellNoisePass0->setSize(w, h);
	if (cellNoisePass1) cellNoisePass1->setSize(w, h);
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

    Vec2 vel = mouse - lastMouse;

    vel.x *= invWidth;
    vel.y *= invHeight;

    vel = vel * 1000.0;

#ifdef USE_ACCUM
    fluidSolver.addForceAtPos((double) x * invWidth,
                              1.0 - (double) y * invHeight, vel.x, -vel.y);
#else
    fluidSolver.addForceAtPos((double) x * invWidth,
                              (double) y * invHeight, vel.x, vel.y);
#endif
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
        fluidSolver.reset();
        particles0->reset();
        particles1->reset();
        cellNoiseRenderer->reload();

        reset = false;
    }

    //int drawTime = glutGet(GLUT_ELAPSED_TIME);
    //double dt = (drawTime - lastDrawTime) / 1000.0;

    //lastDrawTime = drawTime;

    double dt = avgDT < 0 ? 0.01 : avgDT;

#if 0
    // TODO average dt?
    fluidSolver.setDeltaT(dt);

    //dampenFluid(&fluidSolver, 0.9);
    fluidSolver.update();

	particles0->update(dt, &fluidSolver);
	particles1->update(dt, &fluidSolver);

#ifdef USE_ACCUM
	screenPass->begin();

	glClearAccum(0.0, 0.0, 0.0, 0.0);
    glClearColor(1.0, 0.0, 1.0, 1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);

    glColorMask(GL_TRUE, GL_FALSE, GL_TRUE, GL_TRUE);
    cellNoiseRenderer->render(screenPass, particles0, 0.3);
    glAccum(GL_ACCUM, 1.0);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glClearColor(0.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
    cellNoiseRenderer->render(screenPass, particles1, 0.15);
    glAccum(GL_ACCUM, 1.0);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glClearColor(0.0, 0.0, 0.0, 1.0);    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glAccum(GL_RETURN, 1.0);

    screenPass->end();
#else
    glClearColor(1.0, 1.0, 1.0, 1.0);

    cellNoisePass0->begin();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cellNoiseRenderer->render(cellNoisePass0, particles0, 0.3);
    cellNoisePass0->end();

    cellNoisePass1->begin();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cellNoiseRenderer->render(cellNoisePass1, particles1, 0.15);
    cellNoisePass0->end();

    screenPass->begin();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    combineRenderer->render(screenPass);
    screenPass->end();
#endif
#endif

    anemone->update(dt);

    screenPass->begin();
    sceneRenderer->render(screenPass);
    screenPass->end();

	glutSwapBuffers();
	++frames;
}

void updateFramerate(int /* ignored */)
{
    int time = glutGet(GLUT_ELAPSED_TIME);
    int elapsed = time - lastFPSTime;
    float framerate = 1000.0 * (float) frames / (float) elapsed;

    avgDT = 1.0 / framerate;

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

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ACCUM);

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
	fluidSolver.enableRGB(false).setFadeSpeed(0).setDeltaT(0.1).setVisc(0.005)
               .setWrap(true, true).setColorDiffusion(0);

	particles0 = new FluidParticleSystem(200, 0.01, 0.1);
	particles1 = new FluidParticleSystem(1000, 0.01, 0.1);

#ifndef USE_ACCUM
    cellNoisePass0 = new TextureRenderPass(windowWidth, windowHeight);
    cellNoisePass1 = new TextureRenderPass(windowWidth, windowHeight);

    std::vector<TextureRenderPass *> passes;

    passes.push_back(cellNoisePass0);
    passes.push_back(cellNoisePass1);

    combineRenderer = new CombineRenderer(passes);
#endif

	cellNoiseRenderer = new CellNoiseRenderer();

	////

	anemone = new Anemone(100, 10);

	sceneRenderer = new SceneRenderer();
	sceneRenderer->addPrim(anemone);

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
