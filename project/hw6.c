/*
 *  HW6: Textures
 *  Course: CSCI5229
 *  Author: Kathy Grimes
 *  Description:  This program is built upon hwk5 and uses a variety examples including ex15/16/18.
 *                This homework's purpose is to experience texture use with lighting and three dimensional viewing. 
 *                No pyramids, GLU/GLUT objects
 *
 *    Please use the following keys to manipulate the program during run
 *                 1: toggles between Perspective, and First Person
 *              in Perspective...
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
 *     Textures in all views
 *                 t: Toggle between two sets of textures for the cats and stage
 *                 T: Toggle Texture mode (replace/modulate)
 *                 
 */
#include "CSCIx229.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.1415926535
int th, fp_th=0;  //  Azimuth of view angle (and first person version)
int ph, fp_ph=0;  //  Elevation of view angle (and first person version)
int zh=0;         //  For the flying cat and light ball
int axes=1;       //  Display axes
double fp_x, fp_y, fp_z;
int first = 0;
unsigned int current_rand = 1;

/*  Zoom Parameters */
double z;
/* Pause Spin Parameter */
int p;
/*  Projection mode */
int project_mode;
#define PERSPECTIVE 1
#define FIRST_PERSON 2

/* Perspective Mode */
int field_of_view;

/* Current Window Width/Height */
int cur_width = 600;
int cur_height = 600;
double dim;

/* Lighting */
int light=1;      //  Lighting
int one       =   1;  // Unit value
float distance  =   1;  // Light distance
int inc       =   5;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
float ylight  =   0;  // Elevation of light


/* Texture */
int texture_mode=0;       //  Texture mode
int ntex=0;               //  Cube faces
unsigned int texture[13]; //Texture names
unsigned int skybox_texture[6]; //Texture names
unsigned int crate_texture;
unsigned int moon_texture;
unsigned int tree_texture[3]; //Bark, Root,  and Leaf
#define BARK 0
#define ROOT 1
#define LEAF 2


/* Colors */
#define GREEN    0
#define BLUE     1
#define RED      2
#define ORANGE   3
#define PURPLE   4
#define BLACK    5
#define WHITE    6
#define YELLOW   7
#define BROWN    8
#define MAGENTA  9
#define CYAN    10
#define GREY    11
#define LGREEN  12
#define DGREEN  13
#define LBLUE   14
#define DBLUE   15
#define LRED    16
#define PINK    17
#define LORANGE 18
#define DORANGE 19
#define LPURPLE 20
#define DPURPLE 21
#define DWHITE  22
#define LGREY   23
#define DGREY   24
#define	LBLACK  25
#define	LYELLOW 26
#define	DYELLOW 27
#define	LBROWN  28
#define DBROWN  29
#define NUM_COLORS 30

typedef struct {
    float r;
    float g;
    float b;
} color_s;

color_s color_ret[NUM_COLORS];

/* Tree Structures - Root/s, Sub/Branch/es, Trunk, Leaf/ves */
typedef struct {
  int color;       //default color, can be overwritten at display
  float r_offset; //randomized color r offset from main color
  float g_offset; //randomized color g offset from main color
  float b_offset; //randomized color b offset from main color
  double r;        //radius of cone based root
  double l;        //length of root (height of cone) 
  double x_pos;    //x distance from trunk center
  double y_pos;    //y distance from trunk center
  double z_pos;    //z distance from trunk center
  int rdeg;        //rotation degree vector
  double rx;       //x vec
  double rz;       //z vec 
  unsigned int default_texture;  //default texture
} root_s;
typedef struct {
  int num_roots;
  root_s *root;
} roots_s;
typedef struct {
  int color;       //default color, can be overwritten at display
  float r_offset; //randomized color r offset from main color
  float g_offset; //randomized color g offset from main color
  float b_offset; //randomized color b offset from main color
  double r;        //radius of cylinder based trunk
  double h;        //height of cylinder based trunk 
  unsigned int default_texture;  //default texture
} trunk_s;
typedef struct {
  int color;       //default color, can be overwritten at display
  float r_offset; //randomized color r offset from main color
  float g_offset; //randomized color g offset from main color
  float b_offset; //randomized color b offset from main color
  double s;        //size of leaf
  double s_offset; //randomized leaf size offset
  double x_pos;    //x distance from branch center
  double y_pos;    //y distance from branch center
  double z_pos;    //z distance from branch center
  int rdeg;        //rotation degree vector
  double rx;       //x vec
  double rz;       //z vec 
  unsigned int default_texture;  //default texture
} leaf_s;
typedef struct {
  int num_leaves;
  leaf_s *leaf;
} leaves_s;
typedef struct {
  int color;       //default color, can be overwritten at display
  float r_offset; //randomized color r offset from main color
  float g_offset; //randomized color g offset from main color
  float b_offset; //randomized color b offset from main color
  double r;        //radius of branch
  double l;        //length of branch 
  double x_pos;    //x distance from main branch center
  double y_pos;    //y distance from main branch center
  double z_pos;    //z distance from main branch center
  int rdeg;        //rotation degree vector
  double rx;       //x vec
  double ry;       //y vec
  double rz;       //z vec 
  unsigned int default_texture;  //default texture
  int has_leaves;
  leaves_s leaves;
} subbranch_s;
typedef struct {
  int num_subbranches;
  subbranch_s *subbranch;
} subbranches_s;
typedef struct {
  int color;       //default color, can be overwritten at display
  float r_offset; //randomized color r offset from main color
  float g_offset; //randomized color g offset from main color
  float b_offset; //randomized color b offset from main color
  double r;        //radius of branch
  double l;        //length of branch 
  double x_pos;    //x distance from trunk center
  double y_pos;    //y distance from trunk center
  double z_pos;    //z distance from trunk center
  int rdeg;        //rotation degree vector
  double rx;       //x vec
  double ry;       //y vec
  double rz;       //z vec 
  unsigned int default_texture;  //default texture
  int has_branches;
  subbranches_s subbranches;
} branch_s;
typedef struct {
  int num_branches;
  branch_s *branch;
} branches_s;
typedef struct {
  roots_s roots;
  trunk_s trunk;
  branches_s branches;
} tree_s;

#define NUM_TREES 2
tree_s  trees[NUM_TREES];

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

/* Texture defines */
#define C_RIGHT 0
#define C_LEFT  1
#define C_TOP 2
#define C_BOTTOM 3
#define C_BACK 4
#define C_FRONT 5
#define C_SAME 6

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

