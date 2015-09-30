#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Grid.h"

int gridX = 600;
int gridY = 600;
int gridZ = 600;

const double fovy = 50.;
const double clipNear = .01;
const double clipFar = 1000.;
double x = 0;
double y = 0;
double z = -4.0;
double isovalue = 0.3;
bool p = false;

Grid grid;

void printInstructions()
{
    std::cerr << "space: insert metaball\n"
              << "→/←: increase/ decrease isovalue\n"
              << "↑/↓: move in/out\n"
              << "w/s: move up/down\n"
              << "a/d: move left/right\n"
              << "p: pause\n"
              << "escape: exit program\n"
              << std::endl;
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    double aspect = (double)viewport[2] / (double)viewport[3];
    gluPerspective(fovy, aspect, clipNear, clipFar);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(0, 0, z, x, y, 0, 0, 1, 0);
    
    std::vector<Triangle> triangles;
    grid.step(isovalue, triangles);
    
    
    for (size_t i = 0; i < triangles.size(); i++) {
        glColor4f(0, 0, 1, 1.0);
        glBegin(GL_LINE_LOOP);
        glVertex3d(triangles[i].a.x(), triangles[i].a.y(), triangles[i].a.z());
        glVertex3d(triangles[i].b.x(), triangles[i].b.y(), triangles[i].b.z());
        glVertex3d(triangles[i].c.x(), triangles[i].c.y(), triangles[i].c.z());
        glEnd();
    }

    glutSwapBuffers();
}

void timer(int t)
{
    glutPostRedisplay();
    if (!p) {
        glutTimerFunc(t, timer, t);
    }
}

void keyboard(unsigned char key, int x0, int y0)
{
    switch (key) {
        case 27 :
            exit(0);
        case ' ':
            grid.addMetaball();
            break;
        case 'a':
            x -= 0.03;
            break;
        case 'd':
            x += 0.03;
            break;
        case 'w':
            y += 0.03;
            break;
        case 's':
            y -= 0.03;
            break;
        case 'p':
            p = !p;
            if (!p) timer(10);
            break;
    }
    
    glutPostRedisplay();
}

void special(int i, int x0, int y0)
{
    switch (i) {
        case GLUT_KEY_UP:
            z += 0.03;
            break;
        case GLUT_KEY_DOWN:
            z -= 0.03;
            break;
        case GLUT_KEY_LEFT:
            isovalue -= 0.02;
            if (isovalue < 0) isovalue = 0.0;
            break;
        case GLUT_KEY_RIGHT:
            isovalue += 0.02;
            break;
    }
    
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    
    // initialize grid
    grid.init(0.1, -10, 10);
    for (int i = 0; i < 4; i++) {
        grid.addMetaball();
    }
    
    printInstructions();
    glutInitWindowSize(gridX, gridY);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInit(&argc, argv);
    glutCreateWindow("Metaballs");
    init();
    glutDisplayFunc(display);
    timer(10);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMainLoop();
    
    return 0;
}
