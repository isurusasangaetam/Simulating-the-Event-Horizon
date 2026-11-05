#include "Tesseract.h"
#include <math.h>

Tesseract::Tesseract() {
    roomSize = 200.0f;
    nodeRadius = 30.0f; // This makes the tunnels 60x60
}

void Tesseract::init(GLuint bookTex) {
    texBookshelf = bookTex;
}

// ========================================================
// THE X-AXIS TUNNELS (East/West)
// ========================================================
void Tesseract::drawStrutX(float cx, float cy, float cz) {
    float r = nodeRadius;
    float start = cx + r;
    float mid1 = cx + (roomSize / 2.0f) - r; // Start of bookshelf cube
    float mid2 = cx + (roomSize / 2.0f) + r; // End of bookshelf cube
    float end = cx + roomSize - r;

    // Draw the 4 walls of the tunnel (Top, Bottom, Front, Back)
    glBegin(GL_QUADS);

    // --- TOP WALL (Y = cy + r) ---
    glColor3f(0.4f, 0.4f, 0.4f); // Darker color for stretched time
    glTexCoord2f(0.0f, 0.50f); glVertex3f(start, cy + r, cz + r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(start, cy + r, cz - r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(mid1, cy + r, cz - r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(mid1, cy + r, cz + r);

    glColor3f(1.0f, 1.0f, 1.0f); // Bright glow for the Bookshelf Cube
    glTexCoord2f(0.0f, 0.0f); glVertex3f(mid1, cy + r, cz + r);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(mid1, cy + r, cz - r);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(mid2, cy + r, cz - r);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(mid2, cy + r, cz + r);

    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(mid2, cy + r, cz + r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(mid2, cy + r, cz - r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(end, cy + r, cz - r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(end, cy + r, cz + r);

    // --- BOTTOM WALL (Y = cy - r) ---
    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(start, cy - r, cz - r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(start, cy - r, cz + r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(mid1, cy - r, cz + r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(mid1, cy - r, cz - r);

    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(mid1, cy - r, cz - r);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(mid1, cy - r, cz + r);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(mid2, cy - r, cz + r);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(mid2, cy - r, cz - r);

    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(mid2, cy - r, cz - r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(mid2, cy - r, cz + r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(end, cy - r, cz + r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(end, cy - r, cz - r);

    // --- FRONT WALL (Z = cz + r) ---
    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(start, cy - r, cz + r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(start, cy + r, cz + r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(mid1, cy + r, cz + r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(mid1, cy - r, cz + r);

    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(mid1, cy - r, cz + r);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(mid1, cy + r, cz + r);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(mid2, cy + r, cz + r);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(mid2, cy - r, cz + r);

    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(mid2, cy - r, cz + r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(mid2, cy + r, cz + r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(end, cy + r, cz + r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(end, cy - r, cz + r);

    // --- BACK WALL (Z = cz - r) ---
    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(start, cy + r, cz - r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(start, cy - r, cz - r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(mid1, cy - r, cz - r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(mid1, cy + r, cz - r);

    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(mid1, cy + r, cz - r);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(mid1, cy - r, cz - r);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(mid2, cy - r, cz - r);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(mid2, cy + r, cz - r);

    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(mid2, cy + r, cz - r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(mid2, cy - r, cz - r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(end, cy - r, cz - r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(end, cy + r, cz - r);

    glEnd();
}

// ========================================================
// THE Y-AXIS TUNNELS (Up/Down)
// ========================================================
void Tesseract::drawStrutY(float cx, float cy, float cz) {
    float r = nodeRadius;
    float start = cy + r;
    float mid1 = cy + (roomSize / 2.0f) - r;
    float mid2 = cy + (roomSize / 2.0f) + r;
    float end = cy + roomSize - r;

    glBegin(GL_QUADS);
    // FRONT WALL
    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx - r, start, cz + r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx + r, start, cz + r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx + r, mid1, cz + r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx - r, mid1, cz + r);

    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(cx - r, mid1, cz + r);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(cx + r, mid1, cz + r);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(cx + r, mid2, cz + r);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(cx - r, mid2, cz + r);

    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx - r, mid2, cz + r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx + r, mid2, cz + r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx + r, end, cz + r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx - r, end, cz + r);

    // BACK WALL
    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx + r, start, cz - r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx - r, start, cz - r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx - r, mid1, cz - r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx + r, mid1, cz - r);

    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(cx + r, mid1, cz - r);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(cx - r, mid1, cz - r);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(cx - r, mid2, cz - r);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(cx + r, mid2, cz - r);

    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx + r, mid2, cz - r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx - r, mid2, cz - r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx - r, end, cz - r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx + r, end, cz - r);

    // LEFT WALL
    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx - r, start, cz - r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx - r, start, cz + r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx - r, mid1, cz + r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx - r, mid1, cz - r);

    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(cx - r, mid1, cz - r);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(cx - r, mid1, cz + r);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(cx - r, mid2, cz + r);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(cx - r, mid2, cz - r);

    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx - r, mid2, cz - r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx - r, mid2, cz + r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx - r, end, cz + r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx - r, end, cz - r);

    // RIGHT WALL
    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx + r, start, cz + r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx + r, start, cz - r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx + r, mid1, cz - r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx + r, mid1, cz + r);

    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(cx + r, mid1, cz + r);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(cx + r, mid1, cz - r);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(cx + r, mid2, cz - r);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(cx + r, mid2, cz + r);

    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx + r, mid2, cz + r);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx + r, mid2, cz - r);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx + r, end, cz - r);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx + r, end, cz + r);
    glEnd();
}

// ========================================================
// THE Z-AXIS TUNNELS (Forward/Backward)
// ========================================================
void Tesseract::drawStrutZ(float cx, float cy, float cz) {
    float r = nodeRadius;
    float start = cz + r;
    float mid1 = cz + (roomSize / 2.0f) - r;
    float mid2 = cz + (roomSize / 2.0f) + r;
    float end = cz + roomSize - r;

    glBegin(GL_QUADS);
    // TOP WALL
    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx - r, cy + r, start);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx + r, cy + r, start);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx + r, cy + r, mid1);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx - r, cy + r, mid1);

    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(cx - r, cy + r, mid1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(cx + r, cy + r, mid1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(cx + r, cy + r, mid2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(cx - r, cy + r, mid2);

    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx - r, cy + r, mid2);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx + r, cy + r, mid2);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx + r, cy + r, end);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx - r, cy + r, end);

    // BOTTOM WALL
    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx + r, cy - r, start);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx - r, cy - r, start);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx - r, cy - r, mid1);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx + r, cy - r, mid1);

    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(cx + r, cy - r, mid1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(cx - r, cy - r, mid1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(cx - r, cy - r, mid2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(cx + r, cy - r, mid2);

    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx + r, cy - r, mid2);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx - r, cy - r, mid2);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx - r, cy - r, end);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx + r, cy - r, end);

    // LEFT WALL
    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx - r, cy - r, start);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx - r, cy + r, start);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx - r, cy + r, mid1);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx - r, cy - r, mid1);

    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(cx - r, cy - r, mid1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(cx - r, cy + r, mid1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(cx - r, cy + r, mid2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(cx - r, cy - r, mid2);

    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx - r, cy - r, mid2);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx - r, cy + r, mid2);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx - r, cy + r, end);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx - r, cy - r, end);

    // RIGHT WALL
    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx + r, cy + r, start);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx + r, cy - r, start);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx + r, cy - r, mid1);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx + r, cy + r, mid1);

    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(cx + r, cy + r, mid1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(cx + r, cy - r, mid1);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(cx + r, cy - r, mid2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(cx + r, cy + r, mid2);

    glColor3f(0.4f, 0.4f, 0.4f);
    glTexCoord2f(0.0f, 0.50f); glVertex3f(cx + r, cy + r, mid2);
    glTexCoord2f(1.0f, 0.50f); glVertex3f(cx + r, cy - r, mid2);
    glTexCoord2f(1.0f, 0.51f); glVertex3f(cx + r, cy - r, end);
    glTexCoord2f(0.0f, 0.51f); glVertex3f(cx + r, cy + r, end);
    glEnd();
}

void Tesseract::draw() {
    // Disable lighting completely for the environment so the bookshelves are 100% bright
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texBookshelf);

    // FIX: Generate exactly 7 nodes (-3 to +3) to create 6 connecting rooms!
    // Centered around 0, 0, 0 for perfect symmetry
    for (int x = -3; x <= 3; x++) {
        for (int y = -3; y <= 3; y++) {
            for (int z = -3; z <= 3; z++) {

                // Absolute centering math
                float cx = x * roomSize;
                float cy = y * roomSize;
                float cz = z * roomSize;

                // Draw struts only towards the positive adjacent node
                if (x < 3) drawStrutX(cx, cy, cz);
                if (y < 3) drawStrutY(cx, cy, cz);
                if (z < 3) drawStrutZ(cx, cy, cz);
            }
        }
    }

    glDisable(GL_TEXTURE_2D);
    // Turn lighting back on so the Spaceship renders beautifully in the middle
    glEnable(GL_LIGHTING);
}