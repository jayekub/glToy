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

void drawViewportQuad(int width, int height)
{
    glPushMatrix();

    glTranslated((double) width / 2.0, (double) height / 2.0, 0.0);
    glScaled((double) width / 2.0, (double) height / 2.0, 1.0);

    // TODO use VBO
    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2d(0.0, 0.0);
        glVertex2d(-1.0, -1.0);

        glTexCoord2d(1.0, 0.0);
        glVertex2d(1.0, -1.0);

        glTexCoord2d(0.0, 1.0);
        glVertex2d(-1.0, 1.0);

        glTexCoord2d(1.0, 1.0);
        glVertex2d(1.0, 1.0);
    glEnd();

    glPopMatrix();
}
