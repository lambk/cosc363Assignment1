

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
int fpv = 0;

//dog vars
float dog_leg_rate = 1.5;
float dog_leg_angle = 0.0;
float dog_leg_dir = 1;
float dog_wag_rate = 3.0;
float dog_wag_angle = 0.0;
float dog_wag_dir = 1;

float dog_walk_posx = 0.0;
float dog_walk_posz = 0.0;
float dog_walk_speed = 0.2;
int dog_walk_dir = 0;
int dog_walk_angles[4] = { 0, 90, 270, 180 };

float sor_angle = 0.0;

float plane_angle = 0.0;

float plane_smoke_1x = 0.0;
float plane_smoke_1y = 0.0;
float plane_smoke_2x = 0.0;
float plane_smoke_2y = 0.0;
float plane_smoke_3x = 0.0;
float plane_smoke_3y = 0.0;

float para_height = 0.0;
float para_angle = 0.0;

float rotor_angle = 0.0;
int heli_mode = 0;
float heli_height = 0.0;
float heli_posx = 0.0;
float heli_angle = 0.0;
float heli_tracking_speed = 0.2; //How 'fast' the heli tracks the player, a high value will result in the heli looking directly at you even when moving perpendicular at close distances

GLuint texId[9];

GLUquadric *q;

float calcHeliAngleToPlayer()
{
	float helix = heli_posx + 11.25;
	float heliz = -55.0;
	float playx = cam_x;
	float playz = cam_z;
	float diffx = playx - helix;
	float diffz = playz - heliz;

	float angle;
	if (diffx == 0) {
		if (diffz > 0) angle = 90.0; else angle = -90.0;
	}
	else if (diffz == 0) {
		if (diffx > 0) angle = 180.0; else angle = 0.0;
	} else {
		if (diffz > 0 && diffx < 0) angle = atan(diffz / -diffx) / cdr;
		if (diffz > 0 && diffx > 0) angle = 180.0 - atan(diffz / diffx) / cdr;
		if (diffz < 0 && diffx < 0) angle = -atan(diffz / diffx) / cdr;
		if (diffz < 0 && diffx > 0) angle = 180.0 + atan(-diffz / diffx) / cdr;
	}
	if (angle >= 360.0) angle -= 360.0;
	return angle;
}

void timer(int value)
{   //dog animations
    if (dog_leg_dir == 1) dog_leg_angle += dog_leg_rate; else dog_leg_angle -= dog_leg_rate;
    if (dog_leg_angle >= 15.0) dog_leg_dir = 0;
    if (dog_leg_angle <= -15.0) dog_leg_dir = 1;
    if (dog_wag_dir == 1) dog_wag_angle += dog_wag_rate; else dog_wag_angle -= dog_wag_rate;
    if (dog_wag_angle >= 30.0) dog_wag_dir = 0;
    if (dog_wag_angle <= -30.0) dog_wag_dir = 1;
    
    //dog travel
	if (dog_walk_dir == 0) {
		dog_walk_posx -= dog_walk_speed;
		if (dog_walk_posx <= -17) dog_walk_dir = 1;
	}
	else if (dog_walk_dir == 1) {
		dog_walk_posz += dog_walk_speed;
		if (dog_walk_posz >= 34) dog_walk_dir = 2;
	}
	else if (dog_walk_dir == 2) {
		dog_walk_posz -= dog_walk_speed;
		if (dog_walk_posz <= 0) dog_walk_dir = 3;
	}
	else {
		dog_walk_posx += dog_walk_speed;
		if (dog_walk_posx >= 17) dog_walk_dir = 0;
	}
	

	sor_angle += 0.5;

	plane_angle += 1;
	if (fmod(plane_angle, 10.0) < 1) {
		plane_smoke_1x = ((rand() % 10) / 10.0) - 0.5;
		plane_smoke_1y = ((rand() % 10) / 10.0) - 0.5;
		plane_smoke_2x = ((rand() % 10) / 10.0) - 0.5;
		plane_smoke_2y = ((rand() % 10) / 10.0) - 0.5;
		plane_smoke_3x = ((rand() % 10) / 10.0) - 0.5;
		plane_smoke_3y = ((rand() % 10) / 10.0) - 0.5;
	}

	para_height += 0.01;
	para_angle++;
    
	rotor_angle += 7;
	if (heli_mode == 0) heli_height += 0.04;
	if (heli_mode == 1) heli_posx -= 0.1;
	if (heli_mode == 2) heli_height -= 0.04;
	if (heli_mode == 3) heli_posx += 0.1;
	if (heli_mode == 0 && heli_height >= 10.0) {
		if (heli_posx >= 0.0) heli_mode = 1; else heli_mode = 3;
	}
	if (heli_mode == 1 && heli_posx <= -22.5) heli_mode = 2;
	if (heli_mode == 2 && heli_height <= 0) heli_mode = 0;
	if (heli_mode == 3 && heli_posx >= 0) heli_mode = 2;

	float player_angle = calcHeliAngleToPlayer();
	if (heli_angle < player_angle) {
		if (player_angle - heli_angle > 180) heli_angle += 360.0;
		heli_angle += heli_tracking_speed;
	}
	if (heli_angle > player_angle) {
		if (heli_angle - player_angle > 180) heli_angle -= 360.0;
		heli_angle -= heli_tracking_speed;
	}

    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}

