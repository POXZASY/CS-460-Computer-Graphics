#include "pch.h"
#include <iostream>
#include "GL/glut.h"
#include <math.h>

#define PI 3.14159265358979323846264338327950288419716939937510

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
//Roll (rotate around viewer z axis)
void rollMenu(int option) {
	switch (option) {
	case 1:
		eyeX = eyeX * cos(10*PI/180) - eyeY * sin(10 * PI / 180);
		eyeY = eyeX * sin(10 * PI / 180) + eyeY * cos(10 * PI / 180);
		glutPostRedisplay();
		break;
	case 2:
		eyeX = eyeX * cos(-10 * PI / 180) - eyeY * sin(-10 * PI / 180);
		eyeY = eyeX * sin(-10 * PI / 180) + eyeY * cos(-10 * PI / 180);
		glutPostRedisplay();
		break;
	}
}
//Pitch (rotate around viewer x axis)
void pitchMenu(int option) {
	switch (option) {
	case 1:
		eyeY = eyeY * cos(10 * PI / 180) - eyeZ * sin(10 * PI / 180);
		eyeZ = eyeY * sin(10 * PI / 180) + eyeZ * cos(10 * PI / 180);
		glutPostRedisplay();
		break;
	case 2:
		eyeY = eyeY * cos(-10 * PI / 180) - eyeZ * sin(-10 * PI / 180);
		eyeZ = eyeY * sin(-10 * PI / 180) + eyeZ * cos(-10 * PI / 180);
		glutPostRedisplay();
		break;
	}
}
//Yaw (rotate around viewer y axis)
void yawMenu(int option) {
	switch (option) {
	case 1:
		eyeX = eyeX * cos(10 * PI / 180) + eyeZ * sin(10 * PI / 180);
		eyeZ = -eyeX * sin(10 * PI / 180) + eyeZ * cos(10 * PI / 180);
		glutPostRedisplay();
		break;
	case 2:
		eyeX = eyeX * cos(-10 * PI / 180) + eyeZ * sin(-10 * PI / 180);
		eyeZ = -eyeX * sin(-10 * PI / 180) + eyeZ * cos(-10 * PI / 180);
		glutPostRedisplay();
		break;
	}
}

void slideMenu(int option) {
	switch (option) {
	case 1:
		eyeX = eyeX * 1.1;
		eyeY = eyeY * 1.1;
		eyeZ = eyeZ * 1.1;
		glutPostRedisplay();
		break;
	case 2:
		eyeX = eyeX * 0.9;
		eyeY = eyeY * 0.9;
		eyeZ = eyeZ * 0.9;
		glutPostRedisplay();
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
	//glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, ((float)screenx / screeny), 0.01, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, screenx, screeny);
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);

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
		glVertex3f(x , -10, -100);
		glVertex3f(x, -10, 100);
	}
	for (float z = -97.5; z <= 97.5; z=z+5) {
		glVertex3f(-100, -10, z);
		glVertex3f(100, -10, z);
	}
	glEnd();

	/*
	//Cylinder
	glColor3f(0.0, 1.0, 0.0);
	GLUquadricObj* cylinder = gluNewQuadric();
	gluCylinder(cylinder, 10, 10, 50, 32, 32);
	*/
	
	//Setting up the view V1
	

	
	glutPostRedisplay();
	glFlush();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitWindowSize(screenx, screeny);
	glutInitWindowPosition(50, 50);
	//glutInitDisplayMode(GLUT_DOUBLE);
	glutCreateWindow("Project 3");
	glutDisplayFunc(display);
	//glutMouseFunc(mouseHandler);
	//glutKeyboardFunc(keyboardHandler);
	createMenu();
	glutMainLoop();
	return 0;
}