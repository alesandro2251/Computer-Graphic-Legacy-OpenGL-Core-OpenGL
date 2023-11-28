#include <GL/freeglut.h>

void draw();

void paintQuad(float);

float angle = 15.0;

int main(int count, char **agg) {
    glutInit(&count, agg);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Zdrasti");

    glClearColor(0.0f, 1.0f, 0.6f, 0.0f);

    glutDisplayFunc(draw);
    glutMainLoop();

    return 0;
}

void paintQuad(float width) {
    float wd2 = width / 2;

    glBegin(GL_QUADS);

    glColor3ub(255, 0, 0);
    glVertex3f(-wd2, -wd2, -wd2); // (-1,-1,-1)
    glVertex3f(wd2, -wd2, -wd2);  // (1,-1,-1)
    glVertex3f(wd2, wd2, -wd2);   // (1, 1,-1)
    glVertex3f(-wd2, wd2, -wd2);  // (-1,1,-1)

    glColor3ub(0, 0, 255);
    glVertex3f(wd2, -wd2, -wd2); // (1,-1,-1)
    glVertex3f(wd2, wd2, -wd2); // (1,1,-1)
    glVertex3f(wd2, wd2, wd2); // (1,1,1)
    glVertex3f(wd2, -wd2, wd2); // (1,-1,1)

    glColor3ub(255, 150, 0);
    glVertex3f(-wd2, -wd2, -wd2); // (-1,-1,-1)
    glVertex3f(wd2, -wd2, -wd2); // (1,-1,-1)
    glVertex3f(wd2, -wd2, wd2); // (1,-1,1)
    glVertex3f(-wd2, -wd2, wd2); // (-1,-1,1)

    glEnd();
}

void draw() {
    glDrawBuffer(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(angle, 0., 1.0, 1.0);

    paintQuad(0.8);

    glutSwapBuffers();
}
