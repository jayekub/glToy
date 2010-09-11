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

GLuint makeTestTexture(int size)
{
    int size_2 = size / 2;
    GLubyte tex[size][size][3];

    // red, green, blue, and white quadrants
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            if (x < size_2) {
                // upper
                if (y < size_2) {
                    // left
                    tex[x][y][0] = 255;
                    tex[x][y][1] = 0;
                    tex[x][y][2] = 0;
                } else {
                    // right
                    tex[x][y][0] = 0;
                    tex[x][y][1] = 255;
                    tex[x][y][2] = 0;
                }
            } else {
                // lower
                if (y < size_2) {
                    // left
                    tex[x][y][0] = 0;
                    tex[x][y][1] = 0;
                    tex[x][y][2] = 255;
                } else {
                    // right
                    tex[x][y][0] = 255;
                    tex[x][y][1] = 255;
                    tex[x][y][2] = 255;
                }
            }
        }
    }

    // create texture and read in data
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // XXX gl3
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_WRAP_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_WRAP_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, tex);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

/*
void drawViewportQuad(int width, int height)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslated((double) width / 2.0, (double) height / 2.0, 0.0);
    glScaled((double) width / 2.0, (double) height / 2.0, 1.0);

    glBegin(GL_QUADS);
        // ul
        //glColor3f(1., 0., 0.);
        glTexCoord2f(0., 1.);
        glVertex2f(-1., 1.);

        // ur
        //glColor3f(0., 1., 0.);
        glTexCoord2f(1., 1.);
        glVertex2f(1., 1.);

        // lr
        //glColor3f(1., 1., 1.);
        glTexCoord2f(1., 0.);
        glVertex2f(1., -1.);

        // ll
        //glColor3f(0., 0., 1.);
        glTexCoord2f(0., 0.);
        glVertex2f(-1., -1.);
    glEnd();

    glPopMatrix();
}
*/