void init_color(){

  color_ret[GREEN  ] = (color_s){.r = 0, .g = 1, .b = 0};
  color_ret[BLUE   ] = (color_s){.r = 0, .g = 0, .b = 1};
  color_ret[RED    ] = (color_s){.r = 1, .g = 0, .b = 0};
  color_ret[ORANGE ] = (color_s){.r = 1, .g = .5, .b = 0};
  color_ret[PURPLE ] = (color_s){.r = .9, .g = .3, .b = 1};
  color_ret[BLACK  ] = (color_s){.r = 0, .g = 0, .b = 0};
  color_ret[WHITE  ] = (color_s){.r = 1, .g = 1, .b = 1};
  color_ret[YELLOW ] = (color_s){.r = 1, .g = 1, .b = 0};
  color_ret[BROWN  ] = (color_s){.r = .36, .g = .25, .b = .20};
  color_ret[MAGENTA] = (color_s){.r = 1, .g = 0, .b = 1};
  color_ret[CYAN   ] = (color_s){.r = 0, .g = 1, .b = 1};
  color_ret[GREY   ] = (color_s){.r = .5, .g = .5, .b = .5};
  color_ret[LGREEN ] = (color_s){.r = .8, .g = .9, .b = .5};
  color_ret[DGREEN ] = (color_s){.r = .4, .g = .3, .b = .2};
  color_ret[LBLUE  ] = (color_s){.r = .8, .g = .9, .b = 1};
  color_ret[DBLUE  ] = (color_s){.r = .3, .g = 0, .b = .6};
  color_ret[LRED   ] = (color_s){.r = 1, .g = .2, .b = 0};
  color_ret[PINK   ] = (color_s){.r = 1, .g = .5, .b = .8};
  color_ret[LORANGE] = (color_s){.r = 1, .g = .7, .b = .4};
  color_ret[DORANGE] = (color_s){.r = 1, .g = .4, .b = .1};
  color_ret[LPURPLE] = (color_s){.r = .9, .g = .6, .b = 1};
  color_ret[DPURPLE] = (color_s){.r = .7, .g = 0, .b = .9};
  color_ret[DWHITE ] = (color_s){.r = .9, .g = .9, .b = .9};
  color_ret[LGREY  ] = (color_s){.r = .8, .g = .8, .b = .8};
  color_ret[DGREY  ] = (color_s){.r = .2, .g = .2, .b = .2};
  color_ret[LBLACK ] = (color_s){.r = .1, .g = .1, .b = .1};
  color_ret[LYELLOW] = (color_s){.r = 1, .g = 1, .b = .8};
  color_ret[DYELLOW] = (color_s){.r = 1, .g = .8, .b = .2};
  color_ret[LBROWN ] = (color_s){.r = .54, .g = .396, .b = .114};
  color_ret[DBROWN ] = (color_s){.r = .35, .g = .16, .b = .14};
  //color_ret[] = (color_s){.r = , .g = , .b = };

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
//  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,texture_mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,crate_texture);
   //  Front
   glColor3f(1,0,0);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[7+C_FRONT]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   if (ntex) {
     glTexCoord2f(0,.65); glVertex3f(-1,-1, 1);
     glTexCoord2f(1,.65); glVertex3f(+1,-1, 1);
     glTexCoord2f(1,.85); glVertex3f(+1,+1, 1);
     glTexCoord2f(0,.85); glVertex3f(-1,+1, 1);
   }else {
     glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
     glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
     glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
     glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   }

   glEnd();
   //  Back
   glColor3f(0,0,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[7+C_BACK]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   if (ntex) {
     glTexCoord2f(0,.65); glVertex3f(+1,-1,-1);
     glTexCoord2f(1,.65); glVertex3f(-1,-1,-1);
     glTexCoord2f(1,.85); glVertex3f(-1,+1,-1);
     glTexCoord2f(0,.85); glVertex3f(+1,+1,-1);
   } else{
     glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
     glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
     glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
     glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   }
   glEnd();
   //  Right
   glColor3f(1,1,0);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[7+C_RIGHT]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   if (ntex) {
     glTexCoord2f(0,.1); glVertex3f(+1,-1,+1);
     glTexCoord2f(1,.1); glVertex3f(+1,-1,-1);
     glTexCoord2f(1,.4); glVertex3f(+1,+1,-1);
     glTexCoord2f(0,.4); glVertex3f(+1,+1,+1);
   }else {
     glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
     glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
     glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
     glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   }
   glEnd();
   //  Left
   glColor3f(0,1,0);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[7+C_LEFT]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   if (ntex) {
     glTexCoord2f(0,.1); glVertex3f(-1,-1,-1);
     glTexCoord2f(1,.1); glVertex3f(-1,-1,+1);
     glTexCoord2f(1,.4); glVertex3f(-1,+1,+1);
     glTexCoord2f(0,.4); glVertex3f(-1,+1,-1);
   }else {
     glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
     glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
     glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
     glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   }
   glEnd();
   //  Top
   glColor3f(0,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[7+C_TOP]);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glColor3f(1,0,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[7+C_BOTTOM]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(-1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(0,-1); glVertex3f(+1,-1,+1);
   glTexCoord2f(-1,-1); glVertex3f(-1,-1,+1);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
/*
 *  Draw a skycube (vertices will be switched 
 *                  as the textures are on the 
 *                  inside of the cube rather 
 *                  than the outside)
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 *     pass in texture array of size 6
 */
static void skycube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
		 unsigned int *tex)
{
  float ts = 1.0;
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
  //  Enable textures
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,texture_mode?GL_REPLACE:GL_MODULATE);
  glColor3f(1,1,1);
  
  //  Front
  glBindTexture(GL_TEXTURE_2D, tex[C_FRONT]);
  glBegin(GL_QUADS);
  glNormal3f( 0, 0, 1);
  glTexCoord2f(ts, 0); glVertex3f(-1,-1, 1);
  glTexCoord2f(ts,ts); glVertex3f(-1,+1, 1);
  glTexCoord2f( 0,ts); glVertex3f(+1,+1, 1);
  glTexCoord2f( 0, 0); glVertex3f(+1,-1, 1);
  glEnd();
  
  //  Back
  glBindTexture(GL_TEXTURE_2D,tex[C_BACK]);
  glBegin(GL_QUADS);
  glNormal3f( 0, 0,-1);
  glTexCoord2f(ts, 0); glVertex3f(+1,-1,-1);
  glTexCoord2f(ts,ts); glVertex3f(+1,+1,-1);
  glTexCoord2f( 0,ts); glVertex3f(-1,+1,-1);
  glTexCoord2f( 0, 0); glVertex3f(-1,-1,-1);
  glEnd();
  
  //  Right
  glBindTexture(GL_TEXTURE_2D,tex[C_RIGHT]);
  glBegin(GL_QUADS);
  glNormal3f(+1, 0, 0);
  glTexCoord2f( 0, 0); glVertex3f(+1,-1,-1);
  glTexCoord2f(ts, 0); glVertex3f(+1,-1,+1);
  glTexCoord2f(ts,ts); glVertex3f(+1,+1,+1);
  glTexCoord2f( 0,ts); glVertex3f(+1,+1,-1);
  glEnd();
  
  //  Left
  glBindTexture(GL_TEXTURE_2D,tex[C_LEFT]);
  glBegin(GL_QUADS);
  glNormal3f(-1, 0, 0);
  glTexCoord2f(ts,ts); glVertex3f(-1,+1,-1);
  glTexCoord2f( 0,ts); glVertex3f(-1,+1,+1);
  glTexCoord2f( 0, 0); glVertex3f(-1,-1,+1);
  glTexCoord2f(ts, 0); glVertex3f(-1,-1,-1);
  glEnd();
  
  //  Top
  glBindTexture(GL_TEXTURE_2D,tex[C_TOP]);
  glBegin(GL_QUADS);
  glNormal3f( 0,+1, 0);
  glTexCoord2f( 0, 0); glVertex3f(+1,+1,-1);
  glTexCoord2f(ts, 0); glVertex3f(+1,+1,+1);
  glTexCoord2f(ts,ts); glVertex3f(-1,+1,+1);
  glTexCoord2f( 0,ts); glVertex3f(-1,+1,-1);
  glEnd();
  
  //  Bottom
  glBindTexture(GL_TEXTURE_2D,tex[C_BOTTOM]);
  glBegin(GL_QUADS);
  glNormal3f( 0,-1, 0);
  glTexCoord2f(ts, 0); glVertex3f(-1,-1,-1);
  glTexCoord2f(ts,ts); glVertex3f(-1,-1,+1);
  glTexCoord2f( 0,ts); glVertex3f(+1,-1,+1);
  glTexCoord2f( 0, 0); glVertex3f(+1,-1,-1);
  glEnd();
  //  Undo transformations and textures
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
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
   if(color != -1) {
     glColor3f(color_ret[color].r, color_ret[color].g, color_ret[color].b);
   }
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


   //  Set texture
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,moon_texture);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
	glTexCoord2f(1,0); Vertex(th,    ph,WHITE);
	glTexCoord2f(0,0); Vertex(th,ph+inc,WHITE);
      }
      glEnd();
   }
   //  Undo transofrmations and texture
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw a sphere (version 1)
 *     at (x,y,z)
 *     radius (r)
 */

void sphere1(double x,double y,double z,double r, int rotate)
{
   const int d=35;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glRotated(rotate,1,0,0);
   glScaled(r,r,r);

   //  South pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,-90,WHITE);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,d-90,WHITE);
   }
   glEnd();
  
   //  Latitude bands
   ph=d-90;  
   glBegin(GL_QUAD_STRIP);
   glColor3f(1,1,0);
   for (th=0;th<=360;th+=d)
     {
       Vertex(th,ph,GREEN);
       Vertex(th,ph+d,GREEN);
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
       Vertex(th,ph,GREEN);
       Vertex(th,ph+d,GREEN);
     }
   glEnd();

   
   //  Undo transformations
   glPopMatrix();
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 *  Taken directly from Ex 13
 */
static void half_sphere(double x,double y,double z,double r, int rotatex, int rotatey, int rotatez,
			int color, int usetex, unsigned int tex)
{
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   if(rotatex && rotatez && rotatey) {
     glRotated(rotatex,1,1,1);
   } else if(rotatex && rotatez) {
     glRotated(rotatex,1,0,1);
   } else if(rotatey && rotatez) {
     glRotated(rotatey,0,1,1);
   } else if(rotatex && rotatey) {
     glRotated(rotatex,1,1,0);
   } else {
     glRotated(rotatex,1,0,0);
     glRotated(rotatey,0,1,0);
     glRotated(rotatez,0,0,1);
   }
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);


   //  Set texture
   glEnable(GL_TEXTURE_2D);
   if(usetex) {
     glBindTexture(GL_TEXTURE_2D,tex);
   }
   //  Bands of latitude
   for (ph=-90;ph<0;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
	glTexCoord2f(1,0); Vertex(th,    ph,color);
	glTexCoord2f(0,0); Vertex(th,ph+inc,color);
      }
      glEnd();
   }
   //  Undo transofrmations and texture
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
/* Basic flat edge cone with h height and r radius */
void cone(double x,double y,double z,double r, double h, int rotatedeg, float rotatex, float rotatez, 
	      int color, 
	      int usetex, unsigned int tex)
{
  
  int th;

   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(rotatedeg,rotatex,0,rotatez);
   glScaled(r,h,r);

   //  Set texture
   glEnable(GL_TEXTURE_2D);
   if(usetex) {
     glBindTexture(GL_TEXTURE_2D,tex);
   }
   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=5)
     {
       glTexCoord2f(1,(th+5)/360); Vertex(th, 0,color);
       glTexCoord2f(0,  th  /360); Vertex(th,90,color);
     }
   glEnd();

   //  Undo transofrmations and texture
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}


/* flared edge cylinder with h height and r radius */
void flared_cylinder(double x,double y,double z,double r, double h, int rotatex,int rotatez, 
	      int color, 
	      int usetex, unsigned int tex)
{
  
  int th;
  int d = 45;

   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   if(rotatex && rotatez) {
     glRotated(rotatex,1,0,1);
   } else {
     glRotated(rotatex,1,0,0);
     glRotated(rotatez,0,0,1);
   }
   glScaled(r,h,r);

   //  Set texture
   glEnable(GL_TEXTURE_2D);
   if(usetex) {
     glBindTexture(GL_TEXTURE_2D,tex);
   }
   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=d)
     {
       float text_coor0 = th/(float) 360;
       float text_coor1 = (th+d) /(float) 360;

       if(color != -1) {
	 glColor3f(color_ret[color].r, color_ret[color].g, color_ret[color].b);
       }
       glTexCoord2f(text_coor0,0);
       glNormal3d(Sin(th),0,Cos(th));
       glVertex3d(Sin(th),0,Cos(th));

       glTexCoord2f(text_coor0,1);
       glNormal3d(Sin(th),1,Cos(th));
       glVertex3d(Sin(th),1,Cos(th));

       glTexCoord2f(text_coor1,1);
       glNormal3d(Sin(th+d),1,Cos(th+d));
       glVertex3d(Sin(th+d),1,Cos(th+d));
				    
       glTexCoord2f(text_coor1,0);  
       glNormal3d(Sin(th+d),0,Cos(th+d));
       glVertex3d(Sin(th+d),0,Cos(th+d));
     }
   glEnd();

   //  Undo transofrmations and texture
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

