
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
double eyeX2 = 0;
double eyeY2 = 0;
double eyeZ2 = 40;
double centerX2 = 0;
double centerY2 = 0;
double centerZ2 = 0;
float rotater = 0;
bool rot = false;


void processMenu(int option) {
	if (option == 1) rot = !rot;
	glutPostRedisplay();
}
//Roll (rotate around viewer z axis)
void rollMenu(int option) {
	switch (option) {
	case 1:
		eyeX2 = eyeX2 - centerX2;
		eyeY2 = eyeY2 - centerY2;
		eyeZ2 = eyeZ2 - centerZ2;
		eyeX2 = eyeX2 * cos(10*PI/180) - eyeY2 * sin(10 * PI / 180);
		eyeY2 = eyeX2 * sin(10 * PI / 180) + eyeY2 * cos(10 * PI / 180);
		eyeX2 = eyeX2 + centerX2;
		eyeY2 = eyeY2 + centerY2;
		eyeZ2 = eyeZ2 + centerZ2;
		glutPostRedisplay();
		break;
	case 2:
		eyeX2 = eyeX2 - centerX2;
		eyeY2 = eyeY2 - centerY2;
		eyeZ2 = eyeZ2 - centerZ2;
		eyeX2 = eyeX2 * cos(-10 * PI / 180) - eyeY2 * sin(-10 * PI / 180);
		eyeY2 = eyeX2 * sin(-10 * PI / 180) + eyeY2 * cos(-10 * PI / 180);
		eyeX2 = eyeX2 + centerX2;
		eyeY2 = eyeY2 + centerY2;
		eyeZ2 = eyeZ2 + centerZ2;
		glutPostRedisplay();
		break;
	}
}
//Pitch (rotate around viewer x axis)
void pitchMenu(int option) {
	switch (option) {
	case 1:
		eyeX2 = eyeX2 - centerX2;
		eyeY2 = eyeY2 - centerY2;
		eyeZ2 = eyeZ2 - centerZ2;
		eyeY2 = eyeY2 * cos(10 * PI / 180) - eyeZ2 * sin(10 * PI / 180);
		eyeZ2 = eyeY2 * sin(10 * PI / 180) + eyeZ2 * cos(10 * PI / 180);
		eyeX2 = eyeX2 + centerX2;
		eyeY2 = eyeY2 + centerY2;
		eyeZ2 = eyeZ2 + centerZ2;
		glutPostRedisplay();
		break;
	case 2:
		eyeX2 = eyeX2 - centerX2;
		eyeY2 = eyeY2 - centerY2;
		eyeZ2 = eyeZ2 - centerZ2;
		eyeY2 = eyeY2 * cos(-10 * PI / 180) - eyeZ2 * sin(-10 * PI / 180);
		eyeZ2 = eyeY2 * sin(-10 * PI / 180) + eyeZ2 * cos(-10 * PI / 180);
		eyeX2 = eyeX2 + centerX2;
		eyeY2 = eyeY2 + centerY2;
		eyeZ2 = eyeZ2 + centerZ2;
		glutPostRedisplay();
		break;
	}
}
//Yaw (rotate around viewer y axis)
void yawMenu(int option) {
	switch (option) {
	case 1:
		eyeX2 = eyeX2 - centerX2;
		eyeY2 = eyeY2 - centerY2;
		eyeZ2 = eyeZ2 - centerZ2;
		eyeX2 = eyeX2 * cos(10 * PI / 180) + eyeZ2 * sin(10 * PI / 180);
		eyeZ2 = -eyeX2 * sin(10 * PI / 180) + eyeZ2 * cos(10 * PI / 180);
		eyeX2 = eyeX2 + centerX2;
		eyeY2 = eyeY2 + centerY2;
		eyeZ2 = eyeZ2 + centerZ2;
		glutPostRedisplay();
		break;
	case 2:
		eyeX2 = eyeX2 - centerX2;
		eyeY2 = eyeY2 - centerY2;
		eyeZ2 = eyeZ2 - centerZ2;
		eyeX2 = eyeX2 * cos(-10 * PI / 180) + eyeZ2 * sin(-10 * PI / 180);
		eyeZ2 = -eyeX2 * sin(-10 * PI / 180) + eyeZ2 * cos(-10 * PI / 180);
		eyeX2 = eyeX2 + centerX2;
		eyeY2 = eyeY2 + centerY2;
		eyeZ2 = eyeZ2 + centerZ2;
		glutPostRedisplay();
		break;
	}
}

