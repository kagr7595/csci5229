/*
 *  HW5: Lighting
 *  Course: CSCI5229
 *  Author: Kathy Grimes
 *  Description:  This program is built upon hwk4 and uses a variety examples including ex13.
 *                This homework's purpose is experience with determining normal vectors for lighting calculations. 
 *                For debugging and grading, light should be able to be stopped and moved
 *
 *    Please use the following keys to manipulate the program during run
 *                 1: toggles between Orthogonal, Perspective, and First Person
 *              in Orthogonal *                 
 *       PgUp/PgDown: Decrease/increase dim
 *              in Orthogonal or Perspective...
 *                 o: overhead view (y-axis view)  
 *              in First Person...   
 *           5,6,7,8: forward,backward,right,left
 *              in First Person or Perspective
 *               f/F: change field_of_view
 *              in all views
 *            Arrows: move angles of ph and th (for all perspectives)
 *                 p: pause flying kitty AND light ball
 *                 m: change mixed kitty coloring/size
 *                 x: toggle axis
 *                 i: go back to initial state values (in case you get off course in the viewing screen)
 *     Lighting in all views
 *                []: Lower/raise light
 *               q/Q: Lower/raise light distance from (0,0,0)
 *               a/A: Decrease/increase ambient light
 *               d/D: Decrease/increase diffuse light
 *               s/S: Decrease/increase specular light
 *               e/E: Decrease/increase emitted light
 *               n/N: Decrease/increase shininess
 *                 l: Toggles lighting
 *                F1: Toggle Smooth/Flat lighting
 *                F2: Toggle localized lighing
 *                 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int th, fp_th=0;  //  Azimuth of view angle (and first person version)
int ph, fp_ph=0;  //  Elevation of view angle (and first person version)
int zh=0;         //  For the flying cat and light ball
int axes=1;       //  Display axes
double fp_x, fp_y, fp_z;
int first = 0;

/*  Zoom Parameters */
double z;
/* Pause Spin Parameter */
int p;
/*  Projection mode */
int project_mode;
#define ORTHOGONAL 1
#define PERSPECTIVE 2
#define FIRST_PERSON 3

/* Perspective Mode */
int field_of_view;

/* Current Window Width/Height */
int cur_width = 600;
int cur_height = 600;
double dim;
double dimo;

/* Lighting */
int light=1;      //  Lighting
int one       =   1;  // Unit value
float distance  =   1;  // Light distance
int inc       =   30;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
float ylight  =   0;  // Elevation of light

/* MIXED(or random) is 0 for all Cat Body characteristics */
#define MIXED 0
struct mixed_cat
{
  int body_type;
  int leg_color[4];
  int leg_brightness[4];
  int torso_color;
  int torso_brightness;
  int head_color;
  int head_brightness;
  int tail_color;
  int tail_brightness;
  int ear_color[2];
  int ear_brightness[2];
} mc;

/* Cat Body Types */
#define LARGE 1
#define REGULAR 2
#define SMALL 3
#define NUM_BODY_TYPES 3

/* Cat Body Colors */
#define BROWN_CAT 1
#define BLACK_CAT 2
#define WHITE_CAT 3
#define NUM_COLOR_TYPES 3

/* Cat Body Color Brightness */
#define NORMAL 1
#define DARK 2
#define LIGHT 3
#define NUM_BRIGHTNESS_TYPES 3

/* Program unit size */
#define UNIT_SIZE 1

//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}


