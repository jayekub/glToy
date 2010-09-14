#ifndef GLTOY_H_
#define GLTOY_H_

#include <GL3/gl3w.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#endif

#define BUFFER_OFFSET(i) ((char *) NULL + (i))

#include <stdio.h>
#define PRINT_GLERROR(pfx) \
    fprintf(stderr, "%s: %s\n", pfx, gluErrorString(glGetError()))

#endif /* GLTOY_H_ */