void loadGLTextures()				// Load bitmaps And Convert To Textures
{    
	glGenTextures(9, texId); 		// Create texture ids
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

	// *** roof ***
	glBindTexture(GL_TEXTURE_2D, texId[6]);
	loadTGA("resources/roof.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// *** para ***
	glBindTexture(GL_TEXTURE_2D, texId[7]);
	loadTGA("resources/para.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// *** carpet ***
	glBindTexture(GL_TEXTURE_2D, texId[8]);
	loadTGA("resources/carpet.tga");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[6]);
    glBegin(GL_TRIANGLES);
		glTexCoord2f(0, 0);
        glVertex3f(1.0f, 0.0f, -1.0f);
		glTexCoord2f(0.5, 1);
        glVertex3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(1, 0);
        glVertex3f(-1.0f, 0.0f, -1.0f);
        
		glTexCoord2f(0, 0);
        glVertex3f(-1.0f, 0.0f, -1.0f);
		glTexCoord2f(0.5, 1);
        glVertex3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(1, 0);
        glVertex3f(-1.0f, 0.0f, 1.0f);
        
		glTexCoord2f(0, 0);
        glVertex3f(-1.0f, 0.0f, 1.0f);
		glTexCoord2f(0.5, 1);
        glVertex3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(1, 0);
        glVertex3f(1.0f, 0.0f, 1.0f);
        
		glTexCoord2f(0, 0);
        glVertex3f(1.0f, 0.0f, 1.0f);
		glTexCoord2f(0.5, 1);
        glVertex3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(1, 0);
        glVertex3f(1.0f, 0.0f, -1.0f);
    glEnd();
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.4, 0.4, 0.4);
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glEnd();
}

