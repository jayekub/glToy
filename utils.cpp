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
		throw "Unable to open file";
	}

	return contents;
}

void drawViewportQuad(int width, int height)
{
    glPushMatrix();

    glScaled((double) width, (double) height, 1.0);

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
