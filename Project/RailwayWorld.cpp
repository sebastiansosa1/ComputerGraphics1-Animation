//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  FILE NAME: RailwayWorld.cpp
//  USAGE: compile: g++ -Wall RailwayWorld.cpp RailModels.cpp -lGL -lGLU -lglut
//         execute: ./a.out
//  ========================================================================

#include <math.h>
#include <GL/freeglut.h>
#include "RailModels.h"
#include "loadTGA.h"
#include "loadBMP.h"


//------------------------ GLOBAL VARIABLES ---------------------------

float theta = 100.; //Global variable counter for the animation - Train starting point @ train station
int timeCounter = 0;
float ex = -20., ey = 20., ez = 200.; //Global parameters for camera e:position, l:lookAt
float camAngle = -0.1;
float 	lx = ex + 100*sin(camAngle), lz = ez - 100*cos(camAngle);
float sunAngle = 0;
int cameraMode = 0; //Default Mode - Navigational


//----------------------- TEXTURES DEFINITION --------------------------------
GLuint txId[2];
GLUquadricObj* q = gluNewQuadric();

void loadTexture()
{
	glGenTextures(2, txId);

	glBindTexture(GL_TEXTURE_2D, txId[0]);
	loadBMP("grass.bmp");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[1]);
	loadBMP("Sun.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


//----------------------- TIMER --------------------------
// Timer function. Controls the animation. TimeCounter
// restarts every 11sec.
//--------------------------------------------------------
void timer (int value)
{
    sunAngle += 0.5;
    timeCounter ++; // Helper variable for setting train delay at trainstation (2sec)

    if (timeCounter <= 360) theta ++;
    if (timeCounter == 440) timeCounter=0;
    if (sunAngle == 360) sunAngle = 0;
    if (theta == 360) theta = 0;

    glutTimerFunc(25, timer, value);
    glutPostRedisplay();
}


//----------------------- CAMERA -------------------------
// Switch cameras with 'C' or 'c' key. In default mode
// navigation through the scene is possible using arrow keys and 
// 'Pg Up' and 'Pg dn' keys.
//--------------------------------------------------------
void camSpecial(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) camAngle -= 0.1;    // Change direction
	else if(key == GLUT_KEY_RIGHT) camAngle += 0.1;
	else if(key == GLUT_KEY_DOWN) {
		ex -= 2*sin(camAngle);
		ez += 2*cos(camAngle);
	}
	else if(key == GLUT_KEY_UP) {
		ex += 2*sin(camAngle);
		ez -= 2*cos(camAngle);
	}
    else if(key == GLUT_KEY_PAGE_UP) ey += 1;
    else if(key == GLUT_KEY_PAGE_DOWN) ey -= 1;
	lx = ex + 100*sin(camAngle);
	lz = ez - 100*cos(camAngle);
    glutPostRedisplay();
}


void camToggler(unsigned char key, int x, int y)
{
    int numCameras = 3;
    if(key == 'C' || key == 'c') cameraMode ++;
    if (cameraMode == numCameras) cameraMode = 0;
}


void cameraControl(void)
{
   if (cameraMode == 0) gluLookAt(ex, ey, ez, lx, 0, lz, 0, 1, 0);
   else if (cameraMode == 1 || cameraMode == 2) {
        float toRad = 3.14159265/180.0;  //Conversion from degrees to radians
        float radiusTrack = 120;
        float eangle = (theta + 5) * toRad;
        float langle = (theta + 10) * toRad;
        float ex1 = -radiusTrack * sin(eangle);
        float ey1 = 10;
        float ez1 = -radiusTrack * cos(eangle);
        float lx1, ly1, lz1;
        if (cameraMode == 1) {
                lx1 = -radiusTrack * sin(langle);
                ly1 = 8;
                lz1 = -radiusTrack * cos(langle);
        } else if (cameraMode == 2) {
                eangle = (theta - 21) * toRad;
                ex1 = -1.5* sin(eangle);
                ey1 = 57.5;
                ez1 = -1.5* cos(eangle);
                lx1 = -radiusTrack * sin(eangle);
                ly1 = 3;
                lz1 = -radiusTrack * cos(eangle);
        }
        gluLookAt(ex1, ey1, ez1, lx1, ly1, lz1, 0, 1, 0);
    }
}


//----------------------- Window Reshaper -----------------
// Maintains the image ratio when reshaping the window.
//--------------------------------------------------------
void reshape(int width, int height)
{
    float aspectReqd = (1000. / 620.);
    float aspectCurr = (float)width / (float)height;
    if (aspectCurr > aspectReqd) {
        glViewport(0, 0, height * aspectReqd, height);
    } else {
        glViewport(0, 0, width, width / aspectReqd);
    }
}