/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glNormal3f( 0, 0, 1);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(0,0,1);
   glNormal3f( 0, 0,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(1,1,0);
   glNormal3f(+1, 0, 0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(0,1,0);
   glNormal3f(-1, 0, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(0,1,1);
   glNormal3f( 0,+1, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(1,0,1);
   glNormal3f( 0,-1, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph, int color)
{
   double x = Sin(th)*Cos(ph);
   //double y = Cos(th)*Cos(ph);
   double y =         Sin(ph);
   double z = Cos(th)*Cos(ph);
  if(color == 0)
    glColor3f(1, 1, 1);
  if(color == 1) //red
    glColor3f(1, 0, 0);
  if(color == 2) //green
    glColor3f(0, 1, 0);
  if(color == 3) //blue
    glColor3f(0, 0, 1);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 *  Taken directly from Ex 13
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;

   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
	Vertex(th,ph,0);
	Vertex(th,ph+inc,0);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw a sphere (version 1)
 *     at (x,y,z)
 *     radius (r)
 */

void sphere1(double x,double y,double z,double r, int rotate)
{
   const int d=15;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotated(rotate,1,0,0);
   glScaled(r,r,r);

   //  South pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,-90,0);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,d-90,0);
   }
   glEnd();
  
   //  Latitude bands
   ph=d-90;  
   glBegin(GL_QUAD_STRIP);
   glColor3f(1,1,0);
   for (th=0;th<=360;th+=d)
     {
       Vertex(th,ph,-1);
       Vertex(th,ph+d,-1);
     }
   glEnd();
   
   ph+=4*d;
   //glBegin(GL_QUAD_STRIP);
   //glColor3f(1,0,0);
   //for (th=0;th<=360;th+=d)
   //  {
   //    Vertex(th,ph,-1);
   //    Vertex(th,ph+d,-1);
   //  }
   //glEnd();
   
   ph+=4*d;
   glBegin(GL_QUAD_STRIP);
   glColor3f(0,1,0);
   for (th=0;th<=360;th+=d)
     {
       Vertex(th,ph,-1);
       Vertex(th,ph+d,-1);
     }
   glEnd();

   
   //  Undo transformations
   glPopMatrix();
}



/* Get random value between two values */
int rand_b2i(int lower, int upper) 
{
  return ((rand() % (upper-lower + 1)) + lower);
}

/*
 *  Draw kitty rectangular prisms (torso/legs/head/tail)
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
void one_color_cube(double x,double y,double z,
		    double dx,double dy,double dz,
		    int color, int color_brightness)
{
  if((color == 0) || (color > NUM_COLOR_TYPES))
    color = rand_b2i(1,NUM_COLOR_TYPES);
    
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);

   //  Cube
   glBegin(GL_QUADS);
   switch(color)
     {
     case BROWN_CAT:
       if(color_brightness == NORMAL)
	 glColor3f(0.36,0.25,0.20);
       else if(color_brightness == DARK)
	 glColor3f(0.35,0.16,0.14);
       else if(color_brightness == LIGHT)
	 glColor3f(0.647059,0.164706,0.164706);
       break;
     case WHITE_CAT:
       if(color_brightness == NORMAL)
	 glColor3f(0.9,0.9,0.9);
       else if(color_brightness == DARK)
	 glColor3f(0.8,0.8,0.8);
       else if(color_brightness == LIGHT)
	 glColor3f(1,1,1);
       break;
     case BLACK_CAT:
       if(color_brightness == NORMAL)
	 glColor3f(0.1,0.1,0.1);
       else if(color_brightness == DARK)
	 glColor3f(0,0,0);
       else if(color_brightness == LIGHT)
	 glColor3f(.2,.2,.2);
       break;       
     }
   
   //  Front
   glNormal3f( 0, 0, 1);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glNormal3f( 0, 0,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glNormal3f( 1, 0, 0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glNormal3f(-1, 0, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glNormal3f( 0, 1, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glNormal3f( 0,-1, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
}

/*
 * Given three verticies, find and set normal for a specific quad or triangular plane
 *
 */
void quad_triangle_normal(float v0_x,float v0_y,float v0_z,
			  float v1_x,float v1_y,float v1_z,
			  float v2_x,float v2_y,float v2_z)
{
  float edge0_x = v0_x - v1_x;
  float edge0_y = v0_y - v1_y;
  float edge0_z = v0_z - v1_z;
  float edge1_x = v1_x - v2_x;
  float edge1_y = v1_y - v2_y;
  float edge1_z = v1_z - v2_z;

  float cp_x =     edge0_y*edge1_z - edge0_z*edge1_y;
  float cp_y = -1*(edge0_x*edge1_z - edge0_z*edge1_x);
  float cp_z =     edge0_x*edge1_y - edge0_y*edge1_x;

  float cp_len = (float)sqrt(pow((double)cp_x,2) + pow((double)cp_y,2) + pow((double)cp_z,2));
  float normal_x = cp_x/cp_len;
  float normal_y = cp_y/cp_len;
  float normal_z = cp_z/cp_len;
  
  glNormal3f(normal_x, normal_y, normal_z); 
}


void triangular_prism(double x, double y, double z, 
		      double dx,double dy,double dz,
		      int color, int color_brightness, int rotate)  
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(rotate,0,1,0);
   glScaled(dx,dy,dz);

   glBegin(GL_QUADS);
   switch(color)
     {
     case BROWN_CAT:
       if(color_brightness == NORMAL)
	 glColor3f(0.36,0.25,0.20);
       else if(color_brightness == DARK)
	 glColor3f(0.35,0.16,0.14);
       else if(color_brightness == LIGHT)
	 glColor3f(0.647059,0.164706,0.164706);
       break;
     case WHITE_CAT:
       if(color_brightness == NORMAL)
	 glColor3f(0.9,0.9,0.9);
       else if(color_brightness == DARK)
	 glColor3f(0.8,0.8,0.8);
       else if(color_brightness == LIGHT)
	 glColor3f(1,1,1);
       break;
     case BLACK_CAT:
       if(color_brightness == NORMAL)
	 glColor3f(0.1,0.1,0.1);
       else if(color_brightness == DARK)
	 glColor3f(0,0,0);
       else if(color_brightness == LIGHT)
	 glColor3f(.2,.2,.2);
       break;       
     }
   //Be sure to rotate through vertices in correct order to get the correct side normalized
   quad_triangle_normal(1, 0, -1,
			-1,0,1,
			-1, 0, -1);  
   glVertex3f(1, 0, 1);
   glVertex3f(1, 0, -1);
   glVertex3f(-1, 0, -1);
   glVertex3f(-1, 0, 1);

   quad_triangle_normal(1, 2, -1,
			-1,0,-1,
			-1,2,-1);
   glVertex3f(1,0,-1);
   glVertex3f(1,2,-1);
   glVertex3f(-1,2,-1);
   glVertex3f(-1,0,-1);
   
   quad_triangle_normal(-1,2,-1,
			-1,0,1,
			1,0,1);
   glVertex3f(1,1,-1);
   glVertex3f(-1,2,-1);
   glVertex3f(-1,0,1);
   glVertex3f(1,0,1);
   glEnd();


   glBegin(GL_TRIANGLES);
   if(rotate == -90) {
     glNormal3f(-1, 0, 0);
     glVertex3f(-1,0,1);
     glVertex3f(-1,2,-1);
     glVertex3f(-1,0,-1);
     //one side of ear needs to be pink
     glColor3f(0.737255,0.560784,0.560784);
     glNormal3f( 1, 0, 0);
     glVertex3f(1,0,1);
     glVertex3f(1,2,-1);
     glVertex3f(1,0,-1);
   } else {
     glNormal3f( 1, 0, 0);
     glVertex3f(1,0,1);
     glVertex3f(1,2,-1);
     glVertex3f(1,0,-1);
     //one side of ear needs to be pink
     glColor3f(0.737255,0.560784,0.560784);
     glNormal3f(-1, 0, 0);
     glVertex3f(-1,0,1);
     glVertex3f(-1,2,-1);
     glVertex3f(-1,0,-1);
   }   
   glEnd();

   //  Undo transformations
   glPopMatrix();
   
}

void legs(double x, double y, double z, int color, int color_brightness)
{  
  double height = y+UNIT_SIZE*.02;
  double leg_spacing = x+UNIT_SIZE*.015;
  double torso_spacing = z+UNIT_SIZE*.03;
  if(color == MIXED) {
    //back left
    one_color_cube(-leg_spacing,height,-torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, mc.leg_color[0], mc.leg_brightness[0]);
    //back right							    					    
    one_color_cube( leg_spacing,height,-torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, mc.leg_color[1], mc.leg_brightness[1]);
    //front left							    					 
    one_color_cube( leg_spacing,height, torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, mc.leg_color[2], mc.leg_brightness[2]);
    //front right						    					    
    one_color_cube(-leg_spacing,height, torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, mc.leg_color[3], mc.leg_brightness[3]);
  }else {
    //back left
    one_color_cube(-leg_spacing,height,-torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, color, color_brightness);
    //back right							    					    
    one_color_cube( leg_spacing,height,-torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, color, color_brightness);
    //front left							    					 
    one_color_cube( leg_spacing,height, torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, color, color_brightness);
    //front right						    					    
    one_color_cube(-leg_spacing,height, torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, color, color_brightness);
  }
}
void torso(double x, double y, double z, int color, int color_brightness)
{  
  double torso_height = UNIT_SIZE*.02;
  double torso_width = UNIT_SIZE*.018;
  double torso_length = UNIT_SIZE*.04;
  double height = y+UNIT_SIZE*.02+torso_height;
  if(color == MIXED) {
    one_color_cube(x,height,z, torso_width, torso_height, torso_length, mc.torso_color, mc.torso_brightness);
  } else {
    one_color_cube(x,height,z, torso_width, torso_height, torso_length, color, color_brightness);
  }
}

void head(double x, double y, double z, int color, int color_brightness)
{  
  double head_height = UNIT_SIZE*.015;
  double head_width = UNIT_SIZE*.015;
  double head_length = UNIT_SIZE*.015;
  double height = y+UNIT_SIZE*.05+head_height;
  double torso_front = z+UNIT_SIZE*.055;
  if(color == MIXED) {
    one_color_cube(x,height,torso_front, head_width, head_height, head_length, mc.head_color, mc.head_brightness);
  } else {
    one_color_cube(x,height,torso_front, head_width, head_height, head_length, color, color_brightness);
  }
}

void tail(double x, double y, double z, int color, int color_brightness)
{  
  double tail_height = UNIT_SIZE*.005;
  double tail_width = UNIT_SIZE*.005;
  double tail_length = UNIT_SIZE*.03;
  double height = y+UNIT_SIZE*.05+tail_height;
  double torso_back = z-UNIT_SIZE*.065;

  if(color == MIXED) {
    one_color_cube(x,height,torso_back, tail_width, tail_height, tail_length, mc.tail_color, mc.tail_brightness);
  } else {
    one_color_cube(x,height,torso_back, tail_width, tail_height, tail_length, color, color_brightness);
  }
}

void ears(double x, double y, double z, int color, int color_brightness)
{
  double ear_height = UNIT_SIZE*.008;
  double ear_width = UNIT_SIZE*.004;
  double ear_length = UNIT_SIZE*.006;
  double ear_spacing = UNIT_SIZE*.008;
  double head_front = z+UNIT_SIZE*.065;
  double height = y+UNIT_SIZE*.065+ear_height;
  if(color == MIXED) {
    triangular_prism( ear_spacing,height,head_front,ear_width,ear_height,ear_length,mc.ear_color[0],mc.ear_brightness[0],-90);
    triangular_prism(-ear_spacing,height,head_front,ear_width,ear_height,ear_length,mc.ear_color[1],mc.ear_brightness[1],90);
  } else {
    triangular_prism( ear_spacing,height,head_front,ear_width,ear_height,ear_length,color,color_brightness,-90);
    triangular_prism(-ear_spacing,height,head_front,ear_width,ear_height,ear_length,color,color_brightness,90);
  }
}

void eyes(double x, double y, double z)
{
  double eye_radius = UNIT_SIZE*.004;
  double eye_spacing = UNIT_SIZE*.006;
  double head_front = z+UNIT_SIZE*.065+eye_radius;
  double height = y+UNIT_SIZE*.07;
  sphere1(eye_spacing, height, head_front, eye_radius, 90);
  sphere1(-eye_spacing, height, head_front, eye_radius, 90);
}

/*
 *  Create a cat (robot style - blocky)
 *  Accepts parameters about cat body type and color
 *  The cat is made up of multiple body parts including:
 *    4 legs, 1 head w/ears, 1 torso, 1 tail
 */
void cat(double x, double y, double z,
	 double th,
	 int body_type, int color)
{

   //  Save transformation
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   switch(body_type)
     {
     case LARGE:
       glScaled(3,3,3);
       break;
     case REGULAR:
       glScaled(2.3,2.3,2.3);	
       break;
     case SMALL:
       glScaled(1.75,1.75,1.75);       
       break;       
     }

   //legs
   if(color == 0)
     {
       legs(0,0,0, color, MIXED);
       torso(0,0,0, color, MIXED);
       head(0,0,0, color, MIXED);
       tail(0,0,0, color, MIXED);
       ears(0,0,0, color, MIXED);
     }
   else
     {
       legs(0,0,0, color, NORMAL);
       torso(0,0,0, color, DARK);
       head(0,0,0, color, NORMAL);
       tail(0,0,0, color, NORMAL);
       ears(0,0,0, color, LIGHT);
     }
   eyes(0,0,0);
   

   //  Undo transformations
   glPopMatrix();
  
}

/* Function to randomize the mixed cat structure */
void rand_mc()
{
  mc.body_type = rand_b2i(1,NUM_BODY_TYPES);
  for(int i = 0; i < 4; i++) {
    mc.leg_color[i] = rand_b2i(1,NUM_COLOR_TYPES);
    mc.leg_brightness[i] = rand_b2i(1,NUM_BRIGHTNESS_TYPES);
  }
  mc.torso_color = rand_b2i(1,NUM_COLOR_TYPES);
  mc.torso_brightness = rand_b2i(1,NUM_BRIGHTNESS_TYPES);
  mc.head_color = rand_b2i(1,NUM_COLOR_TYPES);
  mc.head_brightness = rand_b2i(1,NUM_BRIGHTNESS_TYPES);
  mc.tail_color = rand_b2i(1,NUM_COLOR_TYPES);
  mc.tail_brightness = rand_b2i(1,NUM_BRIGHTNESS_TYPES);
  for(int i = 0; i < 2; i++) {
    mc.ear_color[i] = rand_b2i(1,NUM_COLOR_TYPES);
    mc.ear_brightness[i] = rand_b2i(1,NUM_BRIGHTNESS_TYPES);
  }
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=UNIT_SIZE*1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();

   if(project_mode == PERSPECTIVE)
     {
       double Ex = -2*dim*Sin(th)*Cos(ph);
       double Ey = +2*dim        *Sin(ph);
       double Ez = +2*dim*Cos(th)*Cos(ph);
       gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
       //  Five pixels from the lower left corner of the window
       glWindowPos2i(5,5);
       Print("Angle=%d,%d  Projection=%s",th,ph,"Perspective",light?"On":"Off");
     }
   else if (project_mode == FIRST_PERSON)
     {
       gluLookAt(fp_x, fp_y, fp_z,
		 Cos(fp_th) + fp_x,
		 Sin(fp_ph) + fp_y,
		 Sin(fp_th) + fp_z,
		 0,1,0);
       //  Five pixels from the lower left corner of the window
       glWindowPos2i(5,5);
       Print("Angle=%d,%d  Position=%.1f,%.1f   Projection=%s",fp_th,fp_ph,fp_x,fp_z,"First Person",light?"On":"Off");
     }
   else
     {
       //  Set view angle
       glRotatef(ph,1,0,0);
       glRotatef(th,0,1,0);
       //  Five pixels from the lower left corner of the window
       glWindowPos2i(5,5);
       Print("Angle=%d,%d  Projection=%s",th,ph,"Orthogonal",light?"On":"Off");	
     }


   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light position
      float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);

   //DRAW OBJECTS
   cube(0,UNIT_SIZE*-0.02,0,
        UNIT_SIZE*.7,UNIT_SIZE*0.02,UNIT_SIZE*.7,
        0);
   cat(UNIT_SIZE*.2, 0, UNIT_SIZE*.2, 45, LARGE, WHITE_CAT);
   cat(UNIT_SIZE*.2, 0, -UNIT_SIZE*.4, 135, SMALL, BROWN_CAT);
   cat(UNIT_SIZE*.4, 0, -UNIT_SIZE*.2, -90, REGULAR, BLACK_CAT);
   cat(-UNIT_SIZE*.2, 0, UNIT_SIZE*.4, 0, mc.body_type, MIXED);
   //testing //glScaled(5,5,5);
   //testing//ears(-.02,-.02,0, WHITE_CAT, LIGHT);
   //testing//triangular_prism(0,0,0,.01,.02,.005,WHITE_CAT,1,-90);

   ////Flying/Moving through other objects cat
   cat(-UNIT_SIZE*.2*Cos(zh), UNIT_SIZE*Sin(zh), UNIT_SIZE*.4*Sin(zh), 0, SMALL, WHITE_CAT);
   
   glDisable(GL_LIGHTING);
   //AXES
   //  White
   glColor3f(1,1,1);
   //  Draw axes
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }


   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%.1f Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }

   //  Render the scene
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
}

