

#include <iostream>
#include <fstream>
#include <climits>
#include <math.h> 
#include "loadTGA.h"
#include <GL/freeglut.h>
#include <cmath>
using namespace std;

#define GL_CLAMP_TO_EDGE 0x812F 
float cam_angle = 0.0;
float cam_height = 20.0;
float cam_x = 0.0;
float cam_z = 0.0;
float cdr=3.14159265/180.0;

GLuint texId[6];

void drawFloor()
{
	bool flag = false;

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for(int x = -400; x <= 400; x += 5)
	{
		for(int z = -400; z <= 400; z += 5)
		{
			if(flag) glColor3f(0.5, 0.8, 0.3);
			else glColor3f(0.3, 0.8, 0.3);
			glVertex3f(x, 0, z);
			glVertex3f(x, 0, z+5);
			glVertex3f(x+5, 0, z+5);
			glVertex3f(x+5, 0, z);
			//if (x % 3 == 0 && z % 3 == 0) flag = true; else flag = false;
		}
	}
	glEnd();
}

void loadGLTextures()				// Load bitmaps And Convert To Textures
{    
	glGenTextures(6, texId); 		// Create texture ids
	// *** left ***
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	loadTGA("resources/skybox/plains-of-abraham_rt.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

	// *** front ***
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	loadTGA("resources/skybox/plains-of-abraham_ft.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

	// *** right ***
	glBindTexture(GL_TEXTURE_2D, texId[2]);
	loadTGA("resources/skybox/plains-of-abraham_lf.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

	// *** back***
	glBindTexture(GL_TEXTURE_2D, texId[3]);
	loadTGA("resources/skybox/plains-of-abraham_bk.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

	// *** top ***
	glBindTexture(GL_TEXTURE_2D, texId[4]);
	loadTGA("resources/skybox/plains-of-abraham_up.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

	// *** down ***
	glBindTexture(GL_TEXTURE_2D, texId[5]);
	loadTGA("resources/skybox/plains-of-abraham_dn.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
}

void drawSkybox()
{
  ////////////////////// LEFT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex3f(-1000,  0, 1000);
  glTexCoord2f(1, 0);
  glVertex3f(-1000, 0., -1000);
  glTexCoord2f(1, 1);
  glVertex3f(-1000, 1000., -1000);
  glTexCoord2f(0, 1);
  glVertex3f(-1000, 1000, 1000);
  glEnd();

  ////////////////////// FRONT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[1]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex3f(-1000,  0, -1000);
  glTexCoord2f(1, 0);
  glVertex3f(1000, 0., -1000);
  glTexCoord2f(1, 1);
  glVertex3f(1000, 1000, -1000);
  glTexCoord2f(0, 1);
  glVertex3f(-1000,  1000, -1000);
  glEnd();

 ////////////////////// RIGHT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[2]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex3f(1000,  0, -1000);
  glTexCoord2f(1, 0);
  glVertex3f(1000, 0, 1000);
  glTexCoord2f(1, 1);
  glVertex3f(1000, 1000,  1000);
  glTexCoord2f(0, 1);
  glVertex3f(1000,  1000,  -1000);
  glEnd();


  ////////////////////// REAR WALL ////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex3f( 1000, 0, 1000);
  glTexCoord2f(1, 0);
  glVertex3f(-1000, 0,  1000);
  glTexCoord2f(1, 1);
  glVertex3f(-1000, 1000,  1000);
  glTexCoord2f(0, 1);
  glVertex3f( 1000, 1000, 1000);
  glEnd();
  
  /////////////////////// TOP //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[4]);
  glBegin(GL_QUADS);
  glTexCoord2f(1, 0);
  glVertex3f(-1000, 1000, -1000);
  glTexCoord2f(1, 1);
  glVertex3f(1000, 1000,  -1000);
  glTexCoord2f(0, 1);
  glVertex3f(1000, 1000,  1000);
  glTexCoord2f(0, 0);
  glVertex3f(-1000, 1000, 1000);
  glEnd();
  
  /////////////////////// FLOOR //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[5]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 1);
  glVertex3f(-1000, 0., 1000);
  glTexCoord2f(0, 0);
  glVertex3f(1000, 0.,  1000);
  glTexCoord2f(1, 0);
  glVertex3f(1000, 0., -1000);
  glTexCoord2f(1, 1);
  glVertex3f(-1000, 0., -1000);
  glEnd();
}

void drawHouse()
{
    glPushMatrix();
        glColor4f(1.0, 0, 0, 1.0);
        glTranslatef(0, 5, -50);
        glutSolidCube(10);
    glPopMatrix();
}

void display()  
{
	float lpos[4] = {-40, 50, -80, 1.0};  //light's position

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    float xdir = -10000.0*sin(cam_angle*cdr);
	float zdir = -10000.0*cos(cam_angle*cdr);
	gluLookAt(cam_x, cam_height, cam_z, xdir, cam_height, zdir, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position

	//drawFloor();
    glDisable(GL_LIGHTING);
    drawSkybox();
    glEnable(GL_LIGHTING);
    drawHouse();

	glFlush();
}

void initialize()
{
    loadGLTextures();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
    
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);
    
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 1, 1, 5000);  //The camera view volume  
    
}

void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) cam_angle += 2;
    else if(key == GLUT_KEY_RIGHT) cam_angle -= 2;
	else if(key == GLUT_KEY_UP) {
        cam_z -= (1*cos(cam_angle * cdr));
        cam_x -= (1*sin(cam_angle * cdr));
    }
	else if(key == GLUT_KEY_DOWN){
        cam_z += (1*cos(cam_angle * cdr));
        cam_x += (1*sin(cam_angle * cdr));
    }
    
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH);
   glutInitWindowSize (600, 600); 
   glutInitWindowPosition (50, 50);
   glutCreateWindow ("Assignment 1");
   initialize();

   glutSpecialFunc(special); 
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}
