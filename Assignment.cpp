

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
float cam_height = 10.0;
float cam_x = 0.0;
float cam_z = 0.0;
float cdr=3.14159265/180.0;


//dog vars
float dog_leg_rate = 1.5;
float dog_leg_angle = 0.0;
float dog_leg_dir = 1;
float dog_wag_rate = 3.0;
float dog_wag_angle = 0.0;
float dog_wag_dir = 1;

float dog_walk_pos = 0.0;
float dog_walk_speed = 0.2;
float dog_walk_dir = 0;

GLuint texId[6];

void timer(int value)
{   //dog animations
    if (dog_leg_dir == 1) dog_leg_angle += dog_leg_rate; else dog_leg_angle -= dog_leg_rate;
    if (dog_leg_angle >= 15.0) dog_leg_dir = 0;
    if (dog_leg_angle <= -15.0) dog_leg_dir = 1;
    if (dog_wag_dir == 1) dog_wag_angle += dog_wag_rate; else dog_wag_angle -= dog_wag_rate;
    if (dog_wag_angle >= 30.0) dog_wag_dir = 0;
    if (dog_wag_angle <= -30.0) dog_wag_dir = 1;
    
    //dog travel
    if (dog_walk_dir == 1) dog_walk_pos += dog_walk_speed; else dog_walk_pos -= dog_walk_speed;
    if (dog_walk_pos >= 17) dog_walk_dir = 0;
    if (dog_walk_pos <= -17) dog_walk_dir = 1;
    
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
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
	loadTGA("resources/skybox/grass.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
}

void drawSkybox()
{
  ////////////////////// LEFT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex3f(-1000,  -400, 1000);
  glTexCoord2f(1, 0);
  glVertex3f(-1000, -400., -1000);
  glTexCoord2f(1, 1);
  glVertex3f(-1000, 600., -1000);
  glTexCoord2f(0, 1);
  glVertex3f(-1000, 600, 1000);
  glEnd();

  ////////////////////// FRONT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[1]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex3f(-1000,  -400, -1000);
  glTexCoord2f(1, 0);
  glVertex3f(1000, -400., -1000);
  glTexCoord2f(1, 1);
  glVertex3f(1000, 600, -1000);
  glTexCoord2f(0, 1);
  glVertex3f(-1000,  600, -1000);
  glEnd();

 ////////////////////// RIGHT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[2]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex3f(1000,  -400, -1000);
  glTexCoord2f(1, 0);
  glVertex3f(1000, -400, 1000);
  glTexCoord2f(1, 1);
  glVertex3f(1000, 600,  1000);
  glTexCoord2f(0, 1);
  glVertex3f(1000,  600,  -1000);
  glEnd();


  ////////////////////// REAR WALL ////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 0);
  glVertex3f( 1000, -400, 1000);
  glTexCoord2f(1, 0);
  glVertex3f(-1000, -400,  1000);
  glTexCoord2f(1, 1);
  glVertex3f(-1000, 600,  1000);
  glTexCoord2f(0, 1);
  glVertex3f( 1000, 600, 1000);
  glEnd();
  
  /////////////////////// TOP //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[4]);
  glBegin(GL_QUADS);
  glTexCoord2f(1, 0);
  glVertex3f(-1000, 600, -1000);
  glTexCoord2f(1, 1);
  glVertex3f(1000, 600,  -1000);
  glTexCoord2f(0, 1);
  glVertex3f(1000, 600,  1000);
  glTexCoord2f(0, 0);
  glVertex3f(-1000, 600, 1000);
  glEnd();
  
  /////////////////////// FLOOR //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[5]);
  glBegin(GL_QUADS);
  glTexCoord2f(0, 200.0);
  glVertex3f(-1000, 0., 1000);
  glTexCoord2f(0, 0);
  glVertex3f(1000, 0.,  1000);
  glTexCoord2f(200.0, 0);
  glVertex3f(1000, 0., -1000);
  glTexCoord2f(200.0, 200.0);
  glVertex3f(-1000, 0., -1000);
  glEnd();
}

void roof()
{
    glBegin(GL_TRIANGLES);
        glVertex3f(1.0f, 0.0f, -1.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, 0.0f, -1.0f);
        
        glVertex3f(-1.0f, 0.0f, -1.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, 0.0f, 1.0f);
        
        glVertex3f(-1.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 1.0f);
        
        glVertex3f(1.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, -1.0f);
    glEnd();
}

void drawFloor()
{
    float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};
	glColor4f(0.9, 0.9, 0.9, 1.0);  //The floor is gray in colour
	glNormal3f(0.0, 1.0, 0.0);

    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glBegin(GL_QUADS);
    float step = 0.5;
	for(float i = -19.0; i < 19.0; i+=step)
	{
		for(float j = -20.0;  j < 20.0; j+=step)
		{
			glVertex3f(i, 0.5, j);
			glVertex3f(i, 0.5, j+step);
			glVertex3f(i+step, 0.5, j+step);
			glVertex3f(i+step, 0.5, j);
		}
	}
	glEnd();
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}

