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
float rotater = 0;


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

void drawPlaneandGrid() {
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
	for (float x = -97.5; x <= 97.5; x = x + 5) {
		glVertex3f(x, -10, -100);
		glVertex3f(x, -10, 100);
	}
	for (float z = -97.5; z <= 97.5; z = z + 5) {
		glVertex3f(-100, -10, z);
		glVertex3f(100, -10, z);
	}
	glEnd();
}

void drawLever() {
	float height = 20;
	//Cylinder
	glColor3f(0.0, 1.0, 0.0);
	//make the wireframe object
	GLUquadricObj* wireobj = gluNewQuadric();
	gluQuadricDrawStyle(wireobj, GLU_LINE);
	//make the base cylinder
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	gluCylinder(wireobj, 2, 2, height, 4, 4);
	glPopMatrix();
	//make the horizontal bar
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glRotatef(rotater, 0, 1, 0); //active rotation
	glTranslatef(0, 0, -height / 2);
	gluCylinder(wireobj, 2, 2, height, 4, 4);
	glPopMatrix();
	//three initial spheres
	glPushMatrix();
	gluSphere(wireobj, 3, 8, 8);
	glPopMatrix();

	glPushMatrix();
	/*
	glTranslatef(height / 2, 0, 0);
	glRotatef(rotater, 0, 1, 0);
	glTranslatef(-height / 2, 0, 0);
	gluSphere(wireobj, 3, 8, 8);
	*/
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-height/2, 0, 0);
	glRotatef(rotater, 0, 1, 0);
	glTranslatef(height/2, 0,  0);
	gluSphere(wireobj, 3, 8, 8);
	glPopMatrix();

}

void display() {
	glClearColor(red, green, blue, 0.0);
	rotater+=.1;
	//glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, ((float)screenx / screeny), 0.01, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//1
	glViewport(screenx / 2, 0, screenx / 2, screeny / 2);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
	drawPlaneandGrid();
	drawLever();

	//2
	glViewport(0, 0, screenx / 2, screeny / 2);
	glLoadIdentity();
	gluLookAt(0, 40, 0, centerX, centerY, centerZ, 0, 0, 1);
	drawPlaneandGrid();
	drawLever();

	//3
	glViewport(0, screeny / 2, screenx / 2, screeny / 2);
	glLoadIdentity();
	gluLookAt(40, 0, 0, centerX, centerY, centerZ, upX, upY, upZ);
	drawPlaneandGrid();
	drawLever();

	//4
	glViewport(screenx / 2, screeny / 2, screenx / 2, screeny / 2);
	glLoadIdentity();
	gluLookAt(0, 0, 40, centerX, centerY, centerZ, upX, upY, upZ);
	drawPlaneandGrid();
	drawLever();

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