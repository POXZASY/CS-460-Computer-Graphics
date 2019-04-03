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
	bool defined = false;
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
					color.defined = true;
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
					color.defined = true;
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
					color.defined = true;
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
					color.defined = true;
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
					structimageq3_new[round(i*xratio3)][round(j*yratio3)] = structimageq3[i][j];
				}
				else { //2nd quadrant
					structimageq2_new[round(i*xratio2)][(int)(((j - (height / 2))*yratio2) + (height / 2) + changeY)] = structimageq2[i][j];
				}
			}
			else { //right half
				if (j < height / 2) {//4th quadrant
					structimageq4_new[(int)(((i - (width / 2))*xratio4) + (width / 2) + changeX)][round(j*yratio4)] = structimageq4[i][j];
				}
				else { //1st quadrant
					structimageq1_new[(int)(((i - (width / 2))*xratio1) + (width / 2) + changeX)][(int)(((j - (height / 2))*yratio1) + (height / 2) + changeY)] = structimageq1[i][j];
				}
			}
		}
	}
	//INTERPOLATION
	//Combine to one matrix, fill in gaps, split back
	vector<vector<Color>> interpolateVec(width, vector<Color>(height));
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (i < width / 2 + changeX) { //left half
				if (j < height / 2 + changeY) {//3rd quadrant
					interpolateVec[i][j] = structimageq3_new[i][j];
				}
				else { //2nd quadrant
					interpolateVec[i][j] = structimageq2_new[i][j];
				}
			}
			else { //right half
				if (j < height / 2 + changeY) {//4th quadrant
					interpolateVec[i][j] = structimageq4_new[i][j];
				}
				else { //1st quadrant
					interpolateVec[i][j] = structimageq1_new[i][j];
				}
			}
		}
	}
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (!interpolateVec[i][j].defined) {
				unsigned int ravg = 0;
				unsigned int gavg = 0;
				unsigned int bavg = 0;
				int count = 0;
				for (int a = -1; a <= 1; a++) {
					for (int b = -1; b <= 1; b++) {
						if (((a != b) || (b != 0)) &&(i+a >= 0 )&& (j+b >=0 )&& ((i+a) < width )&& (j+b < height) && (interpolateVec[i+a][j+b].defined)) {
							ravg += (unsigned int)interpolateVec[i + a][j + b].r;
							gavg += (unsigned int)interpolateVec[i + a][j + b].g;
							bavg += (unsigned int)interpolateVec[i + a][j + b].b;
							count++;
						}
					}
				}
				if (count > 0) {
					ravg = (unsigned int)(ravg / count);
					gavg = (unsigned int)(gavg / count);
					bavg = (unsigned int)(bavg / count);
				}
				Color color;
				color.r = (unsigned char)ravg;
				color.g = (unsigned char)gavg;
				color.b = (unsigned char)bavg;
				color.defined = true;
				interpolateVec[i][j] = color;
			}
		}
	}
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (i < width / 2 + changeX) { //left half
				if (j < height / 2 + changeY) {//3rd quadrant
					structimageq3_new[i][j] = interpolateVec[i][j];
				}
				else { //2nd quadrant
					structimageq2_new[i][j] = interpolateVec[i][j];
				}
			}
			else { //right half
				if (j < height / 2 + changeY) {//4th quadrant
					structimageq4_new[i][j] = interpolateVec[i][j];
				}
				else { //1st quadrant
					structimageq1_new[i][j] = interpolateVec[i][j];
				}
			}
		}
	}
	structimageq1 = structimageq1_new;
	structimageq2 = structimageq2_new;
	structimageq3 = structimageq3_new;
	structimageq4 = structimageq4_new;
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
		if (changeX >= width / 2) changeX = (width / 2) - 1;
		if (changeX < -width / 2) changeX = -width / 2;
		changeY = -(y - initialMouseY);
		if (changeY >= height / 2) changeY = (height / 2) - 1;
		if (changeY < -height / 2) changeY = -height / 2;
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