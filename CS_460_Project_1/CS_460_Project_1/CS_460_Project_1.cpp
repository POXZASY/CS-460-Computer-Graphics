#include <Windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <cstdlib>

using namespace std;

bool lineCreation = false;
float linex1; //beginning of the line x
float liney1; //beginning of the line y
float linex2; //end of the line x
float liney2; //end of the line y
float mousex; //opengl
float mousey; //opengl
int mousexglut; //glut
int mouseyglut; //glut
int screenx = 1000;
int screeny = 1000;

vector<tuple <int, int>> lines; //midpoint algo
vector<tuple <int, int>> lines_current; //midpoint algo
vector<tuple <int, int>> bren; //brensenham algo, ints only
tuple<float, float> glutToGLCoords(int x, int y) {
	float GLx = ((float)x) / (screenx / 2) - 1;
	float GLy = -1 * (((float)y) / (screeny / 2) - 1);
	return make_tuple(GLx, GLy);
}
void brensenhamneg(int x_1, int y_1, int x_2, int y_2) {
	int dx = x_2 - x_1;
	int dy = y_2 - y_1;
	int ysign = 1;
	if (dy < 0) {
		ysign = -1;
		dy = -dy;
	}
	int D = 2 * dy - dx;
	int y = y_1;
	for (int x = x_1; x <= x_2; x++) {
		bren.push_back(make_tuple(x, y));
		if (D > 0) {
			y = y + ysign;
			D = D - 2 * dx;
		}
		D = D + 2 * dy;
	}
}
void brensenhamabove1(int x_1, int y_1, int x_2, int y_2) {
	int dx = x_2 - x_1;
	int dy = y_2 - y_1;
	int xsign = 1;
	if (dx < 0) {
		xsign = -1;
		dx = -dx;
	}
	int D = 2 * dx - dy;
	int x = x_1;
	for (int y = y_1; y <= y_2; y++) {
		bren.push_back(make_tuple(x, y));
		if (D > 0) {
			x = x + xsign;
			D = D - 2 * dy;
		}
		D = D + 2 * dx;
	}
}
void brensenham(int x_1, int y_1, int x_2, int y_2) {
	if (abs(y_2 - y_1) < abs(x_2 - x_1)) {
		if (x_1 > x_2) {
			brensenhamneg(x_2, y_2, x_1, y_1);
		}
		else {
			brensenhamneg(x_1, y_1, x_2, y_2);
		}
	}
	else {
		if (y_1 > y_2) {
			brensenhamabove1(x_2, y_2, x_1, y_1);
		}
		else {
			brensenhamabove1(x_1, y_1, x_2, y_2);
		}
	}
}
void midpoint(int x_1, int y_1, int x_2, int y_2) {
	//---------------------------------------------
	//adjust variables to first octant
	bool reflecty = false;
	bool flipyx = false;
	//if the second point is behind the first, flip them
	if (x_2 < x_1) {
		int temp = x_1;
		x_1 = x_2;
		x_2 = temp;
		temp = y_1;
		y_1 = y_2;
		y_2 = temp;
	}
	//bring the points to the origin to prepare for surgery
	int tempx = x_1;
	int tempy = y_1;
	x_1 = x_1 - tempx;
	y_1 = y_1 - tempy;
	x_2 = x_2 - tempx;
	y_2 = y_2 - tempy;
	//if the second point is below the first
	if (y_2 - y_1 < 0) {
		y_2 = -1 * y_2;
		reflecty = true;
	}
	int dx = x_2 - x_1;
	int dy = y_2 - y_1;
	float m = (float)dy / (float)dx;
	//if the line is above y=x
	if (m > 1) {
		int temp = x_2;
		x_2 = y_2;
		y_2 = temp;
		flipyx = true;
	}
	//initialize variables
	dx = x_2 - x_1;
	dy = y_2 - y_1;
	int D = 2 * dy - dx;
	int deltaE = 2 * dy;
	int deltaNE = 2 * (dy - dx);
	int x = x_1;
	int y = y_1;
	//---------------------------------------------
	//ALGORITHM
	vector<tuple<int, int>> points;
	points.push_back(make_tuple(x_1, y_1));
	while (x < x_2) {
		if (D <= 0) {
			D = D + deltaE;
			x++;
		}
		else {
			D = D + deltaNE;
			x++;
			y++;
		}
		points.push_back(make_tuple(x, y));
	}
	//---------------------------------------------
	//Convert Back
	for (tuple<int, int> t : points) {
		tuple<int, int> temp = t;
		if (flipyx) {
			temp = make_tuple(get<1>(temp), get<0>(temp));
		}
		if (reflecty) {
			temp = make_tuple(get<0>(temp), -1 * get<1>(temp));
		}
		temp = make_tuple(get<0>(temp) + tempx, get<1>(temp) + tempy);
		lines.push_back(temp);
	}
}
void midpoint_current(int x_1, int y_1, int x_2, int y_2) {
	//---------------------------------------------
	//adjust variables to first octant
	bool reflecty = false;
	bool flipyx = false;
	//if the second point is behind the first, flip them
	if (x_2 < x_1) {
		int temp = x_1;
		x_1 = x_2;
		x_2 = temp;
		temp = y_1;
		y_1 = y_2;
		y_2 = temp;
	}
	//bring the points to the origin to prepare for surgery
	int tempx = x_1;
	int tempy = y_1;
	x_1 = x_1 - tempx;
	y_1 = y_1 - tempy;
	x_2 = x_2 - tempx;
	y_2 = y_2 - tempy;
	//if the second point is below the first
	if (y_2 - y_1 < 0) {
		y_2 = -1 * y_2;
		reflecty = true;
	}
	int dx = x_2 - x_1;
	int dy = y_2 - y_1;
	float m = (float)dy / (float)dx;
	//if the line is above y=x
	if (m > 1) {
		int temp = x_2;
		x_2 = y_2;
		y_2 = temp;
		flipyx = true;
	}
	//initialize variables
	dx = x_2 - x_1;
	dy = y_2 - y_1;
	int D = 2 * dy - dx;
	int deltaE = 2 * dy;
	int deltaNE = 2 * (dy - dx);
	int x = x_1;
	int y = y_1;
	//---------------------------------------------
	//ALGORITHM
	vector<tuple<int, int>> points;
	points.push_back(make_tuple(x_1, y_1));
	while (x < x_2) {
		if (D <= 0) {
			D = D + deltaE;
			x++;
		}
		else {
			D = D + deltaNE;
			x++;
			y++;
		}
		points.push_back(make_tuple(x, y));
	}
	//---------------------------------------------
	//Convert Back
	for (tuple<int, int> t : points) {
		tuple<int, int> temp = t;
		if (flipyx) {
			temp = make_tuple(get<1>(temp), get<0>(temp));
		}
		if (reflecty) {
			temp = make_tuple(get<0>(temp), -1 * get<1>(temp));
		}
		temp = make_tuple(get<0>(temp) + tempx, get<1>(temp) + tempy);
		lines_current.push_back(temp);
	}
}
void display() {
	glClearColor(1.0, 1.0, 1.0, 1.0); //bg color
	glClear(GL_COLOR_BUFFER_BIT); // clear the color buffer??
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	//W
	glVertex2f(0, .5);
	glVertex2f(.05, .4);

	glVertex2f(.05, .4);
	glVertex2f(.1, .5);

	glVertex2f(.1, .5);
	glVertex2f(.15, .4);

	glVertex2f(.15, .4);
	glVertex2f(.2, .5);
	//A
	glVertex2f(.25, .4);
	glVertex2f(.3, .5);

	glVertex2f(.3, .5);
	glVertex2f(.35, .4);

	glVertex2f(.275, .45);
	glVertex2f(.325, .45);
	//K
	glVertex2f(.4, .4);
	glVertex2f(.4, .5);

	glVertex2f(.4, .45);
	glVertex2f(.5, .5);

	glVertex2f(.4, .45);
	glVertex2f(.5, .4);
	//E
	glVertex2f(.55, .4);
	glVertex2f(.55, .5);

	glVertex2f(.55, .45);
	glVertex2f(.65, .45);

	glVertex2f(.55, .5);
	glVertex2f(.65, .5);

	glVertex2f(.55, .4);
	glVertex2f(.65, .4);
	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	//Already drawn lines
	for (tuple<int, int> t : lines) {
		glBegin(GL_POINTS);
		glVertex2f(get<0>(glutToGLCoords(get<0>(t), get<1>(t))), get<1>(glutToGLCoords(get<0>(t), get<1>(t))));
		glEnd();
	}
	//Current Line
	lines_current.clear();
	if (lineCreation) {
		midpoint_current(linex1, liney1, mousexglut, mouseyglut);
		for (tuple<int, int> t : lines_current) {
			glBegin(GL_POINTS);
			glVertex2f(get<0>(glutToGLCoords(get<0>(t), get<1>(t))), get<1>(glutToGLCoords(get<0>(t), get<1>(t))));
			glEnd();
		}
	}
	//XYZ Coordinate System
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	//Y
	glVertex2f(.5, -.4);
	glVertex2f(.5, 0);
	glVertex2f(.5, .05);
	glVertex2f(.5, .1);
	glVertex2f(.5, .1);
	glVertex2f(.475, .15);
	glVertex2f(.5, .1);
	glVertex2f(.525, .15);
	//X
	glVertex2f(.5, -.4);
	glVertex2f(.9, -.4);
	glVertex2f(.925, -.35);
	glVertex2f(.975, -.45);
	glVertex2f(.925, -.45);
	glVertex2f(.975, -.35);
	//Z
	glVertex2f(.5, -.4);
	glVertex2f(.2, -.65);
	glVertex2f(.175, -.7);
	glVertex2f(.225, -.7);
	glVertex2f(.225, -.7);
	glVertex2f(.175, -.775);
	glVertex2f(.175, -.775);
	glVertex2f(.225, -.775);

	glEnd();

	//Brensenham's Algorithm

	//W
	brensenham(100, 100, 125, 150);
	brensenham(125, 150, 150, 100);
	brensenham(150, 100, 175, 150);
	brensenham(175, 150, 200, 100);
	//A
	brensenham(225, 150, 250, 100);
	brensenham(250, 100, 275, 150);
	brensenham(237.5, 125, 262.5, 125);
	//K
	brensenham(300, 100, 300, 150);
	brensenham(300, 125, 325, 100);
	brensenham(300, 125, 325, 150);
	//E
	brensenham(350, 100, 350, 150);
	brensenham(350, 100, 400, 100);
	brensenham(350, 125, 400, 125);
	brensenham(350, 150, 400, 150);

	//XYZ
	brensenham(250, 700, 250, 500);
	brensenham(250, 700, 450, 700);
	brensenham(250, 700, 150, 800);
	brensenham(475, 675, 525, 725);
	brensenham(475, 725, 525, 675);
	brensenham(250, 475, 250, 435);
	brensenham(250, 435, 225, 400);
	brensenham(250, 435, 275, 400);
	brensenham(125, 825, 175, 825);
	brensenham(175, 825, 125, 875);
	brensenham(125, 875, 175, 875);
	
	glBegin(GL_POINTS);
	glColor3f(0.0, 1.0, 0.0);
	for (tuple<int, int> t : bren) {
		float tempx = get<0>(glutToGLCoords(get<0>(t), get<1>(t)));
		float tempy = get<1>(glutToGLCoords(get<0>(t), get<1>(t)));
		glVertex2f(tempx, tempy);
	}
	glEnd();

	glutReshapeWindow(screenx, screeny);
	glFlush();//render
	glutPostRedisplay();
}
void mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (!lineCreation) { //if we haven't started drawing lines
			lines.clear();
			lineCreation = true;
			linex1 = mousexglut;
			liney1 = mouseyglut;
		}
		else { //if we are already drawing lines
			linex2 = mousexglut; 
			liney2 = mouseyglut;
			midpoint(linex1, liney1, linex2, liney2);
			linex1 = linex2;
			liney1 = liney2;
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		linex2 = mousexglut;
		liney2 = mouseyglut;
		midpoint(linex1, liney1, linex2, liney2);
		lineCreation = false;
	}
}
void mousePos(int x, int y) { //convert glut to opengl coords
	mousexglut = x;
	mouseyglut = y;
	mousex = ((float)x) / (screenx / 2) - 1;
	mousey = -1*(((float)y) / (screeny / 2) - 1);
	//cout << x << ", " << y << endl;
}
int main(int argc, char** argv) {
	glutInit(&argc, argv); //Initialize glut
	glutCreateWindow("Project 1");
	glutInitWindowSize(screenx, screeny);
	glutInitWindowPosition(50, 50);
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(mousePos);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}