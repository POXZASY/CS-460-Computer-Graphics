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
float lightx = 20.0;
float lighty = 5.0;
float lightz = 20.0;
bool useWireframe = true;
bool flatLighting = true;
float diffuseDelta = 0.1;
float shinyVal = 51.0;

//B-Spline
bool bSplineOn = false;    
bool mouseCurrentlyDown = false;
int selectedPoint = -1;
bool foundPoint = false;

struct Point {
	float x;
	float y;
	float z;
};

vector<Point> controlpoints;

void bSplineInit() {
	Point cp1;
	cp1.x = 400; cp1.y = 400; cp1.z = 0;
	Point cp2;
	cp2.x = 100; cp2.y = 400; cp2.z = 0;
	Point cp3;
	cp3.x = 100; cp3.y = 250; cp3.z = 0;
	Point cp4;
	cp4.x = 400; cp4.y = 250; cp4.z = 0;
	Point cp5;
	cp5.x = 400; cp5.y = 100; cp5.z = 0;
	Point cp6;
	cp6.x = 100; cp6.y = 100; cp6.z = 0;
	controlpoints.clear();
	controlpoints.push_back(cp1);
	controlpoints.push_back(cp2);
	controlpoints.push_back(cp3);
	controlpoints.push_back(cp4);
	controlpoints.push_back(cp5);
	controlpoints.push_back(cp6);
}

vector<vector<Point>> surface(4, vector<Point>(4));

void processMenu(int option) {
	switch (option) {
	case 1:
		useWireframe = !useWireframe;
		break;
	case 2:
		flatLighting = !flatLighting;
		break;
	case 3:
		bSplineOn = !bSplineOn;
		if (bSplineOn) bSplineInit();
		break;
	}
	glutPostRedisplay();
}

