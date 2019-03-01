#include "pch.h"
#include "Windows.h"
#include "GL/glut.h"
#include <vector>
#include <tuple>
#include <iostream>

using namespace std;

int screenx = 500;
int screeny = 500;
float red = 0.0;
float green = 0.0;
float blue = 0.0;
vector<tuple<float, float>> polygonPoints;
bool currentlyDrawing = false;
bool closePoly = false;

tuple<float, float> glutToGLCoords(int x, int y) {
	float GLx = ((float)x) / (screenx / 2) - 1;
	float GLy = -1 * (((float)y) / (screeny / 2) - 1);
	return make_tuple(GLx, GLy);
}

void processMenu(int option) {
	switch (option) {
	case 1:
		red = 1.0;
		blue = 0.0;
		green = 0.0;
		break;
	case 2:
		red = 0.0;
		blue = 1.0;
		green = 0.0;
		break;
	case 3:
		red = 0.0;
		blue = 0.0;
		green = 1.0;
		break;
	}
}

void createMenu() {
	int menu;
	menu = glutCreateMenu(processMenu);
	glutAddMenuEntry("Polygon Clipping", 1);
	glutAddMenuEntry("Region Filling", 2);
	glutAddMenuEntry("Window-to-Viewport Mapping", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void mouseHandler(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (!currentlyDrawing) {
			polygonPoints.clear();
			closePoly = false;
		}
		currentlyDrawing = true;
		polygonPoints.push_back(glutToGLCoords(x, y));
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		if (currentlyDrawing) {
			currentlyDrawing = false;
			closePoly = true;
		}
	}
}

void display() {
	createMenu();
	glClearColor(red, green, blue, 1.0); //bg color
	glClear(GL_COLOR_BUFFER_BIT);

	//drawing the polygon
	glBegin(GL_LINES);
	bool first = true;
	for (tuple<float, float> t : polygonPoints) {
		if (first) {
			glVertex2f(get<0>(t), get<1>(t));
			first = false;
		}
		else {
			glVertex2f(get<0>(t), get<1>(t));
			glVertex2f(get<0>(t), get<1>(t));
		}
	}
	if (closePoly) {
		glVertex2f(get<0>(polygonPoints.front()), get<1>(polygonPoints.front()));
	}
	glEnd();

	//dashed lines
	glLineStipple(1, 0xFF00);
	glEnable(GL_LINE_STIPPLE);
	//draw square
	glBegin(GL_LINES);
	glVertex2f(-.5, .5);
	glVertex2f(-.5, -.5);
	glVertex2f(-.5, -.5);
	glVertex2f(.5, -.5);
	glVertex2f(.5, -.5);
	glVertex2f(.5, .5);
	glVertex2f(.5, .5);
	glVertex2f(-.5, .5);
	glEnd();
	//glPopAttrib();
	glDisable(GL_LINE_STIPPLE);

	glutReshapeWindow(500, 500);
	glFlush();
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv); //Initialize glut
	glutCreateWindow("Project 2");
	glutInitWindowSize(screenx, screeny);
	glutInitWindowPosition(50, 50);
	glutDisplayFunc(display);
	glutMouseFunc(mouseHandler);
	glutMainLoop();
	return 0;
}