void drawHouse()
{
    //Walls
    //front
    glPushMatrix(); //patio
        glColor3f(0.0, 1.4, 1.4);
        glTranslatef(0, 0, 24);
        glScalef(12, 1, 8);
        glutSolidCube(1);
    glPopMatrix();
    glColor3f(0.8, 0.5, 0.0);
    glPushMatrix(); //doorway top
        glTranslatef(0, 18, 20);
        glScalef(4, 2, 1);
        glutSolidCube(2);
    glPopMatrix();    
    glPushMatrix(); //left of doorway
        glTranslatef(-12, 10, 20);
        glScalef(1.6, 2, 0.2);
        glutSolidCube(10);
    glPopMatrix();
    glPushMatrix(); //right of doorway
        glTranslatef(12, 10, 20);
        glScalef(1.6, 2, 0.2);
        glutSolidCube(10);
    glPopMatrix();
    //left
    glPushMatrix(); 
        glTranslatef(-19, 10, 0);
        glScalef(0.2, 2, 4);
        glutSolidCube(10);
    glPopMatrix();
    //back
    glPushMatrix();
        glTranslatef(0, 10, -20);
        glScalef(4, 2, 0.2);
        glutSolidCube(10);
    glPopMatrix();
    //right
    glPushMatrix();
        glTranslatef(19, 10, 0);
        glScalef(0.2, 2, 4);
        glutSolidCube(10);
    glPopMatrix();
    //Roof
    glColor3f(0.5, 0.8, 0.8);
    glPushMatrix();
        glTranslatef(0, 20, 0);
        glScalef(22, 20, 23);
        roof();
    glPopMatrix();
    //floor
    //glPushMatrix();
    //    glColor3f(0.8, 0.8, 0.8);
    //    glTranslatef(0, 0, 0);
    //    glScalef(3.8, 0.1, 4);
    //    glutSolidCube(10);
    //glPopMatrix();
    drawFloor();
}