float change = 30; //change constant
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
	int lightconst = 5;
	switch (option) {
	case 1:
		lightx = lightx + lightconst;
		glutPostRedisplay();
		break;
	case 2:
		lightx = lightx - lightconst;
		glutPostRedisplay();
		break;
	
	case 3:
		lighty = lighty + lightconst;
		glutPostRedisplay();
		break;
	case 4:
		lighty = lighty - lightconst;
		glutPostRedisplay();
		break;

	case 5:
		lightz = lightz + lightconst;
		glutPostRedisplay();
		break;
	case 6:
		lightz = lightz - lightconst;
		glutPostRedisplay();
		break;
		}
}
void diffuseMenu(int option) {
	switch (option) {
	case 1:
		diffuseDelta += 0.1;
		break;
	case 2:
		diffuseDelta -= 0.1;
		break;
	}
}
void specularMenu(int option) {
	switch (option) {
	case 1:
		shinyVal += 25;
		break;
	case 2:
		shinyVal -= 25;
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
	glutAddMenuEntry("X+", 1);
	glutAddMenuEntry("X-", 2);
	glutAddMenuEntry("Y+", 3);
	glutAddMenuEntry("Y-", 4);
	glutAddMenuEntry("Z+", 5);
	glutAddMenuEntry("Z-", 6);
	int diffuse = glutCreateMenu(diffuseMenu);
	glutAddMenuEntry("+", 1);
	glutAddMenuEntry("-", 2);
	int specular = glutCreateMenu(specularMenu);
	glutAddMenuEntry("+", 1);
	glutAddMenuEntry("-", 2);
	//Main Menu
	int menu = glutCreateMenu(processMenu);
	glutAddSubMenu("Control Point 1", menu1);
	glutAddSubMenu("Control Point 2", menu2);
	glutAddSubMenu("Control Point 3", menu3);
	glutAddSubMenu("Control Point 4", menu4);
	glutAddSubMenu("Zoom", zoom);
	glutAddSubMenu("Light Shift", light);
	glutAddMenuEntry("Surface", 1);
	glutAddMenuEntry("Flat/Smooth Shading", 2);
	glutAddSubMenu("Diffuse", diffuse);
	glutAddSubMenu("Specular", specular);
	glutAddMenuEntry("B-Spline", 3);
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

Point normalVec(Point a, Point b, Point c) {
	Point v1;
	v1.x = b.x - a.x;
	v1.y = b.y - a.y;
	v1.z = b.z - a.z;
	Point v2;
	v2.x = c.x - a.x;
	v2.y = c.y - a.y;
	v2.z = c.z - a.z;
	return crossProduct(v1, v2);
}

bool visible(Point a, Point b, Point c) {
	Point norm = normalVec(a, b, c);
	Point view;
	view.x = xloc;
	view.y = yloc;
	view.z = zloc;
	double angle = acos(dotProduct(view, norm)/(vectorLength(view)*vectorLength(norm)));
	return angle < PI / 2;
}

void lightInit() {

	//Lighting
	if (flatLighting) glShadeModel(GL_FLAT);
	else glShadeModel(GL_SMOOTH);

	////////////////

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);



	GLfloat material_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat material_diffuse[] = { 0.7 + diffuseDelta, 0.0 + diffuseDelta, 0.7 + diffuseDelta, 1.0 };
	GLfloat material_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat material_shininess[] = { shinyVal };

	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);

	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat light_specular[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat light_position[] = { lightx, lighty, lightz, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

}

float dist(Point a, Point b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

int radius = 5;

void drawPoint(Point p, int numtriangles) {
	float angleinc = (float)2 * PI / ((float)numtriangles);
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < numtriangles; i++) {
		glVertex3f(p.x + radius*cos(i*angleinc) , p.y + radius * sin(i*angleinc), 0);
		glVertex3f(p.x + radius * cos((i+1)*angleinc), p.y + radius * sin((i+1)*angleinc), 0);
		glVertex3f(p.x, p.y, 0);
	}
	glEnd();
}

//0 to 1 (parametric)
vector<float> knotVector;
void populateKnotVector() {
	float inc = 1.0 / (controlpoints.size() + 4 - 1);
	float temp = 0;
	for (int i = 0; i <= controlpoints.size()-1 + 4; i++) { //"4" -> cubic
		knotVector.push_back(temp);
		temp += inc;
	}
}


float bSplineBasis(int i, int j, float t) {
	if (j == 0) {
		if (knotVector[i] <= t && t < knotVector[i + 1]) return 1;
		return 0;
	}
	return ((t - knotVector[i]) / (knotVector[i + j] - knotVector[i])) * bSplineBasis(i, j - 1, t) + ((knotVector[i + j + 1] - t) / (knotVector[i + j + 1] - knotVector[i + 1])) * bSplineBasis(i + 1, j - 1, t);
}

Point createBSplinePoint(float t) {
	populateKnotVector();
	Point p;
	p.z = 0;
	//X
	float sumx = 0;
	for (int i = 0; i < controlpoints.size(); i++) {
		sumx += controlpoints[i].x * bSplineBasis(i, 3, t);
	}
	p.x = sumx;
	//Y
	float sumy = 0;
	for (int i = 0; i < controlpoints.size(); i++) {
		sumy += controlpoints[i].y * bSplineBasis(i, 3, t);
	}
	p.y = sumy;
	return p;
}

void drawBSpline() {
	for (Point p : controlpoints) {
		drawPoint(p, 16);
	}
	glBegin(GL_LINES);
	for (float f = 0.0; f <= 1; f+=.01) {
		Point p1 = createBSplinePoint(f);
		Point p2 = createBSplinePoint(f+.01);
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
	}
	glEnd();
}


void display() {
	glClearColor(red, green, blue, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (bSplineOn) {
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glEnable(GL_NORMALIZE);

		//0 to 500
		glViewport(0, 0, screenx, screeny);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90, ((float)screenx / screeny), 0.01, 500);
		gluLookAt(screenx/2, screeny/2, screenx/2, screenx/2, screeny/2, 0, 0, 1, 0);

		drawBSpline();
	}
	else {

		lightInit();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glEnable(GL_NORMALIZE);

		glViewport(0, 0, screenx, screeny);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, ((float)screenx / screeny), 0.01, 500);
		gluLookAt(xloc, yloc, zloc, 0, 0, 0, 0, 1, 0);



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

		if (useWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 1.0);
		for (float u = 0; u < 1; u = u + .05) {
			for (float v = 0; v < 1; v = v + .05) {
				Point p1 = Q(u, v);
				Point p2 = Q(u, v + .05);
				Point p3 = Q(u + .05, v + .05);
				Point p4 = Q(u + .05, v);
				if (visible(p1, p2, p4)) {
					Point norm = normalVec(p1, p2, p4);
					glNormal3f(norm.x, norm.y, norm.z);
					glVertex3f(p1.x, p1.y, p1.z);
					glNormal3f(norm.x, norm.y, norm.z);
					glVertex3f(p2.x, p2.y, p2.z);
					glNormal3f(norm.x, norm.y, norm.z);
					glVertex3f(p4.x, p4.y, p4.z);
				}
				if (visible(p2, p3, p4)) {
					Point norm = normalVec(p2, p3, p4);
					glNormal3f(norm.x, norm.y, norm.z);
					glVertex3f(p2.x, p2.y, p2.z);
					glNormal3f(norm.x, norm.y, norm.z);
					glVertex3f(p3.x, p3.y, p3.z);
					glNormal3f(norm.x, norm.y, norm.z);
					glVertex3f(p4.x, p4.y, p4.z);
				}
			}
		}
		glEnd();
	}
	
	glutPostRedisplay();
	glFlush();
}

void mouseHandler(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (!mouseCurrentlyDown) {
			Point temp;
			temp.x = x;
			temp.y = screeny-y;
			temp.z = 0;
			for (int i = 0; i < controlpoints.size(); i++) {
				if (dist(temp, controlpoints[i]) <= radius) {
					selectedPoint = i + 1;
					foundPoint = true;
					break;
				}
			}
			mouseCurrentlyDown = true;
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		mouseCurrentlyDown = false;
		foundPoint = false;
	}
}

void mousePos(int x, int y) {
	if (mouseCurrentlyDown && foundPoint) {
		controlpoints[selectedPoint - 1].x=x;
		controlpoints[selectedPoint - 1].y=screeny-y;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(screenx, screeny);
	glutInitWindowPosition(50, 50);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Project 5");
	glutDisplayFunc(display);
	glutMouseFunc(mouseHandler);
	glutMotionFunc(mousePos);
	//glutMouseFunc(mouseHandler);
	//glutKeyboardFunc(keyboardHandler);
	createMenu();
	createSurface(); //initialize the surface
	glutMainLoop();
	return 0;
}