void drawFloor()
{
    float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};
	glColor4f(0.9, 0.9, 0.9, 1.0);  //The floor is gray in colour
	glNormal3f(0.0, 1.0, 0.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[8]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glBegin(GL_QUADS);
    float step = 0.5;
	for(float i = -19.0; i < 19.0; i+=step)
	{
		for(float j = -20.0;  j < 20.0; j+=step)
		{
			glTexCoord2f((i + 19) / 19.0, (j + 20)/20.0);
			glVertex3f(i, 0.5, j);
			glTexCoord2f((i + 19) / 19.0, (j + step + 20) / 20.0);
			glVertex3f(i, 0.5, j+step);
			glTexCoord2f((i + step + 19) / 19.0, (j + step + 20) / 20.0);
			glVertex3f(i+step, 0.5, j+step);
			glTexCoord2f((i + step + 19) / 19.0, (j + 20) / 20.0);
			glVertex3f(i+step, 0.5, j);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
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
    glPushMatrix();
        glTranslatef(0, 20, 0);
        glScalef(22, 20, 23);
        roof();
    glPopMatrix();
    //floor
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

void normal(float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
	float nx, ny, nz;
	nx = y1*(z2 - z3) + y2*(z3 - z1) + y3*(z1 - z2);
	ny = z1*(x2 - x3) + z2*(x3 - x1) + z3*(x1 - x2);
	nz = x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2);

	glNormal3f(nx, ny, nz);
}

void drawSoR()
{
	const int N = 10;
	float vx[N] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	float vy[N] = { 0, 1, 2, 1, 0, 1, -1, 0, 0.5, 0.6 };
	float vz[N] = { 0 };
	float wx[N], wy[N], wz[N];

	float step = 10.0 * cdr;
	glColor3f(0.5, 0, 0.5);
	for (int i = 0; i < 36; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int o = 0; o < N; o++) {
			wx[o] = vx[o] * cos(step) + vz[o] * sin(step);
			wy[o] = vy[o];
			wz[o] = (-vx[o]) * sin(step) + vz[o] * cos(step);
		}

		for (int o = 0; o < N; o++) {
			if (o > 0) normal(wx[o - 1], wy[o - 1], wz[o - 1], vx[o - 1], vy[o - 1], vz[o - 1], vx[o], vy[o], vz[o]);
			glVertex3f(vx[o], vy[o], vz[o]);
			if (o > 0) normal(wx[o - 1], wy[o - 1], wz[o - 1], vx[o], vy[o], vz[o], wx[o], wy[o], wz[o]);
			glVertex3f(wx[o], wy[o], wz[o]);
		}

		for (int o = 0; o < N; o++) {
			vx[o] = wx[o];
			vy[o] = wy[o];
			vz[o] = wz[o];
		}
		glEnd();
	}
}

void drawPlane()
{
	glColor3f(1, 1, 1);
	glPushMatrix();
	gluCylinder(q, 0.5, 0.5, 4, 20, 20);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslatef(0, 0, -1);
	gluCylinder(q, 0, 0.5, 1, 20, 20);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(plane_smoke_1x, plane_smoke_1y, 5);
	gluCylinder(q, 0, 0.3, 0.5, 20, 20);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(plane_smoke_2x, plane_smoke_2y, 4.5);
	gluCylinder(q, 0, 0.25, 0.5, 20, 20);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(plane_smoke_3x, plane_smoke_3y, 5.4);
	gluCylinder(q, 0, 0.2, 0.4, 20, 20);
	glPopMatrix();
	glColor3f(0.2, 0.2, 0.2);
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5, 0, 3);
	glVertex3f(-3, 0, 3);
	glVertex3f(-0.5, 0, 0.5);
	glVertex3f(0.5, 0, 3);
	glVertex3f(0.5, 0, 0.5);
	glVertex3f(3, 0, 3);
	glVertex3f(0, 0.5, 3.3);
	glVertex3f(0, 0.5, 4.2);
	glVertex3f(0, 1.8, 4.3);
	glVertex3f(-0.5, 0, 3.3);
	glVertex3f(-1.8, 0, 4.3);
	glVertex3f(-0.5, 0, 4.2);
	glVertex3f(0.5, 0, 3.3);
	glVertex3f(0.5, 0, 4.2);
	glVertex3f(1.8, 0, 4.3);
	glEnd();
}

void drawHPara()
{
	float c = 0.6;
	float a = 3.0;
	float b = 3.0;
	float vy[30] = { 0 };
	for (float x = -15; x < 15.0; x++) {
		vy[(int)x + 15] = (float)(c*(pow(-15, 2)/pow(a, 2) - pow(x, 2) / pow(b, 2)));
	}
	float wy[30] = { 0 };

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[7]);
	for (float z = -15; z < 15; z++) {
		glBegin(GL_QUAD_STRIP);
		for (float x = -15; x < 15.0; x++) {
			wy[(int)x + 15] = (float)(c*(pow(z+1, 2) / pow(a, 2) - pow(x, 2) / pow(b, 2)));
		}
		for (int x = 0; x < 30; x++) {
			if (x > 0) normal(x - 16, vy[x - 1], z, x - 15, wy[x], z + 1, x-15, vy[x], z);
			glTexCoord2f(x/30.0, (z+15)/30.0);
			glVertex3f(x-15, vy[x], z);
			glTexCoord2f(x / 30.0, (z + 16) / 30.0);
			glVertex3f(x-15, wy[x], z + 1);
		}
		for (int i = 0; i < 30; i++) {
			vy[i] = wy[i];
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	
}

void drawHeli()
{
	//body
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0, 1.2, 0);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, 1.2, 0);
	glScalef(3, 0.2, 0.2);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.5, 1.5, 0);
	glScalef(0.3, 1.2, 0.2);
	glutSolidCube(1);
	glPopMatrix();

	glColor3f(0, 0, 0);

	//rotors
	glPushMatrix();
	glTranslatef(0, 2.5, 0);
	glRotatef(rotor_angle + 45, 0, 1, 0);
	glScalef(6, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 2.5, 0);
	glRotatef(rotor_angle - 45, 0, 1, 0);
	glScalef(6, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 2.2, 0);
	glScalef(0.4, 0.6, 0.4);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.5, 1.5, 0.15);
	glRotatef(rotor_angle, 0, 0, 1);
	glScalef(0.1, 1.2, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();

	glPopMatrix();

	//skids
	glPushMatrix();
	glTranslatef(0, 0.1, 0);
	glScalef(0.5, 0.4, 1);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, 0.5);
	glScalef(3, 0.2, 0.2);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, -0.5);
	glScalef(3, 0.2, 0.2);
	glutSolidCube(1);
	glPopMatrix();
}

