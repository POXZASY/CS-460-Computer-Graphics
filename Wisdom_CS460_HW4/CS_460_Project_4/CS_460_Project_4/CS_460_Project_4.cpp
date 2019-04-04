#include "pch.h"
#include <iostream>
#include "GL/glut.h"
#include <vector>
#include <fstream>
#include <math.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#ifndef GL_BGR
#define GL_BGR 0x80E0

using namespace std;

int screenx = 500;
int screeny = 500;
//bg color
float red = 0.0;
float green = 0.0;
float blue = 0.0;
bool rotating = false;
float rotater = 0;
double eyeX = 0;
double eyeY = 0;
double eyeZ = 0;
double centerX = 0;
double centerY = 0;
double centerZ = 0;
double upX = 0;
double upY = 1;
double upZ = 0;

bool flower = true;
bool teacup = false;
bool cube = false;

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

void processMenu(int option) {
	if (option == 1) {
		flower = true;
		teacup = false;
		cube = false;
	}
	else if (option == 2) {
		flower = false;
		teacup = true;
		cube = false;
	}
	else if (option == 3) {
		flower = false;
		teacup = false;
		cube = true;
	}
}

void createMenu() {
	int menu = glutCreateMenu(processMenu);
	glutAddMenuEntry("Flower", 1);
	glutAddMenuEntry("Teacup", 2);
	glutAddMenuEntry("Cube", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

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

bool loadObj(const char * path, vector < glm::vec3 > & out_vertices, vector < glm::vec2 > & out_uvs, vector < glm::vec3 > & out_normals) {
	vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	vector< glm::vec3 > temp_vertices;
	vector< glm::vec2 > temp_uvs;
	vector< glm::vec3 > temp_normals;
	FILE * file = fopen(path, "r");
	while (1) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);



		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	

		// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
		// Read our .obj file
		
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (flower) {
		//Rotate camera
		if (rotating) rotater += .25;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(rotater, 0, 0, 1);
		//gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);


		//Draw Scene
		glClearColor(red, green, blue, 0);
		glColor3f(1.0, 1.0, 1.0);

		//populate "image" array
		readBMP("flower.bmp");
		bitsToStructs();
		if (modifiedImage) morphImage();
		structsToBits();

		//draw image
		GLuint texName;

		glClearColor(0.0, 0.0, 0.0, 0.0);
		glShadeModel(GL_FLAT);
		glEnable(GL_DEPTH_TEST);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &texName);
		glBindTexture(GL_TEXTURE_2D, texName);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width,
			height, 0, GL_BGR, GL_UNSIGNED_BYTE,
			image);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glBindTexture(GL_TEXTURE_2D, texName);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-.75, -.5, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-.75, .5, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(.75, .5, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(.75, -.5, 0.0);
		glEnd();
		glFlush();
		glDisable(GL_TEXTURE_2D);
	}
	else if(teacup){
		/*
		std::vector< glm::vec3 > vertices;
		std::vector< glm::vec2 > uvs;
		std::vector< glm::vec3 > normals; // Won't be used at the moment.
		bool res = loadObj("teapot.obj", vertices, uvs, normals);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		*/
	}
	else if (cube) {

	}
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

void keyboardHandler(unsigned char key, int x, int y) {
	if (key == 'R' || key == 'r') {
		rotating = !rotating;
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
	glutKeyboardFunc(keyboardHandler);
	createMenu();
	glutMainLoop();
	return 0;
}
#endif