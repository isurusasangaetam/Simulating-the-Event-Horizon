#include "Camera.h"
#include <math.h>
#include <glut.h>

void Camera::applyView(const Spaceship& ship) {
    // Zoom out for the massive Endurance, zoom in for the Ranger
    float CHASE_DISTANCE = ship.isDocked ? 16.0f : 3.0f;
    float CHASE_HEIGHT = ship.isDocked ? 4.0f : 0.8f;

    // THE FIX: Combine the ship's actual rotation with your mouse movement
    // This forces the camera to act as a 3rd-person follow-cam when you steer.
    float totalYaw = ship.yaw + mouseYawOffset;

    // Calculate camera position in an orbit BEHIND the ship
    float camX = ship.x + sin(totalYaw) * cos(mousePitch) * CHASE_DISTANCE;
    float camY = ship.y + sin(mousePitch) * CHASE_DISTANCE + CHASE_HEIGHT;
    float camZ = ship.z + cos(totalYaw) * cos(mousePitch) * CHASE_DISTANCE;

    // The camera looks directly at a point slightly ahead of the ship's nose
    float lookTargetX = ship.x - sin(ship.yaw) * 2.0f;
    float lookTargetY = ship.y + sin(ship.pitch) * 2.0f;
    float lookTargetZ = ship.z - cos(ship.yaw) * 2.0f;

    gluLookAt(camX, camY, camZ,
        lookTargetX, lookTargetY, lookTargetZ,
        0.0f, 1.0f, 0.0f);

    // ==========================================
    // THE BLACK HOLE FIX
    // Ensures the 2D Black Hole always perfectly faces the camera
    float relDistXZ = sqrt(camX * camX + camZ * camZ);
    relCamYaw = atan2(camX, camZ);
    relCamPitch = atan2(camY, (relDistXZ > 0.001f ? relDistXZ : 0.001f));
}