/*
 * Initial State
 *
 */
void initialState()
{
  //Zoom Paramter
  z = UNIT_SIZE*1.5;
  dim=UNIT_SIZE*2.5-z;
  dimo=UNIT_SIZE*2.5-z;

  //Pause Parameter
  p = 0;

  //th,ph
  th = -40;
  ph = 15;

  //Randomize mixed cat
  rand_mc();
  
  //project_mode initialize as ORTHOGONAL
  project_mode = ORTHOGONAL;

  //field_of_view
  field_of_view = 75;

  //first person x,y,z,th,ph
  fp_x = 1.0;
  fp_y = 1.0;
  fp_z = 1.0;
  fp_th = 230;
  fp_ph = -55;  

  //Lighting
  light=1;      //  Lighting
  one       =   1;  // Unit value
  distance  =   1;  // Light distance
  inc       =   30;  // Ball increment
  smooth    =   1;  // Smooth/Flat shading
  local     =   0;  // Local Viewer Model
  emission  =   0;  // Emission intensity (%)
  ambient   =  30;  // Ambient intensity (%)
  diffuse   = 100;  // Diffuse intensity (%)
  specular  =   0;  // Specular intensity (%)
  shininess =   0;  // Shininess (power of two)
  shiny   =   1;  // Shininess (value)
  ylight  =   0;  // Elevation of light
}

