#ifndef TESSERACT_H
#define TESSERACT_H

#include <stdlib.h> 
#include <glut.h>

class Tesseract {
public:
    GLuint texBookshelf;
    float roomSize;
    float nodeRadius;

    Tesseract();
    void init(GLuint bookTex);
    void draw();

private:
    void drawNode(float cx, float cy, float cz);
    void drawStrutX(float cx, float cy, float cz);
    void drawStrutY(float cx, float cy, float cz);
    void drawStrutZ(float cx, float cy, float cz);
};

#endif