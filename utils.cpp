#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/foreach.hpp>

#include "utils.h"

std::string readFile(const std::string &filename) {
	std::ifstream file(filename.c_str());
	std::string contents;

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			contents += line + '\n';
		}
	} else {
	    fprintf(stderr, "Unable to open file %s\n", filename.c_str());
		throw "Unable to open file";
	}

	return contents;
}

GLuint makeTestTexture()
{
    uint t[12]; // 2 x 2 image with 3 color components for each pixel

    t[0] = 1; t[1] = 0; t[2] = 0;
    t[3] = 0; t[4] = 1; t[5] = 0;
    t[6] = 0; t[7] = 0; t[8] = 1;
    t[9] = 1; t[10] = 1; t[11] = 1;

    // create texture and read in data
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_WRAP_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_WRAP_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 2, 2, 0, GL_RGB,
                 GL_UNSIGNED_INT, t);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void drawViewportQuad(int width, int height)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslated((double) width / 2.0, (double) height / 2.0, 0.0);
    glScaled((double) width / 2.0, (double) height / 2.0, 1.0);

    // TODO use VBO
    glBegin(GL_TRIANGLE_STRIP);
        glColor3f(1.0, 0.0, 0.0);
        glTexCoord2d(0.0, 0.0);
        glVertex2d(-1.0, -1.0);

        glColor3f(0.0, 1.0, 0.0);
        glTexCoord2d(1.0, 0.0);
        glVertex2d(1.0, -1.0);

        glColor3f(0.0, 0.0, 1.0);
        glTexCoord2d(0.0, 1.0);
        glVertex2d(-1.0, 1.0);

        glColor3f(0.0, 0.0, 0.0);
        glTexCoord2d(1.0, 1.0);
        glVertex2d(1.0, 1.0);
    glEnd();

    glPopMatrix();
}
