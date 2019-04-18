#include "pch.h"
#include <cstdlib>
#include "GL/glut.h"
#include <vector>
#include <math.h>

#define PI 3.14159265358979323846264338327950288419716939937510

using namespace std;

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

struct Point {
	float x;
	float y;
	float z;
};
vector<vector<Point>> surface(4, vector<Point>(4));

void processMenu(int option) {
	glutPostRedisplay();
}

float change = 5; //change constant
//Control Point 1
void cp1(int option) {
	switch (option) {
	case 1:
		surface[1][1].x += change;
		glutPostRedisplay();
		break;
	case 2:
		surface[1][1].x -= change;
		glutPostRedisplay();
		break;
	case 3:
		surface[1][1].y += change;
		glutPostRedisplay();
		break;
	case 4:
		surface[1][1].y -= change;
		glutPostRedisplay();
		break;
	case 5:
		surface[1][1].z += change;
		glutPostRedisplay();
		break;
	case 6:
		surface[1][1].z -= change;
		glutPostRedisplay();
		break;
	}
}
//Control Point 2
void cp2(int option) {
	switch (option) {
	case 1:
		surface[2][1].x += change;
		glutPostRedisplay();
		break;
	case 2:
		surface[2][1].x -= change;
		glutPostRedisplay();
		break;
	case 3:
		surface[2][1].y += change;
		glutPostRedisplay();
		break;
	case 4:
		surface[2][1].y -= change;
		glutPostRedisplay();
		break;
	case 5:
		surface[2][1].z += change;
		glutPostRedisplay();
		break;
	case 6:
		surface[2][1].z -= change;
		glutPostRedisplay();
		break;
	}
}
//Control Point 3
void cp3(int option) {
	switch (option) {
	case 1:
		surface[2][2].x += change;
		glutPostRedisplay();
		break;
	case 2:
		surface[2][2].x -= change;
		glutPostRedisplay();
		break;
	case 3:
		surface[2][2].y += change;
		glutPostRedisplay();
		break;
	case 4:
		surface[2][2].y -= change;
		glutPostRedisplay();
		break;
	case 5:
		surface[2][2].z += change;
		glutPostRedisplay();
		break;
	case 6:
		surface[2][2].z -= change;
		glutPostRedisplay();
		break;
	}
}
//Control Point 4
void cp4(int option) {
	switch (option) {
	case 1:
		surface[1][2].x += change;
		glutPostRedisplay();
		break;
	case 2:
		surface[1][2].x -= change;
		glutPostRedisplay();
		break;
	case 3:
		surface[1][2].y += change;
		glutPostRedisplay();
		break;
	case 4:
		surface[1][2].y -= change;
		glutPostRedisplay();
		break;
	case 5:
		surface[1][2].z += change;
		glutPostRedisplay();
		break;
	case 6:
		surface[1][2].z -= change;
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

//Did up to 30 instead of 20 for easier rounding
void createSurface() {
	for (int i = 0; i <= 30; i = i + 10) {
		for (int j = 0; j <= 30; j = j + 10) {
			Point p;
			p.x = i;
			p.y = 0;
			p.z = j;
			surface[i/10][j/10] = p;
		}
	}
}

//https:// web.cs.wpi.edu/~matt/courses/cs563/talks/surface/bez_surf.html for the theory
float B(int i, float u) {
	if (i == 0) return pow(1 - u, 3);
	if (i == 1) return 3 * u*pow(1 - u, 2);
	if (i == 2) return 3 * pow(u, 2)*(1 - u);
	if (i == 3) return pow(u, 3);
}
Point Q(float u, float v) {
	float totalx = 0;
	float totaly = 0;
	float totalz = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			totalx += surface[i][j].x * B(i, u) * B(j, v);
			totaly += surface[i][j].y * B(i, u) * B(j, v);
			totalz += surface[i][j].z * B(i, u) * B(j, v);
		}
	}
	Point p;
	p.x = totalx;
	p.y = totaly;
	p.z = totalz;
	return p;
}

Point crossProduct(Point v1, Point v2) {
	//determinant
	Point retVal;
	retVal.x = v1.y*v2.z - v1.z*v2.y;
	retVal.y = -(v1.x*v2.z - v1.z*v2.x);
	retVal.z = v1.x*v2.y - v1.y*v2.x;
	return retVal;
}

float dotProduct(Point u, Point v) {
	return u.x*v.x + u.y*v.y + u.z*v.z;
}
float vectorLength(Point u) {
	return sqrt(pow(u.x, 2)+pow(u.y, 2)+ pow(u.z, 2));
}

bool visible(Point a, Point b, Point c) {
	Point v1;
	v1.x = b.x - a.x;
	v1.y = b.y - a.y;
	v1.z = b.z - a.z;
	Point v2;
	v2.x = c.x - a.x;
	v2.y = c.y - a.y;
	v2.z = c.z - a.z;
	Point norm = crossProduct(v1, v2);
	Point view;
	view.x = xloc;
	view.y = yloc;
	view.z = zloc;
	double angle = acos(dotProduct(view, norm)/(vectorLength(view)*vectorLength(norm)));
	return angle < PI / 2;
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
	//Bezier Patch
	glColor3f(1.0, 1.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	for (float u = 0; u < 1; u = u + .05) {
		for (float v = 0; v < 1; v = v + .05) {
			Point p1 = Q(u, v);
			Point p2 = Q(u, v + .05);
			Point p3 = Q(u + .05, v + .05);
			Point p4 = Q(u + .05, v);
			if (visible(p1, p2, p4)) {
				glVertex3f(p1.x, p1.y, p1.z);
				glVertex3f(p2.x, p2.y, p2.z);
				glVertex3f(p4.x, p4.y, p4.z);
			}
			if (visible(p2, p3, p4)) {
				glVertex3f(p2.x, p2.y, p2.z);
				glVertex3f(p3.x, p3.y, p3.z);
				glVertex3f(p4.x, p4.y, p4.z);
			}
		}
	}
	glEnd();

	//glutPostRedisplay();
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
	createSurface(); //initialize the surface
	glutMainLoop();
	return 0;
}