void drawDog()
{
    //body
    glColor3f(0.8, 0.7, 0.2);
    glPushMatrix();
        glScalef(1, 0.3, 0.3);
        glutSolidCube(5);
    glPopMatrix();
    //head
    glPushMatrix();
        glTranslatef(-3, 0, 0);
        glPushMatrix(); 
            glutSolidCube(1.1);
        glPopMatrix();
        glPushMatrix(); //left ear
            glTranslatef(0, 0.9, 0.3);
            glScalef(0.05, 0.5, 0.3);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix(); //right ear
            glTranslatef(0, 0.9, -0.3);
            glScalef(0.05, 0.5, 0.3);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix(); //snoot
            glTranslatef(-0.9, -0.25, 0);
            glScalef(0.8, 0.5, 0.5);
            glutSolidCube(1);
        glPopMatrix();
        glPushMatrix(); //left eye
            glColor3f(1, 1, 1);
            glTranslatef(-0.55, 0.3, 0.3);
            glScalef(0.2, 1, 1);
            glutSolidCube(0.25);
            glPushMatrix();
                glColor3f(0, 0, 0);
                glTranslatef(-0.2, 0, -0.05);
                glutSolidCube(0.15);
            glPopMatrix();
        glPopMatrix();
        glPushMatrix(); //right eye
            glColor3f(1, 1, 1);
            glTranslatef(-0.55, 0.3, -0.3);
            glScalef(0.2, 1, 1);
            glutSolidCube(0.25);
            glPushMatrix();
                glColor3f(0, 0, 0);
                glTranslatef(-0.2, 0, 0.05);
                glutSolidCube(0.15);
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
    
    glColor3f(0.8, 0.7, 0.2); //color reset
    //tail
    glPushMatrix();
        glTranslatef(2.3, 0.55, 0);
        glRotatef(dog_wag_angle, 0, 1, 0);
        glRotatef(30, 0, 0, 1);
        glTranslatef(1.5, 0, 0);
        glScalef(3, 0.5, 0.5);
        glutSolidCube(1);
    glPopMatrix();
    //fl leg
    glPushMatrix();
        glTranslatef(-2.2, -0.75, 0.45);
        glRotatef(dog_leg_angle, 0, 0, 1);
        glTranslatef(0, -1, 0);
        glScalef(0.5, 2, 0.5);
        glutSolidCube(1);
    glPopMatrix();
    //fr leg
    glPushMatrix();
        glTranslatef(-2.2, -0.75, -0.45);
        glRotatef(-dog_leg_angle, 0, 0, 1);
        glTranslatef(0, -1, 0);
        glScalef(0.5, 2, 0.5);
        glutSolidCube(1);
    glPopMatrix();
    //bl leg
    glPushMatrix();
        glTranslatef(2.2, -0.75, 0.45);
        glRotatef(dog_leg_angle, 0, 0, 1);
        glTranslatef(0, -1, 0);
        glScalef(0.5, 2, 0.5);
        glutSolidCube(1);
    glPopMatrix();
    //br leg
    glPushMatrix();
        glTranslatef(2.2, -0.75, -0.45);
        glRotatef(-dog_leg_angle, 0, 0, 1);
        glTranslatef(0, -1, 0);
        glScalef(0.5, 2, 0.5);
        glutSolidCube(1);
    glPopMatrix();
}

void display()  
{
	float lpos[4] = {-200, 50, -200, 1.0};  //light's position
    float spot_pos[] = {0.0f, 30.0f, -80.0f, 1.0f};
    float spot_dir[] = {0, -1, 0};

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    float xdir = -10000.0*sin(cam_angle*cdr);
	float zdir = -10000.0*cos(cam_angle*cdr);
	gluLookAt(cam_x, cam_height, cam_z, xdir, cam_height, zdir, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);
    glLightfv(GL_LIGHT1, GL_POSITION, spot_pos); 

	//drawFloor();
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    drawSkybox();
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    
    glPushMatrix();
        glTranslatef(0, 0, -80);
        glScalef(1.8, 1, 1.8);
        drawHouse();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(dog_walk_pos, 0, 0);
        glTranslatef(0, 3.25, -100);
        if (dog_walk_dir == 1) {
            glRotatef(180, 0, 1, 0);
        }
        drawDog();
    glPopMatrix();
    
    glColor4f(1.0, 1.0, 1.0, 1.0);
    
	glFlush();
}

void initialize()
{
    loadGLTextures();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 70.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,10);
    
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);
    
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 1, 1, 5000);  //The camera view volume  
    
}

void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) cam_angle += 4;
    else if(key == GLUT_KEY_RIGHT) cam_angle -= 4;
	else if(key == GLUT_KEY_UP) {
        cam_z -= (2*cos(cam_angle * cdr));
        cam_x -= (2*sin(cam_angle * cdr));
    }
	else if(key == GLUT_KEY_DOWN){
        cam_z += (2*cos(cam_angle * cdr));
        cam_x += (2*sin(cam_angle * cdr));
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

   glutTimerFunc(10, timer, 0);
   glutSpecialFunc(special); 
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;
}
