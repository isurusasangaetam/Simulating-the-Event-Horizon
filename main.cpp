#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <glut.h> 

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ==========================================
// SHIP & FLIGHT STATE
// ==========================================
// Ship starts at a safe cinematic distance
float shipX = 0.0f;
float shipY = 1.0f;
float shipZ = 24.0f;

// Velocity for smooth space movement (momentum ready)
float velX = 0.0f;
float velY = 0.0f;
float velZ = 0.0f;

// Ship orientation angles (driven by mouse)
float shipYaw = 0.0f;
float shipPitch = 0.0f;

// Relative camera angle to black hole (for lensing deformation)
float relCamYaw = 0.0f;
float relCamPitch = 0.0f;

// Third-person chase camera offsets
const float CHASE_DISTANCE = 2.4f;
const float CHASE_HEIGHT = 0.65f;

// Key states for fluid multi-key input
bool keys[256] = { false };
bool specialKeys[256] = { false };

// Mouse tracking
int lastMouseX = -1;
int lastMouseY = -1;
bool firstMouse = true;

// Texture and animation
GLuint textureLava;
GLfloat uvOffset = 0.0f;

// ==========================================
// TEXTURE LOADER
// ==========================================
GLuint loadTex(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }
    else {
        std::cout << "Failed to load texture: " << filename << std::endl;
    }
    stbi_image_free(data);
    return textureID;
}

void init(void) {
    glClearColor(0.005f, 0.005f, 0.01f, 1.0f); // Deep cosmos void
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    // Load your accretion disk texture (ensure texture_1.jpg is in the project root)
    textureLava = loadTex("texture_1.jpg");
}

