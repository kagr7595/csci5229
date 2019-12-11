/*
 *  Project: Count Learning Activity for my Niece
 *  Course: CSCI5229
 *  Author: Kathy Grimes
 *  Description:  This program is built upon all the previous hwk
 *                 and examples in class as well as using the opengl
 *                 online resources for formulae/ideas/inspiration
 *
 *                 Although not One of the main things taught in this course,
 *                 I relearned pointers so that I could create a more realistic
 *                 tree (not a simple pine  tree) that could be randomized to 
 *                 be different each run.  I was even ready to use mutliple random
 *                 trees but had to give up that idea.
 *
 *                 The program starts with the tree as an added element, 
 *                 however, due to the number and types of objects being 
 *                 rendered, it was very slow to use with the learning 
 *                 activity so I remove it when I start the game until I 
 *                 have all the CULL FACEs fixed.  I was not able to finish 
 *                 fixing all of them so I commented them out in my code.
 *
 *                 The starting screen as informative lighting elements on 
 *                 the screen to make it easier to understand the starting mode.  
 *                  As in previous assignments, I left these special keys to change 
 *                 the various lighting parameters in and have found some 
 *                 interesting looking environments.
 *
 *                 Press <Enter> to progress to the learning/counting activity 
 *                 (which is only one activity as it took me much longer 
 *                 than I thought to make my tree)
 *
 *                 The most important keys are <Enter>, Up/Down Arrows 
 *                 (for counting the objects during the activity), 
 *                 and Left/Right Arrows to change the view angle(th).
 *                 To progress you must answer correctly otherwise you will be 
 *                 returned to 'try again'
 *
 *                 Answer is: 7 apples, 4 balls, 3 cats.
 *
 *                 <Esc> to exit
 *
 *                 I tried to make it look like the three cats owned the 
 *                 three houses and they each had their own color coded ball 
 *                 and they liked to eat apples ;)
 *
 *                 The one portion of the project I was unable to get working 
 *                 was the object select drag feature.  I was trying to implement
 *                 based off of color and making all the objects a 'unique' color 
 *                 but found that this wouldn't work for what I wanted to use it 
 *                 for and I needed to follow a different path that I wasn't able 
 *                 to implement in time.  Due to not having the object selection I 
 *                 revised my activities so that I could use the majority of the pieces 
 *                 I had created for the color/alphabet in the counting activity
 * 
 *
 *    Please use the following keys to manipulate the program during run
 *              in Perspective...
 *                 o: overhead view (y-axis view)
 *               f/F: change field_of_view
 *              in all views
 * Left/Right Arrows: move angles of th
 *    Up/Down Arrows: when menu overlay in effect, up/down arrows are to control counting of objects in game
 *                  : when old style text on screen (game not occurring) move angles of ph 
 *                 p: pause light ball
 *                 x: toggle axis
 *                 i: go back to initial state values (in case you get off want to start the game over)
 *     Lighting
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
 *     Textures
 *                 t: Toggle between two sets of textures for the light orb/moon orb
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
int th;   //  Azimuth of view angle   
int ph;   //  Elevation of view angle 
int zh=0;         //  For the flying cat and light ball
int axes=0;       //  Display axes
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
double asp = 1;

/****************************************************************
 *****************GAME SCENE VARIABLES***************************
 ***************************************************************/
#define FIRST 1
#define SECOND 2
#define THIRD 3
#define FOURTH 4
#define APPLE 1
#define BALL 2
#define CAT 3
#define CORRECT 4   
#define TRYAGAIN 5
#define GREATJOB 6
int scene_count1_on = APPLE;
int apple_count = 0;
int ball_count = 0;
int cat_count = 0;
int num_cat = 3;
int num_ball = 4;
int num_apple = 7;
/****************************************************************
 *****************END GAME SCENE VARIABLES***********************
 ***************************************************************/


/* Lighting */
int light=1;      //  Lighting
int one       =   1;  // Unit value
float distance  = 1.5;// Light distance
int inc       =   15; // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =  40;  // Emission intensity (%)
int ambient   =  90;  // Ambient intensity (%)
int diffuse   =  30;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
float ylight  =   0;  // Elevation of light


/* Texture defines */
#define C_RIGHT 0
#define C_LEFT  1
#define C_TOP 2
#define C_BOTTOM 3
#define C_BACK 4
#define C_FRONT 5
#define C_SAME 0

/* Texture */
int texture_mode=0;       //  Texture mode
int ntex=0;               //  Cube faces

unsigned int blank_texture;
unsigned int skybox_texture[6]; //skybox texture
unsigned int crate_texture[1];
unsigned int moon_texture[1];
unsigned int catfur_texture[1];
unsigned int catface_texture[1];
unsigned int grass_texture[1];
unsigned int apple_texture[1];
unsigned int ball_texture[1]; 

#define M_COUNT   0
#define M_COUNT_Q 1
#define M_COLOR   2
#define M_COLOR_Q 3
#define M_FDBK    4
#define M_NUM     5
unsigned int menu_texture[M_NUM];

#define WALL    0
#define ROOF    1
#define DOOR    2  
#define STEP    3
#define WINDOW  4 
#define CHIMNEY 5
#define NUM_HOUSE_TEXTURES 6
unsigned int house_texture[NUM_HOUSE_TEXTURES];

