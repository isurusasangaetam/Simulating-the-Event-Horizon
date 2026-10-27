#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <glut.h>
#include "Spaceship.h"
#include "Camera.h"
#include "BlackHole.h"
#include "TextureLoader.h"
#include "Environment.h"

Spaceship playerShip;
Camera mainCamera;
BlackHole gargantua;
Environment deepSpace;

bool keys[256] = { false };
bool specialKeys[256] = { false };
int winWidth = 1280, winHeight = 720;

void init(void) {
    glClearColor(0.01f, 0.01f, 0.015f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    gargantua.setTexture(TextureLoader::loadTex("texture_1.jpg"));
    deepSpace.init();

    GLuint texSolar = TextureLoader::loadTex("solar.jpg");
    GLuint texMetal = TextureLoader::loadTex("metal.jpg");
    playerShip.initTextures(texSolar, texMetal);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    mainCamera.applyView(playerShip);
    deepSpace.draw();
    gargantua.draw(mainCamera.relCamYaw, mainCamera.relCamPitch);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat lightPos[] = { 1.0f, 0.8f, 1.0f, 0.0f };
    GLfloat lightDiffuse[] = { 1.2f, 1.2f, 1.2f, 1.0f };
    GLfloat lightAmbient[] = { 0.3f, 0.3f, 0.35f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);

    playerShip.draw();

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_COLOR_MATERIAL);

    glutSwapBuffers();
}

void update(int value) {
    playerShip.update(keys, specialKeys);
    gargantua.updateAnimation();

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void mouseMotion(int x, int y) {
    int centerX = winWidth / 2;
    int centerY = winHeight / 2;
    if (x == centerX && y == centerY) return;

    float sensitivity = 0.005f;

    // FIXED: Restored the missing minus (-) signs so the mouse works again!
    mainCamera.mouseYawOffset -= (float)(x - centerX) * sensitivity;
    mainCamera.mousePitch -= (float)(y - centerY) * sensitivity;

    if (mainCamera.mousePitch > 1.5f) mainCamera.mousePitch = 1.5f;
    if (mainCamera.mousePitch < -1.5f) mainCamera.mousePitch = -1.5f;

    glutWarpPointer(centerX, centerY);
}

void keyDown(unsigned char key, int x, int y) {
    keys[key] = true;
    int mod = glutGetModifiers();
    specialKeys[GLUT_ACTIVE_SHIFT] = (mod & GLUT_ACTIVE_SHIFT);
    specialKeys[GLUT_ACTIVE_CTRL] = (mod & GLUT_ACTIVE_CTRL);

    if (key == 'f' || key == 'F') {
        playerShip.toggleDocking();
        mainCamera.mouseYawOffset = playerShip.yaw;
    }

    if (key == 'r' || key == 'R') playerShip.spinEndurance = !playerShip.spinEndurance;
    if (key == 27) exit(0);
}

void keyUp(unsigned char key, int x, int y) {
    keys[key] = false;
    specialKeys[GLUT_ACTIVE_SHIFT] = false;
    specialKeys[GLUT_ACTIVE_CTRL] = false;
}

void reshape(GLsizei w, GLsizei h) {
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // THE FIX: Increased the far clipping plane from 500.0 to 20000.0
    gluPerspective(55.0, (GLfloat)w / (GLfloat)h, 0.1, 20000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Interstellar Simulation");

    init();

    glutSetCursor(GLUT_CURSOR_NONE);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(mouseMotion);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutWarpPointer(winWidth / 2, winHeight / 2);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}