#include "Spaceship.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ==========================================
// SCALE CONSTANT (Cinematic Black Hole effect)
// ==========================================
const float SCALE = 0.15f; // Shrinks the ship to make Gargantua look colossal

Spaceship::Spaceship() {
    x = 0.0f; y = 1.0f; z = 120.0f;
    velX = 0.0f; velY = 0.0f; velZ = 0.0f;
    yaw = 0.0f; pitch = 0.0f;

    auxX = 0.0f; auxY = 0.0f; auxZ = 0.0f;
    auxVelX = 0.0f; auxVelY = 0.0f; auxVelZ = 0.0f;
    auxYaw = 0.0f; auxPitch = 0.0f;

    isDocked = true;
    spinEndurance = false;
    endRingRotation = 0.0f;
}

void Spaceship::initTextures(GLuint solar, GLuint metal) {
    texSolar = solar;
    texMetal = metal;
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);
}

void Spaceship::update(bool keys[256], bool specialKeys[256]) {
    if (spinEndurance) {
        endRingRotation += 0.25f;
        if (endRingRotation > 360.0f) endRingRotation -= 360.0f;
    }

    // 1. ROTATIONAL STEERING FIX
    float turnSpeed = isDocked ? 0.008f : 0.03f;

    // A turns Left (increases yaw), D turns Right (decreases yaw)
    if (keys['a'] || keys['A']) { yaw += turnSpeed; }
    if (keys['d'] || keys['D']) { yaw -= turnSpeed; }

    // 2. FORWARD/BACKWARD THRUST
    float fwdX = -sin(yaw) * cos(pitch);
    float fwdY = sin(pitch);
    float fwdZ = -cos(yaw) * cos(pitch);

    float thrust = isDocked ? 0.008f : 0.02f;

    // W pushes you directly along the nose, S pushes you in reverse
    if (keys['w'] || keys['W']) { velX += fwdX * thrust; velY += fwdY * thrust; velZ += fwdZ * thrust; }
    if (keys['s'] || keys['S']) { velX -= fwdX * thrust; velY -= fwdY * thrust; velZ -= fwdZ * thrust; }

    // Space/C for Vertical Hover
    if (keys[' '] || specialKeys[GLUT_ACTIVE_SHIFT]) { velY += thrust; }
    if (keys['c'] || keys['C'] || specialKeys[GLUT_ACTIVE_CTRL]) { velY -= thrust; }

    // Space Friction
    velX *= 0.92f; velY *= 0.92f; velZ *= 0.92f;

    // Move Active Ship
    x += velX; y += velY; z += velZ;

    if (!isDocked) {
        auxX += auxVelX; auxY += auxVelY; auxZ += auxVelZ;
    }

    // NEW: We multiplied the 4.6f boundary by 10 to match the scaled Black Hole!
    float distToHole = sqrt(x * x + y * y + z * z);
    if (distToHole < 46.0f) {
        float pushFactor = 46.0f / distToHole;
        x *= pushFactor; y *= pushFactor; z *= pushFactor;
    }
}

void Spaceship::draw() {
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);

    if (isDocked) {
        // Draw Active Endurance
        glTranslatef(x, y, z);

        // THE FIX: Removed the negative sign from 'yaw'. 
        // Now the 3D model's nose perfectly matches the math vector!
        glRotatef(yaw * (180.0f / M_PI), 0.0f, 1.0f, 0.0f);
        glRotatef(pitch * (180.0f / M_PI), 1.0f, 0.0f, 0.0f);

        glScalef(SCALE, SCALE, SCALE);

        drawEndurance();

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 3.2f);
        drawRanger();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -3.2f);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        drawRanger();
        glPopMatrix();
    }
    else {
        // Draw Inactive Endurance
        glPushMatrix();
        glTranslatef(auxX, auxY, auxZ);

        // THE FIX: Removed negative sign from auxYaw
        glRotatef(auxYaw * (180.0f / M_PI), 0.0f, 1.0f, 0.0f);
        glRotatef(auxPitch * (180.0f / M_PI), 1.0f, 0.0f, 0.0f);
        glScalef(SCALE, SCALE, SCALE);

        drawEndurance();
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 3.2f);
        drawRanger();
        glPopMatrix();
        glPopMatrix();

        // Draw Active Ranger
        glPushMatrix();
        glTranslatef(x, y, z);

        // THE FIX: Removed negative sign from yaw
        glRotatef(yaw * (180.0f / M_PI), 0.0f, 1.0f, 0.0f);
        glRotatef(pitch * (180.0f / M_PI), 1.0f, 0.0f, 0.0f);
        glScalef(SCALE, SCALE, SCALE);

        drawRanger();
        glPopMatrix();
    }

    glPopMatrix();
}

