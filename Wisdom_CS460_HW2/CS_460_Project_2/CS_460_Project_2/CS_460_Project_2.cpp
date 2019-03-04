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
int menux;
int menuy;
float red = 0.0;
float green = 1.0;
float blue = 0.0;
int clipperx1 = 200;
int clippery1 = 200;
int clipperx2 = 300;
int clippery2 = 300;
vector<tuple<int, int>> polygon;
vector<tuple<float, float>> polygonPointsOpenGL;
bool currentlyFilling = false;
vector<tuple<int, int>> fillPoints;
vector<tuple<int, int>> clipper = {make_tuple(clipperx1, clippery1), make_tuple(clipperx1, clippery2), make_tuple(clipperx2, clippery2), make_tuple(clipperx2, clippery1)};
bool currentlyDrawing = false;
bool closePoly = false;
bool findingMenuPoint = true;
bool drawingRectangle = false;
tuple<int, int> rect1;
tuple<int, int> rect2;
int rectpoint = 1;
vector<tuple<int, int>> viewport;
bool drawingViewport = false;
bool scalingViewport = false;
int newViewportx1;
int newViewporty1;
int newViewportx2;
int newViewporty2;
bool scalingWindow = false;
int newWindowx1;
int newWindowy1;
int newWindowx2;
int newWindowy2;
bool panningWindow = false;
int newPanningx1;
int newPanningy1;
int newPanningx2;
int newPanningy2;

tuple<float, float> glutToGLCoords(int x, int y) {
	float GLx = ((float)x) / ((float)screenx / 2) - 1;
	float GLy = -1 * (((float)y) / ((float)screeny / 2) - 1);
	return make_tuple(GLx, GLy);
}

tuple<int, int> GLtoGlutCoords(float x, float y) {
	int glutX = (x + 1)*(screenx / 2);
	int glutY = (-1 * y + 1)*(screeny / 2);
	return make_tuple(glutX, glutY);

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

//function to check if vector contains a point
bool contains(vector<tuple<int, int>> vec, tuple<int,int> pt) {
	for (tuple<int, int> t : vec) {
		if (t == pt) return true;
	}
	return false;
}

//Boundary Fill Algorithm
struct Color {
	public:
		GLfloat r;
		GLfloat g;
		GLfloat b;
};
bool sameColor(Color lhs, Color rhs) {
	return (lhs.r == rhs.r && lhs.b == rhs.b && lhs.g == rhs.g);
}
Color getPixelColor(int x, int y) {
	Color color;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &color);
	//cout << x << ", " << y << endl;
	//cout << color.r << ", " << color.g << ", " << color.b << endl;
	return color;
}
void boundaryFill4(int x, int y){
	if (0 <= x && x <= screenx && 0 <= y && y <= screeny) { //if point is on the screen
		Color blue;
		blue.r = 0.0; blue.g = 0.0; blue.b = 1.0;
		Color red;
		red.r = 1.0; red.g = 0.0; red.b = 0.0;
		
		//cout << sameColor(getPixelColor(x, y), black) << endl;
		if (!(sameColor(getPixelColor(x, y), blue) && !(sameColor(getPixelColor(x, y), red)))) {
			cout << x << ", " << y << endl;
			cout << getPixelColor(x,y).r << ", " << getPixelColor(x, y).g << ", " << getPixelColor(x, y).b << endl;
			glBegin(GL_POINTS);
			glColor3f(1.0, 0.0, 0.0);
			float xval = get<0>(glutToGLCoords(x, y));
			float yval = get<1>(glutToGLCoords(x, y));
			glVertex2f(xval, yval);
			glEnd();
			cout << getPixelColor(x, y).r << ", " << getPixelColor(x, y).g << ", " << getPixelColor(x, y).b << endl;
			//fillPoints.push_back(make_tuple(x, y));
			boundaryFill4(x + 1, y);
			boundaryFill4(x, y + 1);
			boundaryFill4(x - 1, y);
			boundaryFill4(x, y - 1);
		}
	}
}

