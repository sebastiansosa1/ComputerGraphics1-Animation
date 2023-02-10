//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  RailModels.cpp
//  A collection of functions for generating the models for a railway scene
//  ========================================================================

#include <cmath>
#include <GL/freeglut.h>
#include "RailModels.h"


//######################### STATIC OBJECTS ###########################


//--------------- GROUND PLANE ------------------------------------
// This is a square shaped region on the xz-plane of size 400x400 units
// centered at the origin.  This region is constructed using small quads
// of unit size, to facilitate rendering of spotlights
//-----------------------------------------------------------------
void floor(GLUquadricObj* q)
{
	float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};
	glColor3f(0.0f, 1.0f, 0.0f);	// Green - Improve the colour of the texture mapping
	glNormal3f(0.0, 1.0, 0.0);	// Defines normal vector for light reflection
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glPushMatrix();
	  glRotatef(90, 1, 0, 0);
	  gluDisk(q, 0, 200, 100, 100);
	  gluQuadricDrawStyle(q, GLU_FILL);
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}


//--------------- RAILWAY TRACK ------------------------------------
// A circular railway track of specified median radius and width
// The tracks are modelled using small quadrilateral segments each covering 5 deg arc
// The tracks have a height of 1 unit (refer to lab notes)
//-----------------------------------------------------------------
void tracks(float medRadius, float width)  
{
	float inRad  = medRadius - width * 0.5;
	float outRad = medRadius + width * 0.5;
	float angle1,angle2, ca1,sa1, ca2,sa2;
	float x1,z1, x2,z2, x3,z3, x4,z4;    // Four points of a quad
    float toRad = 3.14159265/180.0;    // Conversion from degrees to radians

	glColor4f(0.0, 0.0, 0.3, 1.0);
    glBegin(GL_QUADS);
	float radius = inRad;
	for (int i = 0; i < 2; i++)   // Two parallel tracks (radius = inRad, outRad)
	{
		for (int i = 0; i < 360; i += 5)    //5 deg intervals
		{
			angle1 = i * toRad;       //Computation of angles, cos, sin etc
			angle2 = (i + 5) * toRad;
			ca1 = cos(angle1); ca2 = cos(angle2);
			sa1 = sin(angle1); sa2 = sin(angle2);
			x1 = (radius - 0.5)*sa1; z1 = (radius - 0.5)*ca1;
			x2 = (radius + 0.5)*sa1; z2 = (radius + 0.5)*ca1;
			x3 = (radius + 0.5)*sa2; z3 = (radius + 0.5)*ca2;
			x4 = (radius - 0.5)*sa2; z4 = (radius - 0.5)*ca2;

			glNormal3f(0., 1., 0.);       //Quad 1 facing up
			glVertex3f(x1, 1.0, z1);
			glVertex3f(x2, 1.0, z2);
			glVertex3f(x3, 1.0, z3);
			glVertex3f(x4, 1.0, z4);

			glNormal3f(-sa1, 0.0, -ca1);   //Quad 2 facing inward
			glVertex3f(x1, 0.0, z1);
			glVertex3f(x1, 1.0, z1);
			glNormal3f(-sa2, 0.0, -ca2);
			glVertex3f(x4, 1.0, z4);
			glVertex3f(x4, 0.0, z4);

			glNormal3f(sa1, 0.0, ca1);   //Quad 3 facing outward
			glVertex3f(x2, 1.0, z2);
			glVertex3f(x2, 0.0, z2);
			glNormal3f(sa2, 0.0, ca2);
			glVertex3f(x3, 0.0, z3);
			glVertex3f(x3, 1.0, z3);
		}
		radius = outRad;
	}
	glEnd();
}


// ------------ The ties underneath the track -------------
void tracksTies (void)
{
	glColor4f(0.5, 0, 0, 1);
	for (int i=0; i<365; i += 4)
	{
		glPushMatrix();
		  glRotatef(i, 0, 1, 0);
		  glTranslatef(120, 0.2, 0);
		  glScalef(15, 0.2, 4);
		  glutSolidCube(1.0);
		glPopMatrix();
	}
}


