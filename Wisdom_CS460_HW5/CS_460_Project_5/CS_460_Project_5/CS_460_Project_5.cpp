#include "pch.h"
#include "GL/glut.h"

int screenx = 500;
int screeny = 500;
float red = 0.0;
float blue = 0.0;
float green = 0.0;
int xloc = 45;
int yloc = 45;
int zloc = 45;
float lightx = 50.0;
float lighty = 50.0;
float lightz = 50.0; 

void processMenu(int option) {
	glutPostRedisplay();
}

int change = 1; //change constant
//Control Point 1
void cp1(int option) {
	switch (option) {
	case 1:
		
		glutPostRedisplay();
		break;
	case 2:
		
		glutPostRedisplay();
		break;
	case 3:
		
		glutPostRedisplay();
		break;
	case 4:

		glutPostRedisplay();
		break;
	case 5:

		glutPostRedisplay();
		break;
	case 6:

		glutPostRedisplay();
		break;
	}
}
//Control Point 2
void cp2(int option) {
	switch (option) {
	case 1:
		
		glutPostRedisplay();
		break;
	case 2:
		
		glutPostRedisplay();
		break;
	}
}
//Control Point 3
void cp3(int option) {
	switch (option) {
	case 1:
		
		glutPostRedisplay();
		break;
	case 2:
		
		glutPostRedisplay();
		break;
	}
}
//Control Point 4
void cp4(int option) {
	switch (option) {
	case 1:
		
		glutPostRedisplay();
		break;
	case 2:
		
		glutPostRedisplay();
		break;
	}
}
void zoomMenu(int option) {
	switch (option) {
	case 1:
		zloc = zloc + 5;
		glutPostRedisplay();
		break;
	case 2:
		zloc = zloc - 5;
		glutPostRedisplay();
		break;
	}
}
void lightMenu(int option) {
	switch (option) {
	case 1:
		lighty = lighty + 5;
		glutPostRedisplay();
		break;
	case 2:
		lighty = lighty - 5;
		glutPostRedisplay();
		break;
	}
}

void createMenu() {
	//Control Point 1
	int menu1 = glutCreateMenu(cp1);
	glutAddMenuEntry("X+", 1);
	glutAddMenuEntry("X-", 2);
	glutAddMenuEntry("Y+", 3);
	glutAddMenuEntry("Y-", 4);
	glutAddMenuEntry("Z+", 5);
	glutAddMenuEntry("Z-", 6);
	//Control Point 2
	int menu2 = glutCreateMenu(cp2);
	glutAddMenuEntry("X+", 1);
	glutAddMenuEntry("X-", 2);
	glutAddMenuEntry("Y+", 3);
	glutAddMenuEntry("Y-", 4);
	glutAddMenuEntry("Z+", 5);
	glutAddMenuEntry("Z-", 6);
	//Control Point 3
	int menu3 = glutCreateMenu(cp3);
	glutAddMenuEntry("X+", 1);
	glutAddMenuEntry("X-", 2);
	glutAddMenuEntry("Y+", 3);
	glutAddMenuEntry("Y-", 4);
	glutAddMenuEntry("Z+", 5);
	glutAddMenuEntry("Z-", 6);
	//Control Point 4
	int menu4 = glutCreateMenu(cp4);
	glutAddMenuEntry("X+", 1);
	glutAddMenuEntry("X-", 2);
	glutAddMenuEntry("Y+", 3);
	glutAddMenuEntry("Y-", 4);
	glutAddMenuEntry("Z+", 5);
	glutAddMenuEntry("Z-", 6);
	//Zoom Menu
	int zoom = glutCreateMenu(zoomMenu);
	glutAddMenuEntry("Z+", 1);
	glutAddMenuEntry("Z-", 2);
	//Light Shift Menu
	int light = glutCreateMenu(lightMenu);
	glutAddMenuEntry("Y+", 1);
	glutAddMenuEntry("Y-", 2);
	//Main Menu
	int menu = glutCreateMenu(processMenu);
	glutAddSubMenu("Control Point 1", menu1);
	glutAddSubMenu("Control Point 2", menu2);
	glutAddSubMenu("Control Point 3", menu3);
	glutAddSubMenu("Control Point 4", menu4);
	glutAddSubMenu("Zoom", zoom);
	glutAddSubMenu("Light Shift", light);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void display() {
	glClearColor(red, green, blue, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, screenx, screeny);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, ((float)screenx / screeny), 0.01, 500);
	gluLookAt(xloc, yloc, zloc, 0, 0, 0, 0, 1, 0);
	/*
	//Lighting
	GLfloat lightpos[] = {lightx, lighty, lightz, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	*/
	//Axes
	glBegin(GL_LINES);
	//Red x-axis
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-100.0, 0.0, 0.0);
	glVertex3f(100.0, 0.0, 0.0);
	//Blue y-axis
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, -100.0, 0.0);
	glVertex3f(0.0, 100.0, 0.0);
	//Green z-axis
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, -100.0);
	glVertex3f(0.0, 0.0, 100.0);
	glEnd();
	//Control Points
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	for (float i = 0.0; i < 4; i++) {
		for (float j = 0.0; j < 4; j++) {
			glVertex3f(20*i, 0.0, 20*j);
		}
	}
	glEnd();


	glutPostRedisplay();
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(screenx, screeny);
	glutInitWindowPosition(50, 50);
	//glutInitDisplayMode(GLUT_DOUBLE);
	glutCreateWindow("Project 5");
	glutDisplayFunc(display);
	//glutMouseFunc(mouseHandler);
	//glutKeyboardFunc(keyboardHandler);
	createMenu();
	glutMainLoop();
	return 0;
}