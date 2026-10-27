#ifndef CAMERA_H
#define CAMERA_H

#include "Spaceship.h"

class Camera {
public:
    float relCamYaw;
    float relCamPitch;

    // NEW: The camera now remembers its own mouse offsets!
    float mouseYawOffset = 0.0f;
    float mousePitch = 0.4f;

    void applyView(const Spaceship& ship);
};

#endif