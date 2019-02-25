#include "Windows.h"
#include "GL/glut.h"

int screenx = 500;
int screeny = 500;

void createMenu() {
	int menu = glutCreateMenu();
	glutSetMenu(menu);
	glutAddMenuEntry("Polygon Clipping", 1);
	glutAddMenuEntry("Region Filling", 2);
	glutAddMenuEntry("Window-to-Viewport Mapping", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void display() {
	glClearColor(0.0, 1.0, 0.0, 1.0); //bg color
	glClear(GL_COLOR_BUFFER_BIT);
	glutReshapeWindow(500, 500);
	glFlush();
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv); //Initialize glut
	glutCreateWindow("Project 2");
	glutInitWindowSize(screenx, screeny);
	glutInitWindowPosition(50, 50);
	createMenu();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}