/*
 *  HW2: Lorenz Attractor
 *  Course: CSCI5229
 *  Author: Kathy Grimes
 *  Description:
 *    Code Initially was from Example 5 class code
 *    Code will reproduce lorenz attractor graph 
 *    Graph can be modified interactively using the following:
 *       s/S,b/B,r/R: increment/decrement s, r, or b values by .5
 *               z/Z: zoom inword/outward by 5
 *     up/down_arrow: spin around z axis
 *  left/right_arrow: increment/decrement angle around y axis
 *                 i: return to inital settings
 *                 p: toggle pause idle spinning
 *                 a: toggle axis
 */
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
/* Axis Parameter */
int axes;

/*  Angle Parameters  */
int th;  //  Azimuth of view angle
int zh;  //  Spin around z axis

/*  Lorenz Parameters  */
double s;
double b;
double r;

/*  Zoom Parameters */
double z;

/* Pause Spin Parameter */
int p;


/*
 * Initial State of Lorenz Attractor
 *
 */
void initialState()
{
  //Axis
  axes = 0;

  //Angle Parameters
  th=0;  //  Azimuth of view angle
  zh=0;  //  Spin around z axis

  //Lorenz Parameters
  s = 10;
  b = 2.6666;
  r = 28;

  //Zoom Paramter
  z = 0;

  //Pause Parameter
  p = 0;
  
}


/*
 * Draw Lorenz Attractor Line Strip
 * Formula taken from lorenz.c file provided to class as a starting point
 */
void drawLorenzeAttractorLineStrip()
{

  glBegin(GL_LINE_STRIP); //begin of GL_LINE_STRIP

  int i, colorR, colorG, colorB;
  /*  Coordinates  */
  double x = 1;
  double y = 1;
  double z = 1;
  /*  Time step  */
  double dt = 0.001;
  
  /*
   *  Integrate 50,000 steps (50 time units with dt = 0.001)
   *  Explicit Euler integration
   */
  for (i=0;i<50000;i++)
    {
      double dx = s*(y-x);
      double dy = x*(r-z)-y;
      double dz = x*y - b*z;
      x += dt*dx;
      y += dt*dy;
      z += dt*dz;

      //Add Color for Vertex
      colorR = i<(50000*1/3) ? 1 : 0;
      colorG = (i>=(50000*1/3)) && (i<(50000*2/3)) ? 1 : 0;
      colorB = i>=(50000*2/3) ? 1 : 0;
      glColor3f(colorR,colorG,colorB);
      //Create 3d Vertex
      glVertex3d(x,y,z);
    }

  glEnd(); //end of GL_LINE_STRIP

}


/*
 * Convenience function for text
 */
