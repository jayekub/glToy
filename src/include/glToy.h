#ifndef GLTOY_H_
#define GLTOY_H_

#include <sys/types.h>

#ifdef _WIN32
#  define USE_GL3W
#endif

#ifdef USE_GL3W
#  include <GL3/gl3w.h>
#else
// use GL3 unless we're on osx
#  ifndef __APPLE__
#    define GL3_PROTOTYPES
#    include <GL3/gl3.h>
#  endif
#endif

#ifdef __APPLE__
// prevents gl.h from making conflicting gl function declarations
//#  define GL_GLEXT_FUNCTION_POINTERS
#  include <GLUT/glut.h>
#else
// prevent glut.h from including glext.h, which conflicts with gl3.h
#  define GL_GLEXT_LEGACY
#  include <GL/glut.h>
#endif

#define BUFFER_OFFSET(i) ((char *) NULL + (i))

#include <stdio.h>
#define PRINT_GLERROR(pfx) \
    fprintf(stderr, "%s: %s\n", pfx, gluErrorString(glGetError()))

#endif /* GLTOY_H_ */
