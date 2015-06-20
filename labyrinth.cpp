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
int h = 5; //altezza muri
int c = 6; //determina la grandezza del labirinto
const int confine = 1.8;
int saveX, saveY;
int length = c * 36; //lunghezza matrice
int width = c * 29; //larghezza matrice
float a = 0.0; //angolo
float da = 1;
float l = 0.6;
float d = 1.3; //dimensione cubo rotante
bool aerial = false; //segnala se la visuale e dall'alto
float rotate = 0;
float spec[] =
{ 1.0, 1.0, 1.0, 1.0 };
float sheen[] =
{ 25.0 };
float lightPosition[] =
{ (float)width, (float)length, 20.0, 5.0 };
float light[] =
{ 1.0, 1.0, 1.0, 1.0 };
float ambiente[] =
{ 0.6, 0.6, 0.6, 1.0 };
float floorColor[] =
{ 0.9, 0.9, 0.9 };
float ceilingColor[] =
{ 1.0, 1.0, 1.0 };
float OutWallColor[] =
{ 0.0, 0.0, 1.0 };
float InWallColor[] =
{ 0.0, 1.0, 0.0 };
float CubeColor[] =
{ 1.0, 0.0, 0.0 };
int map[29][36];
struct Vettori //gestiscono la lookat
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
int generateX()
{
	srand(time(0));
	int random = rand() % 29;
	return random;
}
int generateY()
{
	srand(time(0));
	int random = rand() % 36;
	return random;
}
void loadMap()
{
	srand(time(0));
	int random = rand() % 4;
	cout << "Stai giocando con il labirinto numero " << random + 1
			<< " Buona Fortuna" << endl;
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
	saveX = vettori.eye[0];
	saveY = vettori.eye[1];
	h = 1;
	c = 2;
	aerial = true;
	length = c * 36;
	width = c * 29;
	vettori.eye[0] = width / 2;
	vettori.eye[1] = length / 2;
	vettori.eye[2] = 90;
	d = 0.8;
}
void standardWiew()
{
	h = 5;
	c = 6;
	length = c * 36;
	width = c * 29;
	aerial = false;
	vettori.eye[0] = saveX;
	vettori.eye[1] = saveY;
	vettori.eye[2] = 0.88;
	d = 1.3;
}
void verify(int&x, int&y)
{
	if (x <= confine)
		x += confine;
	if (y <= confine)
		y += confine;
	if (x >= width - confine)
		x -= confine;
	if (y >= length - confine)
		y -= confine;
}
void initializateVision()
{
	int x = generateX();
	int y = generateY();
	while (map[x][y] == 0)
	{
		x = generateX();
		y = generateY();
	}
	x *= c;
	y *= c;
	verify(x, y);
	vettori.eye[0] = x;
	vettori.eye[1] = y;
	vettori.eye[2] = 0.88;
	vettori.direction[0] = 1;
	vettori.direction[1] = 0;
	vettori.direction[2] = 1;
	vettori.up[0] = 0;
	vettori.up[1] = 0;
	vettori.up[2] = 2;

}
void reset()
{
	h = 7;
	c = 8;
	aerial = true;
	initializateVision();
}
bool goForwardIn()
{
	int x = (int) ((vettori.eye[0] / c) + l * cos((a * 3.14) / 180));
	int y = (int) ((vettori.eye[1] / c) + l * sin((a * 3.14) / 180));
	if (map[x][y] == 1)
	{
		cout << "conflict with wall" << endl;
		return false;
	}
	return true;
}
bool goBackwardIn()
{
	int x = (int) ((vettori.eye[0] / c) - l * cos((a * 3.14) / 180));
	int y = (int) ((vettori.eye[1] / c) - l * sin((a * 3.14) / 180));
	if (map[x][y] == 1)
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
	else
		return false;
}
bool goBackwardOut()
{
	if ((vettori.eye[0] - l * cos((a * 3.14) / 180) > confine)
			&& (vettori.eye[1] - l * sin((a * 3.14) / 180) > confine)
			&& (vettori.eye[0] - l * cos((a * 3.14) / 180) < width - confine)
			&& (vettori.eye[1] - l * sin((a * 3.14) / 180) < length - confine))
		return true;
	else
		return false;
}
void init()
{
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambiente);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glClearColor(1, 1, 1, 0.0);
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
	} else if (key == GLUT_KEY_RIGHT)
	{
		cout << "right" << endl;
		a = a - da;
		vettori.direction[0] = vettori.eye[0] + cos((a * 3.14) / 180);
		vettori.direction[1] = vettori.eye[1] + sin((a * 3.14) / 180);
	} else if (key == GLUT_KEY_UP && goForwardIn() && goForwardOut() && !aerial)
	{
		vettori.eye[0] = vettori.eye[0] + l * cos((a * 3.14) / 180);
		vettori.eye[1] = vettori.eye[1] + l * sin((a * 3.14) / 180);
		vettori.direction[0] = vettori.eye[0] + cos((a * 3.14) / 180);
		vettori.direction[1] = vettori.eye[1] + sin((a * 3.14) / 180);
		cout << "forward-" << "current position" << vettori.eye[0] << ","
				<< vettori.eye[1] << endl;
	} else if (key == GLUT_KEY_DOWN && goBackwardIn() && goBackwardOut()
			&& !aerial)
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
	} else if (key == 'a' || key == 'A')
	{
		aerialWiew();
		glutPostRedisplay();
	} else if (key == 's' || key == 'S')
	{
		standardWiew();
		glutPostRedisplay();
	} else if (key == 'r' || key == 'R')
	{
		reset();
		glutPostRedisplay();
	} else if (key == 27)
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
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, floorColor);
	glBegin(GL_QUADS);
	glVertex3f(width, 0, 0);
	glVertex3f(width, length, 0);
	glVertex3f(0, length, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	glPopMatrix();
}
void drawCeiling()
{
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ceilingColor);
	glBegin(GL_QUADS);
	glVertex3f(width, 0, h);
	glVertex3f(width, length, h);
	glVertex3f(0, length, h);
	glVertex3f(0, 0, h);
	glEnd();
	glPopMatrix();
}
void drawinteriorWall(int i, int j)
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
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(0, length, 0);
	glVertex3f(0, length, h);
	glVertex3f(0, 0, h);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(width, length, 0);
	glVertex3f(width, 0, 0);
	glVertex3f(width, 0, h);
	glVertex3f(width, length, h);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(width, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, h);
	glVertex3f(width, 0, h);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(width, length, 0);
	glVertex3f(0, length, 0);
	glVertex3f(0, length, h);
	glVertex3f(width, length, h);
	glEnd();
}
void positionRotatingCube(int i, int j)
{
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef((i * c) + 1.5, (j * c) + 1.5, 1.5);
	glRotatef(rotate, 0, 0, 5);
	glutSolidCube(d);
	glPopMatrix();
	glColor3f(0, 1, 0);
}
void display(void)
{
//	float fogColor[] =
//	{ 0.1, 0.1, 0.1, 1.0 };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glFogfv(GL_FOG_COLOR, fogColor);
//	glFogi(GL_FOG_MODE, GL_LINEAR);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	gluLookAt(vettori.eye[0], vettori.eye[1], vettori.eye[2],
			vettori.direction[0], vettori.direction[1], vettori.direction[2],
			vettori.up[0], vettori.up[1], vettori.up[2]);
	drawFloor();
	if (!aerial)
		drawCeiling();
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, OutWallColor);
	drawOuterWall();
	glPopMatrix();
	glColor3f(0, 1, 0);
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, InWallColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialfv(GL_FRONT, GL_SHININESS, sheen);
	for (int i = 0; i < 29; i++)
	{
		for (int j = 0; j < 36; j++)
		{
			if (map[i][j] == 1)
				drawinteriorWall(j, i);
			else if (map[i][j] == 2)
			{
				glPushMatrix();
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, CubeColor);
				positionRotatingCube(i, j);
				glPopMatrix();
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, InWallColor);
				glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
				glMaterialfv(GL_FRONT, GL_SHININESS, sheen);
			}
		}
	}
	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
}
void rotateCube(int value)
{
	rotate += 1.0f;
	if (rotate > 360)
	{
		rotate -= 360;
	}
	glutPostRedisplay();
	glutTimerFunc(25, rotateCube, 0);
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
	glutTimerFunc(25, rotateCube, 0);
	glutMainLoop();
}
