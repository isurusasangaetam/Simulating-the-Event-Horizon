#include "Environment.h"
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Environment::Environment() {}

// --- THE PROCEDURAL GALAXY MATH ---
void Environment::buildProceduralGalaxy(GalaxyData& g) {
    int numArms = 2; // Classic spiral galaxy
    int particlesPerGalaxy = 600; // Complexity!

    for (int p = 0; p < particlesPerGalaxy; ++p) {
        GalaxyParticle particle;

        float t = (float)p / particlesPerGalaxy;
        float angle = t * 12.0f;
        float armOffset = (p % numArms) * M_PI;

        // FIXED: Restored missing subtraction operators to center the random scatter
        float scatterX = (((float)rand() / RAND_MAX) - 0.5f) * 0.8f;
        float scatterZ = (((float)rand() / RAND_MAX) - 0.5f) * 0.8f;

        float radius = t * 2.0f;

        particle.x = (radius * cos(angle + armOffset)) + scatterX;
        // FIXED: Restored subtraction to keep the Y-axis tight and form a flat disk
        particle.y = (((float)rand() / RAND_MAX) - 0.5f) * 0.15f * (1.0f - t);
        particle.z = (radius * sin(angle + armOffset)) + scatterZ;

        // Procedural Color Grading
        if (t < 0.3f) {
            particle.r = 1.0f; particle.g = 0.9f; particle.b = 0.6f;
        }
        else if (t < 0.7f) {
            particle.r = 0.5f; particle.g = 0.4f; particle.b = 0.9f;
        }
        else {
            particle.r = 0.1f; particle.g = 0.3f; particle.b = 0.8f;
        }

        g.particles.push_back(particle);
    }
}

void Environment::init() {
    // 1. Generate 500 distant stars
    for (int i = 0; i < 500; i++) {
        StarData s;
        // Pushed stars deep into the background (2000 to 5000 units away)
        float radius = 2000.0f + (rand() % 3000);
        float theta = (rand() % 360) * (M_PI / 180.0f);
        float phi = (rand() % 180) * (M_PI / 180.0f);

        s.x = radius * sin(phi) * cos(theta);
        s.y = radius * cos(phi);
        s.z = radius * sin(phi) * sin(theta);

        // Scaled up so they remain visible from immense distances
        s.scale = 4.0f + (rand() % 100) / 10.0f;

        int colorType = rand() % 3;
        if (colorType == 0) { s.r = 0.8f; s.g = 0.9f; s.b = 1.0f; }
        else if (colorType == 1) { s.r = 1.0f; s.g = 1.0f; s.b = 1.0f; }
        else { s.r = 1.0f; s.g = 0.8f; s.b = 0.6f; }

        stars.push_back(s);
    }

    // 2. Generate 10 Procedural Galaxies
    for (int i = 0; i < 10; i++) {
        GalaxyData g;
        // Pushed galaxies even further into the void
        float radius = 3000.0f + (rand() % 4000);
        float theta = (rand() % 360) * (M_PI / 180.0f);
        float phi = (rand() % 180) * (M_PI / 180.0f);

        g.x = radius * sin(phi) * cos(theta);
        g.y = radius * cos(phi);
        g.z = radius * sin(phi) * sin(theta);

        g.rotX = (rand() % 360);
        g.rotY = (rand() % 360);
        g.rotZ = (rand() % 360);

        // Scaled galaxies up significantly to match the new background distance
        g.scale = 150.0f + (rand() % 200);

        buildProceduralGalaxy(g);
        galaxies.push_back(g);
    }
}

void Environment::drawSingleStarTemplate() {
    glutSolidSphere(1.0, 8, 8);
}

void Environment::draw() {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    // 1. Stamp out the distant stars
    for (size_t i = 0; i < stars.size(); i++) {
        glPushMatrix();
        glTranslatef(stars[i].x, stars[i].y, stars[i].z);
        glScalef(stars[i].scale, stars[i].scale, stars[i].scale);
        glColor3f(stars[i].r, stars[i].g, stars[i].b);
        drawSingleStarTemplate();
        glPopMatrix();
    }

    // 2. Stamp out the procedural galaxies
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);

    glEnable(GL_POINT_SMOOTH);
    glPointSize(3.0f);

    for (size_t i = 0; i < galaxies.size(); i++) {
        glPushMatrix();
        glTranslatef(galaxies[i].x, galaxies[i].y, galaxies[i].z);
        glRotatef(galaxies[i].rotX, 1.0f, 0.0f, 0.0f);
        glRotatef(galaxies[i].rotY, 0.0f, 1.0f, 0.0f);
        glRotatef(galaxies[i].rotZ, 0.0f, 0.0f, 1.0f);
        glScalef(galaxies[i].scale, galaxies[i].scale, galaxies[i].scale);

        glBegin(GL_POINTS);
        for (size_t p = 0; p < galaxies[i].particles.size(); p++) {
            glColor3f(galaxies[i].particles[p].r, galaxies[i].particles[p].g, galaxies[i].particles[p].b);
            glVertex3f(galaxies[i].particles[p].x, galaxies[i].particles[p].y, galaxies[i].particles[p].z);
        }
        glEnd();

        glPopMatrix();
    }

    glDisable(GL_POINT_SMOOTH);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}