void Spaceship::toggleDocking() {
    if (isDocked) {
        // EJECT
        isDocked = false;

        // Save Endurance to Background (Aux)
        auxX = x; auxY = y; auxZ = z;
        auxYaw = yaw; auxPitch = pitch;
        auxVelX = velX; auxVelY = velY; auxVelZ = velZ;

        float fwdX = -sin(yaw) * cos(pitch);
        float fwdY = sin(pitch);
        float fwdZ = -cos(yaw) * cos(pitch);

        // Spawn Ranger slightly ahead of the front port
        x = auxX + (fwdX * 4.5f * SCALE);
        y = auxY + (fwdY * 4.5f * SCALE);
        z = auxZ + (fwdZ * 4.5f * SCALE);

        // Flip camera 180 to face the Endurance
        yaw = auxYaw + M_PI;
        pitch = -auxPitch;

        // Push Ranger away
        velX = auxVelX + (fwdX * 0.05f);
        velY = auxVelY + (fwdY * 0.05f);
        velZ = auxVelZ + (fwdZ * 0.05f);
    }
    else {
        // DOCK
        float dist = sqrt(pow(x - auxX, 2) + pow(y - auxY, 2) + pow(z - auxZ, 2));
        if (dist < 15.0f * SCALE) { // Scaled docking tolerance
            isDocked = true;
            x = auxX; y = auxY; z = auxZ;
            yaw = auxYaw; pitch = auxPitch;
            velX = auxVelX; velY = auxVelY; velZ = auxVelZ;
        }
    }
}


// ========================================================
// GEOMETRY FUNCTIONS (Bound to the Spaceship class)
// ========================================================
void Spaceship::drawTexturedBox(float w, float h, float d, GLuint texFront) {
    glEnable(GL_TEXTURE_2D);
    float tile = 2.0f;
    glColor3f(1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, texFront);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0, 0);       glVertex3f(-w, -h, d);
    glTexCoord2f(tile, 0);    glVertex3f(w, -h, d);
    glTexCoord2f(tile, tile); glVertex3f(w, h, d);
    glTexCoord2f(0, tile);    glVertex3f(-w, h, d);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, texMetal);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0, 0);       glVertex3f(w, -h, -d);
    glTexCoord2f(tile, 0);    glVertex3f(-w, -h, -d);
    glTexCoord2f(tile, tile); glVertex3f(-w, h, -d);
    glTexCoord2f(0, tile);    glVertex3f(w, h, -d);

    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0, 0);       glVertex3f(-w, h, d);
    glTexCoord2f(tile, 0);    glVertex3f(w, h, d);
    glTexCoord2f(tile, tile); glVertex3f(w, h, -d);
    glTexCoord2f(0, tile);    glVertex3f(-w, h, -d);

    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0, 0);       glVertex3f(-w, -h, -d);
    glTexCoord2f(tile, 0);    glVertex3f(w, -h, -d);
    glTexCoord2f(tile, tile); glVertex3f(w, -h, d);
    glTexCoord2f(0, tile);    glVertex3f(-w, -h, d);

    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0, 0);       glVertex3f(w, -h, d);
    glTexCoord2f(tile, 0);    glVertex3f(w, -h, -d);
    glTexCoord2f(tile, tile); glVertex3f(w, h, -d);
    glTexCoord2f(0, tile);    glVertex3f(w, h, d);

    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0, 0);       glVertex3f(-w, -h, -d);
    glTexCoord2f(tile, 0);    glVertex3f(-w, -h, d);
    glTexCoord2f(tile, tile); glVertex3f(-w, h, d);
    glTexCoord2f(0, tile);    glVertex3f(-w, h, -d);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void Spaceship::drawRanger() {
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMetal);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, 0.0f, -2.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.8f, 0.0f, 1.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.8f, 0.0f, 1.5f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.0f, 0.25f, -0.2f);
    drawTexturedBox(0.35f, 0.2f, 0.9f, texMetal);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.1f, 1.2f);
    drawTexturedBox(0.7f, 0.25f, 0.4f, texMetal);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void Spaceship::drawEnduranceModule(int type) {
    GLuint frontTex = (type == 0 || type == 1) ? texSolar : texMetal;
    drawTexturedBox(1.6f, 1.2f, 1.5f, frontTex);

    glPushMatrix();
    glTranslatef(0.0f, 1.25f, 0.0f);
    drawTexturedBox(1.7f, 0.1f, 1.4f, texMetal);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -1.25f, 0.0f);
    drawTexturedBox(1.7f, 0.1f, 1.4f, texMetal);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.65f, 0.0f, 0.0f);
    drawTexturedBox(0.1f, 1.0f, 1.0f, texMetal);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.65f, 0.0f, 0.0f);
    drawTexturedBox(0.1f, 1.0f, 1.0f, texMetal);
    glPopMatrix();

    if (type == 2) {
        glColor3f(0.3f, 0.3f, 0.3f);
        glPushMatrix();
        glTranslatef(0.0f, -1.25f, 0.0f);
        drawTexturedBox(1.3f, 0.1f, 0.8f, texMetal);
        glPopMatrix();

        glColor3f(0.8f, 0.8f, 0.8f);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texMetal);

        float nRadius = 0.45f;
        glPushMatrix();
        glTranslatef(0.0f, -1.35f, 0.0f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

        glPushMatrix();
        glTranslatef(0.0f, 0.4f, 0.0f);
        gluCylinder(quadric, nRadius, nRadius * 1.3f, 0.8f, 16, 4);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.65f, -0.3f, 0.0f);
        gluCylinder(quadric, nRadius, nRadius * 1.3f, 0.8f, 16, 4);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0.65f, -0.3f, 0.0f);
        gluCylinder(quadric, nRadius, nRadius * 1.3f, 0.8f, 16, 4);
        glPopMatrix();

        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
}