//--------------- TUNNEL ----------------------------------
// This simple model of the tunnel for which the train
// will pass through.
//--------------------------------------------------------
void tunnel(float inRadius, float outRadius, int sides, int rings, float medRadius)
{
	for (int i = 0; i < 90; i += 3)
	{
		if (i % 2 == 0) glColor4f(1.0f, 0.0f, 0.0f, 0.0f);    //Red
		else glColor4f(1.0f, 1.0f, 1.0f, 0.0f);    // White	
		glPushMatrix();
		  glRotatef(i, 0, 1, 0);
	  	  glTranslatef(medRadius, 15, 0);
	  	  glutSolidTorus(inRadius, outRadius, sides, rings);
		glPopMatrix();
	}
}


//--------------- STATION ----------------------------------
// This simple model of the station, where the train will
// stop on each lap.
//--------------------------------------------------------
void station(void)
{
	GLUquadricObj* q = gluNewQuadric();
	// Foundations
	glColor4f(0.2, 0.2, 0.2, 1.0);    // Grey
	glPushMatrix();
	  glTranslatef(0, 2.001, 0);
	  glScalef(100, 4, 40);
	  glutSolidCube(1.0);
	glPopMatrix();
	// Wall
	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);    // White
	glPushMatrix();
	  glTranslatef(0, 13.75, -17.5);
	  glScalef(100, 19.5, 5);
	  glutSolidCube(1.0);
	glPopMatrix();
	// Roof
	glColor4f(0.5, 0, 0, 1);    // Red
	glPushMatrix();
	  glTranslatef(0, 22, -5.1);
	  glRotatef(10, 1, 0, 0);
	  glScalef(100.1, 2, 30);
	  glutSolidCube(1.0);
	glPopMatrix();
	// Columns
	glColor4f(1.0f, 1.0f, 0.0f, 0.0f);    //Yellow
	for (int i = -40; i <= 40; i += 20) {
		glPushMatrix();
		glTranslatef(i, 4, 4);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(q, 1.5, 1.5, 17, 16, 1);
		glPopMatrix();
	}
}


//--------------- CONTROL TOWER --------------------------
// The tower at the center of the map, that turns the yellow
// light on during nightime. The train can also be 
// observed from here.
//--------------------------------------------------------
void (controlTower)(void)
{
	GLUquadricObj* q = gluNewQuadric();
	// Walls
	glColor4f(0.8f, 0.8f, 0.8f, 0.0f); //white	
	glPushMatrix();
	 glRotatef(-90, 1, 0, 0);
	 gluCylinder(q, 15, 15, 50, 20, 1);
	 gluQuadricDrawStyle(q, GLU_FILL);
	glPopMatrix();
	// Roof
	glColor4f(1.0f, 0.0f, 0.0f, 0.0f); //red
	glPushMatrix();
	 glTranslatef(0, 65, 0);
	 glRotatef(-90, 1, 0, 0);
	 glutSolidCone(20, 20, 20, 4);
	glPopMatrix();
	// Columns
	glColor4f(1.0f, 1.0f, 0.0f, 0.0f);    //Yellow
	for (int i = 0; i < 360; i += 120) {
		glPushMatrix();
		  glRotatef(i, 0, 1, 0);
		  glTranslatef(13.5, 50, 0);
		  glRotatef(-90, 1, 0, 0);
		  gluCylinder(q, 1.5, 1.5, 15, 16, 1);
		glPopMatrix();
	}
}


//####################### ANIMATED OBJECTS #########################


