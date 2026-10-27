#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H
#include <stdlib.h>
#include <glut.h>

class TextureLoader {
public:
    static GLuint loadTex(const char* filename);
};

#endif