#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string>
#include <sstream>

#include "glToy.h"
#include "utils.h"

#include "Listener.h"

#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "Anemone.h"
#include "SceneRenderVisitor.h"
#include "TextureRenderer.h"

#include "ofxMSAFluidSolver.h"
#include "FluidParticleSystem.h"
#include "TextureRenderPass.h"
#include "DepthRenderPass.h"
#include "ScreenRenderPass.h"
#include "CellNoiseRenderer.h"

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

Scene *anemoneScene;
Anemone *anemone;
DepthRenderPass *anemoneShadowPass;
SceneRenderVisitor *sceneRenderer;
TextureRenderPass *testRenderPass;
TextureRenderer *textureRenderer;

ofxMSAFluidSolver fluidSolver;
FluidParticleSystem *particles0, *particles1;

TextureRenderPass *cellNoisePass0 = NULL, *cellNoisePass1 = NULL;
ScreenRenderPass *screenPass;

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

        // XXX blech
        Vec3 pos = Vec3(2. * x / (float) windowWidth - 1,
                        2. * (windowHeight - y) / (float) windowHeight - 1, 0.);

        anemone->setMagnet(pos, .3);
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        anemone->setMagnet(Vec3(), 0.);
    }
}

void draw() {
    if (reset) {
        //fluidSolver.reset();
        //particles0->reset();
        //particles1->reset();

        Listener::reloadAll();

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

    /*
    sceneRenderer->setRenderPass(testRenderPass);
    sceneRenderer->setCameraName("keyLightShadowCam");
    sceneRenderer->render(anemoneScene);

    textureRenderer->setTexture(testRenderPass->getTexture());
    textureRenderer->render();
    */

    sceneRenderer->setRenderPass(screenPass);
    sceneRenderer->setCameraName("anemoneCamera");
    sceneRenderer->render(anemoneScene);

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

Scene *buildAnemoneScene()
{
    Scene *scene = new Scene("sceneRoot");

    // Camera
    Camera *camera = new Camera("anemoneCamera");

    camera->position = Vec3(0., 0., -3.);
    camera->center = Vec3(0., 0., 0.);
    camera->up = Vec3(0., 1., 0.);
    camera->nearClip = 2.;
    camera->farClip = 100.;//4.;

    scene->addGlobal(camera);

    // Light
    Transform *keyLightTransform = new Transform("keyLightTransform");

    keyLightTransform->translation = Vec3(5., 5., -5.);

    scene->addGlobal(keyLightTransform);

    Light *keyLight = new Light("keyLight");

    keyLight->type = Light::POINT;
    keyLight->color = Vec3(1., 1., 1.);

    keyLightTransform->addChild(keyLight);

    // Shadow
    Camera *keyLightShadowCam = new Camera("keyLightShadowCam");

    Vec3 OtoShad = Vec3(5., 5., -5.).normalize();

    keyLightShadowCam->position = -4. * OtoShad;//Vec3(5., 5., -5.);
    keyLightShadowCam->center = Vec3(0., 0., 0.);
    keyLightShadowCam->up = Vec3(0., 1., 0.);
    keyLightShadowCam->nearClip = 3.;
    keyLightShadowCam->farClip = 5.;

    keyLightShadowCam->isShadowCamera = true;
    keyLightShadowCam->light = keyLight;

    scene->addGlobal(keyLightShadowCam);

    keyLight->hasShadow = true;
    keyLight->shadowTexture = testRenderPass->getDepthTexture();

    // Anemone
    Transform *anemoneTransform = new Transform("anemoneTransform");

    // Leave it at the origin for now

    scene->addChild(anemoneTransform);

    anemone = new Anemone("anemone",
                          /* numTentacles */ 25, //100,
                          /* numSegments */ 4,
                          /* maxWidth */ 25, //7.,
                          /* wiggle */ .06);

    anemoneTransform->addChild(anemone);

    return scene;
}

void buildCellNoiseScene()
{
    fluidSolver.setup();
    fluidSolver.enableRGB(false).setFadeSpeed(0).setDeltaT(0.1).setVisc(0.005)
               .setWrap(true, true).setColorDiffusion(0);

    particles0 = new FluidParticleSystem(200, 0.01, 0.1);
    particles1 = new FluidParticleSystem(1000, 0.01, 0.1);

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

    srandom((unsigned int) time(NULL));

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH /*| GLUT_ACCUM*/);

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

	checkExtension(GL_VERSION_3_2);
	checkExtension(GLEW_ARB_geometry_shader4);

	//anemoneShadowPass = new DepthRenderPass(windowWidth, windowHeight);
    screenPass = new ScreenRenderPass(windowWidth, windowHeight);

	// for debugging

    testRenderPass = new TextureRenderPass(windowWidth, windowHeight);

    textureRenderer = new TextureRenderer();
	textureRenderer->setProgram(new Program(
	    new Program::Shader("shaders/test.fs", GL_FRAGMENT_SHADER)))
                    .setTexture(testRenderPass->getTexture())
                    .setRenderPass(screenPass);

	anemoneScene = buildAnemoneScene();

	sceneRenderer = new SceneRenderVisitor(screenPass);

    resize(windowWidth, windowHeight);

    glutDisplayFunc(draw);
    glutIdleFunc(draw);
    glutTimerFunc(1000, updateFramerate, 0);

    glutReshapeFunc(resize);
    glutKeyboardFunc(handleKey);

    glutMotionFunc(handleMouseMotion);
    glutMouseFunc(handleMouse);

	glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glutMainLoop();

	} catch(const char *err) {
	    fprintf(stderr, "Error: %s\n", err);

        return 1;
	}

	return 0;
}
