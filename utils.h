#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>

#include <stdlib.h>

#include "glToy.h"

#define max(a, b) ((a) > (b) ? (a) : (b))
#define fwrap(x, m) fmod(fmod((x), (m)) + (m), (m))

#define randFloat() ((float) random() / (float) RAND_MAX)

std::string readFile(const std::string &filename);
void drawViewportQuad(int width, int height);

#endif /* _UTILS_H_ */