void circle(double x, double y, double z, double r, int rotatex, int rotatez,
	    int color, 
	    int usetex, unsigned int tex)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   if(rotatex && rotatez) {
     glRotated(rotatex,1,0,1);
   } else {
     glRotated(rotatex,1,0,0);
     glRotated(rotatez,0,0,1);
   }
   glScaled(r,0,r);
   //  Set texture
   glEnable(GL_TEXTURE_2D);
   if(usetex) {
     glBindTexture(GL_TEXTURE_2D,tex);
   }
   if(color != -1) {
     glColor3f(color_ret[color].r, color_ret[color].g, color_ret[color].b);
   }
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,0,color);
   glTexCoord2f(0.5, 0.5);
   for (int th=0; th<=360; th+=5) {
     glTexCoord2f(sin(th), cos(th)); Vertex(th,0,color);
   }
   glEnd();
   //  Undo transofrmations and texture
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}


/* Basic flat edge cylinder with h height and r radius */
void cylinder(double x,double y,double z,double r, double h, int rotatex, int rotatez,
	      int color, 
	      int usetex, unsigned int tex)
{
  
   int th;
   int d = 30;
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   if(rotatex && rotatez) {
     glRotated(rotatex,1,0,1);
   } else {
     glRotated(rotatex,1,0,0);
     glRotated(rotatez,0,0,1);
   }
   glScaled(r,h,r);


   //bottom circle
   circle(0,0,0,
	 1,
	 0,0,
	 color,
	 usetex, tex);

   //top circle
   circle(0,1,0,
	 1,
	 0,0,
	 color,
	 usetex, tex);

   //  Set texture
   glEnable(GL_TEXTURE_2D);
   if(usetex) {
     glBindTexture(GL_TEXTURE_2D,tex);
   }
   if(color != -1) {
     glColor3f(color_ret[color].r, color_ret[color].g, color_ret[color].b);
   }
   glBegin(GL_QUADS);
   for (th=0;th<=360;th+=d)
     {
       float text_coor0 = th/(float) 360;
       float text_coor1 = (th+d) /(float) 360;

       quad_triangle_normal(Sin(th),0,Cos(th),
			    Sin(th+d),0,Cos(th+d),
			    Sin(th+d),1,Cos(th+d)); 
       glTexCoord2f(text_coor0,0); glVertex3f(Sin(th),0,Cos(th));
       glTexCoord2f(text_coor0,1); glVertex3f(Sin(th),1,Cos(th));
       glTexCoord2f(text_coor1,1); glVertex3f(Sin(th+d),1,Cos(th+d));
       glTexCoord2f(text_coor1,0); glVertex3f(Sin(th+d),0,Cos(th+d));
     }
   glEnd();

   //  Undo transofrmations and texture
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

/* Rounded edge cylinder with h height and r radius
 * rounded portion will have half sphere with radius/2 height
 */
void rounded_cylinder(double x,double y,double z,double r, double h, int rotatedeg, double rotatex, double rotatez, 
		      int color, int usetex, unsigned int tex)
{
  //double vec1 = sqrt(rotatex*rotatex + rotatez*rotatez);
  
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(rotatedeg,rotatex,0,rotatez);
   glScaled(1,1,1);


   //bottom sphere
   half_sphere(0,0,0,
	       r,
	       0,0,0,
	       color,
	       usetex, tex);
   cylinder(0,0,0,
	    r,h,
	    0,0,
	    color,
	    0,tex);
   //bottom sphere
   half_sphere(0,0+h,0,
	       r,
	       180,0,0,
	       color,
	       usetex, tex);
   
   //  Undo transofrmations and texture
   glPopMatrix();
}

void draw_leaf(double x,double y,double z, double s, int rotatedeg, double rotatex, double rotatez, 
	  int color, int usetex, unsigned int tex)
{
  float r = 0.01;
  
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(rotatedeg,rotatex,0,rotatez);
   glScaled(s,s,s);
   glEnable(GL_CULL_FACE);

//  Set texture
   glEnable(GL_TEXTURE_2D);
   if(usetex) {
     glBindTexture(GL_TEXTURE_2D,tex);
   }
   if(color != -1) {
     glColor3f(color_ret[color].r, color_ret[color].g, color_ret[color].b);
   }
   //leaf front
   glBegin(GL_TRIANGLES);
   quad_triangle_normal(-.25,.75,0,
			-.50,1.0,0,
			-.50,.50,0); 
   glTexCoord2f(0,0); glVertex3f(-.25,.75,r);
   glTexCoord2f(0,1); glVertex3f(-.50,1.0,r);
   glTexCoord2f(1,1); glVertex3f(-.50,.50,r);
   glEnd();
   glBegin(GL_TRIANGLES);
   quad_triangle_normal(-.50,.50,0,
			0.50,.50,0,
			0.00,1.0,0); 
   glTexCoord2f(1,1); glVertex3f(-.50,.50,r);
   glTexCoord2f(0,0); glVertex3f(0.50,.50,r);
   glTexCoord2f(0,1); glVertex3f(0.00,1.0,r);
   glEnd();
   glBegin(GL_TRIANGLES);
   quad_triangle_normal(0.50,.50,0,
			0.50,1.0,0,
			0.25,.75,0);
   glTexCoord2f(0,0); glVertex3f(0.50,.50,r);
   glTexCoord2f(0,1); glVertex3f(0.50,1.0,r); 
   glTexCoord2f(1,1); glVertex3f(0.25,.75,r);
   glEnd();
   //leaf back
   glBegin(GL_TRIANGLES);
   quad_triangle_normal(-.25,.75,-r,
			-.50,.50,-r,
			-.50,1.0,-r); 
   glTexCoord2f(0,0); glVertex3f(-.25,.75,-r);
   glTexCoord2f(0,1); glVertex3f(-.50,1.0,-r);
   glTexCoord2f(1,1); glVertex3f(-.50,.50,-r);
   glEnd();
   glBegin(GL_TRIANGLES);
   quad_triangle_normal(-.50,.50,-r,
			0.00,1.0,-r,
			0.50,.50,-r); 
   glTexCoord2f(1,1); glVertex3f(-.50,.50,-r);
   glTexCoord2f(0,0); glVertex3f(0.50,.50,-r);
   glTexCoord2f(0,1); glVertex3f(0.00,1.0,-r);
   glEnd();
   glBegin(GL_TRIANGLES);
   quad_triangle_normal(0.50,.50,-r,
			0.25,.75,-r,
			0.50,1.0,-r);
   glTexCoord2f(0,0); glVertex3f(0.50,.50,-r);
   glTexCoord2f(0,1); glVertex3f(0.50,1.0,-r); 
   glTexCoord2f(1,1); glVertex3f(0.25,.75,-r);
   glEnd();
   //leaf side
   
   glBegin(GL_QUADS);
   quad_triangle_normal(0.50,.50,-r,
			0.50,1.0,-r,
			0.50,1.0,+r);
   glTexCoord2f(0,0); glVertex3f(0.50,.50,-r);
   glTexCoord2f(0,0); glVertex3f(0.50,.50,+r);
   glTexCoord2f(0,1); glVertex3f(0.50,1.0,+r); 
   glTexCoord2f(0,1); glVertex3f(0.50,1.0,-r); 

   glEnd();
   glBegin(GL_QUADS);
   quad_triangle_normal(0.50,1.0,-r,
			0.25,.75,-r,
			0.25,.75,+r);
   glTexCoord2f(0,0); glVertex3f(0.50,1.0,-r);
   glTexCoord2f(0,0); glVertex3f(0.50,1.0,+r);
   glTexCoord2f(0,1); glVertex3f(0.25,.75,+r); 
   glTexCoord2f(0,1); glVertex3f(0.25,.75,-r);

   glEnd();
   glBegin(GL_QUADS);
   quad_triangle_normal(0.25,.75,-r,
			0.00,1.0,-r,
			0.00,1.0,+r);
   glTexCoord2f(0,0); glVertex3f(0.25,.75,-r);
   glTexCoord2f(0,0); glVertex3f(0.25,.75,+r);
   glTexCoord2f(0,1); glVertex3f(0.00,1.0,+r); 
   glTexCoord2f(0,1); glVertex3f(0.00,1.0,-r);

   glEnd();
   glBegin(GL_QUADS);
   quad_triangle_normal(0.00,1.0,-r,
			0.25,.75,-r,
			0.25,.75,+r);
   glTexCoord2f(0,0); glVertex3f(0.00,1.0,-r);
   glTexCoord2f(0,0); glVertex3f(0.00,1.0,+r);
   glTexCoord2f(0,1); glVertex3f(-.25,.75,+r); 
   glTexCoord2f(0,1); glVertex3f(-.25,.75,-r); 
   
   glEnd();
   glBegin(GL_QUADS);
   quad_triangle_normal(0.25,.75,-r,
			-.50,1.0,-r,
			-.50,1.0,+r);
   glTexCoord2f(0,0); glVertex3f(-.25,.75,-r);
   glTexCoord2f(0,0); glVertex3f(-.25,.75,+r);
   glTexCoord2f(0,1); glVertex3f(-.50,1.0,+r); 
   glTexCoord2f(0,1); glVertex3f(-.50,1.0,-r); 

   glEnd();
   glBegin(GL_QUADS);
   quad_triangle_normal(-.50,1.0,-r,
			-.50,.50,-r,
			-.50,.50,+r);
   glTexCoord2f(0,0); glVertex3f(-.50,1.0,-r);
   glTexCoord2f(0,0); glVertex3f(-.50,1.0,+r);
   glTexCoord2f(0,1); glVertex3f(-.50,.50,+r); 
   glTexCoord2f(0,1); glVertex3f(-.50,.50,-r); 

   glEnd();
   glBegin(GL_QUADS);
   quad_triangle_normal(-.50,.50,-r,
			0.50,.50,-r,
			0.50,.50,+r);
   glTexCoord2f(0,0); glVertex3f(-.50,.50,-r);
   glTexCoord2f(0,0); glVertex3f(-.50,.50,+r);
   glTexCoord2f(0,1); glVertex3f(0.50,.50,+r); 
   glTexCoord2f(0,1); glVertex3f(0.50,.50,-r); 
   glEnd();

   //Stem
   
   rounded_cylinder(0,0,0,
		    r,.50,
		    0,0,0,
		    color,
		    0,tex);
   glDisable(GL_CULL_FACE);
   //  Undo transofrmations and texture
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}


void draw_subbranch(subbranch_s *subbranch, double x,double y,double z,double r, double h, int rotatedeg, double rotatex,double rotatey, double rotatez, 
		 int color, int usetex, unsigned int tex)

{
  //  Save transformation
  glPushMatrix();
  //  Offset, scale and rotate
  glTranslated(x,y,z);
  glRotated(rotatedeg,rotatex,rotatey,rotatez);
  glScaled(1,1,1);
  
  //subbranch
  //rounded_cylinder(x,y,z,r,h,rotatedeg,rotatex,rotatez,color,usetex,tex);  
  rounded_cylinder(0,0,0,subbranch->r,subbranch->l,0,0,0,subbranch->color,usetex,tex);  

  //leaves
  for(int i = 0; i< subbranch->leaves.num_leaves; i++)
    {
      leaf_s *leaf = subbranch->leaves.leaf + i;
      draw_leaf(leaf->x_pos,leaf->y_pos,leaf->z_pos,
		leaf->s,
		leaf->rdeg,leaf->rx,leaf->rz,
		leaf->color,
		usetex, tex);
    }

  //  Undo transofrmations and texture
  glPopMatrix();
}
void draw_branch(branch_s *branch, double x,double y,double z,double r, double h, int rotatedeg, double rotatex, double rotatez, 
		 int color, int usetex, unsigned int tex)

{

  //  Save transformation
  glPushMatrix();
  //  Offset, scale and rotate
  glTranslated(x,y,z);
  glRotated(rotatedeg,rotatex,0,rotatez);
  glScaled(1,1,1);
  glEnable(GL_CULL_FACE);  
  //main branch
  //rounded_cylinder(x,y,z,r,h,rotatedeg,rotatex,rotatez,color,usetex,tex); //already translated/rotated 
  rounded_cylinder(0,0,0,r,h,0,0,0,color,usetex,tex);  

  //subbranches
  for(int i = 0; i< branch->subbranches.num_subbranches; i++)
    {
      subbranch_s *subbranch = branch->subbranches.subbranch + i;
      draw_subbranch(subbranch, subbranch->x_pos,subbranch->y_pos,subbranch->z_pos,
		     r,h,
		     subbranch->rdeg,subbranch->rx,subbranch->ry,subbranch->rz,
		     color,            
		     usetex,tex);      
    }
  glDisable(GL_CULL_FACE);  
  //  Undo transofrmations and texture
  glPopMatrix();
}

void number(int number, double x,double y,double z,double r, double h, int rotatex, int rotatez, 
	    int color, int usetex, unsigned int tex)
{
  //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   if(rotatex && rotatez) {
     glRotated(rotatex,1,0,1);
   } else {
     glRotated(rotatex,1,0,0);
     glRotated(rotatez,0,0,1);
   }
   
   if((number != 4) && 
      (number != 7) &&
      (number != 9))
     {
       rounded_cylinder(0,0,0,
			r,h/2,
			90,1,0,
			color,
			usetex,tex);
     }
   if((number != 0) && 
      (number != 1) &&
      (number != 7))
     {
       rounded_cylinder(0,h/2,0,
			r,h/2,
			90,1,0,
			color,
			usetex,tex);
     }
   if((number != 1) && 
      (number != 4) &&
      (number != 6))
     {
       rounded_cylinder(0,h,0,
			r,h/2,
			90,1,0,
			color,
			usetex,tex);
     }
   if((number != 1) && 
      (number != 2) &&
      (number != 5) &&
      (number != 6) &&
      (number != 7))
     {
       rounded_cylinder(0,0,h/2,
			r,h,
			0,0,0,
			color,
			usetex,tex);
     }
   if((number == 2))
     {
       rounded_cylinder(0,0,0,
			r,h/2,
			0,0,0,
			color,
			usetex,tex);
       rounded_cylinder(0,h/2,h/2,
			r,h/2,
			0,0,0,
			color,
			usetex,tex);
     }
   if((number == 0) || 
      (number == 6) ||
      (number == 8))
     {
       rounded_cylinder(0,0,0,
			r,h,
			0,0,0,
			color,
			usetex,tex);
     }
   if((number == 4) || 
      (number == 5) ||
      (number == 9))
     {
       rounded_cylinder(0,h/2,0,
			r,h/2,
			0,0,0,
			color,
			usetex,tex);
     }
   if((number == 5) || 
      (number == 6))
     {
       rounded_cylinder(0,0,h/2,
			r,h/2,
			0,0,0,
			color,
			usetex,tex);
     }
   if((number == 1))
     {
       rounded_cylinder(0,3*h/4,0,
			r,1.4*h/4,
			45,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,0,h/4,
			r,h,
			0,0,0,
			color,
			usetex,tex);
     }
   if((number == 7))
     {
       rounded_cylinder(0,0,h/5,
			r,1.02*h,
			16,1,0,
			color,
			usetex,tex);
     }
 
   
   //  Undo transofrmations and texture
   glPopMatrix();

}