/*
 * set/change projection (added to reshape as that is another location we modify the view)
 *
 */
void set_or_change_projection()
{
  //  Ratio of the width to the height of the window
  double w2h = (cur_height>0) ? (double)cur_width/cur_height : 1;

  //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();

   if(project_mode == PERSPECTIVE || project_mode == FIRST_PERSON)
    {
      dim = UNIT_SIZE*2.5-z;
      gluPerspective(field_of_view, w2h*dim,dim/4,4*dim);
    }
   else 
    {
      dim = dimo;
      //  Orthogonal projection
      glOrtho(-w2h*dim,+w2h*dim, -dim,+dim, -dim,+dim);
    }




   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();

}


/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
  if(project_mode == FIRST_PERSON)
    {
      //  Right arrow key - increase angle by 5 degrees
      if (key == GLUT_KEY_RIGHT)
	fp_th += 5;
      //  Left arrow key - decrease angle by 5 degrees
      else if (key == GLUT_KEY_LEFT)
	fp_th -= 5;
      //  Up arrow key - increase elevation by 5 degrees
      else if (key == GLUT_KEY_UP && fp_ph<90)
	fp_ph += 5;
      //  Down arrow key - decrease elevation by 5 degrees
      else if (key == GLUT_KEY_DOWN && fp_ph>-90)
	fp_ph -= 5;
      //  Keep angle to +/-360 degrees
      fp_th %= 360;
    }
  else 
    {
      //  Right arrow key - increase angle by 5 degrees
      if (key == GLUT_KEY_RIGHT)
	th += 5;
      //  Left arrow key - decrease angle by 5 degrees
      else if (key == GLUT_KEY_LEFT)
	th -= 5;
      //  Up arrow key - increase elevation by 5 degrees
      else if (key == GLUT_KEY_UP)
	ph += 5;
      //  Down arrow key - decrease elevation by 5 degrees
      else if (key == GLUT_KEY_DOWN)
	ph -= 5;
      //  Keep angles to +/-360 degrees
      th %= 360;
      ph %= 360;
    }
   //  Smooth color model
   if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dimo += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dimo -= 0.1;

  set_or_change_projection();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 *  Program directly calls reshape for zoom functionality
 */
