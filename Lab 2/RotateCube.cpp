#include <GL/freeglut.h>
#include <stdio.h>

float angle = 15.0;

void draw();

void keyboard(unsigned char key, int x, int y);


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Zdrasti");
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glutDisplayFunc(draw);

    glutKeyboardFunc(keyboard);  // позволява да използваме клавиатурата

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return 0;
}

void drawCube() {
    glBegin(GL_QUADS);

    glColor3ub(255, 0, 0);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);


    glColor3ub(200, 100, 0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    glColor3ub(0, 255, 0);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);

    glColor3ub(0, 0, 255);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    glColor3ub(0, 255, 255);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);


    glColor3ub(255, 0, 255);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, -0.5);

    glEnd();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
        angle += 5.0; // движи се на ляво
        break;
    case 'd':
        angle -= 5.0; // движи се на дясно
        break;
    }
    glutPostRedisplay();
}


void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(angle, 0., 1.0, 1.0);

    drawCube();

    glutSwapBuffers();
}