//Boundary Fill Attempt 2
Color ** screen = new Color*[screenx];

void setScreen() {
	for (int i = 0; i < screenx; i++) {
		screen[i] = new Color[screeny];
	}
	for (int i = 0; i < screenx; i++) {
		//cout << i << endl;
		for (int j = 0; j < screeny; j++) {
			Color color;
			glReadPixels(i, j, 1, 1, GL_RGB, GL_FLOAT, &color);
			screen[i][j] = color;
		}
	}
}
void boundaryFill(int x, int y) {
	if (0 <= x && x <= screenx && 0 <= y && y <= screeny) { //if point is on the screen
		Color blue;
		blue.r = 0.0; blue.g = 0.0; blue.b = 1.0;
		Color red;
		red.r = 1.0; red.g = 0.0; red.b = 0.0;
		if (!(sameColor(screen[x][y], blue)) && !(sameColor(screen[x][y], red))) { //if not blue or red pixel, set red
			//cout << "Coordinates: "<< x << ", " << y << " Color: " << screen[x][y].r << ", " << screen[x][y].g << ", " << screen[x][y].b <<endl;
			screen[x][y].r = 1.0;
			screen[x][y].g = 0.0;
			screen[x][y].b = 0.0;
			boundaryFill(x + 1, y);
			boundaryFill(x, y + 1);
			boundaryFill(x - 1, y);
			boundaryFill(x, y - 1);
		}
	}
}

//Window-to-Viewport
void windowToViewport() {
	viewport.clear();

	//send values to origin
	for (tuple<int, int> t : polygon) {
		viewport.push_back(make_tuple(get<0>(t) - clipperx1, get<1>(t) - clippery1));
	}
	//dilate values
	float xfact = abs(((float)(get<0>(rect2) - get<0>(rect1))) / ((float)clipperx2 - (float)clipperx1));
	float yfact = abs(((float)(get<1>(rect2) - get<1>(rect1))) / ((float)clippery2 - (float)clippery1));
	for (int i = 0; i < viewport.size(); i++) {
		viewport[i] = make_tuple(round(get<0>(viewport[i])*xfact), round(get<1>(viewport[i])*yfact));
	}
	//send values to new rect
	for (int i = 0; i < viewport.size(); i++) {
		viewport[i] = make_tuple(get<0>(viewport[i]) + get<0>(rect1), get<1>(viewport[i]) + get<1>(rect1));
	}
}

void scaleViewport() {
	rect2 = make_tuple(get<0>(rect2) + (newViewportx2-newViewportx1), get<1>(rect2) + (newViewporty2 - newViewporty1));
	windowToViewport();
}

void scaleWindow() {
	clipperx2 = clipperx2 + (newWindowx2 - newWindowx1);
	clippery2 = clippery2 + (newWindowy2 - newWindowy1);
	clipper = { make_tuple(clipperx1, clippery1), make_tuple(clipperx1, clippery2), make_tuple(clipperx2, clippery2), make_tuple(clipperx2, clippery1) };
	polygon = shAlgo(polygon, clipper);
	windowToViewport();
}

void panWindow() {
	clipperx1 = clipperx1 + (newPanningx2 - newPanningx1);
	clippery1 = clippery1 + (newPanningy2 - newPanningy1);
	clipperx2 = clipperx2 + (newPanningx2 - newPanningx1);
	clippery2 = clippery2 + (newPanningy2 - newPanningy1);
	clipper = { make_tuple(clipperx1, clippery1), make_tuple(clipperx1, clippery2), make_tuple(clipperx2, clippery2), make_tuple(clipperx2, clippery1) };
	polygon = shAlgo(polygon, clipper);
	windowToViewport();
}

