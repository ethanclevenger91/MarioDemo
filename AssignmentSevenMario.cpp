/*
 *author: Ethan Clevenger
 *date: 11/12/12
 */

#include <stdlib.h>
#include "stdafx.h"
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "Windows.h"

using namespace std;

#define TRUE 1
#define FALSE 0

double marioPos = 0.0;
double marioHops = 0.0;
int time = 0;
bool up = false;
bool canJump = true;
bool facingRight = true;
bool movingRight = false;
bool movingLeft = false;
bool hor = false;

GLfloat vertices[] = {-1.0,-1.0,0.0, 1.0,-1.0,0.0, 1.0,1.0,0.0, -1.0,1.0,0.0};
GLfloat texVertices[] = {0.0,1.0, 1.0,1.0, 1.0,0.0, 0.0,0.0};

GLfloat vertices2[] = {-1.0,-1.0,1.0, 1.0,-1.0,1.0, 1.0,1.0,1.0, -1.0,1.0,1.0};
GLfloat texVertices2[] = {0.0,1.0, 1.0,1.0, 1.0,0.0, 0.0,0.0};

GLfloat vertices3[] = {-1.0,-1.0,-1.0, 1.0,-1.0,-1.0, 1.0,1.0,-1.0, -1.0,1.0,-1.0};
GLfloat texVertices3[] = {0.0,1.0, 1.0,1.0, 1.0,0.0, 0.0,0.0};

GLfloat vertices4[] = {-1.0,-1.0,-0.5, 1.0,-1.0,-0.5, 1.0,1.0,-0.5, -1.0,1.0,-0.5};
GLfloat texVertices4[] = {0.0,1.0, 1.0,1.0, 1.0,0.0, 0.0,0.0};


#define WIDTH 512
#define HEIGHT 500
#define TRUE 1
#define FALSE 0

int texWidth, texHeight;
GLubyte * texture=NULL;

int texWidth2, texHeight2;
GLubyte * texture2=NULL;

int texWidth3, texHeight3;
GLubyte * texture3=NULL;

int texWidth4, texHeight4;
GLubyte * texture4=NULL;

int texWidth5, texHeight5;
GLubyte * texture5=NULL;

GLuint texName[5];


GLubyte*   //Thanks to Tim Urness for this function
glmReadPPM(char* filename, int* width, int* height)
{
    FILE* fp;
    int i, j,w, h, d, r,g,b,c;
    unsigned char* image;
    char head[70];          /* max line <= 70 in PPM (per spec). */
    
    fp = fopen(filename, "r");
    if (!fp) {
        printf("error opening file %s\n",filename);
        return NULL;
    }
    
	/* read and discard first two lines */
	fgets(head, 128, fp);
	fgets(head, 128, fp);
	
	/* read image size and (?)max component value */
	fscanf(fp, "%d %d", &w, &h);
	fscanf(fp, "%d", &d);
	image = (unsigned char*)malloc(sizeof(unsigned char)*w*h*4);
	
	c = 0;
	for (i=0; i<h; i++) {
		for (j=0; j<w; j++) {
			fscanf(fp, "%d %d %d", &r, &g, &b);
			image[c++] = (GLubyte) r;
			image[c++] = (GLubyte) g;
			image[c++] = (GLubyte) b;
			//if pixel is mostly black, make it transparent
			if (r<50 && g<50 && b<50)
				image[c++] = (GLubyte) 0;
			else
				image[c++] = (GLubyte) 255;
		}
	}
	
    *width = w;
    *height = h;
    return image;
}


