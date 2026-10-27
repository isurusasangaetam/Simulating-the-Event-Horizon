#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <stdlib.h>
#include <glut.h>
#include <vector>

// Mathematical point inside a galaxy
struct GalaxyParticle {
    float x, y, z;
    float r, g, b; // Color of this specific particle
};

struct GalaxyData {
    float x, y, z;
    float rotX, rotY, rotZ;
    float scale;
    std::vector<GalaxyParticle> particles; // Stores the procedural spiral shape
};

struct StarData {
    float x, y, z;
    float scale;
    float r, g, b;
};

class Environment {
private:
    std::vector<GalaxyData> galaxies;
    std::vector<StarData> stars;

    void drawSingleStarTemplate();
    void buildProceduralGalaxy(GalaxyData& g); // Math function to generate the spiral

public:
    Environment();
    // Removed the texture parameter entirely!
    void init();
    void draw();
};

#endif
