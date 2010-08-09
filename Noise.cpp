#include <stdlib.h>

#include "Noise.h"

int GLSLNoise::_perm[256] = {
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,
    142,8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,
    203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,
    74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230
    ,220,105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,
    76,132,187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,
    173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,
    207,206,59,227,47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,
    154,163,70,221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,
    113,224,232,178,185,112,104,218,246,97,228,251,34,242,193,238,210,144,12,
    191,179,162,241,81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,
    157,184,84,204,176,115,121,50,45,127,4,150,254,138,236,205,93,222,114,67,
    29,24,72,243,141,128,195,78,66,215,61,156,180};

/* These are Ken Perlin's proposed gradients for 3D noise. I kept them for
   better consistency with the reference implementation, but there is really
   no need to pad this to 16 gradients for this particular implementation.
   If only the "proper" first 12 gradients are used, they can be extracted
   from the grad4[][] array: grad3[i][j] == grad4[i*2][j], 0<=i<=11, j=0,1,2
*/
int GLSLNoise::_grad3[16][3] =
    {{0,1,1},{0,1,-1},{0,-1,1},{0,-1,-1},
     {1,0,1},{1,0,-1},{-1,0,1},{-1,0,-1},
     {1,1,0},{1,-1,0},{-1,1,0},{-1,-1,0}, // 12 cube edges
     {1,0,-1},{-1,0,-1},{0,-1,1},{0,1,1}}; // 4 more to make 16

/* These are my own proposed gradients for 4D noise. They are the coordinates
   of the midpoints of each of the 32 edges of a tesseract, just like the 3D
   noise gradients are the midpoints of the 12 edges of a cube.
*/
int GLSLNoise::_grad4[32][4] =
    {{0,1,1,1}, {0,1,1,-1}, {0,1,-1,1}, {0,1,-1,-1}, // 32 tesseract edges
     {0,-1,1,1}, {0,-1,1,-1}, {0,-1,-1,1}, {0,-1,-1,-1},
     {1,0,1,1}, {1,0,1,-1}, {1,0,-1,1}, {1,0,-1,-1},
     {-1,0,1,1}, {-1,0,1,-1}, {-1,0,-1,1}, {-1,0,-1,-1},
     {1,1,0,1}, {1,1,0,-1}, {1,-1,0,1}, {1,-1,0,-1},
     {-1,1,0,1}, {-1,1,0,-1}, {-1,-1,0,1}, {-1,-1,0,-1},
     {1,1,1,0}, {1,1,-1,0}, {1,-1,1,0}, {1,-1,-1,0},
     {-1,1,1,0}, {-1,1,-1,0}, {-1,-1,1,0}, {-1,-1,-1,0}};

/*
 * initPermTexture(GLuint *texID) - create and load a 2D texture for
 * a combined index permutation and gradient lookup table.
 * This texture is used for 2D and 3D noise, both classic and simplex.
 */
// static
GLuint GLSLNoise::makePermutationTexture()
{
  GLuint texID;
  char *pixels;
  int i,j;

  glGenTextures(1, &texID); // Generate a unique texture ID
  glBindTexture(GL_TEXTURE_2D, texID); // Bind the texture to texture unit 0

  pixels = (char*)malloc( 256*256*4 );
  for(i = 0; i<256; i++)
    for(j = 0; j<256; j++) {
      int offset = (i*256+j)*4;
      char value = _perm[(j+_perm[i]) & 0xFF];
      pixels[offset] = _grad3[value & 0x0F][0] * 64 + 64;   // Gradient x
      pixels[offset+1] = _grad3[value & 0x0F][1] * 64 + 64; // Gradient y
      pixels[offset+2] = _grad3[value & 0x0F][2] * 64 + 64; // Gradient z
      pixels[offset+3] = value;                     // Permuted index
    }

  // GLFW texture loading functions won't work here - we need GL_NEAREST lookup.
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

  free(pixels);

  return texID;
}

/*
 * initGradTexture(GLuint *texID) - create and load a 2D texture
 * for a 4D gradient lookup table. This is used for 4D noise only.
 */
// static
GLuint GLSLNoise::makeGradientTexture()
{
  GLuint texID;
  char *pixels;
  int i,j;

  //glActiveTexture(GL_TEXTURE1); // Activate a different texture unit (unit 1)

  glGenTextures(1, &texID); // Generate a unique texture ID
  glBindTexture(GL_TEXTURE_2D, texID); // Bind the texture to texture unit 2

  pixels = (char*)malloc( 256*256*4 );
  for(i = 0; i<256; i++)
    for(j = 0; j<256; j++) {
      int offset = (i*256+j)*4;
      char value = _perm[(j+_perm[i]) & 0xFF];
      pixels[offset] = _grad4[value & 0x1F][0] * 64 + 64;   // Gradient x
      pixels[offset+1] = _grad4[value & 0x1F][1] * 64 + 64; // Gradient y
      pixels[offset+2] = _grad4[value & 0x1F][2] * 64 + 64; // Gradient z
      pixels[offset+3] = _grad4[value & 0x1F][3] * 64 + 64; // Gradient w
    }

  free(pixels);

  // GLFW texture loading functions won't work here - we need GL_NEAREST lookup.
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

  //glActiveTexture(GL_TEXTURE0); // Switch active texture unit back to 0 again

  return texID;
}

#define PERMUTATION \
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140,\
    36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120,\
    234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,\
    88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,\
    134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133,\
    230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161,\
    1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130,\
    116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250,\
    124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227,\
    47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44,\
    154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19,\
    98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,\
    251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235,\
    249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176,\
    115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 2

int ClassicNoise::_p[512] = { PERMUTATION, PERMUTATION };

#undef PERMUTATION
