#include <GL/glut.h>
#include <math.h>
#include <iostream>
const int h = 2;
const int hOuter = 15;
const int c=4;//determina la grandezza
const int length=c*36;
const int width=c*29;
using namespace std;
float a = 0.0;
float da = 1;
float l = 0.5;
struct Vettori
{
	float eye[3];
	float direction[3];
	float up[3];
} vettori;
const int map[29][37] =
{
{ 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
		0, 1, 0, 1, 0, 1, 1, 0, 0, 0 }, //0
		{ 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1,
				1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0 }, //1
		{ 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0,
				1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1 }, //2
		{ 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0,
				1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1 }, //3
		{ 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1,
				1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1 }, //4
		{ 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 }, //5
		{ 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1,
				1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0 }, //6
		{ 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
				1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0 }, //7
		{ 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1,
				1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1 }, //8
		{ 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
				0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0 }, //9
		{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1,
				0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0 }, //10
		{ 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, //11
		{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
				1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1 }, //12
		{ 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
				0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 }, //13
		{ 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1,
				0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0 }, //14
		{ 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
				0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0 }, //15
		{ 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0 }, //16
		{ 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0,
				1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0 }, //17
		{ 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
				0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 }, //18
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0,
				1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1 }, //19
		{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, //20
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0 }, //21
		{ 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
				0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0 }, //22
		{ 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1,
				1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1 }, //23
		{ 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
				1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 }, //24
		{ 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0,
				1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 }, //25
		{ 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //26
		{ 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0,
				1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0 }, //27
		{ 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0,
				1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0 } //28
};
void aerialWiew()
{
	vettori.eye[2] = 90;
}
void standardWiew()
{
	vettori.eye[2] = 1.5;
}
void initializateVision()
{
	vettori.eye[0] = 0;
	vettori.eye[1] = 0;
	vettori.eye[2] = 1.5;
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
bool goForward()
{
	if ((vettori.eye[0] + l * cos((a * 3.14) / 180) < 115.92)
			&& (vettori.eye[1] + l * sin((a * 3.14) / 180) < 145.92)
			&& (vettori.eye[0] + l * cos((a * 3.14) / 180) > 0.06)
			&& (vettori.eye[1] + l * sin((a * 3.14) / 180) > 0.06))
		return true;
	cout << "stop" << endl;
	return false;
}
bool goBackward()
{
	if ((vettori.eye[0] - l * cos((a * 3.14) / 180) > 0.06)
			&& (vettori.eye[1] - l * sin((a * 3.14) / 180) > 0.06)
			&& (vettori.eye[0] - l * cos((a * 3.14) / 180) < 115.92)
			&& (vettori.eye[1] - l * sin((a * 3.14) / 180) < 145.92))
		return true;
	cout << "stop" << endl;
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
	if (key == GLUT_KEY_UP && goForward())
	{
		vettori.eye[0] = vettori.eye[0] + l * cos((a * 3.14) / 180);
		vettori.eye[1] = vettori.eye[1] + l * sin((a * 3.14) / 180);
		vettori.direction[0] = vettori.eye[0] + cos((a * 3.14) / 180);
		vettori.direction[1] = vettori.eye[1] + sin((a * 3.14) / 180);
		cout << "forward-" << "current position" << vettori.eye[0] << ","
				<< vettori.eye[1] << endl;
	}
	if (key == GLUT_KEY_DOWN && goBackward())
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
	if (key == 'f')
	{
		glutFullScreen();
		glutPostRedisplay();
	}
	if (key == 'a')
	{
		aerialWiew();
		glutPostRedisplay();
	}
	if (key == 's')
	{
		standardWiew();
		glutPostRedisplay();
	}
	if (key == 'r')
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
	glColor3f(0, 0, 0);
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