void processMenu(int option) {
	switch (option) {
	
	//Polygon Clipping
	case 1:
		polygon= shAlgo(polygon, clipper);
		glutPostRedisplay();
		findingMenuPoint = true;
		break;
	//Region Filling
	case 2:
		setScreen();
		boundaryFill(menux, menuy);
		currentlyFilling = true;
		glutPostRedisplay();
		findingMenuPoint = true;
		break;
	//Window-to-Viewport Mapping
	case 3:
		findingMenuPoint = true;
		drawingViewport = !drawingViewport;
		windowToViewport();
		glutPostRedisplay();
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
		if (drawingRectangle) {
			if (rectpoint == 1) rect1 = make_tuple(x, y);
			else if (rectpoint == 2) rect2 = make_tuple(x, y);
			rectpoint = rectpoint % 2 + 1;
			glutPostRedisplay();
		}
		else if(scalingViewport){
			newViewportx1 = x;
			newViewporty1 = y;
		}
		else if (scalingWindow) {
			newWindowx1 = x;
			newWindowy1 = y;
		}
		else if (panningWindow) {
			newPanningx1 = x;
			newPanningy1 = y;
		}
		else {
			if (!currentlyDrawing) {
				polygon.clear();
				closePoly = false;
				currentlyFilling = false;
				fillPoints.clear();
			}
			currentlyDrawing = true;
			polygon.push_back(make_tuple(x, y));
			glutPostRedisplay();
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (scalingViewport) {
			newViewportx2 = x;
			newViewporty2 = y;
			scaleViewport();
			glutPostRedisplay();
		}
		else if (scalingWindow) {
			newWindowx2 = x;
			newWindowy2 = y;
			scaleWindow();
			glutPostRedisplay();
		}
		else if (panningWindow) {
			newPanningx2 = x;
			newPanningy2 = y;
			panWindow();
			glutPostRedisplay();
		}
	}
}

void keyBoard(unsigned char key, int x, int y) {
	if (key == 'p' || key == 'P') {
		if (currentlyDrawing) {
			currentlyDrawing = false;
			closePoly = true;
			glutPostRedisplay();
		}
	}
	else if (key == 'o' || key == 'O') {
		drawingRectangle = !drawingRectangle;
		scalingViewport = false;
		scalingWindow = false;
		panningWindow = false;
	}
	else if (key == 'i' || key == 'I') {
		drawingRectangle = false;
		scalingWindow = false;
		panningWindow = false;
		scalingViewport = !scalingViewport;
	}
	else if (key == 'u' || key == 'U') {
		drawingRectangle = false;
		scalingViewport = false;
		panningWindow = false;
		scalingWindow = !scalingWindow;
	}
	else if (key == 'y' || key == 'Y') {
		drawingRectangle = false;
		scalingViewport = false;
		scalingWindow = false;
		panningWindow = !panningWindow;
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

	//Drawing the Viewport
	glLineStipple(1, 0xFF00);
	glEnable(GL_LINE_STIPPLE);

	glBegin(GL_LINES);
	glVertex2f(get<0>(glutToGLCoords(get<0>(rect1), 0)), get<1>(glutToGLCoords(0, get<1>(rect1))));
	glVertex2f(get<0>(glutToGLCoords(get<0>(rect2), 0)), get<1>(glutToGLCoords(0, get<1>(rect1))));

	glVertex2f(get<0>(glutToGLCoords(get<0>(rect2), 0)), get<1>(glutToGLCoords(0, get<1>(rect1))));
	glVertex2f(get<0>(glutToGLCoords(get<0>(rect2), 0)), get<1>(glutToGLCoords(0, get<1>(rect2))));

	glVertex2f(get<0>(glutToGLCoords(get<0>(rect2), 0)), get<1>(glutToGLCoords(0, get<1>(rect2))));
	glVertex2f(get<0>(glutToGLCoords(get<0>(rect1), 0)), get<1>(glutToGLCoords(0, get<1>(rect2))));

	glVertex2f(get<0>(glutToGLCoords(get<0>(rect1), 0)), get<1>(glutToGLCoords(0, get<1>(rect2))));
	glVertex2f(get<0>(glutToGLCoords(get<0>(rect1), 0)), get<1>(glutToGLCoords(0, get<1>(rect1))));
	glEnd();

	glDisable(GL_LINE_STIPPLE);

	//filling the polygon
	if (currentlyFilling) {
		Color red;
		red.r = 1.0; red.g = 0.0; red.b = 0.0;
		for (int i = 0; i < screenx; i++) {
			for (int j = 0; j < screeny; j++) {
				if (sameColor(screen[i][j], red)) {
					glBegin(GL_POINTS);
					glColor3f(1.0, 0.0, 0.0);
					float xval = get<0>(glutToGLCoords(i, j));
					float yval = get<1>(glutToGLCoords(i, screeny - j));
					glVertex2f(xval, yval);
					glEnd();
				}
			}
		}
	}
	glColor3f(0.0, 0.0, 1.0);

	//drawing the viewport
	if (drawingViewport) {
		glBegin(GL_LINES);
		for (int i = 0; i < viewport.size(); i++) {
			glVertex2f(get<0>(glutToGLCoords(get<0>(viewport[i]), get<1>(viewport[i]))), get<1>(glutToGLCoords(get<0>(viewport[i]), get<1>(viewport[i]))));
			glVertex2f(get<0>(glutToGLCoords(get<0>(viewport[(i + 1) % viewport.size()]), get<1>(viewport[(i + 1) % viewport.size()]))), get<1>(glutToGLCoords(get<0>(viewport[(i + 1) % viewport.size()]), get<1>(viewport[(i + 1) % viewport.size()]))));
		}
		glEnd();
	}

	//dashed lines
	glLineStipple(1, 0xFF00);
	glEnable(GL_LINE_STIPPLE);
	//draw square
	glBegin(GL_LINES);
	glVertex2f(get<0>(glutToGLCoords(clipperx1, clippery1)), get<1>(glutToGLCoords(clipperx1, clippery1)));
	glVertex2f(get<0>(glutToGLCoords(clipperx2, clippery1)), get<1>(glutToGLCoords(clipperx2, clippery1)));
	glVertex2f(get<0>(glutToGLCoords(clipperx2, clippery1)), get<1>(glutToGLCoords(clipperx2, clippery1)));
	glVertex2f(get<0>(glutToGLCoords(clipperx2, clippery2)), get<1>(glutToGLCoords(clipperx2, clippery2)));
	glVertex2f(get<0>(glutToGLCoords(clipperx2, clippery2)), get<1>(glutToGLCoords(clipperx2, clippery2)));
	glVertex2f(get<0>(glutToGLCoords(clipperx1, clippery2)), get<1>(glutToGLCoords(clipperx1, clippery2)));
	glVertex2f(get<0>(glutToGLCoords(clipperx1, clippery2)), get<1>(glutToGLCoords(clipperx1, clippery2)));
	glVertex2f(get<0>(glutToGLCoords(clipperx1, clippery1)), get<1>(glutToGLCoords(clipperx1, clippery1)));
	glEnd();
	//glPopAttrib();
	glDisable(GL_LINE_STIPPLE);

	//glutReshapeWindow(screenx, screeny);
	glFlush();
}
void menuFunc(int status, int x, int y) {
	if (GLUT_MENU_IN_USE && findingMenuPoint) {
		menux = x;
		menuy = y;
		findingMenuPoint = false;
	}
}

void mousePos(int x, int y) {
	//cout << "Red: " << getPixelColor(x, y).r << " Green: " << getPixelColor(x, y).g << " Blue: "<< getPixelColor(x, y).b << endl;
}
int main(int argc, char** argv) {
	glutInit(&argc, argv); //Initialize glut
	glutInitWindowSize(screenx, screeny);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Project 2");
	glutDisplayFunc(display);
	glutMouseFunc(mouseHandler);
	glutKeyboardFunc(keyBoard);
	glutPassiveMotionFunc(mousePos);
	glutMenuStatusFunc(menuFunc);
	createMenu();
	glutMainLoop();
	/*
	for (int i = 0; i < screenx; i++) {
		delete[] screen[i];
	}
	delete[] screen;
	*/
	return 0;
}