#define BARK 0
#define ROOT 1
#define LEAF 2
#define NUM_TREE_TEXTURES 3
unsigned int tree_texture[NUM_TREE_TEXTURES]; //Bark, Root,  and Leaf


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

#define NUM_TREES 1
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

/******************************************************************************************
 *********************************MATH/FORMULA FUNCTION CODE*******************************
 ******************************************************************************************/
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
  color_ret[DGREEN ] = (color_s){.r = 0, .g = .5, .b = .1};
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


/* Get random value between two values */
int rand_b2i(int lower, int upper) 
{
  return ((rand() % (upper-lower + 1)) + lower);
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
 *  Draw vertex in polar coordinates 
 */
static void Vertex(int th,int ph, int color)
{
   double x = Cos(th)*Cos(ph);
   double y = Sin(th)*Cos(ph);
   double z =         Sin(ph);
   if(color != -1) {
     glColor3f(color_ret[color].r, color_ret[color].g, color_ret[color].b);
   }
   glNormal3d(x,y,z);
   glTexCoord2d(th/360.0,ph/180.0+0.5);
   glVertex3d(x,y,z);
}

/******************************************************************************************
 *********************************END MATH/FORMULA FUNCTION CODE**************************
 ******************************************************************************************/


/******************************************************************************************
 *********************************BUILDING BLOCKS CODE*************************************
 ******************************************************************************************/

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

   //glCullFace(GL_FRONT);
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
   if(ntex)
     glBindTexture(GL_TEXTURE_2D,moon_texture[C_SAME]);
   else     
     glBindTexture(GL_TEXTURE_2D,blank_texture);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
	Vertex(th,    ph,WHITE);
	Vertex(th,ph+inc,WHITE);
      }
      glEnd();
   }
   //  Undo transofrmations and texture
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   //glCullFace(GL_BACK);
}
/*
 *  Draw a basic sphere
 *     at (x,y,z)
 *     radius (r)
 *  Taken mostly from Ex 13
 */
static void sphere(double x,double y,double z,double r, int rotatedeg, int color, int usetex, unsigned int tex)
{
   int th,ph;

   //glCullFace(GL_FRONT);
   //glDisable(GL_CULL_FACE);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(rotatedeg,1,0,0);
   glScaled(r,r,r);
   
   glColor3f(color_ret[color].r, color_ret[color].g, color_ret[color].b);
   //  Set texture
   glEnable(GL_TEXTURE_2D);
   if(usetex) glBindTexture(GL_TEXTURE_2D,tex);
   else
     glBindTexture(GL_TEXTURE_2D,blank_texture);

   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
	 Vertex(th,    ph,color);
	 Vertex(th,ph+inc,color);
      }
      glEnd();
   }
   //  Undo transofrmations and texture
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   //glEnable(GL_CULL_FACE);
   //glCullFace(GL_BACK);
}

/*
 *  Draw a cat eye sphere
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
   Vertex(0,-90,BLACK);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,d-90,GREEN);
   }
   glEnd();
   //  South pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,90,BLACK);
   for (th=0;th<=360;th+=d)
   {
      Vertex(th,90-d,GREEN);
   }
   glEnd();
  
   //  Latitude bands
   ph=d-90;  
   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=d)
     {
       Vertex(th,ph,GREEN);
       Vertex(th,ph+d,GREEN);
     }
   glEnd();
   
   ph+=4*d;
   
   ph+=4*d;
   glBegin(GL_QUAD_STRIP);
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
 *  Draw a half sphere
 *     at (x,y,z)
 *     radius (r)
 *  Taken directly from Ex 13
 */
static void half_sphere(double x,double y,double z,double r, int rotatex, int rotatey, int rotatez,
			int color, int usetex, unsigned int tex)
{
   int th,ph;

   ////glCullFace(GL_FRONT);
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
   } else
     glBindTexture(GL_TEXTURE_2D,blank_texture);
   //  Bands of latitude
   for (ph=-90;ph<0;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
	Vertex(th,    ph,color);
	Vertex(th,ph+inc,color);
      }
      glEnd();
   }
   //  Undo transofrmations and texture
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   ////glCullFace(GL_BACK);
}

//Basic circle
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
   } else
     glBindTexture(GL_TEXTURE_2D, blank_texture);
   if(color != -1) {
     glColor3f(color_ret[color].r, color_ret[color].g, color_ret[color].b);
   }
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,0,color);
   glTexCoord2f(0.5, 0.5);
   for (int th=0; th<=360; th+=5) {
     Vertex(th,0,color);
   }
   glEnd();
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
   } else
     glBindTexture(GL_TEXTURE_2D, blank_texture);
   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=5)
     {
       Vertex(th, 0,color);
       Vertex(th,90,color);
     }
   glEnd();

   //  Undo transofrmations and texture
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}


/* flared edge cylinder with h height and r radius */
void flared_cylinder(double x,double y,double z,double r, double h, int rotatex,int rotatez, 
		     int color, int usetex, unsigned int tex)
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
   glBindTexture(GL_TEXTURE_2D,blank_texture);
   if(usetex) {
     glBindTexture(GL_TEXTURE_2D,tex);
   }  else
     glBindTexture(GL_TEXTURE_2D, blank_texture);
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
   } else
     glBindTexture(GL_TEXTURE_2D, blank_texture);
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
	    usetex,tex);
   //bottom sphere
   half_sphere(0,0+h,0,
	       r,
	       180,0,0,
	       color,
	       usetex, tex);
   
   //  Undo transofrmations and texture
   glPopMatrix();
}

