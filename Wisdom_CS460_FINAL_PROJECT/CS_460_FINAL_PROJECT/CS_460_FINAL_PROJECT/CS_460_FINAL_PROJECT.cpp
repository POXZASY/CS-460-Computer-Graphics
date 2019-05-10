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
float atmosred = (float)135/255;
float atmosgreen = (float)206/255;
float atmosblue = (float)235/255;
float randinc = time(NULL);

float timeval = 123;

int N = 64;
int M = 64;
int dist = N;
float A = .01; //arbitrary "global wave ampliltude"
pair<float, float> L = make_pair(1000, 1000);



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


float Lval = pow(V, 2)/g;


pair<float, float> w_hat = make_pair(1, 0); //wind going in positive x direction

complex T(float x) {
	complex c;
	c.real = cos(2*pi*x/N);
	c.img = sin(2 * pi*x / N);
	return c;
}

float phillipsSpectrum(float n, float m) {
	pair<float, float> k = make_pair(n, m);
	float l = .01;
	float small = .0001;
	if (k.first == 0 && k.second == 0) return A * pow(e, -1 / pow(small*Lval, 2)) / pow(small, 4) * pow(abs(k.first * w_hat.first + k.second * w_hat.second), 2) * pow(e, -small*small*l*l); //handle divide by zero errors
	return A * pow(e, -1/pow(vecMag(k)*Lval, 2))/pow(vecMag(k),4) * pow(abs(k.first * w_hat.first + k.second * w_hat.second), 2)* pow(e, -pow(vecMag(k), 2) *l*l);
}

//Equation 25
complex height0_fourier(float n, float m) {
	complex c;
	c.real = normal();
	c.img = normal();
	return c * (1 / sqrt(2)) * sqrt(phillipsSpectrum(n, m));
}

//Dispersion relation
float w(float n, float m) {
	return sqrt(g * sqrt(pow((2*pi*n - pi*N)/L.first, 2)+pow((2*pi*m - pi*M)/L.second, 2)));
}

//Equation 26: "height amplitude Fourier component"
complex height_fourier(float n, float m, float t) {
	complex c1;
	c1.real = cos(w(n, m)*t);
	c1.img = sin(w(n, m)*t);
	complex c2;
	c2.real = cos(-w(n, m)*t);
	c2.img = sin(-w(n, m)*t);
	return height0_fourier(n, m)*c1 + conjugate(height0_fourier(-n, -m))*c2;
}

vector<pair<complex, complex>> heightvals;

complex height_3p(float x, float m, float t) {
	if (x >= 0) { //second cycle
		pair<complex, complex> temp = heightvals[x];
		return temp.first + temp.second*-1;
	}
	complex sum1;
	complex sum2;
	sum1.real = 0;
	sum1.img = 0;
	sum2.real = 0;
	sum2.img = 0;
	for (int n = 0; n < N / 2; n++) {
		complex exp;
		exp.real = cos(2 * pi*n*x / (N / 2));
		exp.img = sin(2 * pi*n*x / (N / 2));
		sum1 = sum1 + exp * height_fourier(2 * n, m, t);
	}
	complex twiddle = T(x);
	for (int n = 0; n < N / 2; n++) {
		complex exp;
		exp.real = cos(2 * pi*n*x / (N / 2));
		exp.img = sin(2 * pi*n*x / (N / 2));
		sum2 = sum2 + twiddle * exp * height_fourier(2 * n + 1, m, t);
	}
	heightvals.push_back(make_pair(sum1, sum2));
	return sum1+sum2;
}

//Equation 19: "The fft-based representation of a wave height field expresses the wave heighth(x,t) at the horizontal position x= (x,z) as the sum of sinusoids with complex, time-dependent amplitudes"
float height(pair <float, float> xvec, float t) {
	float x = xvec.first;
	float z = xvec.second;

	complex sum;
	sum.real = 0;
	sum.img = 0;
	
	for (int m = 0; m < M; m++) {
		complex exp;
		exp.real = cos(z*(2*pi*m-pi*M) / L.second);
		exp.img = sin(z*(2 * pi*m - pi * M) / L.second);
		complex part2 = height_3p(x, m, t);
		sum = sum + exp * part2;
	}

	return sum.real; //complex part should be 0

	/*
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
	*/
}


struct Color {
	Color operator +(Color c){
		Color temp;
		temp.red = (red + c.red) / 2;
		temp.green = (green + c.green) / 2;
		temp.blue = (blue + c.blue) / 2;
		return temp;
	}
	Color operator *(float f) {
		Color temp;
		temp.red = red * f;
		temp.green = green * f;
		temp.blue = blue * f;
		return temp;
	}
	float red;
	float green;
	float blue;
};

struct Point {
	Color c;
	float x;
	float y;
	float z;
};



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
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			Point p;
			p.x = i - (N/2);
			p.y = height(make_pair(i-(N/2), j-(N/2)), timeval);
			p.z = j - (M/2);
			//cout << "x: " << p.x << "y: " << p.y << "z: " << p.z << endl;
			wavepoints[i][j] = p;
		}
		cout << "x: " << i <<endl;
	}
}

Color lightSun(Point p) {
	Color temp;
	temp.red = 1.0;
	temp.green = 1.0;
	temp.blue = 1.0;
	return temp;
}

Color lightAtmosphere(Point p) {
	//depends only on the sun
	Color temp;
	temp.red = atmosred;
	temp.green = atmosgreen;
	temp.blue = atmosblue;
	return temp;
}

Color lightUnder(Point p) {
	//emperical result
	Color temp;
	temp.red = 0.0;
	temp.green = 0.0;
	temp.blue = 0.0;
	return temp;
}

Color lightAbove(Point p){
	float r = 1;
	float t_u = 0;
	return lightSun(p)*r + lightAtmosphere(p)*r + lightUnder(p)*t_u;
}

void doRadiosity() {
	for (int i = 0; i < dist; i++) {
		for (int j = 0; j < dist; j++) {
			wavepoints[i][j].c = lightAbove(wavepoints[i][j]);
		}
	}
}

void display() {
	
	glClearColor(atmosred, atmosgreen, atmosblue, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, ((float)screenx / screeny), 0.01, 500); //60 deg prob not best FOV
	gluLookAt(dist/2, 10, 0, 0, 0, 0, 0, 1, 0);

	
	
	//flatWave();
	drawWave();
	//doRadiosity();
	

	//Displays wavepoints
	heightvals.clear();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int x = 0; x < dist-1; x++) {
		for (int z = 0; z < dist-1; z++) {
			Point p1 = wavepoints[x][z];
			Point p2 = wavepoints[x][z+1];
			Point p3 = wavepoints[x+1][z+1];
			Point p4 = wavepoints[x+1][z];
			float redval = (p1.c.red + p2.c.red + p3.c.red + p4.c.red) / 4;
			float greenval = (p1.c.green + p2.c.green + p3.c.green + p4.c.green) / 4;
			float blueval = (p1.c.blue + p2.c.blue + p3.c.blue + p4.c.blue) / 4;
			//glColor3f(redval, greenval, blueval);
			glColor3f(0, 0, 0);
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