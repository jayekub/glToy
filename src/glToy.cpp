#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string>
#include <sstream>

#include "glToy.h"
#include "utils.h"

#include "Listener.h"

#include "BubblesScene.h"
#include "CellNoiseScene.h"
#include "SprayScene.h"

#include "Program.h"
#include "ScreenRenderPass.h"

int windowWidth = 853;
int windowHeight = 640;

float invWidth, invHeight;

int _lastDrawTime = -1;
int _lastFpsTime = -1;
int _frames = 0;

bool _pause = false;
bool _reset = false;
bool _debug = false;
bool _grabMouse = false;

Scene *_currentScene;
int _currentSceneIndex = -1;

ScreenRenderPass *_screenPass;

////

void nextScene()
{
    delete _currentScene;

    switch (_currentSceneIndex) {
        case 0:
            _currentScene = new BubblesScene(windowWidth, windowHeight);
            break;
        case 1:
            _currentScene = new CellNoiseScene(windowWidth, windowHeight);
            break;
        case 2:
        default:
            _currentScene = new SprayScene(windowWidth, windowHeight);
            break;
    }

    _currentSceneIndex = (_currentSceneIndex + 1) % 3;
}

void resize(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    invWidth = 1.0 / (double) w;
    invHeight = 1.0 / (double) h;

    Listener::resizeAll(w, h);
}

void handleKey(unsigned char key, int x, int y)
{
    switch(key) {;
        case 'r':
            _reset = true;
            return;

        case 'p':
            _pause = !_pause;
            return;

        case 'm':
            _debug = !_debug;

            if (_debug) {
                glPolygonMode(GL_FRONT, GL_LINE);
                glPolygonMode(GL_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT, GL_FILL);
                glPolygonMode(GL_BACK, GL_FILL);
            }

            return;

        case 'n':
            nextScene();
            return;

        case 'q':
            exit(0);
            return;
    }

    _currentScene->handleKey(key, x, y);
}

void handleMouse(int button, int state, int x, int y)
{
    _currentScene->handleMouse(button, state, x, y);
}

void handleMouseMotion(int x, int y)
{
    _currentScene->handleMouseMotion(x, y);
}

void handlePassiveMouseMotion(int x, int y)
{
    _currentScene->handlePassiveMouseMotion(x, y);
}

void draw() {
    if (_reset) {
        Listener::reloadAll();
        _reset = false;
    }

    int drawTime = glutGet(GLUT_ELAPSED_TIME);
    double dt = (drawTime - _lastDrawTime) / 1000.;

    if (!_pause)
        _currentScene->update(dt);

    _currentScene->render(_screenPass);

    glutSwapBuffers();

    ++_frames;
    _lastDrawTime = drawTime;
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

    glutMouseFunc(handleMouse);
    glutMotionFunc(handleMouseMotion);
    glutPassiveMotionFunc(handlePassiveMouseMotion);

    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);

    _screenPass = new ScreenRenderPass(windowWidth, windowHeight);

    nextScene();

    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
           glGetString(GL_SHADING_LANGUAGE_VERSION));

    _lastDrawTime = glutGet(GLUT_ELAPSED_TIME);
    _lastFpsTime = _lastDrawTime;

    glutMainLoop();

    } catch(const char *err) {
        fprintf(stderr, "Error: %s\n", err);

        return 1;
    }

    return 0;
}