void letter(char letter, double x,double y,double z,double r, double h, int rotatex, int rotatez, 
	    int color, int usetex, unsigned int tex)
{
  //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   if(rotatex && rotatez) {
     glRotated(rotatex,1,0,1);
   } else {
     glRotated(rotatex,1,0,0);
     glRotated(rotatez,0,0,1);
   }
   
  
   if((letter == 'A'))
     {
       rounded_cylinder(0,0,0,
			r,1.02*h,
			14,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,0,h/2,
			r,1.02*h,
			-14,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,h/2,0,
			r,h/2,
			90,1,0,
			color,
			usetex,tex);
     }
   if((letter == 'B'))
     {
       rounded_cylinder(0,0,0,
			r,h,
			0,0,0,
			color,
			usetex,tex);
       rounded_cylinder(0,.02*h,0,
			r,h/2,
			90,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,h/2,0,
			r,h/3,
			90,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,.98*h,0,
			r,h/2,
			90,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,.02*h,h/2,
			r,h/2-.1*h,
			0,0,0,
			color,
			usetex,tex);
       rounded_cylinder(0,.98*h,h/2,
			r,h/2-.1*h,
			180,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,h/2,h/3,
			r,.18*h,
			65,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,h/2,h/3,
			r,.18*h,
			115,1,0,
			color,
			usetex,tex);
     }
   if((letter == 'C'))
     {
       rounded_cylinder(0,.1*h,0,
			r,.8*h,
			0,0,0,
			color,
			usetex,tex);
       rounded_cylinder(0,.1*h,0,
			r,.12*h,
			135,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,0,.1*h,
			r,h/3-.1*h,
			90,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,.9*h,0,
			r,.12*h,
			45,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,h,.1*h,
			r,h/3-.1*h,
			90,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,h,h/3,
			r,.2*h,
			135,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,0,h/3,
			r,.2*h,
			45,1,0,
			color,
			usetex,tex);
     }
   if((letter == 'D'))
     {
       rounded_cylinder(0,0,0,
			r,h,
			0,0,0,
			color,
			usetex,tex);
       rounded_cylinder(0,0,0,
			r,h/4,
			90,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,h,0,
			r,h/4,
			90,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,0,h/4,
			r,.15*h,
			30,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,h,h/4,
			r,.15*h,
			150,1,0,
			color,
			usetex,tex);
       rounded_cylinder(0,.16*h,h/3,
			r,.7*h,
			0,0,0,
			color,
			usetex,tex);
     }
 
   
   //  Undo transofrmations and texture
   glPopMatrix();

}



/* Get random value between two values */
int rand_b2i(int lower, int upper) 
{
  return ((rand() % (upper-lower + 1)) + lower);
}

