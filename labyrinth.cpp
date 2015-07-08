#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include "fstream"
#include "string"
#include "cstring"
#include <cstdlib>
#include <ctime>
#include "tga.h"
#include "iostream"
using namespace std;
int contatore=0;
GLfloat normalUp[3]={0,0,1};
GLfloat normalDown[3]={0,0,-1};
GLfloat normalFront[3]={1,1,0};
GLfloat normalRight[3]={-1,1,0};
GLfloat normalLeft[3]={1,-1,0};
GLfloat normalBack[3]={-1,-1,0};
float c=6;  //determina la grandezza del labirinto
const float h=c/2;  //meta diametro del cubo
float length=c*38;  //lunghezza matrice
float width=c*31;  //larghezza matrice
float a=0.0;  //angolo
float da=1;
float l=0.7;
float rimb=1.1;
bool aerial=false;  //segnala se la visuale e dall'alto
float rotate=0;
float spec[]={0.5,0.5,0.5,1.0};
float sheen[]={20.0};
float lightPosition1[]={width,length,10.0,5.0};
//float lightPosition2[]={0,length,6.0,5.0};
//float lightPosition3[]={0,0,6.0,5.0};
//float lightPosition4[]={width,0,6.0,5.0};
float light[]={0.8,0.8,0.8,0.7};
float environment[]={0.7,0.7,0.7,1.0};
const int numTextures=9;
GLuint textures[numTextures];
const char *textureFile[numTextures]={"texture/ceiling.tga","texture/cube.tga","texture/floor.tga","texture/inWall.tga",
		"texture/outWall.tga","texture/player.tga","texture/sphere.tga","texture/fire.tga","texture/floorFire.tga"};