//--------------- MODEL BASE --------------------------------------
// This is a common base for the locomotive and wagons
// The base is of rectangular shape (20 length x 10 width x 2 height)
// and has wheels and connectors attached.
//-----------------------------------------------------------------
void base(float x, float y, float z, float alpha=0.)
{
	GLUquadricObj* q = gluNewQuadric();
    glColor4f(0.2, 0.2, 0.2, 1.0);    // Base color
    glPushMatrix();
      glRotatef(alpha, 0., 1., 0.);
      glTranslatef(0.0 + x, 4.0 + y, 0.0 + z);
      glScalef(20.0, 2.0, 10.0);    // Size 20x10 units, thickness 2 units.
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();    // Connector between wagons
      glRotatef(alpha, 0., 1., 0.);
      glTranslatef(11.0 + x, 4.0 + y, 0.0 + z);
      glutSolidCube(2.0);
    glPopMatrix();

    // 4 Wheels (radius = 2 units)
	// x, z positions of wheels:
	float wx[4] = {  -8,   8,   -8,    8 }; 
	float wz[4] = { 5.1, 5.1, -5.1, -5.1 };
	float offset = 0.; // For the outer bit of the wheel
	for (int i = 0; i < 4; i++)
	{
		glColor4f(0.5, 0., 0., 1.0);    //Inner Wheel color
		glPushMatrix();
          glRotatef(alpha, 0., 1., 0.);
          glTranslatef(wx[i] + x, 2.0 + y, wz[i] + z);
		  glRotatef(alpha*15, 0, 0, 1);
		  gluDisk(q, 0.0, 2.0, 20, 2);
		glPopMatrix();

		if (wz[i] > 0) offset = 0.1;
		else offset = -0.1;
		glColor4f(1.0f, 1.0f, 0.0f, 0.0f);    //Outer - Yellow
		glPushMatrix();
          glRotatef(alpha, 0., 1., 0.);
          glTranslatef(wx[i] + x, 2.0 + y, wz[i] + z + offset);
		  glRotatef(alpha*15, 0, 0, 1);
		  gluDisk(q, 1., 1.5, 3, 2);
		glPopMatrix();

	}
}


//--------------- LOCOMOTIVE --------------------------------
// This simple model of a locomotive consists of the base,
// cabin and boiler
//-----------------------------------------------------------
void engine(float x, float y, float z, float alpha=0.) //OFFSET added
{
    base(x, y, z, alpha);
	GLUquadricObj* q = gluNewQuadric();

    //Cab
    glColor4f(0.8, 0.8, 0.0, 1.0);
    glPushMatrix();
      glRotatef(alpha, 0., 1., 0.);
      glTranslatef(7.0 + x, 8.5 + y, 0.0 + z);
      glScalef(6.0, 7.0, 10.0);
      glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
      glRotatef(alpha, 0., 1., 0.);
      glTranslatef(6.0 + x, 14.0 + y, 0.0 + z);
      glScalef(4.0, 4.0, 8.0);
      glutSolidCube(1.0);
    glPopMatrix();

    //Boiler
    glPushMatrix();
      glColor4f(0.5, 0., 0., 1.0);
      glRotatef(alpha, 0., 1., 0.);
      glTranslatef(4.0 + x, 10.0 + y, 0.0 + z);
      glRotatef(-90.0, 0., 1., 0.);
      gluCylinder(q, 5.0, 5.0, 14.0, 20, 5);
      glTranslatef(0.0, 0.0, 14.0);
      gluDisk(q, 0.0, 5.0, 20, 3);
      glColor4f(1.0, 1.0, 0.0, 1.0);
      glTranslatef(0.0, 4.0, 0.2);
      gluDisk(q, 0.0, 1.0, 8, 2);    // Headlight
    glPopMatrix();
}


//--------------- WAGON ----------------------------------
// This simple model of a rail wagon consists of the base,
// and a cube(!)
//--------------------------------------------------------
void wagon(float x, float y, float z, float alpha=0.)
{
    base(x, y, z, alpha);

    glColor4f(0.0, 1.0, 1.0, 1.0);
    glPushMatrix();
      glRotatef(alpha, 0, 1, 0);
      glTranslatef(x, 10.0 + y, z);
      glScalef(18.0, 10.0, 10.0);
      glutSolidCube(1.0);
    glPopMatrix();
}


//--------------- SUN ----------------------------------
// This simple model of the sun, is texture mapped and 
// has a light defined in RailwayWorld.cpp
//--------------------------------------------------------
void sun(GLUquadricObj* q)
{	
	glColor4f(1.0f, 1.0f, 0.0f, 0.0f);  // Yellow
	glPushMatrix();	
	  glRotatef(-90., 1.0, 0., 0.0);    // Make the sphere axis vertical
	  gluSphere ( q, 20.0, 180, 85 );
    glPopMatrix();
}