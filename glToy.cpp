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

int windowWidth = 1024;
int windowHeight = 768;

ParticleSystem *particles0, *particles1;
TextureRenderPass *cellNoisePass0, *cellNoisePass1;
ScreenRenderPass *screenPass;

CellNoiseRenderer *cellNoiseRenderer;
FractalRenderer *fractalRenderer;

void resize(int w, int h) {
	windowWidth = w;
	windowHeight = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);
	glOrtho(0, w, h, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	//	glLoadIdentity();

	cellNoisePass0->setSize(w, h);
	cellNoisePass1->setSize(w, h);
	screenPass->setSize(w, h);
}

void handleKey(unsigned char key, int x, int y)
{
	switch(key) {
		case 'r':
			cellNoiseRenderer->reload();
			fractalRenderer->reload();
			break;
		case 'q':
			exit(0);
			break;
	}
}

void draw() {
	particles0->update(1.0);
	particles1->update(1.0);

    cellNoiseRenderer->setNoiseParticles(particles0);
	cellNoiseRenderer->render(cellNoisePass0);

    cellNoiseRenderer->setNoiseParticles(particles1);
    cellNoiseRenderer->render(cellNoisePass1);

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

	glutDisplayFunc(draw);
	glutIdleFunc(draw);
	glutReshapeFunc(resize);
	glutKeyboardFunc(handleKey);
	
	particles0 = new ParticleSystem(200, 0.01);
	particles1 = new ParticleSystem(100, 0.01);

	cellNoisePass0 = new TextureRenderPass(windowWidth, windowHeight);
	cellNoisePass1 = new TextureRenderPass(windowWidth, windowHeight);

	cellNoiseRenderer = new CellNoiseRenderer(particles0, 200.0);

	std::vector<TextureRenderPass *> passes;

	passes.push_back(cellNoisePass0);
    passes.push_back(cellNoisePass1);

    fractalRenderer = new FractalRenderer(passes);
	screenPass = new ScreenRenderPass(windowWidth, windowHeight);

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}
