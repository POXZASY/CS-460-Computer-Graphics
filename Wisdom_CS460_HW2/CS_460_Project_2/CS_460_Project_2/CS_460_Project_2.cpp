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
float green = 1.0;
float blue = 0.5;
vector<tuple<int, int>> polygon;
vector<tuple<float, float>> polygonPointsOpenGL;
vector<tuple<int, int>> fillPoints;
vector<tuple<int, int>> clipper = {make_tuple(125, 125), make_tuple(125, 375), make_tuple(375, 375), make_tuple(375, 125)};
bool currentlyDrawing = false;
bool closePoly = false;

tuple<float, float> glutToGLCoords(int x, int y) {
	float GLx = ((float)x) / (screenx / 2) - 1;
	float GLy = -1 * (((float)y) / (screeny / 2) - 1);
	return make_tuple(GLx, GLy);
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
vector<tuple<int, int>> clipToEdge(vector<tuple<int, int>> polygonPoints, int clipx_1, int clipy_1, int clipx_2, int clipy_2) {
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
		int pos2 = (clipx_2 - clipx_1) * (y_2 - clipy_1) - (clipy_2 - clipy_1) * (x_2 - clipx_1);
		//both inside
		if (pos1 < 0 && pos2 < 0) {
			//add second point
			newPoly.push_back(make_tuple(x_2, y_2));
		}
		//going from outside to inside
		else if (pos1 >= 0 && pos2 < 0) {
			//add intersection point and inside point
			newPoly.push_back(make_tuple(xintercept(clipx_1, clipy_1, clipx_2, clipy_2, x_1, y_1, x_2, y_2), yintercept(clipx_1, clipy_1, clipx_2, clipy_2, x_1, y_1, x_2, y_2)));
			newPoly.push_back(make_tuple(x_2, y_2));
		}
		//first point inside, second point outside
		else if (pos1 < 0 && pos2 >= 0) {
			newPoly.push_back(make_tuple(xintercept(clipx_1, clipy_1, clipx_2, clipy_2, x_1, y_1, x_2, y_2), yintercept(clipx_1, clipy_1, clipx_2, clipy_2, x_1, y_1, x_2, y_2)));
		}
	}
	return newPoly;
}

//Sutherland-Hodgman Algorithm
vector<tuple<int, int>> shAlgo(vector<tuple<int, int>> polygonPoints, vector<tuple<int, int>> clipperPoints) {
	for (int i = 0; i < clipperPoints.size(); i++) {
		int j = (i + 1) % clipperPoints.size();
		 polygonPoints = clipToEdge(polygonPoints, get<0>(clipperPoints[i]), get<1>(clipperPoints[i]), get<0>(clipperPoints[j]), get<1>(clipperPoints[j]));
	}
	return polygonPoints;
}

//Scanline Fill Algorithm
vector<tuple<int, int>> scanLineFill(vector<tuple<int, int>> polygonPoints) {
	//find all points intersected by polygon
	vector<tuple<int, int>> intersectPoints;
	for (int i = 0; i < polygonPoints.size(); i++) { //iterate through each of the sides of the polygon
		tuple<int, int> p1 = polygonPoints[i]; //first point
		tuple<int, int> p2 = polygonPoints[(i + 1) % polygonPoints.size()]; //second point
		int p1x = get<0>(p1);
		int p1y = get<1>(p1);
		int p2x = get<0>(p2);
		int p2y = get<0>(p2);
		float m = (float)(p2y - p1y) / (p2x - p1x); //slope of the line
		for (int j = min(p1x, p2x); j <= max(p1x, p2x); j++) { //iterate through all of the x values between the two points
			intersectPoints.push_back(make_tuple(j, ))
		}
	}
}

void processMenu(int option) {
	switch (option) {
	
	//Polygon Clipping
	case 1:
		polygon= shAlgo(polygon, clipper);
		break;
	//Region Filling
	case 2:
		break;
	//Window-to-Viewport Mapping
	case 3:
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
			polygon.clear();
			closePoly = false;
		}
		currentlyDrawing = true;
		polygon.push_back(make_tuple(x, y));
		//glutPostRedisplay();
	}
}

void keyBoard(unsigned char key, int x, int y) {
	if (key == 'p' || key == 'P') {
		if (currentlyDrawing) {
			currentlyDrawing = false;
			closePoly = true;
			//glutPostRedisplay();
		}
	}
}

void display() {
	glClearColor(red, green, blue, 1.0); //bg color
	glClear(GL_COLOR_BUFFER_BIT);

	//drawing the polygon
	glBegin(GL_LINES);
	bool first = true;
	//change poly points to gl coords
	polygonPointsOpenGL.clear();
	for (int i = 0; i < polygon.size(); i++) {
		polygonPointsOpenGL.push_back(glutToGLCoords(get<0>(polygon[i]), get<1>(polygon[i])));
	}
	for (tuple<float, float> t : polygonPointsOpenGL) {
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
		glVertex2f(get<0>(polygonPointsOpenGL.front()), get<1>(polygonPointsOpenGL.front()));
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
	createMenu();
	glutMainLoop();
	return 0;
}