int map[31][38];
bool lost=false;
bool win=false;
float sphereDim=1.5;
bool profVuoleVincere=false;
struct VectorLookAt  //gestiscono la lookat
{
		float eye[3];
		float direction[3];
		float up[3];
} vectorLookAt;
void
loadTextures()
{
	GLbyte* pBytes;
	GLint iWidth,iHeight,iComponents;
	GLenum eFormat;
	GLubyte i;
	glEnable (GL_TEXTURE_2D);
	glTexEnvi (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glGenTextures (numTextures,textures);
	for (i=0;i<numTextures;i++)  // Load texture
	{
		glBindTexture (GL_TEXTURE_2D,textures[i]);
		pBytes=gltLoadTGA (textureFile[i],&iWidth,&iHeight,&iComponents,&eFormat);
		glTexImage2D (GL_TEXTURE_2D,0,iComponents,iWidth,iHeight,0,eFormat,GL_UNSIGNED_BYTE,pBytes);
		free (pBytes);
		glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri (GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	}
}
void
assignValue(ifstream& in)
{
	for (int y=0;y<31;y++)
	{
		for (int x=0;x<38;x++)
		{
			in>>map[y][x];
		}
	}
}
int
generateXY(int div)
{
	srand (time (0));
	int random=rand ()%div;
	return random;
}
void
zoom(bool operation)
{
	vectorLookAt.eye[0]/=c;
	vectorLookAt.eye[1]/=c;
	if (operation&&c<15)
		c+=1;
	else if (c>1)
		c-=1;
	length=c*38;
	width=c*31;
	vectorLookAt.eye[0]*=c;
	vectorLookAt.eye[1]*=c;
	vectorLookAt.direction[0]=vectorLookAt.eye[0]+cos ((a*3.14)/180);
	vectorLookAt.direction[1]=vectorLookAt.eye[1]+sin ((a*3.14)/180);
}
void
loadMap(int random)
{
	if (random==0)
	{
		ifstream in ("Maps/LabirinthMap1.txt");
		assignValue (in);
		in.close ();
		cout<<"Stai giocando con il labirinto numero "<<random+1<<" Buona Fortuna"<<endl;
	}
	else if (random==1)
	{
		ifstream in ("Maps/LabirinthMap2.txt");
		assignValue (in);
		in.close ();
		cout<<"Stai giocando con il labirinto numero "<<random+1<<" Buona Fortuna"<<endl;
	}
	else if (random==2)
	{
		ifstream in ("Maps/LabirinthMap3.txt");
		assignValue (in);
		in.close ();
		cout<<"Stai giocando con il labirinto numero "<<random+1<<" Buona Fortuna"<<endl;
	}
	else if (random==3)
	{
		ifstream in ("Maps/LabirinthMap4.txt");
		assignValue (in);
		in.close ();
		cout<<"Stai giocando con il labirinto numero "<<random+1<<" Buona Fortuna"<<endl;
	}
	else if (random==4)
	{
		ifstream in ("Maps/GameOver.txt");
		assignValue (in);
		in.close ();
	}
	else if (random==5)
	{
		ifstream in ("Maps/GameWin.txt");
		assignValue (in);
		in.close ();
	}
}
void
aerialWiew()
{
	vectorLookAt.eye[0]/=c;
	vectorLookAt.eye[1]/=c;
	c=2;
	aerial=true;
	length=c*38;
	width=c*31;
	if (!win&&!lost)
	{
		vectorLookAt.eye[0]*=c;
		vectorLookAt.eye[1]*=c;
	}
	else
	{
		vectorLookAt.eye[0]=length/2;
		vectorLookAt.eye[1]=width/2;
	}
	vectorLookAt.direction[0]=vectorLookAt.eye[0]+cos ((a*3.14)/180);
	vectorLookAt.direction[1]=vectorLookAt.eye[1]+sin ((a*3.14)/180);
	vectorLookAt.eye[2]=90;
}
void
standardWiew()
{
	vectorLookAt.eye[0]/=c;
	vectorLookAt.eye[1]/=c;
	c=6;
	length=c*38;
	width=c*31;
	aerial=false;
	vectorLookAt.eye[0]*=c;
	vectorLookAt.eye[1]*=c;
	vectorLookAt.direction[0]=vectorLookAt.eye[0]+cos ((a*3.14)/180);
	vectorLookAt.direction[1]=vectorLookAt.eye[1]+sin ((a*3.14)/180);
	vectorLookAt.eye[2]=1.1;
}
void
verify(int& x,int& y,int& posX,int& posY)
{
	if (map[x+1][y]==0)
		posX+=c;
	else if (map[x][y+1]==0)
		posY+=c;
	else if (map[x-1][y]==0)
		posX-=c;
	else if (map[x][y-1]==0)
		posY-=c;
}
void
initializateVision()
{
	int x=generateXY (29);
	int y=generateXY (36);
	while (map[x][y]==0||map[x][y]==2)
	{
		x=generateXY (29);
		y=generateXY (36);
	}
	int posX=x*c;
	int posY=y*c;
	verify (x,y,posX,posY);
	vectorLookAt.eye[0]=posX;
	vectorLookAt.eye[1]=posY;
	vectorLookAt.eye[2]=1.10;
	vectorLookAt.direction[0]=vectorLookAt.eye[0]+cos ((3.14*a)/180);
	vectorLookAt.direction[1]=vectorLookAt.eye[1]+sin ((3.14*a)/180);
	vectorLookAt.direction[2]=1;
	vectorLookAt.up[0]=0;
	vectorLookAt.up[1]=0;
	vectorLookAt.up[2]=3;
}
void
gameOver(int value)
{
	profVuoleVincere=false;
	lost=true;
	if (win)
		loadMap (5);
	else if (lost)
		loadMap (4);
	aerialWiew ();
	glutPostRedisplay ();
}
bool
moving(int x,int y)
{
	if (map[x][y]==3)
	{
		cout<<"confict with OutWall"<<endl;
		return false;
	}
	if (map[x][y]==1&&!profVuoleVincere)
	{
		cout<<"conflict with wall"<<endl;
		return false;
	}
	else if (map[x][y]==2)
	{
		cout<<"Win!!!"<<endl;
		win=true;
		glutPostRedisplay ();
	}
	return true;
}
void
init()
{
	glLightfv (GL_LIGHT0,GL_POSITION,lightPosition1);
//	glLightfv (GL_LIGHT0,GL_POSITION,lightPosition2);
// 	glLightfv (GL_LIGHT0,GL_POSITION,lightPosition3);
//	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition4);
	glLightfv (GL_LIGHT0,GL_DIFFUSE,light);
	glLightfv (GL_LIGHT0,GL_SPECULAR,light);
	glLightModelfv (GL_LIGHT_MODEL_AMBIENT,environment);
	glEnable (GL_LIGHTING);  //abilita luce ambiente
	glEnable (GL_LIGHT0);  //abilita luci
	glShadeModel (GL_SMOOTH);
	glClearColor (0,0,0,0);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glEnable (GL_DEPTH_TEST);
}
void
reshape(int w,int h)
{
	glViewport (0,0,w,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45.0,(double)w/(double)h,1.0,100);
}
void
drawFloor()
{
	int xi=0;
	int xy=0;
	if (lost||win)
	{
		xi=-50;
		xy=-50;
		length=500;
		width=500;
		glBindTexture (GL_TEXTURE_2D,textures[8]);
	}
	else
		glBindTexture (GL_TEXTURE_2D,textures[2]);
	glPushMatrix ();
	glBegin (GL_QUADS);
	glNormal3fv (normalUp);
	glTexCoord2f (0.0f,0.0f);
	glVertex3f (xi,xy,-h);
	glTexCoord2f (0.0f,1.0f);
	glVertex3f (xi,length,-h);
	glTexCoord2f (1.0f,1.0f);
	glVertex3f (width,length,-h);
	glTexCoord2f (1.0f,0.0f);
	glVertex3f (width,xy,-h);
	glEnd ();
	glPopMatrix ();
}
void
drawCeiling()
{
	int xi=0;
	int xy=0;
	if (lost||win)
	{
		xi=-50;
		xy=-50;
		length=500;
		width=500;
		glBindTexture (GL_TEXTURE_2D,textures[8]);
	}
	else
		glBindTexture (GL_TEXTURE_2D,textures[0]);
	glPushMatrix ();
	glBegin (GL_QUADS);
	glNormal3fv (normalDown);
	glTexCoord2f (0.0f,0.0f);
	glVertex3f (xi,xy,h);
	glTexCoord2f (0.0f,1.0f);
	glVertex3f (xi,length,h);
	glTexCoord2f (1.0f,1.0f);
	glVertex3f (width,length,h);
	glTexCoord2f (1.0f,0.0f);
	glVertex3f (width,xy,h);
	glEnd ();
	glPopMatrix ();
}
void
drawCube()
{
	float i=0;
	float j=0;
	float ic=c-1.2;
	float jc=ic;
	float tall=ic/2;
	glBegin (GL_QUADS);
	glNormal3fv (normalUp);
	glTexCoord2f (1,0);
	glVertex3f (j,i,tall);
	glTexCoord2f (1,1);
	glVertex3f (j,ic,tall);
	glTexCoord2f (0,1);
	glVertex3f (jc,ic,tall);
	glTexCoord2f (0,0);
	glVertex3f (jc,i,tall);
	glNormal3fv (normalFront);
	glTexCoord2f (1,1);
	glVertex3f (jc,ic,tall);
	glTexCoord2f (0,1);
	glVertex3f (j,ic,tall);
	glTexCoord2f (0,0);
	glVertex3f (j,ic,-tall);
	glTexCoord2f (1,0);
	glVertex3f (jc,ic,-tall);
	glNormal3fv (normalLeft);
	glTexCoord2f (1,1);
	glVertex3f (j,ic,tall);
	glTexCoord2f (0,1);
	glVertex3f (j,i,tall);
	glTexCoord2f (0,0);
	glVertex3f (j,i,-tall);
	glTexCoord2f (1,0);
	glVertex3f (j,ic,-tall);
	glNormal3fv (normalFront);
	glTexCoord2f (0,0);
	glVertex3f (j,i,-tall);
	glTexCoord2f (1,0);
	glVertex3f (jc,i,-tall);
	glTexCoord2f (1,1);
	glVertex3f (jc,i,tall);
	glTexCoord2f (0,1);
	glVertex3f (j,i,tall);
	glNormal3fv (normalRight);
	glTexCoord2f (0,1);
	glVertex3f (jc,ic,tall);
	glTexCoord2f (1,1);
	glVertex3f (jc,i,tall);
	glTexCoord2f (1,0);
	glVertex3f (jc,i,-tall);
	glTexCoord2f (0,0);
	glVertex3f (jc,ic,-tall);
	glNormal3fv (normalDown);
	glTexCoord2f (1,0);
	glVertex3f (j,i,-tall);
	glTexCoord2f (1,1);
	glVertex3f (j,ic,-tall);
	glTexCoord2f (0,1);
	glVertex3f (jc,ic,-tall);
	glTexCoord2f (0,0);
	glVertex3f (jc,i,-tall);
	glEnd ();
}
void
positionRotatingCube(int& i,int& j)
{
	glPushMatrix ();
	glTranslatef ((i*c)+c/2,(j*c)+c/2,rimb);
	glRotatef (rotate,0,0,1.6);
	if (win)
	{
		glBindTexture (GL_TEXTURE_2D,textures[6]);
		GLUquadric * q=gluNewQuadric ();
		gluQuadricTexture (q,true);
		gluSphere (q,sphereDim,200,200);
	}
	else
	{
		glTranslatef (-((c-1.2)/2),-((c-1.2)/2),0);
		glBindTexture (GL_TEXTURE_2D,textures[1]);
		drawCube ();
	}
	glPopMatrix ();
}
void
rotateCube(int value)
{
	if (win)
	{
		contatore++;
		rimb-=0.20;
		if (rimb<=-1)
			rimb=1.1;
	}
	rotate+=1.5f;
	if (rotate>360)
		rotate-=360;
	glutPostRedisplay ();
	if (contatore<500)
		glutTimerFunc (25,rotateCube,0);
	else
		gameOver (0);
}
void
reset()
{
	c=5;
	length=c*38;
	width=c*31;
	aerial=lost=win=profVuoleVincere=false;
	contatore=0;
	glutTimerFunc (25,rotateCube,0);
	glutTimerFunc (300000,gameOver,0);
	loadMap (generateXY (4));
	initializateVision ();
	glutPostRedisplay ();
}
void
keyboard(int key,int x,int y)
{
	if (key==GLUT_KEY_LEFT)
	{
		cout<<"left"<<endl;
		a=a+da;
		vectorLookAt.direction[0]=vectorLookAt.eye[0]+cos ((a*3.14)/180);
		vectorLookAt.direction[1]=vectorLookAt.eye[1]+sin ((a*3.14)/180);
	}
	else if (key==GLUT_KEY_RIGHT)
	{
		cout<<"right"<<endl;
		a=a-da;
		vectorLookAt.direction[0]=vectorLookAt.eye[0]+cos ((a*3.14)/180);
		vectorLookAt.direction[1]=vectorLookAt.eye[1]+sin ((a*3.14)/180);
	}
	else if (key==GLUT_KEY_UP)
	{
		int x=(int)(((vectorLookAt.eye[0])/c)+l*cos ((a*3.14)/180));
		int y=(int)(((vectorLookAt.eye[1])/c)+l*sin ((a*3.14)/180));
		if (moving (x,y)&&!win&&!lost)
		{
			vectorLookAt.eye[0]=vectorLookAt.eye[0]+l*cos ((a*3.14)/180);
			vectorLookAt.eye[1]=vectorLookAt.eye[1]+l*sin ((a*3.14)/180);
			vectorLookAt.direction[0]=vectorLookAt.eye[0]+cos ((a*3.14)/180);
			vectorLookAt.direction[1]=vectorLookAt.eye[1]+sin ((a*3.14)/180);
			cout<<"forward-"<<"current position"<<vectorLookAt.eye[0]<<","<<vectorLookAt.eye[1]<<endl;
		}
	}
	else if (key==GLUT_KEY_DOWN)
	{
		int x=(int)(((vectorLookAt.eye[0])/c)-l*cos ((a*3.14)/180));
		int y=(int)(((vectorLookAt.eye[1])/c)-l*sin ((a*3.14)/180));
		if (moving (x,y)&&!lost&&!win)
		{
			vectorLookAt.eye[0]=vectorLookAt.eye[0]-l*cos ((a*3.14)/180);
			vectorLookAt.eye[1]=vectorLookAt.eye[1]-l*sin ((a*3.14)/180);
			vectorLookAt.direction[0]=vectorLookAt.eye[0]+cos ((a*3.14)/180);
			vectorLookAt.direction[1]=vectorLookAt.eye[1]+sin ((a*3.14)/180);
			cout<<"backward-"<<"current position"<<vectorLookAt.eye[0]<<","<<vectorLookAt.eye[1]<<endl;
		}
	}
	glutPostRedisplay ();
}
void
keyPressed(unsigned char key,int x,int y)
{
	if (key=='f'||key=='F')
		glutFullScreen ();
	else if ((key=='z'||key=='Z')&&aerial)
		zoom (true);
	else if ((key=='x'||key=='X')&&aerial)
		zoom (false);
	else if (key=='l'||key=='L')
		profVuoleVincere=false;
	else if (key=='a'||key=='A')
	{
		if (!win&&!lost&&!aerial)
			aerialWiew ();
	}
	else if (key=='s'||key=='S')
	{
		if (!win&&!lost&&aerial)
			standardWiew ();
	}
	else if (key=='r'||key=='R')
		reset ();
	else if (key=='p'||key=='P')
		profVuoleVincere=true;
	else if (key==27)
		exit (0);
	glutPostRedisplay ();
}
void
drawWall(int i,int j)
{
	i=i*c;
	j=j*c;
	int ic=i+c;
	int jc=j+c;
	glBegin (GL_QUADS);
	if (aerial)
	{
		glNormal3fv (normalUp);
		glTexCoord2f (1,0);
		glVertex3f (j,i,h);
		glTexCoord2f (1,1);
		glVertex3f (j,ic,h);
		glTexCoord2f (0,1);
		glVertex3f (jc,ic,h);
		glTexCoord2f (0,0);
		glVertex3f (jc,i,h);
	}
	glNormal3fv (normalFront);
	glTexCoord2f (1,1);
	glVertex3f (jc,ic,h);
	glTexCoord2f (0,1);
	glVertex3f (j,ic,h);
	glTexCoord2f (0,0);
	glVertex3f (j,ic,-h);
	glTexCoord2f (1,0);
	glVertex3f (jc,ic,-h);
	glNormal3fv (normalLeft);
	glTexCoord2f (1,1);
	glVertex3f (j,ic,h);
	glTexCoord2f (0,1);
	glVertex3f (j,i,h);
	glTexCoord2f (0,0);
	glVertex3f (j,i,-h);
	glTexCoord2f (1,0);
	glVertex3f (j,ic,-h);
	glNormal3fv (normalFront);
	glTexCoord2f (0,0);
	glVertex3f (j,i,-h);
	glTexCoord2f (1,0);
	glVertex3f (jc,i,-h);
	glTexCoord2f (1,1);
	glVertex3f (jc,i,h);
	glTexCoord2f (0,1);
	glVertex3f (j,i,h);
	glNormal3fv (normalRight);
	glTexCoord2f (0,1);
	glVertex3f (jc,ic,h);
	glTexCoord2f (1,1);
	glVertex3f (jc,i,h);
	glTexCoord2f (1,0);
	glVertex3f (jc,i,-h);
	glTexCoord2f (0,0);
	glVertex3f (jc,ic,-h);
	glEnd ();
}
void
display(void)
{
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix ();
	gluLookAt (vectorLookAt.eye[0],vectorLookAt.eye[1],vectorLookAt.eye[2],vectorLookAt.direction[0],
			vectorLookAt.direction[1],vectorLookAt.direction[2],vectorLookAt.up[0],vectorLookAt.up[1],
			vectorLookAt.up[2]);
	glMaterialfv (GL_FRONT,GL_SPECULAR,spec);
	glMaterialfv (GL_FRONT,GL_SHININESS,sheen);
	drawFloor ();
	if (!aerial)
		drawCeiling ();
	else if (!win&&!lost)
	{
		glPushMatrix ();
		glTranslatef (vectorLookAt.eye[0],vectorLookAt.eye[1],rimb);
		glRotatef (rotate,0,0,1.6);
		glBindTexture (GL_TEXTURE_2D,textures[5]);
		glTranslatef (-((c-1.2)/2),-((c-1.2)/2),0);
		drawCube ();
		glPopMatrix ();
	}
	for (int i=0;i<31;i++)
	{
		for (int j=0;j<38;j++)
		{
			if (map[i][j]==3)
			{
				glPushMatrix ();
				if (aerial)
					glBindTexture (GL_TEXTURE_2D,textures[3]);
				else
					glBindTexture (GL_TEXTURE_2D,textures[4]);
				drawWall (j,i);
				glPopMatrix ();
			}
			else if (map[i][j]==1&&!profVuoleVincere)
			{
				glPushMatrix ();
				if (!lost)
					glBindTexture (GL_TEXTURE_2D,textures[3]);
				else
					glBindTexture (GL_TEXTURE_2D,textures[7]);
				drawWall (j,i);
				glPopMatrix ();
			}
			else if (map[i][j]==2)
			{
				glPushMatrix ();
				positionRotatingCube (i,j);
				glPopMatrix ();
			}
		}
	}
	glPopMatrix ();
	glutSwapBuffers ();
}
int
main(int argc,char **argv)
{
	srand (time (0));
	int random=rand ()%4;
	loadMap (random);
	glutInit (&argc,argv);
	glutInitDisplayMode (GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize (900,500);
	glutCreateWindow ("Labirinth (project of IG)");
	initializateVision ();
	init ();
	glutKeyboardFunc (keyPressed);
	glutReshapeFunc (reshape);
	glutSpecialFunc (keyboard);
	glutDisplayFunc (display);
	glutPostRedisplay ();
	glutTimerFunc (25,rotateCube,0);
	glutTimerFunc (300000,gameOver,0);  //gioco dura 5 minuti
	loadTextures ();
	glutMainLoop ();
	glDeleteTextures (numTextures,textures);
}