void display()  
{
	float lpos[4] = {-200, 50, -200, 1.0};  //light's position
    float spot_pos[] = {0.0f, 30.0f, -80.0f, 1.0f};
    float spot_dir[] = {0, -1, 0};

	float spot2_pos[] = { dog_walk_posx, 5.0f, -100 + dog_walk_posz, 1.0f };
	float spot2_dirx[4] = { -1, 0, 0, 1 };
	float spot2_dirz[4] = { 0, 1, -1, 0 };
	float spot2_dir[] = { spot2_dirx[dog_walk_dir], -0.5, spot2_dirz[dog_walk_dir] };

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float fpv_offsetx[4] = { -4.2, 0, 0, 4.2 };
	float fpv_offsetz[4] = { 0, 4.2, -4.2, 0 };
    
    float xdir = -10000.0*sin(cam_angle*cdr);
	float zdir = -10000.0*cos(cam_angle*cdr);
	if (fpv == 1) {
		gluLookAt(dog_walk_posx + fpv_offsetx[dog_walk_dir], 3.5, dog_walk_posz - 100 + fpv_offsetz[dog_walk_dir], spot2_dirx[dog_walk_dir] * 10000, 0, spot2_dirz[dog_walk_dir] * 10000, 0, 1, 0);
		//gluLookAt(15 * cos((plane_angle + 25) * cdr), 15, -15 * sin((plane_angle + 25) * cdr) - 80, -10000 * sin(plane_angle * cdr), -1, -10000 * cos(plane_angle*cdr), 0, 1, 0);
	}
	else {
		gluLookAt(cam_x, cam_height, cam_z, xdir, cam_height, zdir, 0, 1, 0);
	}
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);
    glLightfv(GL_LIGHT1, GL_POSITION, spot_pos); 
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot2_dir);
	glLightfv(GL_LIGHT2, GL_POSITION, spot2_pos);

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
        glTranslatef(dog_walk_posx, 0, dog_walk_posz);
        glTranslatef(0, 3.25, -100);
        glRotatef(dog_walk_angles[dog_walk_dir], 0, 1, 0);
		glScalef(1.2, 1.2, 1.2);
        drawDog();
    glPopMatrix();

	glPushMatrix();
		glTranslatef(0, 1.5, -80);
		glRotatef(sor_angle, 0, 1, 0);
		glScalef(1, 1, 1);
		drawSoR();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 15, -80);
	glRotatef(plane_angle, 0, 1, 0);
	glTranslatef(15, 0, 0);
	glRotatef(30, 0, 0, 1);
	glScalef(1.8, 1.8, 1.8);
	drawPlane();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 2*sin(para_height), -80);
	glRotatef(para_angle, 0, 1, 0);
	glTranslatef(0, 10, 0);
	glScalef(0.3, 0.3, 0.3);
	drawHPara();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(heli_posx + 11.25, heli_height + 0.5, -55);
	glScalef(2, 2, 2);
	glRotatef(heli_angle, 0, 1, 0);
	drawHeli();
	glPopMatrix();
    
    glColor4f(1.0, 1.0, 1.0, 1.0);
    
	glFlush();
}

void initialize()
{
    loadGLTextures();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

	q = gluNewQuadric();

	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
    
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);

    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 70.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,10);

	glLightfv(GL_LIGHT2, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT2, GL_SPECULAR, white);

	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 70.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10);
    
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);
    
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 1, 0.1, 5000);  //The camera view volume  
    
}

void special(int key, int x, int y)
{
	if (fpv == 0) {
		if (key == GLUT_KEY_LEFT) cam_angle += 4;
		else if (key == GLUT_KEY_RIGHT) cam_angle -= 4;
		else if (key == GLUT_KEY_UP) {
			cam_z -= (2 * cos(cam_angle * cdr));
			cam_x -= (2 * sin(cam_angle * cdr));
		}
		else if (key == GLUT_KEY_DOWN) {
			cam_z += (2 * cos(cam_angle * cdr));
			cam_x += (2 * sin(cam_angle * cdr));
		}
	}
	if (key == GLUT_KEY_F1) {
		fpv = !fpv;
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
