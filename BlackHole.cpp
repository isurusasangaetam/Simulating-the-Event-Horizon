#include "BlackHole.h"
#include <math.h>
#include <glut.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

BlackHole::BlackHole() : textureID(0), uvOffset(0.0f) {}

void BlackHole::setTexture(GLuint tex) { textureID = tex; }

void BlackHole::updateAnimation() {
    uvOffset -= 0.01f;
    if (uvOffset < -1.0f) uvOffset += 1.0f;
}

void BlackHole::drawSphere() {
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glutSolidSphere(4.0, 70, 70);
    glPopMatrix();
}

void BlackHole::drawVerticalHalo(float innerR, float outerR, float relCamYaw, float relCamPitch) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPushMatrix();
    // FIXED: Restored the missing multiplication (*) operators
    glRotatef(relCamYaw * (180.0f / M_PI), 0.0f, 1.0f, 0.0f);
    glRotatef(-relCamPitch * (180.0f / M_PI), 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -3.8f);

    glBegin(GL_QUAD_STRIP);
    int segments = 80;
    for (int i = 0; i <= segments; ++i) {
        // FIXED: Restored the missing multiplication (*) operators
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float c = cos(theta);
        float s = sin(theta);
        float u = (float(i) / float(segments)) * 3.0f + uvOffset;

        glTexCoord2f(u, 0.5f); glVertex3f(innerR * c, innerR * s, 0.0f);
        glTexCoord2f(u, 1.0f); glVertex3f(outerR * c, outerR * s, 0.0f);
    }
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void BlackHole::drawWarpedDisk(float innerR, float outerR, bool bendUpwards, float relCamYaw) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glColor3f(1.0f, 1.0f, 1.0f);

    float dirX = sin(relCamYaw);
    float dirZ = cos(relCamYaw);
    int radialSegments = 30;
    int angularSegments = 120;
    float haloThicknessFactor = 0.25f;

    for (int r = 0; r < radialSegments; ++r) {
        // FIXED: Restored the missing minus (-) signs in (outerR - innerR)
        float baseInnerR = innerR + (outerR - innerR) * ((float)r / radialSegments);
        float baseOuterR = innerR + (outerR - innerR) * ((float)(r + 1) / radialSegments);
        float v_in = 0.5f + 0.5f * ((float)r / radialSegments);
        float v_out = 0.5f + 0.5f * ((float)(r + 1) / radialSegments);

        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= angularSegments; ++i) {
            // FIXED: Restored the missing multiplication (*) operators
            float theta = 2.0f * M_PI * float(i) / float(angularSegments);
            float base_x_in = baseInnerR * cos(theta);
            float base_z_in = baseInnerR * sin(theta);
            float base_depth_in = -(base_x_in * dirX + base_z_in * dirZ);

            float base_x_out = baseOuterR * cos(theta);
            float base_z_out = baseOuterR * sin(theta);
            float base_depth_out = -(base_x_out * dirX + base_z_out * dirZ);

            float currentInnerR = baseInnerR;
            float currentOuterR = baseOuterR;

            if (base_depth_in > 0.0f) {
                float bend = base_depth_in / innerR;
                if (bend > 1.0f) bend = 1.0f;
                bend = bend * bend * (3.0f - 2.0f * bend);
                float thickness = baseInnerR - innerR;
                currentInnerR = innerR + thickness * (1.0f - bend) + (thickness * haloThicknessFactor) * bend;
            }
            if (base_depth_out > 0.0f) {
                float bend = base_depth_out / innerR;
                if (bend > 1.0f) bend = 1.0f;
                bend = bend * bend * (3.0f - 2.0f * bend);
                float thickness = baseOuterR - innerR;
                currentOuterR = innerR + thickness * (1.0f - bend) + (thickness * haloThicknessFactor) * bend;
            }

            float x_in = currentInnerR * cos(theta);
            float z_in = currentInnerR * sin(theta);
            float depth_in = -(x_in * dirX + z_in * dirZ);
            float u_in = x_in * dirZ - z_in * dirX;

            float x_out = currentOuterR * cos(theta);
            float z_out = currentOuterR * sin(theta);
            float depth_out = -(x_out * dirX + z_out * dirZ);
            float u_out = x_out * dirZ - z_out * dirX;

            float y_in = 0.0f, final_x_in = x_in, final_z_in = z_in;
            if (depth_in > 0.0f) {
                float fold = depth_in / innerR;
                if (fold > 1.0f) fold = 1.0f;
                fold = fold * fold * (3.0f - 2.0f * fold);
                y_in = depth_in * fold;
                float new_depth = depth_in * (1.0f - fold) - 0.05f * fold;
                final_x_in = u_in * dirZ - new_depth * dirX;
                final_z_in = -u_in * dirX - new_depth * dirZ;
            }

            float y_out = 0.0f, final_x_out = x_out, final_z_out = z_out;
            if (depth_out > 0.0f) {
                float fold = depth_out / innerR;
                if (fold > 1.0f) fold = 1.0f;
                fold = fold * fold * (3.0f - 2.0f * fold);
                y_out = depth_out * fold;
                float new_depth = depth_out * (1.0f - fold) - 0.05f * fold;
                final_x_out = u_out * dirZ - new_depth * dirX;
                final_z_out = -u_out * dirX - new_depth * dirZ;
            }

            if (!bendUpwards) { y_in = -y_in; y_out = -y_out; }
            float u_tex = (float(i) / float(angularSegments)) * 6.0f + uvOffset;

            glTexCoord2f(u_tex, v_in); glVertex3f(final_x_in, y_in, final_z_in);
            glTexCoord2f(u_tex, v_out); glVertex3f(final_x_out, y_out, final_z_out);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
}

void BlackHole::draw(float relCamYaw, float relCamPitch) {
    glPushMatrix();

    // ==========================================
    // THE SCALE FIX
    // Makes the Black Hole 10x larger than before!
    // ==========================================
    glScalef(10.0f, 10.0f, 10.0f);

    drawSphere();
    drawVerticalHalo(4.05f, 6.0f, relCamYaw, relCamPitch);
    drawWarpedDisk(4.0f, 11.0f, true, relCamYaw);
    drawWarpedDisk(4.0f, 11.0f, false, relCamYaw);

    glPopMatrix();
}