void slideMenu(int option) {
	switch (option) {
	case 1:
		eyeX2 = eyeX2 - centerX2;
		eyeY2 = eyeY2 - centerY2;
		eyeZ2 = eyeZ2 - centerZ2;
		eyeX2 = eyeX2 * 1.1;
		eyeY2 = eyeY2 * 1.1;
		eyeZ2 = eyeZ2 * 1.1;
		eyeX2 = eyeX2 + centerX2;
		eyeY2 = eyeY2 + centerY2;
		eyeZ2 = eyeZ2 + centerZ2;
		glutPostRedisplay();
		break;
	case 2:
		eyeX2 = eyeX2 - centerX2;
		eyeY2 = eyeY2 - centerY2;
		eyeZ2 = eyeZ2 - centerZ2;
		eyeX2 = eyeX2 * 0.9;
		eyeY2 = eyeY2 * 0.9;
		eyeZ2 = eyeZ2 * 0.9;
		eyeX2 = eyeX2 + centerX2;
		eyeY2 = eyeY2 + centerY2;
		eyeZ2 = eyeZ2 + centerZ2;
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
	gluCylinder(wireobj, 1.5, 1.5, height, 4, 4);
	glPopMatrix();
	//make the horizontal bar
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glRotatef(rotater, 0, 1, 0); //active rotation
	glTranslatef(0, 0, -height / 2);
	gluCylinder(wireobj, 1.5, 1.5, height, 4, 4);
	glPopMatrix();
	//three initial spheres
	glPushMatrix();
	gluSphere(wireobj, 2, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glRotatef(rotater, 0, 1, 0);
	glTranslatef(-height / 2, 0, 0);
	gluSphere(wireobj, 2, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glRotatef(rotater, 0, 1, 0);
	glTranslatef(height/2, 0,  0);
	gluSphere(wireobj, 2, 8, 8);
	glPopMatrix();

	//two hanging spears
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glRotatef(-rotater, 0, 0, 1);
	glTranslatef(-height / 2, 0, 0);
	glRotatef(rotater+180, 1, 0, 0);
	glTranslatef(0, 0, -height / 2);
	gluCylinder(wireobj, 0, 2, height / 2, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glRotatef(-rotater, 0, 0, 1);
	glTranslatef(height / 2, 0, 0);
	glRotatef(-rotater - 180, 1, 0, 0);
	glTranslatef(0, 0, -height / 2);
	gluCylinder(wireobj, 0, 2, height / 2, 4, 4);
	glPopMatrix();

	//two hanging spheres
	glPushMatrix();
	glRotatef(rotater, 0, 1, 0);
	glTranslatef(-height / 2, 0, 0);
	glRotatef(rotater, 1, 0, 0);
	glTranslatef(0, -height / 2, 0);
	gluSphere(wireobj, 4, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glRotatef(rotater, 0, 1, 0);
	glTranslatef(height / 2, 0, 0);
	glRotatef(-rotater, 1, 0, 0);
	glTranslatef(0, -height / 2, 0);
	gluSphere(wireobj, 4, 8, 8);
	glPopMatrix();

}

void keyboardHandler(unsigned char key, int x, int y) {
	if (key == 'w' || key == 'W') {
		eyeZ2--;
		centerZ2--;
	}
	else if (key == 'a' || key == 'A') {
		eyeX2--;
		centerX2--;
	}
	else if (key == 's' || key == 'S') {
		eyeZ2++;
		centerZ2++;
	}
	else if (key == 'd' || key == 'D') {
		eyeX2++;
		centerX2++;
	}
	else if (key == 'r' || key == 'R') {
		eyeY2++;
		centerY2++;
	}
	else if (key == 'f' || key == 'F') {
		eyeY2--;
		centerY2--;
	}
}

void display() {
	glClearColor(red, green, blue, 0.0);
	if(rot) rotater+=.05;
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, ((float)screenx / screeny), 0.01, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//1
	glViewport(screenx / 2, 0, screenx / 2, screeny / 2);
	glLoadIdentity();
	gluLookAt(eyeX2, eyeY2, eyeZ2, centerX2, centerY2, centerZ2, upX, upY, upZ);
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
	glutKeyboardFunc(keyboardHandler);
	createMenu();
	glutMainLoop();
	return 0;
}