/* Creating separate function for current color */
void color_plane(int color, int color_brightness)
{
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
	glColor3f(0.15,0.15,0.15);
      else if(color_brightness == DARK)
	glColor3f(.1,.1,.1);
      else if(color_brightness == LIGHT)
	glColor3f(.2,.2,.2);
      break;       
    }  
}
/* Creating separate function for current color */
int convert_cat_color(int color, int color_brightness)
{
  switch(color)
    {
    case BROWN_CAT:
      if(color_brightness == NORMAL)
	return BROWN;
      else if(color_brightness == DARK)
	return DBROWN;
      else if(color_brightness == LIGHT)
	return LBROWN;
      break;
    case WHITE_CAT:
      if(color_brightness == NORMAL)
	return DWHITE;
      else if(color_brightness == DARK)
	return LGREY;
      else if(color_brightness == LIGHT)
	return WHITE;
      break;
    case BLACK_CAT:
      if(color_brightness == NORMAL)
	return LBLACK;
      else if(color_brightness == DARK)
	return BLACK;
      else if(color_brightness == LIGHT)
	return DGREY;
      break;       
    } 
  return -1;
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

   //KG new for textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, texture_mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D, texture[C_SAME]);

   
   //  Cube
   //  Front
   color_plane(color, color_brightness);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[C_FRONT]);
   glBegin(GL_QUADS);   
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[C_BACK]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[C_RIGHT]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[C_LEFT]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[C_TOP]);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[C_BOTTOM]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
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

   //KG new for textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, texture_mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   if(!ntex) glBindTexture(GL_TEXTURE_2D, texture[C_SAME]);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);

   color_plane(color,color_brightness);
   glBegin(GL_QUADS);
   //Be sure to rotate through vertices in correct order to get the correct side normalized
   quad_triangle_normal(1, 0, -1,
			-1,0,1,
			-1, 0, -1);  
   glTexCoord2f(0,0); glVertex3f(1, 0, 1);
   glTexCoord2f(1,0); glVertex3f(1, 0, -1);
   glTexCoord2f(1,1); glVertex3f(-1, 0, -1);
   glTexCoord2f(0,1); glVertex3f(-1, 0, 1);

   glEnd();
   glBegin(GL_QUADS);
   quad_triangle_normal(1, 2, -1,
			-1,0,-1,
			-1,2,-1);
   glTexCoord2f(0,0); glVertex3f(1,0,-1);
   glTexCoord2f(1,0); glVertex3f(1,2,-1);
   glTexCoord2f(1,1); glVertex3f(-1,2,-1);
   glTexCoord2f(0,1); glVertex3f(-1,0,-1);
   
   quad_triangle_normal(-1,2,-1,
			-1,0,1,
			1,0,1);
   glTexCoord2f(0,0); glVertex3f(1,1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,2,-1);
   glTexCoord2f(1,1); glVertex3f(-1,0,1);
   glTexCoord2f(0,1); glVertex3f(1,0,1);
   glEnd();


   glBegin(GL_TRIANGLES);
   if(rotate == -90) {
     glNormal3f(-1, 0, 0);
     glTexCoord2f(1,0); glVertex3f(-1,0,1);
     glTexCoord2f(1,1); glVertex3f(-1,2,-1);
     glTexCoord2f(0,1); glVertex3f(-1,0,-1);
     glEnd();
     glBegin(GL_TRIANGLES);
     //one side of ear needs to be pink
     glColor3f(0.737255,0.560784,0.560784);
     glNormal3f( 1, 0, 0);
     glTexCoord2f(0,0); glVertex3f(1,0,1);
     glTexCoord2f(1,0); glVertex3f(1,2,-1);
     glTexCoord2f(1,1); glVertex3f(1,0,-1);
   } else {
     glNormal3f( 1, 0, 0);
     glTexCoord2f(0,0); glVertex3f(1,0,1);
     glTexCoord2f(1,0); glVertex3f(1,2,-1);
     glTexCoord2f(1,1); glVertex3f(1,0,-1);
     glEnd();
     glBegin(GL_TRIANGLES);
     //one side of ear needs to be pink
     glColor3f(0.737255,0.560784,0.560784);
     glNormal3f(-1, 0, 0);
     glTexCoord2f(1,0); glVertex3f(-1,0,1);
     glTexCoord2f(1,1); glVertex3f(-1,2,-1);
     glTexCoord2f(0,1); glVertex3f(-1,0,-1);
   }   
   glEnd();

   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   
}

