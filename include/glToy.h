#ifndef GLTOY_H_
#define GLTOY_H_

#ifdef __CYGWIN__
#  define _STDCALL_SUPPORTED
#  define _M_IX86
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>

#  undef min
#  undef max
#endif

//#include <sys/types.h>

#define GL3_PROTOTYPES
#include <GL3/gl3.h>

//#include <GL/glew.h>

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
