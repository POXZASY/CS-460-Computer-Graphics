#include "pch.h"
#include "Windows.h"
#include "GL/glut.h"
#include <vector>
#include <tuple>
#include <iostream>
#include <math.h>

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
}

void keyBoard(unsigned char key, int x, int y) {
	if (key == 'p' || key == 'P') {
		if (currentlyDrawing) {
			currentlyDrawing = false;
			closePoly = true;
		}
	}
}


//returns x-intercept between two lines, uses glut coords
int xintercept(int x_1, int y_1, int x_2, int y_2, int x_3, int y_3, int x_4, int y_4) {
	int numerator = (x_1 * y_2 - y_1 * x_2) * (x_3 - x_4) - (x_1 - x_2) * (x_3 * y_4 - y_3 * x_4);
	int denominator = (x_1 - x_2) * (y_3 - y_4) - (y_1 - y_2) * (x_3 - x_4);
	return round(numerator / denominator);
}

//returns y-intercept between two lines, uses glut coords
int yintercept(int x_1, int y_1, int x_2, int y_2, int x_3, int y_3, int x_4, int y_4) {
	int numerator = (x_1*y_2 - y_1 * x_2) * (y_3 - y_4) - (y_1 - y_2) * (x_3*y_4 - y_3 * x_4);
	int denominator = (x_1 - x_2) * (y_3 - y_4) - (y_1 - y_2) * (x_3 - x_4);
	return round(numerator / denominator);
}

//performs one clip of the Sutherland-Hodgman algorithm
void clipToEdge(vector<tuple<int, int>> polygonPoints, int clipx_1, int clipy_1, int clipx_2, int clipy_2) {
	vector<tuple<int, int>> newPoly;
	for (int i = 0; i < polygonPoints.size(); i++) {
		int j = (i + 1) % polygonPoints.size();
		//points of the polygon line
		int x_1 = get<0>(polygonPoints[i]);
		int y_1 = get<1>(polygonPoints[i]);
		int x_2 = get<0>(polygonPoints[j]);
		int y_2 = get<1>(polygonPoints[j]);

		//finding location of points with respect to the line (inside or outside)
		int pos1 = (clipx_2 - clipx_1) * (y_1 - clipy_1) - (clipy_2 - clipy_1) * (x_1 - clipx_1);
		int pos2 = (clipx_2 - clipx_1) * (y_2 - clipy_1) - (clipy_2 - clipy_1)
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
	glutKeyboardFunc(keyBoard);
	glutMainLoop();
	return 0;
}