#include "pch.h"
#include <iostream>
#include "GL/glut.h"

using namespace std;

int screenx = 700;
int screeny = 700;
float red = 0.0;
float green = 0.0;
float blue = 0.0;
double eyeX = 0;
double eyeY = 0;
double eyeZ = 40; 
double centerX = 0; 
double centerY = 0; 
double centerZ = 0; 
double upX = 0; 
double upY = 1; 
double upZ = 0;

void processMenu(int option) {
	red = 1.0;
	green = 0.0;
	blue = 0.0;
	glutPostRedisplay();
}
void rollMenu(int option) {
	switch (option) {
	case 1:
		red = 1.0;
		green = 0.0;
		blue = 0.0;
		glutPostRedisplay();
		break;
	case 2:

		break;
	}
}

void pitchMenu(int option) {
	switch (option) {
	case 1:
		red = 1.0;
		green = 0.0;
		blue = 0.0;
		break;
	case 2:

		break;
	}
}

void yawMenu(int option) {
	switch (option) {
	case 1:
		red = 1.0;
		green = 0.0;
		blue = 0.0;
		break;
	case 2:

		break;
	}
}

void slideMenu(int option) {
	switch (option) {
	case 1:
		red = 1.0;
		green = 0.0;
		blue = 0.0;
		break;
	case 2:

		break;
	}
}

void createMenu() {
	//Roll
	int rollmenu = glutCreateMenu(rollMenu);
	glutAddMenuEntry("Add", 1);
	glutAddMenuEntry("Subtract", 2);
	//Pitch
	int pitchmenu = glutCreateMenu(pitchMenu);
	glutAddMenuEntry("Add", 1);
	glutAddMenuEntry("Subtract", 2);
	//Yaw
	int yawmenu = glutCreateMenu(yawMenu);
	glutAddMenuEntry("Add", 1);
	glutAddMenuEntry("Subtract", 2);
	//Slide
	int slidemenu = glutCreateMenu(slideMenu);
	glutAddMenuEntry("Add", 1);
	glutAddMenuEntry("Subtract", 2);
	//Main Menu
	int menu = glutCreateMenu(processMenu);
	glutAddSubMenu("Roll", rollmenu);
	glutAddSubMenu("Pitch", pitchmenu);
	glutAddSubMenu("Yaw", yawmenu);
	glutAddSubMenu("Slide", slidemenu);
	glutAddMenuEntry("Lever Rotation", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void display() {
	glClearColor(red, green, blue, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//draw the plane
	glColor3f(1, 1, 1);
	glBegin(GL_POLYGON);
	glVertex3f(-100, -10, -100);
	glVertex3f(-100, -10, 100);
	glVertex3f(100, -10, 100);
	glVertex3f(100, -10, -100);
	glEnd();

	//Grid
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	for (float x = -97.5; x <= 97.5; x=x+5) {
		glVertex3f(x , 0, -100);
		glVertex3f(x, 0, 100);
	}
	for (float z = -97.5; z <= 97.5; z=z+5) {
		glVertex3f(-100, 0, z);
		glVertex3f(100, 0, z);
	}
	glEnd();

	
	//Setting up the view
	glViewport(0, 0, screenx / 2, screeny / 2);
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
	gluPerspective(60, (double)((float)screenx / screeny), 40, 20); //guessing

	//Cylinder
	glColor3f(0.0, 1.0, 0.0);
	GLUquadricObj* cylinder = gluNewQuadric();
	gluCylinder(cylinder, 10, 10, 50, 32, 32);
	
	glFlush();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitWindowSize(screenx, screeny);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Project 3");

	glutDisplayFunc(display);
	//glutMouseFunc(mouseHandler);
	//glutKeyboardFunc(keyboardHandler);
	createMenu();
	glutMainLoop();
	return 0;
}