void init(void)
{
    
    glGenTextures(4, texName);
    
	glClearColor (0.0, 0.0, 0.0, 0.0);
	
    glBindTexture(GL_TEXTURE_2D, texName[0]);
	texture = glmReadPPM("marioASCII.ppm", &texWidth, &texHeight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight,
				 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
    
    glBindTexture(GL_TEXTURE_2D, texName[1]);
	texture2 = glmReadPPM("warpPipeASCII.ppm", &texWidth2, &texHeight2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth2, texHeight2,
				 0, GL_RGBA, GL_UNSIGNED_BYTE, texture2);

	glBindTexture(GL_TEXTURE_2D, texName[2]);
	texture3 = glmReadPPM("bckgrdTexASCII.ppm", &texWidth3, &texHeight3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth3, texHeight3,
				 0, GL_RGBA, GL_UNSIGNED_BYTE, texture3);

	glBindTexture(GL_TEXTURE_2D, texName[3]);
	texture4 = glmReadPPM("warpPipeASCII.ppm", &texWidth4, &texHeight4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth4, texHeight4,
				 0, GL_RGBA, GL_UNSIGNED_BYTE, texture4);

	glBindTexture(GL_TEXTURE_2D, texName[4]);
	texture5 = glmReadPPM("jumpASCII.ppm", &texWidth5, &texHeight5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth5, texHeight5,
				 0, GL_RGBA, GL_UNSIGNED_BYTE, texture5);

	glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1,1,-1,1,1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    gluLookAt(0,0,3.0,0,0,0,0,1,0);
    
    
    
}

void display(void)
{
	//clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	
	glPushMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glPushMatrix();
	{
		glScaled(4, 4, 1);

		glBindTexture(GL_TEXTURE_2D, texName[2]);
		glVertexPointer(3, GL_FLOAT, 0, vertices3);
		glTexCoordPointer(2, GL_FLOAT, 0, texVertices3);
		glDrawArrays(GL_POLYGON, 0, 4);

		glPopMatrix();
	}
	
	glPushMatrix();
	{
		glScaled(.43, .63, 1);
		glTranslated(3.0, -3.45, 0);

		glBindTexture(GL_TEXTURE_2D, texName[3]);
		glVertexPointer(3, GL_FLOAT, 0, vertices4);
		glTexCoordPointer(2, GL_FLOAT, 0, texVertices4);
		glDrawArrays(GL_POLYGON, 0, 4);

		glPopMatrix();
	}

    glPushMatrix();
	{
		glScaled(.25, .25, 1);
		if(movingRight == true)
		{marioPos+=.2;}
		if(movingLeft == true)
		{marioPos-=.2;}
		glTranslated(-7.0 + marioPos, -8.6+marioHops, 0.0);
		if(facingRight == false)
		{
			glRotated(180, 0, 1, 0);
		}

		if(marioHops > 0.3)
		{
			glBindTexture(GL_TEXTURE_2D, texName[4]);
			glVertexPointer(3, GL_FLOAT, 0, vertices);
			glTexCoordPointer(2, GL_FLOAT, 0, texVertices);
			glDrawArrays(GL_POLYGON, 0, 4);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, texName[0]);
			glVertexPointer(3, GL_FLOAT, 0, vertices);
			glTexCoordPointer(2, GL_FLOAT, 0, texVertices);
			glDrawArrays(GL_POLYGON, 0, 4);
			canJump = true;
		}

		glPopMatrix();
	}

	glPushMatrix();
	{
		glScaled(.25, .35, 1);
		glTranslated(-1.7, -3.62, 0);

	    glBindTexture(GL_TEXTURE_2D, texName[1]);
	    glVertexPointer(3, GL_FLOAT, 0, vertices2);
	    glTexCoordPointer(2, GL_FLOAT, 0, texVertices2);
	    glDrawArrays(GL_POLYGON, 0, 4);

		glPopMatrix();
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
    
	
	glPopMatrix();
	glutSwapBuffers();
    
	glDisable(GL_TEXTURE_2D);
}

void keyboard (unsigned char key, int x, int y)
{
	//escape key kills program
	switch (key) {
		case 27:
		exit(0);
			break;
		case ' ':
			if(canJump == true)
			{
				up = true;
				canJump = false;
			}
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void special (int key, int x, int y)
{
	switch(key) {
	case GLUT_KEY_LEFT:
		facingRight = false;
		movingRight = false;
		movingLeft = true;
		break;
	case GLUT_KEY_RIGHT:
		facingRight = true;
		movingRight = true;
		movingLeft = false;
		break;
	default:
		movingRight = false;
		movingLeft = false;
		return;
	}
	glutPostRedisplay();
}
void special_up (int key, int x, int y)
{
	switch(key) {
	case GLUT_KEY_LEFT:
		movingLeft = false;
		break;
	case GLUT_KEY_RIGHT:
		movingRight = false;
		break;
	default:
		return;
	}
	glutPostRedisplay();
}

void specialUp (int key, int x, int y)
{}


void timer(int extra) {
	if(up == true) 
	{
		marioHops+=.33;
		if(marioHops > 5)
		{
			up = false;
		}
	}
	else if(up == false)
	{
		if(marioHops > 0)
		{
			marioHops-=.33;
		}
	}
	glutTimerFunc(extra, timer,extra);
	glutPostRedisplay();
}

/*void mouse_dragged(int x, int y) {
	//allow for rotation of object when mouse is dragged
	double thetaY, thetaX;
	if (left_button_down) {
		thetaY = 360.0 *(x-prevMouseX)/WIDTH;
		thetaX = 360.0 *(y-prevMouseY)/HEIGHT;
		prevMouseX = x;
		prevMouseY = y;
		view_rotx += thetaX;
		view_roty += thetaY;
	}
	else if (right_button_down) {
		z_distance = 5.0*(prevMouseY-y)/HEIGHT;
	}
	glutPostRedisplay();
}*/


/*void mouse(int button, int state, int x, int y) {
	//establish point of reference for dragging mouse in window
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		left_button_down = TRUE;
		prevMouseX= x;
		prevMouseY = y;
    }
	
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		right_button_down = TRUE;
		prevMouseX = x;
		prevMouseY = y;
    }
    else if (state == GLUT_UP) {
		left_button_down = FALSE;
		right_button_down = FALSE;
	}
}*/

int main(int argc, char** argv)
{
	PlaySound(TEXT("background.wav"), NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
	glutInit(&argc, argv);
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Drake CS 147");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutSpecialUpFunc(special_up);
	glutTimerFunc(0, timer, 10);
	//glutMouseFunc(mouse);
	//glutMotionFunc(mouse_dragged);
	glutMainLoop();
	return 0;
}