void legs(double x, double y, double z, int color, int color_brightness)
{  
  double height = y+UNIT_SIZE*.02;
  double leg_spacing = x+UNIT_SIZE*.015;
  double torso_spacing = z+UNIT_SIZE*.03;
  //if(color == MIXED) {
  //  //back left
  //  one_color_cube(-leg_spacing,height,-torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, mc.leg_color[0], mc.leg_brightness[0]);
  //  //back right							    					    
  //  one_color_cube( leg_spacing,height,-torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, mc.leg_color[1], mc.leg_brightness[1]);
  //  //front left							    					 
  //  one_color_cube( leg_spacing,height, torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, mc.leg_color[2], mc.leg_brightness[2]);
  //  //front right						    					    
  //  one_color_cube(-leg_spacing,height, torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, mc.leg_color[3], mc.leg_brightness[3]);
  //}else {
  //  //back left
  //  one_color_cube(-leg_spacing,height,-torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, color, color_brightness);
  //  //back right							    					    
  //  one_color_cube( leg_spacing,height,-torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, color, color_brightness);
  //  //front left							    					 
  //  one_color_cube( leg_spacing,height, torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, color, color_brightness);
  //  //front right						    					    
  //  one_color_cube(-leg_spacing,height, torso_spacing, UNIT_SIZE*.005, UNIT_SIZE*.02, UNIT_SIZE*.005, color, color_brightness);
  //}
  if(color == MIXED) {
    //back left
    int leg_color = convert_cat_color(mc.leg_color[0], mc.leg_brightness[0]);
    rounded_cylinder(-leg_spacing,height/2,-torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, 1,texture[C_SAME]);	
    //back right					
    leg_color = convert_cat_color(mc.leg_color[1], mc.leg_brightness[1]);		    				
    rounded_cylinder(leg_spacing,height/2,-torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, 1,texture[C_SAME]);	    
    //front left					
    leg_color = convert_cat_color(mc.leg_color[2], mc.leg_brightness[2]);		    			
    rounded_cylinder(leg_spacing,height/2,torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, 1,texture[C_SAME]);		 
    //front right			
    leg_color = convert_cat_color(mc.leg_color[3], mc.leg_brightness[3]);			    	
    rounded_cylinder(-leg_spacing,height/2,torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, 1,texture[C_SAME]);
  }else {
    int leg_color = convert_cat_color(color, color_brightness);
    //back left
    rounded_cylinder(-leg_spacing,height/2,-torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, 1,texture[C_SAME]);
    //back right							    				
    rounded_cylinder(leg_spacing,height/2,-torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, 1,texture[C_SAME]);	    
    //front left							    			
    rounded_cylinder(leg_spacing,height/2,torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, 1,texture[C_SAME]);		 
    //front right						    	
    rounded_cylinder(-leg_spacing,height/2,torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, 1,texture[C_SAME]);			
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

  //if(color == MIXED) {
  //  one_color_cube(x,height,torso_back, tail_width, tail_height, tail_length, mc.tail_color, mc.tail_brightness);
  //} else {
  //  one_color_cube(x,height,torso_back, tail_width, tail_height, tail_length, color, color_brightness);
  //}
  if(color == MIXED) {
    one_color_cube(x,height,torso_back, tail_width, tail_height, tail_length, mc.tail_color, mc.tail_brightness);
  } else {
    //one_color_cube(x,height,torso_back, tail_width, tail_height, tail_length, color, color_brightness);
    int tail_color = convert_cat_color(color, color_brightness);
    rounded_cylinder(x,y+UNIT_SIZE*.05+UNIT_SIZE*.01,z-UNIT_SIZE*.04, UNIT_SIZE*.005, UNIT_SIZE*.06,-75,1,0,tail_color, 1,texture[C_SAME]);
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

unsigned int rand_tree()
{
  current_rand++;
  return ((rand()*current_rand + (current_rand*3)%2));
}

/* Making a deep copy for trunk struct to avoid pointer problems 
 * (although  trunk is pretty much the only structure that 
 *  does not have a pointer to worry about)
 */
void copy_trunk_s(trunk_s *t_orig, trunk_s *t_copy)
{
  t_copy->color    = t_orig->color;
  t_copy->r_offset = t_orig->r_offset;
  t_copy->g_offset = t_orig->g_offset;
  t_copy->b_offset = t_orig->b_offset;
  t_copy->r 	   = t_orig->r; 	
  t_copy->h        = t_orig->h;   
  t_copy->default_texture = t_orig->default_texture;
}

void create_trunk(trunk_s *trunk)
{
  trunk->color = BROWN;
  //randomize color offsets
  trunk->r_offset = (rand() % 5)/100.0;
  trunk->g_offset = (rand() % 5)/100.0;
  trunk->b_offset = (rand() % 5)/100.0;
  trunk->r = 1.0/16.0;
  trunk->h = 1.0/2.0;
  trunk->default_texture = tree_texture[BARK];
  //fprintf(stdout,"trunk addr = 0x%p\n",trunk); 
}

void copy_root_s(root_s *r_orig, root_s *r_copy)
{
  r_copy->color    	  = r_orig->color;
  r_copy->r_offset 	  = r_orig->r_offset;
  r_copy->g_offset 	  = r_orig->g_offset;
  r_copy->b_offset 	  = r_orig->b_offset;
  r_copy->r 	 	  = r_orig->r; 	
  r_copy->l        	  = r_orig->l;
  r_copy->x_pos  	  = r_orig->x_pos;
  r_copy->y_pos  	  = r_orig->y_pos;
  r_copy->z_pos  	  = r_orig->z_pos;
  r_copy->rdeg            = r_orig->rdeg;
  r_copy->rx              = r_orig->rx;
  r_copy->rz     	  = r_orig->rz;
  r_copy->default_texture = r_orig->default_texture;
}
void print_root_s(root_s *r_orig)
{
  fprintf(stdout,"address of current root = %p\n", r_orig); 
  fprintf(stdout, 
	  "r_orig->color    	  = %d\nr_orig->r_offset 	  = %.3f\nr_orig->g_offset 	  = %.3f\nr_orig->b_offset 	  = %.3f\nr_orig->r 	 	  = %.3f\nr_orig->l        	  = %.3f\nr_orig->x_pos  	  = %.3f\nr_orig->y_pos  	  = %.3f\nr_orig->z_pos  	  = %.3f\nr_orig->rdeg           = %d\nr_orig->rx             = %.3f\nr_orig->rz     	  = %.3f\n",
	  r_orig->color,r_orig->r_offset,r_orig->g_offset,r_orig->b_offset,r_orig->r,r_orig->l,r_orig->x_pos,r_orig->y_pos,r_orig->z_pos,r_orig->rdeg,r_orig->rx,r_orig->rz);	    
}

void create_root(root_s *root, int current_num, int rand_root)
{
  int rand_positive = 0;
  int rand_value = 0;
  double rand_rdiv = 8.0;
  root->color = BROWN;
  //randomize color offsets
  root->r_offset = (rand_tree() % 5)/100.0;
  root->g_offset = (rand_tree() % 5)/100.0;
  root->b_offset = (rand_tree() % 5)/100.0;
  rand_value = rand_root ? rand_tree() % 11 : 0;
  rand_positive = rand_root ? rand_tree() % 2 : 0;
  rand_rdiv = rand_root ? rand_tree() % 4 + 16.0 : 16.0;
  root->r = rand_positive ? (1.0 + rand_value/100.0)/rand_rdiv : (1.0 - rand_value/100.0)/rand_rdiv;
  rand_value = rand_root ? rand_tree() % 21 : 0;
  rand_positive = rand_root ? rand_tree() % 2 : 0;
  root->l = rand_positive ? (1.0 + rand_value/100.0)/5.0 : (1.0 - rand_value/100.0)/5.0;
  root->x_pos = 0;
  root->y_pos = 0;
  root->z_pos = 0;
  rand_value = rand_root ? rand_tree() % 11 : 0;
  rand_positive = rand_root ? rand_tree() % 2 : 0;
  root->rdeg  = rand_positive ? 100 + rand_value : 100 - rand_value;
  rand_positive = rand_tree() % 2;
  root->rdeg  = rand_positive ? root->rdeg : -root->rdeg;
  root->rx    = 0;
  root->rz    = 0;
  if(((root->rx < 35) && (root->rx > -35)) && ((root->rz < 35) && (root->rz > -35)))
    {
      rand_value = rand_tree() % 101;
      rand_positive = rand_tree() % 2;
      root->rx    = rand_positive ? rand_value: -rand_value;
      rand_value = rand_tree() % 101;
      rand_positive = rand_tree() % 2;
      root->rz    = rand_positive ? rand_value: -rand_value;
    }
  root->default_texture = tree_texture[ROOT];
}

void copy_roots_s(roots_s *rs_orig, roots_s *rs_copy)
{
  rs_copy->num_roots = rs_orig->num_roots;

  //allocate space for roots array/pointer
  rs_copy->root = malloc(sizeof(root_s)*(rs_copy->num_roots));  //remember to free this memory

  for(int i = 0; i < rs_copy->num_roots; i++)
    {
      copy_root_s(rs_orig->root + i, rs_copy->root + i);
    }
}

void copy_branch_s(branch_s *r_orig, branch_s *r_copy)
{
  r_copy->color    	  = r_orig->color;
  r_copy->r_offset 	  = r_orig->r_offset;
  r_copy->g_offset 	  = r_orig->g_offset;
  r_copy->b_offset 	  = r_orig->b_offset;
  r_copy->r 	 	  = r_orig->r; 	
  r_copy->l        	  = r_orig->l;
  r_copy->x_pos  	  = r_orig->x_pos;
  r_copy->y_pos  	  = r_orig->y_pos;
  r_copy->z_pos  	  = r_orig->z_pos;
  r_copy->rdeg            = r_orig->rdeg;
  r_copy->rx              = r_orig->rx;
  r_copy->rz     	  = r_orig->rz;
  r_copy->default_texture = r_orig->default_texture;
}
void print_branch_s(branch_s *b_orig)
{
  fprintf(stdout,"address of current branch = %p\n", b_orig); 
  fprintf(stdout, 
	  "b_orig->color    	  = %d\nb_orig->r_offset 	  = %.3f\nb_orig->g_offset 	  = %.3f\nb_orig->b_offset 	  = %.3f\nb_orig->r 	 	  = %.3f\nb_orig->l        	  = %.3f\nb_orig->x_pos  	  = %.3f\nb_orig->y_pos  	  = %.3f\nb_orig->z_pos  	  = %.3f\nb_orig->rdeg           = %d\nb_orig->rx             = %.3f\nb_orig->rz     	  = %.3f\n",
	  b_orig->color,b_orig->r_offset,b_orig->g_offset,b_orig->b_offset,b_orig->r,b_orig->l,b_orig->x_pos,b_orig->y_pos,b_orig->z_pos,b_orig->rdeg,b_orig->rx,b_orig->rz);	    
}

void create_leaf(leaf_s *leaf, int current_num, double r, double l, int rand_leaf)
{
  int rand_color = rand_tree() % 2;
  leaf->color = rand_color ? LGREEN : GREEN;
  //randomize color offsets
  leaf->r_offset = (rand_tree() % 5)/100.0;
  leaf->g_offset = (rand_tree() % 5)/100.0;
  leaf->b_offset = (rand_tree() % 5)/100.0;

  leaf->s = .06;
  leaf->x_pos = 0;  
  if(current_num == 0)
    leaf->y_pos = l+r;
  else
    {
      int cnum2 = (current_num-1)/2;
      leaf->y_pos = (l+2*r)*(1.0-cnum2/3.0);
    }
  leaf->z_pos = 0;

  if(current_num == 0)
    leaf->rdeg = 0;
  else
    leaf->rdeg  = current_num %2 ? 90 : -90;

  leaf->rx    = 1;
  leaf->rz    = 1;
  leaf->default_texture = tree_texture[LEAF];  
}
void create_subbranch(subbranch_s *subbranch, int current_num, double r, double l, int rand_subbranch)
{
  subbranch->color = DGREEN;
  //randomize color offsets
  subbranch->r_offset = (rand_tree() % 5)/100.0;
  subbranch->g_offset = (rand_tree() % 5)/100.0;
  subbranch->b_offset = (rand_tree() % 5)/100.0;

  subbranch->r = r/4.0;
  subbranch->l = l;
  subbranch->x_pos = 0;  
  if(current_num == 0)
    subbranch->y_pos = l;
  else
    {
      int cnum2 = (current_num-1)/2;
      subbranch->y_pos = (l+2*r)*(1.0-cnum2/4.0);
    }
  subbranch->z_pos = 0;

  if(current_num == 0)
    subbranch->rdeg = 0;
  else
    subbranch->rdeg  = current_num %2 ? 80 : -80;

  subbranch->rx    = 1;
  subbranch->ry    = 1;
  subbranch->rz    = 1;
  subbranch->default_texture = tree_texture[ROOT];

  subbranch->has_leaves = 1;
  //randomize number of leaves between 7, 9, 11, 13
  subbranch->leaves.num_leaves = (rand_tree() % 4)*2 + 7;      
  
  //allocate space for subranches array/pointer
  subbranch->leaves.leaf = malloc(sizeof(leaf_s)*(subbranch->leaves.num_leaves));  //remember to free this memory
  
  for(int i = 0; i < subbranch->leaves.num_leaves; i++)
    {	  
      create_leaf(subbranch->leaves.leaf + i, i,subbranch->r,subbranch->l, 0); //don't randomize for now
    }
  
}

void create_branch(branch_s *branch, int current_num, int rand_branch)
{
  int rand_positive = 0;
  int rand_value = 0;

  branch->color = BROWN;
  //randomize color offsets
  branch->r_offset = (rand_tree() % 5)/100.0;
  branch->g_offset = (rand_tree() % 5)/100.0;
  branch->b_offset = (rand_tree() % 5)/100.0;

  rand_value = rand_branch ? rand_tree() % 21 : 0;
  rand_positive = rand_branch ? rand_tree() % 2 : 0;
  branch->r = rand_positive ? (1.0 + rand_value/100.0)/(24.0+current_num) : (1.0 - rand_value/100.0)/(24.0+current_num);

  rand_value = rand_branch ? rand_tree() % 21 : 0;
  rand_positive = rand_branch ? rand_tree() % 2 : 0;
  branch->l = rand_positive ? (1.0 + rand_value/100.0)/(4.5+.5*(current_num/6)) : (1.0 - rand_value/100.0)/(4.5+.5*(current_num/6));

  branch->x_pos = 0;
  branch->y_pos = 3/8+(current_num*.05);
  branch->z_pos = 0;

  rand_value = rand_branch ? rand_tree() % 11 : 0;
  rand_positive = rand_branch ? rand_tree() % 2 : 0;
  branch->rdeg  = rand_positive ? 85 + rand_value : 85 - rand_value;
  rand_positive = rand_branch ? rand_tree() % 2 : 0;
  branch->rdeg  = rand_positive ? branch->rdeg: -branch->rdeg;
  branch->rx = rand_tree() % 3 ? 1 : 0;
  branch->rz = rand_tree() % 3 ? 1 : 0;
  if(((branch->rx > -10) && (branch->rx < 10)) && ((branch->rz > -10) && (branch->rz < 10)) && !((branch->rx == 0) && (branch->rz == 0)))
    {
      rand_value = rand_tree() % 101;
      rand_positive = rand_tree() % 2;
      branch->rx    = rand_positive ? rand_value: -rand_value;
      rand_value = rand_tree() % 101;
      rand_positive = rand_tree() % 2;
      branch->rz    = rand_positive ? rand_value: -rand_value;
    }
  if((branch->rz==0) && (branch->rx == 0))
    {
      branch->l = branch->l / 4 ;
    } 
   
  //randomize number of subranches between 9, 11, 13, 15
  branch->subbranches.num_subbranches = (rand_tree() % 4)*2 + 9;      
  
  //allocate space for subranches array/pointer
  branch->subbranches.subbranch = malloc(sizeof(subbranch_s)*(branch->subbranches.num_subbranches));  //remember to free this memory
  
  for(int i = 0; i < branch->subbranches.num_subbranches; i++)
    {	  
      create_subbranch(branch->subbranches.subbranch + i, i,branch->r,branch->l, 0); //don't randomize for now
    }
      
  branch->default_texture = tree_texture[BARK];
}
   
void copy_branches_s(branches_s *rs_orig, branches_s *rs_copy)
{
  rs_copy->num_branches = rs_orig->num_branches;

  //allocate space for branchs array/pointer
  rs_copy->branch = malloc(sizeof(branch_s)*(rs_copy->num_branches));  //remember to free this memory
  
  for(int i = 0; i < rs_copy->num_branches; i++)
    {
      copy_branch_s(rs_orig->branch + i, rs_copy->branch + i);
    }
}

void copy_tree_s(tree_s *tree_orig, tree_s *tree_copy)
{
  //copy trunk_s
  copy_trunk_s(&(tree_orig->trunk), &(tree_copy->trunk));
  //copy roots_s
  copy_roots_s(&(tree_orig->roots), &(tree_copy->roots));
  //copy branches_s
  copy_branches_s(&(tree_orig->branches), &(tree_copy->branches));
}

void draw_trunk(double x, double y, double z, 
		double r, double h, 
		trunk_s *trunk, 
		int usecolor, int color, 
		int usetex, unsigned int tex)
{
  if(usecolor && (usetex == 1))
    {
      rounded_cylinder(x,y,z,
		       trunk->r*r,trunk->h*h,
		       0,0,0,
		       color,   //passed in color 
		       1,tex);	//passed in texture    
    }
  else if(usecolor && (usetex == 2))
    {
      rounded_cylinder(x,y,z,
		       trunk->r*r,trunk->h*h,
		       0,0,0,
		       color,                    //passed in color 
		       1,trunk->default_texture); //default texture    
    }
  else if(usecolor) 
    {
      rounded_cylinder(x,y,z,
		       trunk->r*r,trunk->h*h,
		       0,0,0,
		       color, //passed in color 
		       0,0);  //no texture    
    }
  else if(usetex == 1)
    {
      rounded_cylinder(x,y,z,
		       trunk->r*r,trunk->h*h,
		       0,0,0,
		       trunk->color,    //default color                  
		       1,tex);         //passed in texture    
    }
  else if(usetex == 2)
    {
      rounded_cylinder(x,y,z,
		       trunk->r*r,trunk->h*h,
		       0,0,0,
		       trunk->color,               //default color   
		       1,trunk->default_texture);  //default texture    
    }
  else 
    {
      rounded_cylinder(x,y,z,
		       trunk->r*r,trunk->h*h,
		       0,0,0,
		       trunk->color, //default color
		       0,0);        //no texture    
    }
}

void draw_roots(double x, double y, double z, 
		double r, double l, 
		roots_s *roots, 
		int usecolor, int color, 
		int usetex, unsigned int tex)
{
  root_s *root_base = roots->root;
  for(int i = 0; i < (roots->num_roots); i++)
    {
//      fprintf(stdout,"(root_base+%d)->r = %f (root_base + %d) = %p\n",
//	      i, (root_base+i)->r, i, (root_base+i)); 
      if(usecolor && (usetex == 1))
	{
	  cone(x,y,z, 
	       (root_base+i)->r*r, (root_base+i)->l*l, 
	       (root_base+i)->rdeg, (root_base+i)->rx, (root_base+i)->rz,
	       color,    //passed in color     
	       1, tex);  //passed in texture   
	}
      else if(usecolor && (usetex == 2))
	{
	  cone(x,y,z, 
	       (root_base+i)->r*r, (root_base+i)->l*l, 
	       (root_base+i)->rdeg, (root_base+i)->rx, (root_base+i)->rz,
	       color,                               //passed in color      
	       1, (root_base+i)->default_texture);  //default texture    
	}
      else if(usecolor) 
	{
	  cone(x,y,z, 
	       (root_base+i)->r*r, (root_base+i)->l*l, 
	       (root_base+i)->rdeg, (root_base+i)->rx, (root_base+i)->rz,
	       color, //passed in color
	       0, 0); //no texture    
	}
      else if(usetex == 1)
	{ 
	  cone(x,y,z, 
	       (root_base+i)->r*r, (root_base+i)->l*l, 
	       (root_base+i)->rdeg, (root_base+i)->rx, (root_base+i)->rz,
	       (root_base+i)->color, //default color     
	       1, tex);		     //passed in texture          
	}
      else if(usetex == 2)
	{
	  cone(x,y,z, 
	       (root_base+i)->r*r, (root_base+i)->l*l, 
	       (root_base+i)->rdeg, (root_base+i)->rx, (root_base+i)->rz,
	       (root_base+i)->color,               //default color        
	       1, (root_base+i)->default_texture); //default texture     
	}
      else 
	{
	  
	  cone(x,y,z, 
	       (root_base+i)->r*r, (root_base+i)->l*l, 
	       (root_base+i)->rdeg, (root_base+i)->rx, (root_base+i)->rz,
	       (root_base+i)->color, //default color 
	       0, 0);		     //no texture       
	}
    }
}
void draw_branches(double x, double y, double z, 
		   double r, double h, 
		   branches_s *branches, 
		   int usecolor, int color, 
		   int usetex, unsigned int tex)
{
  branch_s *branch_base = branches->branch;
  for(int i = 0; i < (branches->num_branches); i++)
    {
      if(usecolor && (usetex == 1))
	{
	  draw_branch(branch_base+i, x+(branch_base+i)->x_pos,y+(branch_base+i)->y_pos,z+(branch_base+i)->z_pos,
		      (branch_base+i)->r*r,(branch_base+i)->l*h,
		      (branch_base+i)->rdeg,(branch_base+i)->rx,(branch_base+i)->rz,
		      color,   //passed in color 
		      1,tex);	//passed in texture    
	}
      else if(usecolor && (usetex == 2))
	{
	  draw_branch(branch_base+i, x+(branch_base+i)->x_pos,y+(branch_base+i)->y_pos,z+(branch_base+i)->z_pos,
		      (branch_base+i)->r*r,(branch_base+i)->l*h,
		      (branch_base+i)->rdeg,(branch_base+i)->rx,(branch_base+i)->rz,
		      color,                    //passed in color 
		      1,(branch_base+i)->default_texture); //default texture    
	}
      else if(usecolor) 
	{
	  draw_branch(branch_base+i, x+(branch_base+i)->x_pos,y+(branch_base+i)->y_pos,z+(branch_base+i)->z_pos,
		      (branch_base+i)->r*r,(branch_base+i)->l*h,
		      (branch_base+i)->rdeg,(branch_base+i)->rx,(branch_base+i)->rz,
		      color, //passed in color 
		      0,0);  //no texture    
	}
      else if(usetex == 1)
	{
	  draw_branch(branch_base+i, x+(branch_base+i)->x_pos,y+(branch_base+i)->y_pos,z+(branch_base+i)->z_pos,
		      (branch_base+i)->r*r,(branch_base+i)->l*h,
		      (branch_base+i)->rdeg,(branch_base+i)->rx,(branch_base+i)->rz,
		      (branch_base+i)->color,    //default color                  
		      1,tex);         //passed in texture    
	}
      else if(usetex == 2)
	{
	  draw_branch(branch_base+i, x+(branch_base+i)->x_pos,y+(branch_base+i)->y_pos,z+(branch_base+i)->z_pos,
		      (branch_base+i)->r*r,(branch_base+i)->l*h,
		      (branch_base+i)->rdeg,(branch_base+i)->rx,(branch_base+i)->rz,
		      (branch_base+i)->color,               //default color   
		      1,(branch_base+i)->default_texture);  //default texture    
	}
      else 
	{
	  draw_branch(branch_base+i, x+(branch_base+i)->x_pos,y+(branch_base+i)->y_pos,z+(branch_base+i)->z_pos,
		      (branch_base+i)->r*r,(branch_base+i)->l*h,
		      (branch_base+i)->rdeg,(branch_base+i)->rx,(branch_base+i)->rz,
		      (branch_base+i)->color, //default color
		      0,0);        //no texture    
	}
    }
}
  
void draw_tree(double x, double y, double z, double r, double h,
	       tree_s *tree, int usecolor, int color, int usetex, unsigned int *tex)
{
  draw_roots(x, y, z, r, h,
	     &(tree->roots), usecolor, color, usetex, tex[BARK]);
  draw_trunk(x, y, z, r, h, 
	     &(tree->trunk), usecolor, color, usetex, tex[BARK]);
  draw_branches(x,y+h/2,z,r,h,
		&(tree->branches), usecolor, color, usetex, tex[BARK]);
		
}

// Tree randomization done at init so redisplay does not change the values
void tree_init()
{
  srand(time(NULL));
  for(int tree_num = 0; tree_num < NUM_TREES; tree_num++)
    {
      //tree_s  tree = trees[tree_num];
      
      //TRUNK
      //fprintf(stdout,"trees[tree_num] addr = 0x%p\ntrees[tree_num].trunk addr = 0x%p\ntrees[tree_num].roots addr = 0x%p\n,sizeof(trees[tree_num].trunk) = %ld\n",&trees[tree_num], &(trees[tree_num].trunk),&(trees[tree_num].roots), sizeof(trees[tree_num].trunk));
      create_trunk(&(trees[tree_num].trunk));
      fprintf(stdout,"trees[%d] addr = 0x%p\ntrees[%d].trunk addr = 0x%p\n",tree_num,&trees[tree_num],tree_num, &(trees[tree_num].trunk));
      
      //ROOTS
      //randomize number of roots between 3 and 12
      trees[tree_num].roots.num_roots = (rand_tree(tree_num)*(tree_num+1)+tree_num) % 10 + 3;
      
      //fprintf(stdout,"num_roots = %d\n",trees[tree_num].roots.num_roots);

      //allocate space for roots array/pointer
      trees[tree_num].roots.root = malloc(sizeof(root_s)*(trees[tree_num].roots.num_roots));  //remember to free this memory
      //fprintf(stdout,"trees[tree_num] addr = %p\ntrees[tree_num].trunk addr = %p\n",&trees[tree_num], &(trees[tree_num].trunk));
      //fprintf(stdout,"root addr = %p\n",trees[tree_num].roots.root);
      
      //create values for each root
      for(int i = 0; i < trees[tree_num].roots.num_roots; i++) 
	{
	  create_root(trees[tree_num].roots.root + i,i, 1); //randomized for now
	  //fprintf(stdout,"root %d created = %p\n",i,trees[tree_num].roots.root + i);
	  //print_root_s(trees[tree_num].roots.root + i);
	}
      
      //BRANCHES
      //randomize number of branches between 15 and 25
      trees[tree_num].branches.num_branches = 15+ ((rand_tree(tree_num)*(tree_num+1)+tree_num) % 11);
      
      //allocate space for branches array/pointer
      trees[tree_num].branches.branch = malloc(sizeof(branch_s)*(trees[tree_num].branches.num_branches));  //remember to free this memory
      //fprintf(stdout,"trees[tree_num].branches.branch addr = %p\n",trees[tree_num].branches.branch);
      
      //create values for each branch
      for(int i = 0; i < trees[tree_num].branches.num_branches; i++) 
	{
	  create_branch(trees[tree_num].branches.branch + i, i, 0); //don't randomize for now
	  //fprintf(stdout,"trees[tree_num].branches.branch %d created = %p\n",i,trees[tree_num].branches.branch + i);
	  //print_branch_s(trees[tree_num].branches.branch + i);
	}      
      
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
       gluLookAt(Ex,Ey,Ez , 0,1,0 , 0,Cos(ph),0);
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
   cube(0,UNIT_SIZE*-.01,0,
        UNIT_SIZE*2.0,UNIT_SIZE*.02,UNIT_SIZE*2.0,
        0);
//   cat(UNIT_SIZE*.2, 0, UNIT_SIZE*.2, 45, LARGE, WHITE_CAT);
//   cat(UNIT_SIZE*.2, 0, -UNIT_SIZE*.4, 135, SMALL, BROWN_CAT);
//   cat(UNIT_SIZE*.4, 0, -UNIT_SIZE*.2, -90, REGULAR, BLACK_CAT);
//   cat(-UNIT_SIZE*.2, 0, UNIT_SIZE*.4, 0, mc.body_type, MIXED);
//
//
//   //bottom circle
//   circle(0,0,0,
//	 1,
//	 0,0,
//	 MAGENTA,
//	 1, texture[0]);
//
//   half_sphere(0,1.2,0,
//	       0.1,
//	       0,0,0,
//	       DORANGE,
//	       0,texture);
//   rounded_cylinder(-.1,.2,0,
//		    0.01,.4,
//		    0,0,0,
//		    LBROWN,
//		    1,tree_texture[0]);
//   cylinder(0,.2,0,
//		    0.01,.4,
//		    0,0,
//		    LBROWN,
//		    1,tree_texture[0]);
//
//   number(0,-.5,.6,.4,
//	  0.01,.4,
//	  0,0,
//	  PINK,
//	  0,texture);
//   number(1,-.3,.6,.4,
//	  0.01,.4,
//	  0,0,
//	  PINK,
//	  0,texture);
//   number(2,-.1,.6,.4,
//	  0.01,.4,
//	  0,0,
//	  PINK,
//	  0,texture);
//   number(3,.1,.6,.4,
//	  0.01,.4,
//	  0,0,
//	  ORANGE,
//	  0,texture);
//   letter('A',.4,.6,-.4,
//	  0.01,.4,
//	  0,0,
//	  PURPLE,
//	  0,texture);
//   letter('B',.2,.6,-.4,
//	  0.01,.4,
//	  0,0,
//	  RED,
//	  0,texture);
//   letter('C',.0,.6,-.4,
//	  0.01,.4,
//	  0,0,
//	  LBLUE,
//	  0,texture);
////   letter('D',-.2,.6,-.4,
////	  0.01,.4,
////	  0,0,
////	  DPURPLE,
////	  0,0);
//   draw_leaf(0,.2,0,
//	0.3,
//	0,0,
//	DYELLOW,
//	1,tree_texture[LEAF]);
//   tree(0,0,0,
//	.75,1.5,
//	0,
//	DBROWN,DGREEN,
//	0,texture);
//   cone(-.3, .3, .3, .1, .4, 100, 1,1, 
//   	BROWN,1,texture[BARK]);

   draw_tree(0,0,0,1.0,2.0,
	     trees+0,0,0,2,tree_texture);
   draw_tree(0.6,0,.6,1.0,2.0,
	     trees+1,0,0,2,tree_texture);
//   draw_tree(.6,0,-.6,.75,2.5,
//	     trees+0,0,0,2,tree_texture);
//   draw_tree(.5,0,0,.75,2.0,
//	     trees+1,0,0,2,tree_texture);
     
//   cone(.3, .3, .3, .1, .4, 100, 1,1, 
//   	YELLOW,1,texture[LEAF]);

//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //I still can see edges in my vbox linux - not sure what others can see.
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   skycube(UNIT_SIZE*0.0,UNIT_SIZE*0.75,UNIT_SIZE*0.0,
	   UNIT_SIZE*4.0,UNIT_SIZE*4.0,UNIT_SIZE*4.0,
	   0,
	   skybox_texture);

   

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


   glWindowPos2i(5,45);
   Print("Model=%s LocalViewer=%s Distance=%.1f Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
   if (light)
   {
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }

   ErrCheck("display");
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

  //Pause Parameter
  p = 0;

  //th,ph
  th = -40;
  ph = 15;

  //Randomize mixed cat
  rand_mc();
  
  //project_mode 
  project_mode = PERSPECTIVE;

  //field_of_view
  field_of_view = 75;

  //first person x,y,z,th,ph
  fp_x = 0.0;
  fp_y = 0.1;
  fp_z = 0.0;
  fp_th = 0;
  fp_ph = 0;  

  //Lighting
  light=1;      //  Lighting
  one       =   1;  // Unit value
  distance  =   1;  // Light distance
  inc       =   15;  // Ball increment
  smooth    =   1;  // Smooth/Flat shading
  local     =   0;  // Local Viewer Model
  emission  =  10;  // Emission intensity (%)
  ambient   =  30;  // Ambient intensity (%)
  diffuse   = 100;  // Diffuse intensity (%)
  specular  =   0;  // Specular intensity (%)
  shininess =   0;  // Shininess (power of two)
  shiny   =   1;  // Shininess (value)
  ylight  =   0;  // Elevation of light

  init_color();

  tree_init();
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
      gluPerspective(field_of_view, w2h*dim,.1f,UNIT_SIZE*10);
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
   else if (ch == 'p')
     {
       if (p == 1)
	 p = 0;
       else 
	 p = 1;
     }
   else if (ch == '1')
     {
       if(project_mode == 2)
	 project_mode = 1;
       else
	 project_mode += 1;       
     }
   else if (ch == 'f' && field_of_view <100)
     field_of_view += 1;
   else if (ch == 'F' && field_of_view >1)
     field_of_view -= 1;
   //  Toggle textures mode
   else if (ch == 't')
      ntex = 1-ntex;
   //  Toggle texture mode
   else if (ch == 'T')
      texture_mode = 1-texture_mode;
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
   glutCreateWindow("HW6 Textures - CSCI5229 - Kathy Grimes");
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
   
   
   //  Load textures
   skybox_texture[C_FRONT]  =LoadTexBMP("hills_ft.bmp");
   skybox_texture[C_LEFT]   =LoadTexBMP("hills_lf.bmp");
   skybox_texture[C_BACK]   =LoadTexBMP("hills_bk.bmp");
   skybox_texture[C_RIGHT]  =LoadTexBMP("hills_rt.bmp");
   skybox_texture[C_TOP]    =LoadTexBMP("hills_up.bmp");
   skybox_texture[C_BOTTOM] =LoadTexBMP("hills_dn.bmp"); 
   crate_texture     = LoadTexBMP("Alphabet_Main.bmp");//LoadTexBMP("crate.bmp");
   moon_texture      = LoadTexBMP("moon.bmp");
   texture[C_SAME]   = LoadTexBMP("gray-fur-texture.bmp");
   texture[C_FRONT]  = LoadTexBMP("softfur.bmp");//LoadTexBMP("cat_fur.bmp");
   texture[C_LEFT]   = texture[C_FRONT];
   texture[C_BACK]   = texture[C_FRONT];
   texture[C_RIGHT]  = texture[C_FRONT];
   texture[C_TOP]    = texture[C_FRONT];
   texture[C_BOTTOM] = texture[C_FRONT];
   texture[7+C_FRONT]  = LoadTexBMP("mrow.bmp");
   texture[7+C_LEFT]   = texture[7+C_FRONT];
   texture[7+C_BACK]   = texture[7+C_FRONT];
   texture[7+C_RIGHT]  = texture[7+C_FRONT];
   texture[7+C_TOP]    = LoadTexBMP("clawmark.bmp");
   texture[7+C_BOTTOM] = texture[7+C_TOP];
   tree_texture[BARK]  = LoadTexBMP("tree_bark.bmp");
   tree_texture[ROOT]  = LoadTexBMP("tree_bark_32.bmp");
   tree_texture[LEAF]  = LoadTexBMP("leaf.bmp");
 

   // Use inital state parameters
   initialState();
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}