// ==========================================
// SHIP GEOMETRY (Realistic Scale Prototype)
// ==========================================
void drawSpaceship() {
    glPushMatrix();
    // Position the ship in world space
    glTranslatef(shipX, shipY, shipZ);

    // Orient the ship to match mouse heading
    glRotatef(-shipYaw * (180.0f / M_PI), 0.0f, 1.0f, 0.0f);
    glRotatef(shipPitch * (180.0f / M_PI), 1.0f, 0.0f, 0.0f);

    // Scale: Very small compared to the black hole (radius 4.0)
    glScalef(0.12f, 0.12f, 0.12f);

    glDisable(GL_TEXTURE_2D);

    // 1. Central Hull (Cockpit Shuttle)
    glColor3f(0.85f, 0.85f, 0.9f);
    glPushMatrix();
    glScalef(0.6f, 0.35f, 2.0f);
    glutSolidCube(1.0);
    glPopMatrix();

    // 2. Rotating Habitat Ring (Endurance style)
    glColor3f(0.7f, 0.72f, 0.75f);
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glutSolidTorus(0.15, 1.4, 16, 32);
    glPopMatrix();

    // 3. Thruster module (Glowing Cyan)
    glColor3f(0.2f, 0.7f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 1.1f);
    glScalef(0.3f, 0.2f, 0.3f);
    glutSolidSphere(0.8, 12, 12);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

// ==========================================
// BLACK HOLE MESH GENERATION
// ==========================================
void drawGargantua() {
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glutSolidSphere(4.0, 70, 70);
    glPopMatrix();
}

void drawVerticalHalo(float innerR, float outerR) {
    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureLava);

    glPushMatrix();
    // Billboard halo relative to camera vector
    glRotatef(relCamYaw * (180.0f / M_PI), 0.0f, 1.0f, 0.0f);
    glRotatef(-relCamPitch * (180.0f / M_PI), 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -3.8f);

    glBegin(GL_QUAD_STRIP);
    int segments = 80;
    for (int i = 0; i <= segments; ++i) {
        float theta = 2.0f * M_PI * float(i) / float(segments);
        float c = cos(theta);
        float s = sin(theta);
        float u = (float(i) / float(segments)) * 3.0f + uvOffset;

        glTexCoord2f(u, 0.0f);
        glVertex3f(innerR * c, innerR * s, 0.0f);
        glTexCoord2f(u, 1.0f);
        glVertex3f(outerR * c, outerR * s, 0.0f);
    }
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawWarpedAccretionDisk(float innerR, float outerR, bool bendUpwards) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureLava);
    glColor3f(1.0f, 1.0f, 1.0f);

    float dirX = sin(relCamYaw);
    float dirZ = cos(relCamYaw);

    int radialSegments = 30;
    int angularSegments = 120;
    float haloThicknessFactor = 0.25f;

    for (int r = 0; r < radialSegments; ++r) {
        float baseInnerR = innerR + (outerR - innerR) * ((float)r / radialSegments);
        float baseOuterR = innerR + (outerR - innerR) * ((float)(r + 1) / radialSegments);

        float v_in = (float)r / radialSegments;
        float v_out = (float)(r + 1) / radialSegments;

        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= angularSegments; ++i) {
            float theta = 2.0f * M_PI * float(i) / float(angularSegments);

            float base_x_in = baseInnerR * cos(theta);
            float base_z_in = baseInnerR * sin(theta);
            float base_depth_in = -(base_x_in * dirX + base_z_in * dirZ);

            float base_x_out = baseOuterR * cos(theta);
            float base_z_out = baseOuterR * sin(theta);
            float base_depth_out = -(base_x_out * dirX + base_z_out * dirZ);

            float currentInnerR = baseInnerR;
            float currentOuterR = baseOuterR;

            // Compression for narrow arches
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

            float y_in = 0.0f;
            float final_x_in = x_in;
            float final_z_in = z_in;

            if (depth_in > 0.0f) {
                float fold = depth_in / innerR;
                if (fold > 1.0f) fold = 1.0f;
                fold = fold * fold * (3.0f - 2.0f * fold);
                y_in = depth_in * fold;
                float new_depth = depth_in * (1.0f - fold) - 0.05f * fold;
                final_x_in = u_in * dirZ - new_depth * dirX;
                final_z_in = -u_in * dirX - new_depth * dirZ;
            }

            float y_out = 0.0f;
            float final_x_out = x_out;
            float final_z_out = z_out;

            if (depth_out > 0.0f) {
                float fold = depth_out / innerR;
                if (fold > 1.0f) fold = 1.0f;
                fold = fold * fold * (3.0f - 2.0f * fold);
                y_out = depth_out * fold;
                float new_depth = depth_out * (1.0f - fold) - 0.05f * fold;
                final_x_out = u_out * dirZ - new_depth * dirX;
                final_z_out = -u_out * dirX - new_depth * dirZ;
            }

            if (!bendUpwards) {
                y_in = -y_in;
                y_out = -y_out;
            }

            float u_tex = (float(i) / float(angularSegments)) * 6.0f + uvOffset;

            glTexCoord2f(u_tex, v_in);
            glVertex3f(final_x_in, y_in, final_z_in);

            glTexCoord2f(u_tex, v_out);
            glVertex3f(final_x_out, y_out, final_z_out);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
}

// ==========================================
// DISPLAY & CAMERA PIPELINE
// ==========================================
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 1. Calculate Ship's Forward and Right vectors from mouse angles
    float fwdX = -sin(shipYaw) * cos(shipPitch);
    float fwdY = sin(shipPitch);
    float fwdZ = -cos(shipYaw) * cos(shipPitch);

    // 2. Position the Third-Person Camera behind and above the ship
    float camX = shipX - fwdX * CHASE_DISTANCE;
    float camY = shipY - fwdY * CHASE_DISTANCE + CHASE_HEIGHT;
    float camZ = shipZ - fwdZ * CHASE_DISTANCE;

    // Look slightly ahead of the spacecraft
    float lookTargetX = shipX + fwdX * 2.0f;
    float lookTargetY = shipY + fwdY * 2.0f;
    float lookTargetZ = shipZ + fwdZ * 2.0f;

    gluLookAt(camX, camY, camZ,
        lookTargetX, lookTargetY, lookTargetZ,
        0.0f, 1.0f, 0.0f);

    // 3. Compute relative camera angles for dynamic black hole deformation
    float relDistXZ = sqrt(camX * camX + camZ * camZ);
    relCamYaw = atan2(camX, camZ);
    relCamPitch = atan2(camY, (relDistXZ > 0.001f ? relDistXZ : 0.001f));

    // 4. Render Celestial Bodies
    drawGargantua();
    drawVerticalHalo(4.05f, 6.0f);
    drawWarpedAccretionDisk(4.0f, 11.0f, true);
    drawWarpedAccretionDisk(4.0f, 11.0f, false);

    // 5. Render Player's Ship
    drawSpaceship();

    glutSwapBuffers();
}

