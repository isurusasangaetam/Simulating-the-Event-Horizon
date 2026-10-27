#ifndef BLACKHOLE_H
#define BLACKHOLE_H
#include <stdlib.h>
#include <glut.h>

class BlackHole {
private:
    GLuint textureID;
    float uvOffset;
    void drawSphere();
    void drawVerticalHalo(float innerR, float outerR, float relCamYaw, float relCamPitch);
    void drawWarpedDisk(float innerR, float outerR, bool bendUpwards, float relCamYaw);

public:
    BlackHole();
    void setTexture(GLuint tex);
    void updateAnimation();
    void draw(float relCamYaw, float relCamPitch);
};

#endif