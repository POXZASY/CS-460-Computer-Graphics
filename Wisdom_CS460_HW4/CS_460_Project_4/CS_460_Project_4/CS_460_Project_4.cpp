#include "pch.h"
#include <iostream>
#include "GL/glut.h"
#include <vector>
#include <fstream>
#include <math.h>

using namespace std;

int screenx = 500;
int screeny = 500;
//bg color
float red = 0.0;
float green = 0.0;
float blue = 0.0;

bool start = true;

bool mouseCurrentlyDown = false;
bool modifiedImage = false;
int initialMouseX;
int initialMouseY;
int changeX;
int changeY;

const int width = 384;
const int height = 256;

unsigned char image[width * height * 3];


void readBMP(const char * filename) {
	FILE* img = fopen(filename, "rb");

	//get rid of the .bmp header
	unsigned char header[54];
	fread(header, sizeof(unsigned char), 54, img);
	//get width, height
	//width = (int)(header[18]);
	//height = (int)(header[22]);

	fread(image, sizeof(unsigned char), width*height*3, img);
	fclose(img);
	
}

struct Color {
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

vector<vector<Color>> structimageq1(width, vector<Color>(height));
vector<vector<Color>> structimageq2(width, vector<Color>(height));
vector<vector<Color>> structimageq3(width, vector<Color>(height));
vector<vector<Color>> structimageq4(width, vector<Color>(height));

void bitsToStructs() {
	int inc = 0;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (i < width / 2) { //left half
				if (j < height / 2) {//3rd quadrant
					Color color;
					color.b = image[inc];
					inc++;
					color.g = image[inc];
					inc++;
					color.r = image[inc];
					inc++;
					structimageq3[i][j] = color;
				}
				else { //2nd quadrant
					Color color;
					color.b = image[inc];
					inc++;
					color.g = image[inc];
					inc++;
					color.r = image[inc];
					inc++;
					structimageq2[i][j] = color;
				}
			}
			else { //right half
				if (j < height / 2) {//4th quadrant
					Color color;
					color.b = image[inc];
					inc++;
					color.g = image[inc];
					inc++;
					color.r = image[inc];
					inc++;
					structimageq4[i][j] = color;
				}
				else { //1st quadrant
					Color color;
					color.b = image[inc];
					inc++;
					color.g = image[inc];
					inc++;
					color.r = image[inc];
					inc++;
					structimageq1[i][j] = color;
				}
			}
		}
	}
}

void morphImage() {
	vector<vector<Color>> structimageq1_new(width, vector<Color>(height));
	vector<vector<Color>> structimageq2_new(width, vector<Color>(height));
	vector<vector<Color>> structimageq3_new(width, vector<Color>(height));
	vector<vector<Color>> structimageq4_new(width, vector<Color>(height));

	//Quadrant 1
	float xratio1 = (float)(width / 2 - changeX) / (width / 2);
	float yratio1 = (float)(height / 2 - changeY) / (height / 2);
	//Quadrant 2
	float xratio2 = (float)(width / 2 + changeX) / (width / 2);
	float yratio2 = (float)(height / 2 - changeY) / (height / 2);
	//Quadrant 3
	float xratio3 = (float)(width / 2 + changeX) / (width / 2);
	float yratio3 = (float)(height / 2 + changeY) / (height / 2);
	//Quadrant 4
	float xratio4 = (float)(width / 2 - changeX) / (width / 2);
	float yratio4 = (float)(height / 2 + changeY) / (height / 2);

	//Initial Point Translation
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (i < width / 2) { //left half
				if (j < height / 2) {//3rd quadrant
					cout << round(i*xratio3) << ", " << round(j*yratio3) << endl;
					structimageq3_new[round(i*xratio3)][round(j*yratio3)] = structimageq3[i][j];
				}
				else { //2nd quadrant
					structimageq2_new[round(i*xratio2)][round((j-height/2)*yratio2 + height/2)] = structimageq2[i][j];
				}
			}
			else { //right half
				if (j < height / 2) {//4th quadrant
					structimageq4_new[round(i*xratio4)][round(j*yratio4)] = structimageq4[i][j];
				}
				else { //1st quadrant
					structimageq1_new[round((i - width/2)*xratio1 + width/2)][round(j*yratio1)] = structimageq1[i][j];
				}
			}
		}
	}


	//INTERPOLATION




	//structimageq1 = structimageq1_new;
	//structimageq2 = structimageq2_new;
	structimageq3 = structimageq3_new;
	//structimageq4 = structimageq4_new;

}

void structsToBits() {
	int inc = 0;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (i < width / 2 + changeX) { //left half
				if (j < height / 2 + changeY) {//3rd quadrant
					image[inc] = structimageq3[i][j].b;
					inc++;
					image[inc] = structimageq3[i][j].g;
					inc++;
					image[inc] = structimageq3[i][j].r;
					inc++;
				}
				else { //2nd quadrant
					image[inc] = structimageq2[i][j].b;
					inc++;
					image[inc] = structimageq2[i][j].g;
					inc++;
					image[inc] = structimageq2[i][j].r;
					inc++;
				}
			}
			else { //right half
				if (j < height / 2 + changeY) {//4th quadrant
					image[inc] = structimageq4[i][j].b;
					inc++;
					image[inc] = structimageq4[i][j].g;
					inc++;
					image[inc] = structimageq4[i][j].r;
					inc++;
				}
				else { //1st quadrant
					image[inc] = structimageq1[i][j].b;
					inc++;
					image[inc] = structimageq1[i][j].g;
					inc++;
					image[inc] = structimageq1[i][j].r;
					inc++;
				}
			}
		}
	}
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

	//populate "image" array
	readBMP("flower.bmp");
	bitsToStructs();
	if(modifiedImage) morphImage();
	structsToBits();
	
	glRasterPos2f(-.75, -.75);
	glDrawPixels(width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, image);
	//glRasterPos2f(-1, .1);
	//glDrawPixels(width/2, height/2, GL_BGR_EXT, GL_UNSIGNED_BYTE, imageq1);
	/*
	glRasterPos2f(-0.5, 0.5);
	glDrawPixels(width/2, height/2, GL_BGR_EXT, GL_UNSIGNED_BYTE, imageq2);
	glRasterPos2f(-0.5, -0.5);
	glDrawPixels(width/2, height/2, GL_BGR_EXT, GL_UNSIGNED_BYTE, imageq3);
	glRasterPos2f(0.25, -0.5);
	glDrawPixels(width/2, height/2, GL_BGR_EXT, GL_UNSIGNED_BYTE, imageq4);
	*/
	glutPostRedisplay();
	glFlush();
}

void mouseHandler(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (!mouseCurrentlyDown) {
			cout << "here" << endl;
			initialMouseX = x;
			initialMouseY = y;
			mouseCurrentlyDown = true;
			modifiedImage = true;
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		mouseCurrentlyDown = false;
	}
}

void mousePos(int x, int y) {
	if (mouseCurrentlyDown) {
		changeX = x - initialMouseX;
		changeY = -(y - initialMouseY);
		//cout << "X: " << changeX << ", Y: " << changeY << endl;
	}
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitWindowSize(screenx, screeny);
	glutInitWindowPosition(50, 50);
	//glutInitDisplayMode(GLUT_DOUBLE);
	glutCreateWindow("Project 4");
	glutDisplayFunc(display);
	glutMouseFunc(mouseHandler);
	glutMotionFunc(mousePos);
	//glutKeyboardFunc(keyboardHandler);
	glutMainLoop();
	return 0;
}