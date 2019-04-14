//headers
#include "pch.h"
#include <Windows.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<stdio.h>
//globals
GLuint teapot;
float teapotrot;
char ch = '1';
//other functions and main
//wavefront .obj loader code begins
void loadObj(const char *fname)
{
	FILE *fp;
	int read;
	GLfloat x, y, z;
	char ch;
	teapot = glGenLists(1);
	fp = fopen(fname, "r");
	glPointSize(2.0);
	glNewList(teapot, GL_COMPILE);
	{
		glPushMatrix();
		glBegin(GL_POINTS);
		while (!(feof(fp)))
		{
			read = fscanf(fp, "%c %f %f %f", &ch, &x, &y, &z);
			if (read == 4 && ch == 'v')
			{
				glVertex3f(x, y, z);
			}
		}
		glEnd();
	}
	glPopMatrix();
	glEndList();
	fclose(fp);
}
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}
void drawTeapot()
{
	glPushMatrix();
	glTranslatef(0, -0.25, -1);
	glColor3f(1, 1, 1);
	glScalef(0.1, 0.1, 0.1);
	glRotatef(teapotrot, 0, 1, 0);
	glCallList(teapot);
	glPopMatrix();
	teapotrot = teapotrot + 0.01;
	if (teapotrot > 360)teapotrot = teapotrot - 360;
}
void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	drawTeapot();
	glutSwapBuffers(); //swap the buffers
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 450);
	glutInitWindowPosition(20, 20);
	glutCreateWindow("ObjLoader");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);
	loadObj("teapot.obj");//replace elepham.obj withp orsche.obj or radar.obj or any other .obj to display it
	glutMainLoop();
	return 0;
}