#define LEN 8192  //  Maximum amount of text
void Print(const char* format , ...)
{
   char    buf[LEN]; // Text storage
   char*   ch=buf;   // Text pointer
   //  Create text to be display
   va_list args;
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display text string
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

void drawAxis(double len)
{
  //  Draw axes
  if (axes)
    {
      //  White
      glBegin(GL_LINES);
      glColor3f(1,1,1);
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
}

/*
 *  Function to print any errors encountered
 */
void ErrCheck(char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

/*
 * This function is called by GLUT to display the scene
 */
void display()
{
   //  Clear screen and Z-buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   glMatrixMode(GL_PROJECTION);

   //  Reset transformations
   glLoadIdentity();
   
   //  Orthogonal projection
   glOrtho(-100.0+z, 100.0-z, -100.0+z, 100.0-z, -100.0+z, 100.0-z);

   //  Rotate around Y axis
   glRotatef(th,0.0,1.0,0.0);
   //  Spin around Z axis
   glRotatef(zh,0.0,0.0,1.0);

   glMatrixMode(GL_MODELVIEW);
   //  Reset transformations
   glLoadIdentity();

   //draw Axis
   drawAxis((100-z)/2.0);
   //draw Lorenz Attractor Line Strip
   drawLorenzeAttractorLineStrip();



   //  Display rotation angle
   glColor3f(1,1,1);
   glWindowPos2i(5,30);
   Print("Angle th=%d",th);
   glWindowPos2i(5,5);
   Print("Angle zh=%d",zh);

   //Display Zoom value (max value is 95 as 100 is where it would be 0 left,0 right
   if(z == 95)
     {
       glColor3f(1,0,0);
       glWindowPos2i(165,5);
       Print("Zoom=MAX");
     }
   else
     {
       glColor3f(1,0,1);
       glWindowPos2i(165,5);
       Print("Zoom=%.0f",z);
     }
   
   // Display s,r,b values
   glColor3f(0,1,1);
   glWindowPos2i(305,5);
   Print("(s,r,b)=(%.1f,%.1f,%.1f)",s,r,b);
   
   


   //  Sanity check
   ErrCheck("display");
   //  Flush and swap buffer
   glFlush();
   glutSwapBuffers();
}
/*
 * This function is called by GLUT when regular keys are pressed
 */
void key(unsigned char key,int x,int y)
{
   
   if (key == 's')
     s += .5;
   else if (key == 'S')
     s -= .5;
   else if (key == 'r')
     r += .5;
   else if (key == 'R')
     r -= .5;
   else if (key == 'b')
     b += .5;
   else if (key == 'B')
     b -= .5;
   else if (key == 'z')
     {
       z += 5;
       if (z > 95)
	 z = 95;
     }
   else if (key == 'Z')
     z -= 5;
   else if (key == 'i')
     initialState();
   else if (key == 'a')
     {
       if (axes == 1)
	 axes = 0;
       else 
	 axes = 1;
     }
   else if (key == 'p')
     {
       if (p == 1)
	 p = 0;
       else 
	 p = 1;
     }
   //  Request display update
   glutPostRedisplay();
}

/*
 * This function is called by GLUT when special keys are pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow - increase rotation by 5 degree
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow - decrease rotation by 5 degree
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow - decrease rotation by 5 degree
   else if (key == GLUT_KEY_UP)
      zh -= 5;
   //  Down arrow - decrease rotation by 5 degree
   else if (key == GLUT_KEY_DOWN)
      zh -= 5;

   //  Keep angles to +/-360 degrees
   th %= 360;
   zh %= 360;
   //  Request display update
   glutPostRedisplay();
}

/*
 * This function is called by GLUT when the window is resized
 */
void reshape(int width,int height)
{
  double val = 100.0-z;
   //  Calculate width to height ratio
  double w2h = (height>0) ? (double)width/(double)height : 1;
   //  Set viewport as entire window
   glViewport(0,0, width,height);
   //  Select projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Set projection to identity
   glLoadIdentity();
   //  Orthogonal projection:  cube adjusted for aspect ratio
   glOrtho(-val*w2h,val*w2h,-val,val,-val,val);
   //  Select model view matrix
   glMatrixMode(GL_MODELVIEW);
   //  Set model view to identity
   glLoadIdentity();
}

/*
 * This function is called by GLUT when idle
 */
void idle()
{
  if(p == 0)
    {
      //  Get elapsed (wall) time in seconds
      double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
      //  Calculate spin angle 90 degrees/second
      zh = fmod(90*t,360);
      //  Calculate spin angle 90 degrees/second
      th = fmod(90*t,360);
      //  Request display update
      glutPostRedisplay();
    }
}

/*
 * GLUT based Hello World
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);

   //  Request double buffered true color window with Z-buffer
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

   //Make initial window size 600x600pix
   glutInitWindowSize(600,600);

   //  Create window  
   glutCreateWindow("HW2 Lorenz Attractor - CSCI5229 - Kathy Grimes");

   //  Register display, reshape, idle and key callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutIdleFunc(idle);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);

   //  Enable Z-buffer depth test
   glEnable(GL_DEPTH_TEST);

   // Use inital state parameters
   initialState();

   //  Pass control to GLUT for events
   glutMainLoop();

   //  Return to OS
   return 0;
}