void triangular_prism(double x, double y, double z, 
		      double dx,double dy,double dz,
		      int color, int rotate, int usetex, unsigned int tex, int cat_special)  
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
   if(usetex) glBindTexture(GL_TEXTURE_2D, tex);
   else
     glBindTexture(GL_TEXTURE_2D, blank_texture);

   if(color != -1) {
     glColor3f(color_ret[color].r, color_ret[color].g, color_ret[color].b);
   }
   glBegin(GL_QUADS);
   //Be sure to rotate through vertices in correct order to get the correct side normalized
   //Bottom
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
   glTexCoord2f(0,1); glVertex3f(-1,0,-1);
   glTexCoord2f(1,1); glVertex3f(-1,2,-1);
   glTexCoord2f(1,0); glVertex3f(1,2,-1);
   glTexCoord2f(0,0); glVertex3f(1,0,-1);
   
   quad_triangle_normal(-1,2,-1,
			-1,0,1,
			1,0,1);
   glTexCoord2f(0,0); glVertex3f(1,2,-1);
   glTexCoord2f(1,0); glVertex3f(-1,2,-1);
   glTexCoord2f(1,1); glVertex3f(-1,0,1);
   glTexCoord2f(0,1); glVertex3f(1,0,1);
   glEnd();


   glBegin(GL_TRIANGLES);
   if(rotate == -90) {
     glNormal3f(-1, 0, 0);
     glTexCoord2f(1,0); glVertex3f(-1,0,1);
     glTexCoord2f(0,1); glVertex3f(-1,0,-1);
     glTexCoord2f(1,1); glVertex3f(-1,2,-1);
     glEnd();
     glBegin(GL_TRIANGLES);
     if(cat_special)
       //one side of ear needs to be pink
       glColor3f(0.737255,0.560784,0.560784);
     glNormal3f( 1, 0, 0);
     glTexCoord2f(0,0); glVertex3f(1,0,1);
     glTexCoord2f(1,0); glVertex3f(1,2,-1);
     glTexCoord2f(1,1); glVertex3f(1,0,-1);
   } else {
     glNormal3f( 1, 0, 0);
     glTexCoord2f(0,0); glVertex3f(1,0,1);
     glTexCoord2f(1,1); glVertex3f(1,0,-1);
     glTexCoord2f(1,0); glVertex3f(1,2,-1);
     glEnd();
     glBegin(GL_TRIANGLES);
     if(cat_special)
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


/*
 *  Draw rectangular prisms 
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
void one_color_cube(double x,double y,double z,
		    double dx,double dy,double dz,
		    int color, int usetex, unsigned int tex)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glScaled(dx,dy,dz);

   //KG new for textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, texture_mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   if(usetex)
     glBindTexture(GL_TEXTURE_2D, tex);
   else
     glBindTexture(GL_TEXTURE_2D, blank_texture);

   if(color != -1) {
     glColor3f(color_ret[color].r, color_ret[color].g, color_ret[color].b);
   }
   
   //  Cube
   //  Front
   glBegin(GL_QUADS);   
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
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
/******************************************************************************************
 *********************************END BUILDING BLOCKS CODE*********************************
 ******************************************************************************************/

/******************************************************************************************
 *********************************BASIC OBJECTS CODE***************************************
 ******************************************************************************************/
/* Basic Background House - Cube plus triangular prism roof
 */
void draw_basic_house(double x,double y,double z,double dx, double dy, double dz, int rotatedeg,  
		      int color, int usetex, unsigned int *tex)
{ 
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(0,dy/2,0);
   glRotated(rotatedeg,0,1,0);
   
   //triangle roof with trim
   triangular_prism( x,y+dy/2,z+1.1*dz/2,1.1*dx,dy/3,1.1*dz/2,BROWN,0, usetex,tex[ROOF],0);
   triangular_prism( x,y+dy/2,z-1.1*dz/2,1.1*dx,dy/3,1.1*dz/2,BROWN,180, usetex,tex[ROOF],0);
   one_color_cube(x, y+dy/2, z, 1.12*dx, dy/24.0, 1.1*dz, DBROWN, usetex, tex[ROOF]);
   //square house
   //cube(x, y, z, dx, dy/2, dz, 0);
   one_color_cube(x, y, z, dx, dy/2.0, dz, color, usetex, tex[WALL]);
   //chimney
   one_color_cube(x+dx/2.0, y+2.0*dy/3.0, z+2.0*dz/3.0, dx/8.0, dy/2.0, dz/8, DBROWN, usetex, tex[CHIMNEY]);
   //door
   one_color_cube(x+dx, y-dy/6.0, z-dz/2.0, dx/16.0, dy/3.5, dz/4.0, WHITE, usetex, tex[DOOR]);
   //front door step
   one_color_cube(x+dx, y-dy/2.0, z-dz/2.0, dx/4.0, dy/32, dz/4.0, BROWN, usetex, tex[STEP]);
   //front window
   one_color_cube(x+dx, y-dy/7.0, z+dz/2.0, dx/16.0, dy/7.0, dz/4.0, WHITE, usetex, tex[WINDOW]);
   //left side window 1
   one_color_cube(x-dx/2.0, y-dy/7.0, z+dz, dx/4.0, dy/7.0, dz/16.0, WHITE, usetex, tex[WINDOW]);
   //left side window 2
   one_color_cube(x+dx/2.0, y-dy/7.0, z+dz, dx/4.0, dy/7.0, dz/16.0, WHITE, usetex, tex[WINDOW]);
   //right side window 1
   one_color_cube(x-dx/2.0, y-dy/7.0, z-dz, dx/4.0, dy/7.0, dz/16.0, WHITE, usetex, tex[WINDOW]);
   //right side window 2
   one_color_cube(x+dx/2.0, y-dy/7.0, z-dz, dx/4.0, dy/7.0, dz/16.0, WHITE, usetex, tex[WINDOW]);

   //  Undo transformations and textures
   glPopMatrix();
   
}

void draw_box(double x, double y, double z, double s, int rotatedeg, int color, int usetex, unsigned int tex)
{
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(0,s,0);
   glRotated(rotatedeg,0,1,0);
   one_color_cube(x, y, z, s, s, s, color, usetex, tex);
   //  Undo transformations and textures
   glPopMatrix();
}

void draw_apple(double x, double y, double z, double r, double h, int rotatedeg, 
		int color, int usetex, unsigned int *tex)
{
  //glDisable(GL_CULL_FACE);
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(rotatedeg,0,1,0);
   glScaled(r,h,r);

   rounded_cylinder(0,.8,0,
		    .1,.5,
		    15,1,0,
		    BROWN,
		    1,tree_texture[BARK]);
   sphere(0,0,0,1,-90,color,usetex,tex[0]);
   
   //  Undo transofrmations and texture
   glPopMatrix();   
   //glEnable(GL_CULL_FACE);
}
/******************************************************************************************
 *********************************END BASIC OBJECTS CODE***********************************
 ******************************************************************************************/


/******************************************************************************************
 *********************************LETTER/NUMBER CODE***************************************
 ******************************************************************************************/
void number(int number, double x,double y,double z,double r, double h, int rotatedeg, 
	    int color, int usetex, unsigned int tex)
{
  //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(rotatedeg,0,1,0);
   
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

void letter(char letter, double x,double y,double z,double r, double h, int rotatedeg, 
	    int color, int usetex, unsigned int tex)
{
  //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(rotatedeg,0,1,0);
   
  
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
/******************************************************************************************
 *********************************END LETTER/NUMBER CODE***********************************
 ******************************************************************************************/

/******************************************************************************************
 *********************************CAT CODE ************************************************
 ******************************************************************************************/

void legs(double x, double y, double z, int color, int color_brightness, int usetex, unsigned int tex)
{  
  double height = y+UNIT_SIZE*.02;
  double leg_spacing = x+UNIT_SIZE*.015;
  double torso_spacing = z+UNIT_SIZE*.03;
  int leg_color = convert_cat_color(color, color_brightness);
  if(color == MIXED) {
    //back left
    leg_color = convert_cat_color(mc.leg_color[0], mc.leg_brightness[0]);
    rounded_cylinder(-leg_spacing,height/2,-torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, usetex,tex);	
    //back right					
    leg_color = convert_cat_color(mc.leg_color[1], mc.leg_brightness[1]);		    				
    rounded_cylinder(leg_spacing,height/2,-torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, usetex,tex);	    
    //front left					
    leg_color = convert_cat_color(mc.leg_color[2], mc.leg_brightness[2]);		    			
    rounded_cylinder(leg_spacing,height/2,torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, usetex,tex);		 
    //front right			
    leg_color = convert_cat_color(mc.leg_color[3], mc.leg_brightness[3]);			    	
    rounded_cylinder(-leg_spacing,height/2,torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, usetex,tex);
  }else {
    //back left
    rounded_cylinder(-leg_spacing,height/2,-torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, usetex,tex);
    //back right							    				
    rounded_cylinder(leg_spacing,height/2,-torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, usetex,tex);	    
    //front left							    			
    rounded_cylinder(leg_spacing,height/2,torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, usetex,tex);		 
    //front right						    	
    rounded_cylinder(-leg_spacing,height/2,torso_spacing, UNIT_SIZE*.0075, UNIT_SIZE*.02, 0,0,0,leg_color, usetex,tex);			
  }
}
void torso(double x, double y, double z, int color, int color_brightness, int usetex, unsigned int tex)
{  
  double torso_height = UNIT_SIZE*.02;
  double torso_width = UNIT_SIZE*.018;
  double torso_length = UNIT_SIZE*.04;
  double height = y+UNIT_SIZE*.02+torso_height;
  int torso_color = convert_cat_color(color, color_brightness);
  if(color == MIXED) 
    torso_color = convert_cat_color(mc.torso_color, mc.torso_brightness);

  one_color_cube(x,height,z, torso_width, torso_height, torso_length, torso_color, usetex, tex);
}

void head(double x, double y, double z, int color, int color_brightness, int usetex, unsigned int tex)
{  
  double head_height = UNIT_SIZE*.015;
  double head_width = UNIT_SIZE*.015;
  double head_length = UNIT_SIZE*.015;
  double height = y+UNIT_SIZE*.05+head_height;
  double torso_front = z+UNIT_SIZE*.055;
  int head_color = convert_cat_color(color, color_brightness);
  if(color == MIXED)
    head_color = convert_cat_color(mc.head_color, mc.head_brightness);
  
  one_color_cube(x,height,torso_front, head_width, head_height, head_length, head_color, usetex, tex);
  
}

void tail(double x, double y, double z, int color, int color_brightness, int usetex, unsigned int tex)
{  

  int tail_color = convert_cat_color(color, color_brightness);
  if(color == MIXED)
    tail_color = convert_cat_color(mc.tail_color, mc.tail_brightness);
 
  rounded_cylinder(x,y+UNIT_SIZE*.05+UNIT_SIZE*.01,z-UNIT_SIZE*.04, UNIT_SIZE*.005, UNIT_SIZE*.06,-75,1,0,tail_color, usetex, tex);
}

void ears(double x, double y, double z, int color, int color_brightness, int usetex, unsigned int tex)
{
  double ear_height = UNIT_SIZE*.008;
  double ear_width = UNIT_SIZE*.004;
  double ear_length = UNIT_SIZE*.006;
  double ear_spacing = UNIT_SIZE*.008;
  double head_front = z+UNIT_SIZE*.065;
  double height = y+UNIT_SIZE*.065+ear_height;
  int ear_color0 = convert_cat_color(color, color_brightness);
  int ear_color1 = ear_color0;
  if(color == MIXED) {
    ear_color0 = convert_cat_color(mc.ear_color[0],mc.ear_brightness[0]);
    ear_color1 = convert_cat_color(mc.ear_color[1],mc.ear_brightness[1]);
  }
 
  triangular_prism( ear_spacing,height,head_front,ear_width,ear_height,ear_length,ear_color0,-90, usetex,tex,1);
  triangular_prism(-ear_spacing,height,head_front,ear_width,ear_height,ear_length,ear_color1, 90, usetex,tex,1);  
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
	 int body_type, int color, int usetex, unsigned int tex)
{

  //glDisable(GL_CULL_FACE);
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

   if(color == 0)
     {
       legs(0,0,0, color, MIXED, usetex,tex);
       torso(0,0,0, color, MIXED, usetex,tex);
       head(0,0,0, color, MIXED, usetex,tex);
       tail(0,0,0, color, MIXED, usetex,tex);
       ears(0,0,0, color, MIXED, usetex,tex);
     }
   else
     {
       legs(0,0,0, color, NORMAL, usetex,tex);
       torso(0,0,0, color, DARK, usetex,tex);
       head(0,0,0, color, NORMAL, usetex,tex);
       tail(0,0,0, color, NORMAL, usetex,tex);
       ears(0,0,0, color, LIGHT, usetex,tex);
     }
   eyes(0,0,0);
   

   //  Undo transformations
   glPopMatrix();
   // glEnable(GL_CULL_FACE);
  
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


/******************************************************************************************
 *********************************END CAT CODE*********************************************
 ******************************************************************************************/



/******************************************************************************************
 *********************************TREE CODE ***********************************************
 ******************************************************************************************/
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
  //  Undo transofrmations and texture
  glPopMatrix();
}


unsigned int rand_tree()
{
  current_rand++;
  return ((rand()*current_rand + (current_rand*3)%2));
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
  rand_rdiv = rand_root ? rand_tree() % 2 + 16.0 : 16.0;
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
  subbranch->color = BROWN;
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
  //glDisable(GL_CULL_FACE);
  draw_roots(x, y, z, r, h,
	     &(tree->roots), usecolor, color, usetex, tex[BARK]);
  draw_trunk(x, y, z, r, h, 
	     &(tree->trunk), usecolor, color, usetex, tex[BARK]);
  draw_branches(x,y+h/2,z,r,h,
		&(tree->branches), usecolor, color, usetex, tex[BARK]);
  //glEnable(GL_CULL_FACE);
		
}

// Tree randomization done at init so redisplay does not change the values
void tree_init()
{
  srand(time(NULL));
  for(int tree_num = 0; tree_num < NUM_TREES; tree_num++)
    {
      //tree_s  tree = trees[tree_num];
      
      //TRUNK
      create_trunk(&(trees[tree_num].trunk));
      
      //ROOTS
      //randomize number of roots between 3 and 12
      trees[tree_num].roots.num_roots = (rand_tree(tree_num)*(tree_num+1)+tree_num) % 10 + 3;
      
      //fprintf(stdout,"num_roots = %d\n",trees[tree_num].roots.num_roots);

      //allocate space for roots array/pointer
      trees[tree_num].roots.root = malloc(sizeof(root_s)*(trees[tree_num].roots.num_roots));  //remember to free this memory
      
      //create values for each root
      for(int i = 0; i < trees[tree_num].roots.num_roots; i++) 
	{
	  create_root(trees[tree_num].roots.root + i,i, 1); //randomized for now
	}
      
      //BRANCHES
      //randomize number of branches between 15 and 25
      trees[tree_num].branches.num_branches = 15+ ((rand_tree(tree_num)*(tree_num+1)+tree_num) % 11);
      
      //allocate space for branches array/pointer
      trees[tree_num].branches.branch = malloc(sizeof(branch_s)*(trees[tree_num].branches.num_branches));  //remember to free this memory
      
      //create values for each branch
      for(int i = 0; i < trees[tree_num].branches.num_branches; i++) 
	{
	  create_branch(trees[tree_num].branches.branch + i, i, 0); //don't randomize for now
	}      
      
    }
}
/******************************************************************************************
 *********************************END TREE CODE********************************************
 ******************************************************************************************/

/******************************************************************************************
 *********************************MENU DISPLAY CODE****************************************
 ******************************************************************************************/

//Send tex bind coordinates based on which section I am trying to access.
double ret_tex_pos(int tex_pos, int tex_coor)
{
  if(tex_pos == 1 || tex_pos== 4) 
    {
      if(tex_coor == FIRST)
	return 0;
      else if(tex_coor == SECOND)
	return 1;
      else if(tex_coor == THIRD)
	return .65;
      else if(tex_coor == FOURTH)
	return .95;
    }
  else if(tex_pos == 2) 
    {
      if(tex_coor == FIRST)
	return 0;
      else if(tex_coor == SECOND)
	return 1;
      else if(tex_coor == THIRD)
	return .36;
      else if(tex_coor == FOURTH)
	return .66;
    }
  else if(tex_pos == 3) 
    {
      if(tex_coor == FIRST)
	return 0;
      else if(tex_coor == SECOND)
	return 1;
      else if(tex_coor == THIRD)
	return .06;
      else if(tex_coor == FOURTH)
	return .36;
    }
  else if(tex_pos== 4) 
    {
      if(tex_coor == FIRST)
	return 0;
      else if(tex_coor == SECOND)
	return 1;
      else if(tex_coor == THIRD)
	return .69;
      else if(tex_coor == FOURTH)
	return .99;
    }
  else if(tex_pos== 5) 
    {
      if(tex_coor == FIRST)
	return 0;
      else if(tex_coor == SECOND)
	return 1;
      else if(tex_coor == THIRD)
	return .39;
      else if(tex_coor == FOURTH)
	return .69;
    }
  else if(tex_pos == 3 || tex_pos== 6) 
    {
      if(tex_coor == FIRST)
	return 0;
      else if(tex_coor == SECOND)
	return 1;
      else if(tex_coor == THIRD)
	return .09;
      else if(tex_coor == FOURTH)
	return .39;
    }
  return 0;

}


/*
 *  Draw the bottom menu 
 *  
 */
void menu_bottom(unsigned int tex, int tex_pos)
{
   double x_b = ret_tex_pos(tex_pos, FIRST); 
   double x_t = ret_tex_pos(tex_pos, SECOND); 
   double y_b = ret_tex_pos(tex_pos, THIRD); 
   double y_t = ret_tex_pos(tex_pos, FOURTH); 

   //  Draw Question area
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, tex);
   glBegin(GL_QUADS);
   glTexCoord2d(x_b,y_b);glVertex2f(-1,-1);
   glTexCoord2d(x_t,y_b);glVertex2f(+1,-1);
   glTexCoord2d(x_t,y_t);glVertex2f(+1,-.75);
   glTexCoord2d(x_b,y_t);glVertex2f(-1,-.75);
   glEnd();
   glDisable(GL_TEXTURE_2D);

}


/*
 *  Draw the menu as an overlay
 *  Must be called last
 */
void menu_overlay()
{
  int color_a = BLACK;
  int color_b = BLACK;
  int color_c = BLACK;
  if(scene_count1_on == APPLE) color_a = PINK;
  else if(scene_count1_on == BALL) color_b = BLUE;
  else if(scene_count1_on == CAT) color_c = GREY;
   //  Save transform attributes (Matrix Mode and Enabled Modes)
   glPushAttrib(GL_TRANSFORM_BIT|GL_ENABLE_BIT);
   //  Save projection matrix and set unit transform
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   glOrtho(-asp,+asp,-1,1,-1,1);
   //  Save model view matrix and set to indentity
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();

   if(scene_count1_on > CAT)
     menu_bottom(menu_texture[M_FDBK],scene_count1_on);
   else
     menu_bottom(menu_texture[M_COUNT_Q],scene_count1_on);
   
   //APPLE COUNT
   number(apple_count,-.70,.70,0,
	  0.008,.08,
	  90,
	  color_a,
	  0,0);
   draw_apple(-.5,.75,0,.05,.04,90,PINK,1,apple_texture);

   //BALL COUNT
   number(ball_count,-.15,.70,0,
	  0.008,.08,
	  90,
	  color_b,
	  0,0);
   sphere(0.05,.75,0,.05,90,BLUE,0,ball_texture[0]);
   //CAT COUNT
   number(cat_count,.40,.70,0,
	  0.008,.08,
	  90,
	  color_c,
	  0,0);
   
  cat(.6, .65, 0, 30, SMALL, WHITE_CAT, 0,catfur_texture[0]);

   //  Reset model view matrix
   glPopMatrix();
   //  Reset projection matrix
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   //  Pop transform attributes (Matrix Mode and Enabled Modes)
   glPopAttrib();
}


/******************************************************************************************
 *********************************DISPLAY CODE*********************************************
 ******************************************************************************************/

void scene_count1() 
{
  num_cat = 3;
  num_ball = 4;
  num_apple = 7;
  //1 house
  draw_basic_house(-.8,0,-1.0,.3,.4,.3,0,LBLUE,1,house_texture);
  draw_basic_house(-.8,0,-.2,.35,.4,.35,0,BLUE,1,house_texture);
  draw_basic_house(-.8,0,.6,.3,.5,.3,0,ORANGE,1,house_texture);
  letter('C',-.8,.9,-1.0,
	  0.01,.3,
	  180,
	  LBLUE,
	  0,0);
  letter('B',-.8,.9,-.2,
	  0.01,.3,
	  180,
	  BLUE,
	  0,0);
   letter('A',-.8,.9,.6,
	  0.01,.3,
	  180,
	  ORANGE,
	  0,0);
  //only draw in non-game mode as it is too slow on my computer and I wasn't able to get the CULLing to completely work by turn in date
  if(scene_count1_on == 0)
    draw_tree(0,0,1,.75,1.2,trees+0,0,0,2,tree_texture);

  //quadrant 1, 1
  //1 house
  //draw_basic_house(-.9,0,-.4,.2,.3,.2,100,YELLOW,1,house_texture);
  //3 box
  draw_box(.7,0,.2,.1,15,WHITE,1,crate_texture[0]);
  draw_box(.3,0,.2,.07,15,WHITE,1,crate_texture[0]);
  draw_box(.3,0,.6,.15,15,WHITE,1,crate_texture[0]);
  draw_box(.6,0,.6,.07,0,WHITE,1,crate_texture[0]);

  ////3 cats
  cat(1.3, 0, .6, 135, mc.body_type, MIXED, 0,catfur_texture[0]);
  cat(1.2, 0, .3, 70, REGULAR, BROWN_CAT, 0,catfur_texture[0]);
  cat(.3, 0, .3, 105, LARGE, WHITE_CAT, 0,catfur_texture[0]);
  //
  ////4 ball
  sphere(-.18,.03,-.7,.03,90,LBLUE,0,ball_texture[0]);
  sphere(1.47,.03,.45,.03,90,BLUE,0,ball_texture[0]);
  sphere(-.2,.03,.8,.03,90,ORANGE,0,ball_texture[0]);
  sphere(1,.03,.8,.03,90,YELLOW,0,ball_texture[0]);
  //
  ////7 apples
  draw_apple(.72,.23,.10,.04,.03,0,RED,1,apple_texture); //on mid-size crate
  draw_apple(.37,.34,.6,.05,.04,0,PINK,1,apple_texture); //on large crate
  draw_apple(.45,.35,.5,.06,.05,0,PINK,1,apple_texture); //on large crate
  draw_apple(-.25,.055,.25,.04,.03,0,PINK,1,apple_texture); //by blue house
  draw_apple(.2,.045,-1.4,.05,.04,0,PINK,1,apple_texture);
  draw_apple(.2,.045,-1.2,.05,.04,0,PINK,1,apple_texture);
  draw_apple(.3,.045,-1.2,.04,.03,0,PINK,1,apple_texture);


}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=UNIT_SIZE*1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Undo previous transformations
   glLoadIdentity();

   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   gluLookAt(Ex,Ey,Ez , 0,1,0 , 0,Cos(ph),0);

   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   ////glCullFace(GL_BACK);
   //glFrontFace(GL_CCW);
   //glEnable(GL_CULL_FACE);
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
   one_color_cube(0,-2.0,0,4.0,2.0,4.0,DGREEN,1,skybox_texture[C_BOTTOM]);

   scene_count1();

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


   //  Disable Z-buffering in OpenGL
   glDisable(GL_DEPTH_TEST);
   //  Draw menu
  if(scene_count1_on != 0)
      menu_overlay();
   //  Display parameters
   else
   {
     //  Five pixels from the lower left corner of the window
     glWindowPos2i(5,5);
     Print("Angle=%d,%d  Projection=%s",th,ph,"Perspective",light?"On":"Off");
     glWindowPos2i(5,45);
     Print("Model=%s LocalViewer=%s Distance=%.1f Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
     if (light)
       {
	 glWindowPos2i(5,25);
	 Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
       }
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
  th = 215;
  ph = 30;

  //Randomize mixed cat
  rand_mc();
  
  //project_mode 
  project_mode = PERSPECTIVE;

  //field_of_view
  field_of_view = 100;


  apple_count = 0;
  ball_count = 0;
  cat_count = 0;

  scene_count1_on = 0;




  //Lighting
  light=1;      //  Lighting
  one       =   1;  // Unit value
  distance  =   1.5;  // Light distance
  inc       =   15;  // Ball increment
  smooth    =   1;  // Smooth/Flat shading
  local     =   0;  // Local Viewer Model
  emission  =   0;  // Emission intensity (%)
  ambient   =  30;  // Ambient intensity (%)
  diffuse   = 100;  // Diffuse intensity (%)
  specular  =   0;  // Specular intensity (%)
  shininess =   0;  // Shininess (power of two)
  shiny   =   1;  // Shininess (value)
  ylight  =   1.5;  // Elevation of light

}

/*
 * set/change projection (added to reshape as that is another location we modify the view)
 *
 */
void set_or_change_projection()
{
  //  Ratio of the width to the height of the window
  asp = (cur_height>0) ? (double)cur_width/cur_height : 1;

  //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();

   if(project_mode == PERSPECTIVE)
    {
      dim = UNIT_SIZE*2.5-z;
      gluPerspective(field_of_view, asp*dim,.1f,UNIT_SIZE*10);
    }
   //  Orthogonal transformation
   else
      glOrtho(-asp*dim,asp*dim,-dim,+dim,-dim,+dim);

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
  //  Right arrow key - increase angle by 5 degrees
  if (key == GLUT_KEY_LEFT)
    th += 5;
  //  Right arrow key - decrease angle by 5 degrees
  else if (key == GLUT_KEY_RIGHT)
    th -= 5;
  //  Up arrow key - increase elevation by 5 degrees
  else if (key == GLUT_KEY_UP)
    {
      if(scene_count1_on)
	{
	  if(scene_count1_on == APPLE && apple_count < 9) 
	    apple_count++;
	  if(scene_count1_on == BALL && ball_count < 9) 
	    ball_count++;
	  if(scene_count1_on == CAT && cat_count < 9) 
	    cat_count++;
	} else if (ph < 175)
	{     
	  ph += 5;
	}
    }
  //  Down arrow key - decrease elevation by 5 degrees
  else if (key == GLUT_KEY_DOWN)
    {
      if(scene_count1_on)
	{
	  if(scene_count1_on == APPLE && apple_count > 0) 
	    apple_count--;
	  if(scene_count1_on == BALL && ball_count > 0) 
	    ball_count--;
	  if(scene_count1_on == CAT && cat_count > 0) 
	    cat_count--;
	} else if (ph > 5)
	{     
	  ph -= 5;
	}
    }

  //  Keep angles to +/-360 degrees
  th %= 360;
  ph %= 360;
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
    {
      for(int i = 0; i < NUM_TREES; i++) {
	free(trees[i].roots.root);
	for(int j = 0; j<trees[i].branches.num_branches; j++)
	  {
	    for(int k = 0; k<trees[i].branches.branch[j].subbranches.num_subbranches; k++)
	      {
		free(trees[i].branches.branch[j].subbranches.subbranch[k].leaves.leaf);
	      }
	    free(trees[i].branches.branch[j].subbranches.subbranch);
	  }
	free(trees[i].branches.branch);

      }
      exit(0);
    }
   else if (ch == 13) //enter
     {
       if(scene_count1_on == 6)
	 scene_count1_on = 0;
       else if(scene_count1_on < 3)
	 scene_count1_on++;
       else if(scene_count1_on == 3)
	 {
	   if((num_cat == cat_count) && (num_ball == ball_count) && (num_apple == apple_count))
	     scene_count1_on = 4;
	   else
	     scene_count1_on = 5;
	   
	   //reset counts
	   apple_count = 0;
	   ball_count = 0;
	   cat_count = 0;
	 }
       else if(scene_count1_on == 4)
	 scene_count1_on = 6;
       else if(scene_count1_on == 5)
	 scene_count1_on = 1;
     }
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
   //else if (ch == '1')
   //  {
   //    if(project_mode == 2)
   //      project_mode = 1;
   //    else
   //      project_mode += 1;       
   //  }
   else if (ch == 'f' && field_of_view <100)
     field_of_view += 1;
   else if (ch == 'F' && field_of_view >80)
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
   else if ((ch=='[') && (ylight > 0))
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   // Ball Distance
   else if ((ch=='q')&& (distance > 0.1))
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

   if(project_mode == PERSPECTIVE)
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
      zh = fmod(10*t,360);
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
   glutCreateWindow("Project: Counting Program for my Niece - CSCI5229 - Kathy Grimes");
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
   //skybox_texture[C_BOTTOM] =LoadTexBMP("hills_dn.bmp"); 
   skybox_texture[C_BOTTOM] =LoadTexBMP("grass_bottom.bmp");
   
   blank_texture            = LoadTexBMP("blank1.bmp");
   
   catfur_texture[C_SAME]   = LoadTexBMP("gray-fur-texture.bmp");

   crate_texture[C_SAME]    = LoadTexBMP("crate.bmp");//LoadTexBMP("Alphabet_Main.bmp");

   moon_texture[C_SAME]     = LoadTexBMP("moon.bmp");

   tree_texture[BARK]  	    = LoadTexBMP("tree_bark.bmp");
   tree_texture[ROOT]  	    = LoadTexBMP("tree_bark_32.bmp");
   tree_texture[LEAF]  	    = LoadTexBMP("leaf.bmp");

   house_texture[WALL]      = LoadTexBMP("whitebrick.bmp");
   house_texture[STEP]      = blank_texture; //LoadTexBMP("");
   house_texture[WINDOW]    = LoadTexBMP("window2.bmp");
   house_texture[DOOR]      = LoadTexBMP("door.bmp");
   house_texture[ROOF]      = LoadTexBMP("woodroof.bmp");
   house_texture[CHIMNEY]   = house_texture[WALL];//LoadTexBMP("");

   grass_texture[C_SAME]    = LoadTexBMP("grass.bmp");

   apple_texture[C_SAME]    = LoadTexBMP("apple0.bmp");

   catface_texture[C_SAME]  = LoadTexBMP("cat_face.bmp");

   ball_texture[C_SAME]     = LoadTexBMP("redslateroof.bmp");

   menu_texture[M_COUNT]    = LoadTexBMP("Count_Main.bmp");
   menu_texture[M_COUNT_Q]  = LoadTexBMP("Count_Sub1.bmp");
   menu_texture[M_COLOR]    = LoadTexBMP("Colors_Main.bmp");
   menu_texture[M_COLOR_Q]  = LoadTexBMP("Colors_Sub1.bmp");
   menu_texture[M_FDBK]     = LoadTexBMP("feedback.bmp");
 
   init_color();
   tree_init();
   // Use inital state parameters
   initialState();
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}