// ==========================================
// PHYSICS & BOUNDARY UPDATE LOOP
// ==========================================
void update(int value) {
    // Forward, Right, and World-Up direction vectors
    float fwdX = -sin(shipYaw) * cos(shipPitch);
    float fwdY = sin(shipPitch);
    float fwdZ = -cos(shipYaw) * cos(shipPitch);

    float rightX = cos(shipYaw);
    float rightZ = -sin(shipYaw);

    float thrust = 0.015f; // Flight acceleration

    // Standard gaming movement controls
    if (keys['w'] || keys['W']) { velX += fwdX * thrust; velY += fwdY * thrust; velZ += fwdZ * thrust; }
    if (keys['s'] || keys['S']) { velX -= fwdX * thrust; velY -= fwdY * thrust; velZ -= fwdZ * thrust; }
    if (keys['d'] || keys['D']) { velX += rightX * thrust; velZ += rightZ * thrust; }
    if (keys['a'] || keys['A']) { velX -= rightX * thrust; velZ -= rightZ * thrust; }

    // Elevation controls: Space / Shift for UP, C / Ctrl for DOWN
    if (keys[' '] || specialKeys[GLUT_ACTIVE_SHIFT]) { velY += thrust; }
    if (keys['c'] || keys['C'] || specialKeys[GLUT_ACTIVE_CTRL]) { velY -= thrust; }

    // Space drag (dampening) to prevent sliding infinitely
    velX *= 0.92f;
    velY *= 0.92f;
    velZ *= 0.92f;

    // Apply movement
    shipX += velX;
    shipY += velY;
    shipZ += velZ;

    // --- ILLUSION BOUNDARY ENFORCEMENT ---
    // 1. Minimum event horizon distance check (prevent clipping inside black hole)
    float distToHole = sqrt(shipX * shipX + shipY * shipY + shipZ * shipZ);
    if (distToHole < 4.6f) {
        float pushFactor = 4.6f / distToHole;
        shipX *= pushFactor;
        shipY *= pushFactor;
        shipZ *= pushFactor;
    }

    // 2. Polar pitch lock (prevents camera from reaching extreme overhead/underhead angles)
    float horizontalDist = sqrt(shipX * shipX + shipZ * shipZ);
    float maxSafeHeight = horizontalDist * tan(35.0f * (M_PI / 180.0f)); // Max 35 degrees
    if (shipY > maxSafeHeight)  shipY = maxSafeHeight;
    if (shipY < -maxSafeHeight) shipY = -maxSafeHeight;

    // Accretion disk flow animation
    uvOffset -= 0.01f;
    if (uvOffset < -1.0f) uvOffset += 1.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// ==========================================
// INPUT CONTROLLERS
// ==========================================
void mouseMotion(int x, int y) {
    if (firstMouse) {
        lastMouseX = x;
        lastMouseY = y;
        firstMouse = false;
        return;
    }

    float deltaX = (float)(x - lastMouseX);
    float deltaY = (float)(y - lastMouseY);
    lastMouseX = x;
    lastMouseY = y;

    float sensitivity = 0.005f;
    shipYaw -= deltaX * sensitivity;
    shipPitch -= deltaY * sensitivity;

    // Clamp pitch so the player cannot loop upside-down
    if (shipPitch > 1.2f) shipPitch = 1.2f;
    if (shipPitch < -1.2f) shipPitch = -1.2f;
}

void keyDown(unsigned char key, int x, int y) {
    keys[key] = true;
    int mod = glutGetModifiers();
    specialKeys[GLUT_ACTIVE_SHIFT] = (mod & GLUT_ACTIVE_SHIFT);
    specialKeys[GLUT_ACTIVE_CTRL] = (mod & GLUT_ACTIVE_CTRL);
}

void keyUp(unsigned char key, int x, int y) {
    keys[key] = false;
    specialKeys[GLUT_ACTIVE_SHIFT] = false;
    specialKeys[GLUT_ACTIVE_CTRL] = false;
}

void reshape(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    GLfloat aspect = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55.0, aspect, 0.1, 300.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Interstellar Simulation - Third Person Flight");

    init();

    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(mouseMotion);
    glutMotionFunc(mouseMotion); // Active while dragging as well

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}