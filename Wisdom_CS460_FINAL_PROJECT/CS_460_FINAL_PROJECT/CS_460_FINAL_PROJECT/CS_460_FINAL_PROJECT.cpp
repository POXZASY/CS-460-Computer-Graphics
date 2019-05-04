#include "pch.h"
#include <iostream>
#include <utility>
#include <cstdlib>
#include <random>
#include <time.h>
#include <math.h>
#include "GL/glut.h"

using namespace std;

#define pi 3.14159265358979323846264338327950288419716939937510
#define e 2.7182818284590452353602874713527
#define g 9.81
#define V 5 //windspeed in m/s

int screenx = 500;
int screeny = 500;
float red = 1.0;
float green = 1.0;
float blue = 1.0;
float randinc = time(NULL);


//Implementation of http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.161.9102&rep=rep1&type=pdf


//random draws from a gaussian dist with mean 0 stdev 1
float normal() {
	default_random_engine generator(randinc);
	randinc++;
	normal_distribution<double> distribution(0.0, 1.0);
	return distribution(generator);
}

struct complex {
	float real;
	float img;
	complex operator +(complex k) {
		complex c;
		c.real = real + k.real;
		c.img = img + k.img;
		return c;
	}
	complex operator *(complex k) {
		complex c;
		c.real = real * k.real - img * k.img;
		c.img = real * k.img + img * k.real;
		return c;
	}
	complex operator *(float k) {
		complex c;
		c.real = real * k;
		c.img = img * k;
		return c;
	}
};

complex conjugate(complex c) {
	c.img = -c.img;
	return c;
}

float vecMag(pair<float, float> v) {
	return sqrt(pow(v.first, 2) + pow(v.second, 2));
}

pair<float, float> vecNorm(pair<float, float> v) {
	float len = vecMag(v);
	pair<float, float> retVal;
	retVal.first = v.first / len;
	retVal.second = v.second / len;
	return retVal;
}

float A = .001; //arbitrary "global wave ampliltude"
float L = pow(V, 2)/g;
pair<float, float> w_hat = make_pair(1, 0); //wind going in positive x direction

float phillipsSpectrum(pair<float, float> k) {
	float l = .01;
	float small = .0001;
	if (k.first == 0 && k.second == 0) return A * pow(e, -1 / pow(small*L, 2)) / pow(small, 4) * pow(abs(k.first * w_hat.first + k.second * w_hat.second), 2) * pow(e, -small*small*l*l); //handle divide by zero errors
	return A * pow(e, -1/pow(vecMag(k)*L, 2))/pow(vecMag(k),4) * pow(abs(k.first * w_hat.first + k.second * w_hat.second), 2)* pow(e, -pow(vecMag(k), 2) *l*l);
}

//Equation 25
complex height0_fourier(pair<float, float> k) {
	complex c;
	c.real = normal();
	c.img = normal();
	return c * (1 / sqrt(2)) * sqrt(phillipsSpectrum(k));
}

//Dispersion relation
float w(float k) {
	return sqrt(g * k);
}

//Equation 26: "height amplitude Fourier component"
complex height_fourier(pair<float, float> k, float t) {
	complex c1;
	c1.real = cos(w(vecMag(k))*t);
	c1.img = sin(w(vecMag(k))*t);
	complex c2;
	c2.real = cos(-w(vecMag(k))*t);
	c2.img = sin(-w(vecMag(k))*t);
	pair<float, float> kneg = make_pair(-k.first, -k.second);
	return height0_fourier(k)*c1 + conjugate(height0_fourier(kneg))*c2;
}
//Equation 19: "The fft-based representation of a wave height field expresses the wave heighth(x,t) at the horizontal position x= (x,z) as the sum of sinusoids with complex, time-dependent amplitudes"
float height(pair <float, float> x, float t) {
	pair<float, float> L = make_pair(200, 200);
	int N = 128;
	int M = 128;
	complex sum;
	sum.real = 0;
	sum.img = 0;
	for (int n = -N / 2; n < N / 2; n++) {
		for (int m = -M / 2; m < M / 2; m++) {
			pair<float, float> k = make_pair(2 * pi*n / L.first, 2 * pi*m / L.second);
			complex exp;
			exp.real = cos(x.first*k.first + x.second*k.second);
			exp.img = sin(x.first*k.first + x.second*k.second);
			sum = sum + height_fourier(k, t)*exp;
		}
		//cout << "n: " << n << " m: " << " sum: " <<sum.real << endl;
	}
	return sum.real; //complex part should be 0
}

struct Point {
	float x;
	float y;
	float z;
};


float timeval = 123;
int dist = 100;
vector<Point> tempvec(dist);
vector<vector<Point>> wavepoints(dist, tempvec);

//for testing
void flatWave() {
	wavepoints.empty();
	for (int i = 0; i < dist; i++) {
		for (int j = 0; j < dist; j++) {
			Point p;
			p.x = i;
			p.y = 0;
			p.z = j;
			//cout << "x: " << p.x << "y: " << p.y << "z: " << p.z << endl;
			wavepoints[i][j] = p;
		}
	}
}

void drawWave() {
	wavepoints.empty();
	for (int i = 0; i < dist; i++) {
		for (int j = 0; j < dist; j++) {
			Point p;
			p.x = i;
			p.y = height(make_pair(i, j), timeval);
			p.z = j;
			//cout << "x: " << p.x << "y: " << p.y << "z: " << p.z << endl;
			wavepoints[i][j] = p;
		}
		cout << "x: " << i <<endl;
	}
}


void display() {
	glClearColor(red, green, blue, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, ((float)screenx / screeny), 0.01, 500); //60 deg prob not best FOV
	gluLookAt(dist/2, dist/2, 0, dist/2, 0, dist/2, 0, 1, 0);

	
	
	//flatWave();
	drawWave();
	
	//Displays wavepoints
	glColor3f(0.0, 0.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int x = 0; x < dist-1; x++) {
		for (int z = 0; z < dist-1; z++) {
			Point p1 = wavepoints[x][z];
			Point p2 = wavepoints[x][z+1];
			Point p3 = wavepoints[x+1][z+1];
			Point p4 = wavepoints[x+1][z];
			glBegin(GL_TRIANGLES);
			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p4.x, p4.y, p4.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p3.x, p3.y, p3.z);
			glVertex3f(p4.x, p4.y, p4.z);
			glEnd();
		}
	}
	
	glutPostRedisplay();

	timeval=timeval+.1;

	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(screenx, screeny);
	glutInitWindowPosition(50, 50);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("CS 460 FINAL PROJECT");
	glutDisplayFunc(display);
	//glutMouseFunc(mouseHandler);
	//glutMotionFunc(mousePos);
	//glutMouseFunc(mouseHandler);
	//glutKeyboardFunc(keyboardHandler);
	//createMenu();
	//createSurface(); //initialize the surface
	glutMainLoop();
	return 0;
}