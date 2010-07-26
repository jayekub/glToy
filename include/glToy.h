#ifndef GLTOY_H_
#define GLTOY_H_

#include <sys/types.h>

#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define BUFFER_OFFSET(i) ((char *) NULL + (i))

#endif /* GLTOY_H_ */
