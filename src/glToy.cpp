#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string>
#include <sstream>

#include "glToy.h"
#include "utils.h"

#include "Listener.h"

#include "BubblesScene.h"

#include "TextureRenderer.h"
#include "Program.h"
#include "ofxMSAFluidSolver.h"
#include "FluidParticleSystem.h"
#include "TextureRenderPass.h"
#include "ScreenRenderPass.h"
#include "CellNoiseRenderer.h"

int windowWidth = 640;
int windowHeight = 480;

const int fluidSize = 100;

float invWidth, invHeight;
//float fluidWidth, fluidHeight;

Vec2 mouse, lastMouse;

int _lastDrawTime = -1;
int _lastFpsTime = -1;
int _frames = 0;

bool _pause = false;
bool _reset = false;

////

BubblesScene *_bubblesScene;
ScreenRenderPass *_screenPass;

GLuint testTexture;

ofxMSAFluidSolver fluidSolver;
FluidParticleSystem *particles0, *particles1;

TextureRenderPass *cellNoisePass0 = NULL, *cellNoisePass1 = NULL;

CellNoiseRenderer *cellNoiseRenderer;
TextureRenderer *combineRenderer;

void resize(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    invWidth = 1.0 / (double) w;
    invHeight = 1.0 / (double) h;

    fluidSolver.setSize(fluidSize, (float) fluidSize * (float) h / (float) w);

    Listener::resizeAll(w, h);
}

void handleKey(unsigned char key, int x, int y)
{
    switch(key) {;
        case 'r':
            _reset = true;
            break;
        case 'p':
            _pause = !_pause;
            break;
        case 'q':
            exit(0);
            break;
    }

    _bubblesScene->handleKey(key, x, y);
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

void draw() {
    if (_reset) {
        Listener::reloadAll();
        _reset = false;
    }

    int drawTime = glutGet(GLUT_ELAPSED_TIME);
    double dt = (drawTime - _lastDrawTime) / 1000.;

    if (!_pause)
        _bubblesScene->update(dt);

    _bubblesScene->render(_screenPass);

    glutSwapBuffers();

    ++_frames;
    _lastDrawTime = drawTime;

#if 0
    // TODO average dt?
    fluidSolver.setDeltaT(dt);

    //dampenFluid(&fluidSolver, 0.9);
    fluidSolver.update();

    particles0->update(dt, &fluidSolver);
    particles1->update(dt, &fluidSolver);

#ifdef USE_ACCUM
    _screenPass->begin();

    glClearAccum(0.0, 0.0, 0.0, 0.0);
    glClearColor(1.0, 0.0, 1.0, 1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);

    glColorMask(GL_TRUE, GL_FALSE, GL_TRUE, GL_TRUE);
    cellNoiseRenderer->render(_screenPass, particles0, 0.3);
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
}

void updateFramerate(int /* ignored */)
{
    int fpsTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsed = fpsTime - _lastFpsTime;
    float framerate = 1000.0 * (float) _frames / (float) elapsed;

    std::stringstream framerateSS;

    framerateSS << framerate << " fps";
    glutSetWindowTitle(framerateSS.str().c_str());

    _lastFpsTime = fpsTime;
    _frames = 0;

    glutTimerFunc(500, updateFramerate, 0);
}

void buildCellNoiseScene()
{
    fluidSolver.setup();
    fluidSolver.enableRGB(false).setFadeSpeed(0).setDeltaT(0.1).setVisc(0.005)
               .setWrap(true, true).setColorDiffusion(0);

    // XXX rewrite
    //particles0 = new FluidParticleSystem(200, 0.01, 0.1);
    //particles1 = new FluidParticleSystem(1000, 0.01, 0.1);

#ifndef USE_ACCUM
    cellNoisePass0 = new TextureRenderPass(windowWidth, windowHeight);
    cellNoisePass1 = new TextureRenderPass(windowWidth, windowHeight);

    Program *combineProgram = new Program(
            new Program::Shader("shaders/combine.fs", GL_FRAGMENT_SHADER));

    std::vector<GLuint> passes;

    passes.push_back(cellNoisePass0->getTexture());
    passes.push_back(cellNoisePass1->getTexture());

    combineRenderer = new TextureRenderer(passes);
    combineRenderer->setProgram(combineProgram);
#endif

    cellNoiseRenderer = new CellNoiseRenderer();
}

int main(int argc, char **argv) {
    try {

    srand((unsigned int) time(NULL));

    glutInit(&argc, argv);

    //glutInitContextVersion(3, 2);
    //glutInitContextProfile(GLUT_CORE_PROFILE);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

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

#ifdef USE_GL3W
    gl3wInit();
#endif

    //checkExtension(GL_VERSION_3_2);
    //checkExtension(GLEW_ARB_geometry_shader4);

    ////

    glutDisplayFunc(draw);
    glutIdleFunc(draw);
    glutTimerFunc(1000, updateFramerate, 0);

    glutReshapeFunc(resize);
    glutKeyboardFunc(handleKey);

    //glutMotionFunc(handleMouseMotion);
    //glutMouseFunc(handleMouse);

    glEnable(GL_DEPTH_TEST);
//  glEnable(GL_CULL_FACE);

//  glPolygonMode(GL_FRONT, GL_LINE);
//  glPolygonMode(GL_BACK, GL_LINE);

    _screenPass = new ScreenRenderPass(windowWidth, windowHeight);
    _bubblesScene = new BubblesScene(windowWidth, windowHeight);

    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
           glGetString(GL_SHADING_LANGUAGE_VERSION));

    _lastDrawTime = glutGet(GLUT_ELAPSED_TIME);
    _lastFpsTime = _lastDrawTime;

    glutMainLoop();

    delete _bubblesScene;

    } catch(const char *err) {
        fprintf(stderr, "Error: %s\n", err);

        return 1;
    }

    return 0;
}
