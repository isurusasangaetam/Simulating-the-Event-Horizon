#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <glut.h>

class Spaceship {
public:
    // Active Ship State (The Camera tracks this)
    float x, y, z;
    float velX, velY, velZ;
    float yaw, pitch;

    // Inactive Ship State (Where the other ship sits when undocked)
    float auxX, auxY, auxZ;
    float auxVelX, auxVelY, auxVelZ;
    float auxYaw, auxPitch;

    bool isDocked;
    bool spinEndurance;
    float endRingRotation;

    GLuint texSolar, texMetal;
    GLUquadric* quadric;

    Spaceship();

    // Pass textures from main.cpp into the ship
    void initTextures(GLuint solar, GLuint metal);

    void update(bool keys[256], bool specialKeys[256]);
    void draw();
    void toggleDocking();

private:
    // The high-fidelity geometry functions
    void drawTexturedBox(float w, float h, float d, GLuint texFront);
    void drawRanger();
    void drawEnduranceModule(int type);
    void drawEndurance();
};

#endif