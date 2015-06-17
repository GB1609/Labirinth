#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include "fstream"
#include "string"
#include "cstring"
#include <cstdlib>
#include <ctime>
#include "iostream"
using namespace std;
const int h = 3;
const int hOuter = 12;
const int confine = 2;
const int c = 6; //determina la grandezza del labirinto
const int length = c * 36;
const int width = c * 29;
float a = 0.0;
float da = 1;
float l = 0.5;
int map[29][36];
struct Vettori
{
	float eye[3];
	float direction[3];
	float up[3];
} vettori;
void assignValue(ifstream& in)
{
	for (int y = 0; y < 29; y++)
	{
		for (int x = 0; x < 36; x++)
		{
			in >> map[y][x];
		}
	}
}
void loadMap()
{
	srand(time(0));
	int random = rand() % 4;
	cout << random << endl;
	if (random == 0)
	{
		ifstream in("LabirinthMap1.txt");
		assignValue(in);
		in.close();
	} else if (random == 1)
	{
		ifstream in("LabirinthMap2.txt");
		assignValue(in);
		in.close();
	} else if (random == 2)
	{
		ifstream in("LabirinthMap3.txt");
		assignValue(in);
		in.close();
	} else
	{
		ifstream in("LabirinthMap4.txt");
		assignValue(in);
		in.close();
	}
}
void aerialWiew()
{
	vettori.eye[2] = 90;
}
void standardWiew()
{
	vettori.eye[2] = 1;
}
void initializateVision()
{
	vettori.eye[0] = 3;
	vettori.eye[1] = 3;
	vettori.eye[2] = 1;
	vettori.direction[0] = 1;
	vettori.direction[1] = 0;
	vettori.direction[2] = 1;
	vettori.up[0] = 0;
	vettori.up[1] = 0;
	vettori.up[2] = 3;
}
void reset()
{
	initializateVision();
}
bool goForwardIn()
{
	if (map[(int) ((vettori.eye[0] / c) + l * cos((a * 3.14) / 180))][(int) ((vettori.eye[1]
			/ c) + l * sin((a * 3.14) / 180))] == 1)
	{
		cout << "conflict with wall" << endl;
		return false;
	}
	return true;
}
bool goBackwardIn()
{
	if (map[(int) ((vettori.eye[0] / c) - l * cos((a * 3.14) / 180))][(int) ((vettori.eye[1]
			/ c) - l * sin((a * 3.14) / 180))] == 1)
	{
		cout << "conflict with wall" << endl;
		return false;
	}
	return true;
}
bool goForwardOut()
{
	if ((vettori.eye[0] + l * cos((a * 3.14) / 180) < width - confine)
			&& (vettori.eye[1] + l * sin((a * 3.14) / 180) < length - confine)
			&& (vettori.eye[0] + l * cos((a * 3.14) / 180) > confine)
			&& (vettori.eye[1] + l * sin((a * 3.14) / 180) > confine))
		return true;
	return false;
}
bool goBackwardOut()
{
	if ((vettori.eye[0] - l * cos((a * 3.14) / 180) > confine)
			&& (vettori.eye[1] - l * sin((a * 3.14) / 180) > confine)
			&& (vettori.eye[0] - l * cos((a * 3.14) / 180) < width - confine)
			&& (vettori.eye[1] - l * sin((a * 3.14) / 180) < length - confine))
		return true;
	return false;
}
void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
}
void keyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		cout << "left" << endl;
		a = a + da;
		vettori.direction[0] = vettori.eye[0] + cos((a * 3.14) / 180);
		vettori.direction[1] = vettori.eye[1] + sin((a * 3.14) / 180);
	}
	if (key == GLUT_KEY_RIGHT)
	{
		cout << "right" << endl;
		a = a - da;
		vettori.direction[0] = vettori.eye[0] + cos((a * 3.14) / 180);
		vettori.direction[1] = vettori.eye[1] + sin((a * 3.14) / 180);
	}
	if (key == GLUT_KEY_UP && goForwardOut() && goForwardIn())
	{
		vettori.eye[0] = vettori.eye[0] + l * cos((a * 3.14) / 180);
		vettori.eye[1] = vettori.eye[1] + l * sin((a * 3.14) / 180);
		vettori.direction[0] = vettori.eye[0] + cos((a * 3.14) / 180);
		vettori.direction[1] = vettori.eye[1] + sin((a * 3.14) / 180);
		cout << "forward-" << "current position" << (int) vettori.eye[0] << ","
				<< (int) vettori.eye[1] << endl;
	}
	if (key == GLUT_KEY_DOWN && goBackwardOut() && goBackwardIn())
	{
		vettori.eye[0] = vettori.eye[0] - l * cos((a * 3.14) / 180);
		vettori.eye[1] = vettori.eye[1] - l * sin((a * 3.14) / 180);
		vettori.direction[0] = vettori.eye[0] + cos((a * 3.14) / 180);
		vettori.direction[1] = vettori.eye[1] + sin((a * 3.14) / 180);
		cout << "backward-" << "current position" << vettori.eye[0] << ","
				<< vettori.eye[1] << endl;
	}
	glutPostRedisplay();
}
void keyPressed(unsigned char key, int x, int y)
{
	if (key == 'f' || key == 'F')
	{
		glutFullScreen();
		glutPostRedisplay();
	}
	if (key == 'a' || key == 'A')
	{
		aerialWiew();
		glutPostRedisplay();
	}
	if (key == 's' || key == 'S')
	{
		standardWiew();
		glutPostRedisplay();
	}
	if (key == 'r' || key == 'R')
	{
		reset();
		glutPostRedisplay();
	}
	if (key == 27)
		exit(0);
}
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double) w / (double) h, 1.0, 100);
}
void drawFloor()
{
	glColor3f(0, 0, 0.3);
	glBegin(GL_QUADS);
	glVertex3f(width, 0, 0);
	glVertex3f(width, length, 0);
	glVertex3f(0, length, 0);
	glVertex3f(0, 0, 0);
	glEnd();
}
void drawInteriorWall(int i, int j)
{
	i = i * c;
	j = j * c;
	int ic = i + c;
	int jc = j + c;
	glBegin(GL_QUADS);
	glVertex3f(jc, ic, h);
	glVertex3f(j, ic, h);
	glVertex3f(j, i, h);
	glVertex3f(jc, i, h);
	glVertex3f(jc, ic, h);
	glVertex3f(j, ic, h);
	glVertex3f(j, ic, 0);
	glVertex3f(jc, ic, 0);
	glVertex3f(j, ic, 0);
	glVertex3f(j, ic, h);
	glVertex3f(j, i, h);
	glVertex3f(j, i, 0);
	glVertex3f(jc, i, 0);
	glVertex3f(j, i, 0);
	glVertex3f(j, i, h);
	glVertex3f(jc, i, h);
	glVertex3f(jc, ic, h);
	glVertex3f(jc, ic, 0);
	glVertex3f(jc, i, 0);
	glVertex3f(jc, i, h);
	glEnd();
}
void drawOuterWall()
{
	glColor3f(0.0, 0.0, 0.36);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(0, length, 0);
	glVertex3f(0, length, hOuter);
	glVertex3f(0, 0, hOuter);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(width, length, 0);
	glVertex3f(width, 0, 0);
	glVertex3f(width, 0, hOuter);
	glVertex3f(width, length, hOuter);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(width, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, hOuter);
	glVertex3f(width, 0, hOuter);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(width, length, 0);
	glVertex3f(0, length, 0);
	glVertex3f(0, length, hOuter);
	glVertex3f(width, length, hOuter);
	glEnd();
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	gluLookAt(vettori.eye[0], vettori.eye[1], vettori.eye[2],
			vettori.direction[0], vettori.direction[1], vettori.direction[2],
			vettori.up[0], vettori.up[1], vettori.up[2]);
	drawFloor();
	drawOuterWall();
	glColor3f(0, 1, 0);
	for (int i = 0; i < 29; i++)
		for (int j = 0; j < 36; j++)
			if (map[i][j] == 1)
				drawInteriorWall(j, i);
	glPopMatrix();
	glutSwapBuffers();
}
int main(int argc, char **argv)
{
	loadMap();
	initializateVision();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 500);
	glutCreateWindow("IG-Project");
	init();
	glutKeyboardFunc(keyPressed);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboard);
	glutDisplayFunc(display);
	glutPostRedisplay();
	glutMainLoop();
	return 0;
}
