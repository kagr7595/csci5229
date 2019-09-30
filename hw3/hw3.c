/*
 *  HW3: Scene  in 3D
 *  Course: CSCI5229
 *  Author: Kathy Grimes
 *  Description:  A program that creates a multiple instances of a cat object in different locations
 *                using multiple other objects such as cuboids/spheroids/triangles/quads
 *                Inside of the cat object are legs, torso, head, tail, ears, and eyes objects
 *                that have been manipulated by translate/rotate/scaling
 *    Code Initially was from Example 8 class code
 *               z/Z: zoom inword/outward by .5
 *            Arrows: move around axis
 *                 p: pause flying kitty
 *                 m: change mixed kitty coloring/size
 *                 a: toggle axis
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

int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int zh=0;         //  For the flying cat
int axes=1;       //  Display axes

/*  Zoom Parameters */
double z;
/* Pause Spin Parameter */
int p;

/* Current Window Width/Height */
int cur_width = 600;
int cur_height = 600;

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
#define UNIT_SIZE 50

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
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(0,0,1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(1,1,0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(0,1,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(0,1,1);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(1,0,1);
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
 *  Draw vertex in polar coordinates
 */
static void Vertex(double th,double ph, int color)
{
  if(color == 0)
    glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
  if(color == 1) //red
    glColor3f(1, 0, 0);
  if(color == 2) //green
    glColor3f(0, 1, 0);
  if(color == 3) //blue
    glColor3f(0, 0, 1);
  glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
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
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
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
   glVertex3f(1, 0, 1);
   glVertex3f(1, 0, -1);
   glVertex3f(-1, 0, -1);
   glVertex3f(-1, 0, 1);
   
   glVertex3f(1,0,-1);
   glVertex3f(1,2,-1);
   glVertex3f(-1,2,-1);
   glVertex3f(-1,0,-1);
   
   glVertex3f(1,1,-1);
   glVertex3f(-1,2,-1);
   glVertex3f(-1,0,1);
   glVertex3f(1,0,1);
   glEnd();


   glBegin(GL_TRIANGLES);
   if(rotate == -90) {
     glVertex3f(-1,0,1);
     glVertex3f(-1,2,-1);
     glVertex3f(-1,0,-1);
     //one side of ear needs to be pink
     glColor3f(0.737255,0.560784,0.560784);
     glVertex3f(1,0,1);
     glVertex3f(1,2,-1);
     glVertex3f(1,0,-1);
   } else {
     glVertex3f(1,0,1);
     glVertex3f(1,2,-1);
     glVertex3f(1,0,-1);
     //one side of ear needs to be pink
     glColor3f(0.737255,0.560784,0.560784);
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
   const double len=UNIT_SIZE*2;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Set view angle
   glRotatef(ph,1,0,0);
   glRotatef(th,0,1,0);	

   //DRAW OBJECTS
   cube(0,UNIT_SIZE*-0.02,0,
	UNIT_SIZE*.7,UNIT_SIZE*0.02,UNIT_SIZE*.7,
	0);
   cat(UNIT_SIZE*.2, 0, UNIT_SIZE*.2, 45, LARGE, WHITE_CAT);
   cat(UNIT_SIZE*.2, 0, -UNIT_SIZE*.4, 135, SMALL, BROWN_CAT);
   cat(UNIT_SIZE*.4, 0, -UNIT_SIZE*.2, -90, REGULAR, BLACK_CAT);
   cat(-UNIT_SIZE*.2, 0, UNIT_SIZE*.4, 0, mc.body_type, MIXED);

   //Flying/Moving through other objects cat
   cat(-UNIT_SIZE*.2*Cos(zh), UNIT_SIZE*Sin(zh), UNIT_SIZE*.4*Sin(zh), 0, SMALL, WHITE_CAT);
   
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
   //  Five pixels from the lower left corner of the window
   glWindowPos2i(5,25);
   //  Print the text string
   Print("Angle=%d,%d",th,ph);
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

  //Pause Parameter
  p = 0;

  //Randomize mixed cat
  rand_mc();

  
}
/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
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
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 *  Program directly calls reshape for zoom functionality
 */
void reshape(int width,int height)
{
   const double dim=UNIT_SIZE*2.5-z;
   cur_width = width;
   cur_height = height;
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection
   glOrtho(-w2h*dim,+w2h*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm')
     {
       //Randomize mixed cat
       rand_mc();
     }
   else if (ch == 'z')
     {
       z += UNIT_SIZE*.5;
       if (z > UNIT_SIZE*2.5)
       	 z = UNIT_SIZE*2.49;
       reshape(cur_width, cur_height);
     }
   else if (ch == 'Z')
     {
       z -= UNIT_SIZE*.5;
       reshape(cur_width, cur_height);
     }
   else if (ch == 'p')
     {
       if (p == 1)
	 p = 0;
       else 
	 p = 1;
     }

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
   glutCreateWindow("Objects");
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
