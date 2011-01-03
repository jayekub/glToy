#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>

#include <stdlib.h>

#include "glToy.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CLAMP(x, min, max) MIN(MAX((x), (min)), (max))

#define BUFFER_OFFSET(i) ((char *) NULL + (i))

#define fwrap(x, m) fmod(fmod((x), (m)) + (m), (m))

#define randFloat() ((float) rand() / (float) RAND_MAX)
#define btoa(b) (b ? "true" : "false")

std::string readFile(const std::string &filename);
GLuint makeTestTexture(int size);

std::vector<float> makeBlurWeights(int blurDiameter);

#endif /* _UTILS_H_ */