void reshape(int width,int height)
{
   dim=UNIT_SIZE*2.5-z;
   cur_width = width;
   cur_height = height;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   set_or_change_projection();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //initial values
   else if (ch == 'i')
     initialState();
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm')
     {
       //Randomize mixed cat
       rand_mc();
     }
   //disable z as does not work properly // else if (ch == 'z')
   //disable z as does not work properly //   {
   //disable z as does not work properly //     z += UNIT_SIZE*.5;
   //disable z as does not work properly //     if (z > UNIT_SIZE*2.5)
   //disable z as does not work properly //     	 z = UNIT_SIZE*2.49;
   //disable z as does not work properly //     reshape(cur_width, cur_height);
   //disable z as does not work properly //   }
   //disable z as does not work properly // else if (ch == 'Z')
   //disable z as does not work properly //   {
   //disable z as does not work properly //     z -= UNIT_SIZE*.5;
   //disable z as does not work properly //     reshape(cur_width, cur_height);
   //disable z as does not work properly //   }
   else if (ch == 'p')
     {
       if (p == 1)
	 p = 0;
       else 
	 p = 1;
     }
   else if (ch == '1')
     {
       if(project_mode == 3)
	 project_mode = 1;
       else
	 project_mode += 1;       
     }
   else if (ch == 'f' && field_of_view <100)
     field_of_view += 1;
   else if (ch == 'F' && field_of_view >1)
     field_of_view -= 1;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   // Ball Distance
   else if (ch=='q')
     distance -= 0.1;
   else if (ch=='Q')
     distance += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);

   if(project_mode != FIRST_PERSON)
     {
       //  Reset view angle
       if (ch == '0')
	 th = ph = 0;
       else if (ch == 'o')
	 {
	   //overhead view (looking down y-axis)
	   th = 0;
	   ph = 90;
	 }
     }
   else if(project_mode == FIRST_PERSON)
     {
       if (ch == '5')
	 {
	   fp_x += 0.1*Cos(fp_th);
	   fp_z += 0.1*Sin(fp_th);
	 }
       else if (ch == '6')
	 {
	   fp_x -= 0.1*Cos(fp_th);
	   fp_z -= 0.1*Sin(fp_th);
	 }
       else if (ch == '7')
	 {
	   fp_x += 0.1*Cos(fp_th);
	   fp_z -= 0.1*Sin(fp_th);
	 }
       else if (ch == '8')
	 {
	   fp_x -= 0.1*Cos(fp_th);
	   fp_z += 0.1*Sin(fp_th);
	 }
     }

   set_or_change_projection();

   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
  if(p == 0)
    {
      double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
      zh = fmod(90*t,360);
      set_or_change_projection();
      glutPostRedisplay();
    }
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(cur_width,cur_height);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("HW5 Lighting - CSCI5229 - Kathy Grimes");
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   // Use inital state parameters
   initialState();
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