//################### MAIN FUNCTIONS ########################


void initialize(void) 
{
    // LIGHTING
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4] = {1.0, 1.0, 1.0, 1.0};
    float yellow[4] = {1.0, 1.0, 0.0, 1.0};
    glEnable(GL_LIGHTING);
    // glEnable(GL_LIGHT0); // Sky light (DISABLED - used for debug)
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
//  Define Train Light
    glDisable(GL_LIGHT1); // Train light (starts disable)
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.01);
// Define Sun light
    glEnable(GL_LIGHT2); // Sun Light (starts enable)
	glLightfv(GL_LIGHT2, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT2, GL_SPECULAR, white);
// Define Control Tower light
    glDisable(GL_LIGHT3); // Control Tower (starts disable)
    glLightfv(GL_LIGHT3, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, yellow);
    glLightfv(GL_LIGHT3, GL_SPECULAR, yellow);

    // TEXTURE MAPPING
    loadTexture();

	glClearColor (0.0, 0.0, 0.0, 0.0);  //Background colour
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    gluQuadricDrawStyle (q, GLU_FILL );
	gluQuadricNormals	(q, GLU_SMOOTH );
	gluQuadricTexture (q, GL_TRUE);

    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(70., (1000.0/620.0), 5.0, 500.0);   //Perspective projection
}


//-------------------------------------------------------------------
void display(void)
{
    float alpha = -10.5;    // Angle between the train's objects
    float lgt0_pos[] = {-50.0f, 80.0f, -50.0f, 1.0f};  //Sky Light's position
    float lgt1_pos[] = {-10.0, 14.0, 0, 1}; //Train spot light position
    float lgt1_dir[] = {-1, -1, 0.3}; //Train spot light direction
    float lgt2_pos[] = {0, 0, 0, 1}; //Sun light position
    float lgt3_pos[] = {0, 57.5, 0, 1}; //Control Tower

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    cameraControl(); // Calls gluLookAt() with the selected camera's parameters
    glLightfv(GL_LIGHT0, GL_POSITION, lgt0_pos);   //Sky-light position (DISABLED)

    // Light switching - Simulate Sunrise & Sunset
    if (sunAngle == 180) { // Night Time
        glEnable(GL_LIGHT1); // Train
        glEnable(GL_LIGHT3); // ControlTower
        glDisable(GL_LIGHT2); // Sun
    } else if (sunAngle == 0) { // Day time
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT3);
        glEnable(GL_LIGHT2);
    } 

    // Transformation for the train light
    glPushMatrix(); 
      glRotatef(theta, 0, 1, 0);
      glTranslatef(0, 0, -120);
      glLightfv(GL_LIGHT1, GL_POSITION, lgt1_pos);
      glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lgt1_dir);
    glPopMatrix();

    // Texture-mapping & Transformation for the Sun and its light
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[1]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glPushMatrix(); 
      glRotatef(sunAngle, 0, 0, 1);
      glTranslatef(250, 0, 0);
      sun(q);
      glLightfv(GL_LIGHT2, GL_POSITION, lgt2_pos);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    // Floor
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	
    floor(q);
    glDisable(GL_TEXTURE_2D);

    // Tracks and tunnel
    tracks(120, 10);  // Mean radius 120 units, width 10 units
    tracksTies();
    tunnel(5, 25, 10, 20, 120);
    
    // Control Tower and light
    controlTower();
    glLightfv(GL_LIGHT3, GL_POSITION, lgt3_pos);

    // Builds the train station
    glPushMatrix(); 
      glRotatef(75, 0, 1, 0);
      glTranslatef(0, 0, -160);
      station();
    glPopMatrix();

    // Train
    engine(0, 1, -120, 0 * alpha + theta);
    wagon(0, 1, -120, 1 * alpha + theta);
    wagon(0, 1, -120, 2 * alpha + theta);
    wagon(0, 1, -120, 3 * alpha + theta);
    wagon(0, 1, -120, 4 * alpha + theta);

    glutSwapBuffers();   // For animation
}


//---------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (1000, 620); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Assignment 1 - 78555646");
   initialize ();
  
   glutTimerFunc(100, timer, 0);
   glutReshapeFunc(reshape);

   glutDisplayFunc(display); 
   glutSpecialFunc(camSpecial);
   glutKeyboardFunc(camToggler);
   glutMainLoop();
   return 0;
}