void Spaceship::drawEndurance() {
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMetal);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -5.0f);
    gluCylinder(quadric, 0.8f, 0.8f, 10.0f, 24, 10);
    glPopMatrix();

    for (float y = -4.0f; y <= 4.0f; y += 2.0f) {
        glPushMatrix();
        glTranslatef(0.0f, y, 0.0f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        gluCylinder(quadric, 0.85f, 0.85f, 0.5f, 24, 2);
        glPopMatrix();
    }

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glRotatef(i * 90.0f, 0.0f, 1.0f, 0.0f);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.8f);
        gluCylinder(quadric, 0.6f, 0.6f, 2.0f, 16, 4);
        glPopMatrix();
        glColor3f(0.4f, 0.4f, 0.4f);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 2.8f);
        gluCylinder(quadric, 0.65f, 0.65f, 0.4f, 16, 4);
        glPopMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        glPopMatrix();
    }

    glPushMatrix();
    glRotatef(endRingRotation, 0.0f, 0.0f, 1.0f);
    float ringRadius = 14.0f;

    for (int s = 0; s < 2; s++) {
        glPushMatrix();
        glRotatef(s * 180.0f, 0.0f, 0.0f, 1.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        gluCylinder(quadric, 0.8f, 0.8f, ringRadius, 16, 4);
        glPopMatrix();
    }

    for (int i = 0; i < 12; i++) {
        float angle = i * (360.0f / 12.0f);
        glPushMatrix();
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texMetal);
        glColor3f(1.0f, 1.0f, 1.0f);

        glPushMatrix();
        glTranslatef(0.0f, ringRadius, 0.0f);
        glRotatef(-15.0f, 0.0f, 0.0f, 1.0f);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        float chordLength = 2.0f * ringRadius * sin(15.0f * M_PI / 180.0f);
        gluCylinder(quadric, 0.5f, 0.5f, chordLength, 12, 4);
        glPopMatrix();

        glTranslatef(0.0f, ringRadius, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

        int type = 0;
        if (i == 0 || i == 4 || i == 8) type = 2;
        else if (i == 2 || i == 10) type = 1;

        drawEnduranceModule(type);
        glPopMatrix();
    }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}