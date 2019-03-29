#include "pch.h"
#include <iostream>
#include "GL/glut.h"
#include <vector>
#include <fstream>

using namespace std;

int screenx = 500;
int screeny = 500;
//bg color
float red = 0.0;
float green = 0.0;
float blue = 0.0;


struct Color {
	public:
		unsigned char r;
		unsigned char g;
		unsigned char b;
};

vector<vector<Color>> image;
vector<vector<unsigned char>> red;
vector<vector<unsigned char>> blue;
vector<vector<unsigned char>> green;
int width;
int height;

unsigned char* readBMP(char* filename) {
	FILE* img = fopen(filename, "rb");
	//get rid of the .bmp header
	unsigned char header[54];
	fread(header, sizeof(unsigned char), 54, img);
	//get width, height
	width = (int)(header[18]);
	height = (int)(header[22]);
	int padding = 0; 
	
}



void display() {
	glClearColor(red, green, blue, 0);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(-.5, -.5, 0);
	glVertex3f(-.5, .5, 0);
	glVertex3f(.5, .5, 0);
	glVertex3f(.5, -.5, 0);
	glEnd();
	readBMP("flower.bmp");
	for (int i = 0; i < 100; i++) {
		cout << buffer[i] << endl;
	}
	//glutPostRedisplay();
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
	glutMainLoop();
	return 0;
}