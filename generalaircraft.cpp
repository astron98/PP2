#define GL_GLEXT_PROTOTYPES
#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <string>
#include <strings.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <itpp/itbase.h>
#include <complex>
#include <fstream>
#include <itpp/stat/misc_stat.h>
#include <stdio.h>
using namespace std;
//#include ""
using namespace itpp;


int i=0;
double sf,xa,ya,xb,yb,xc,yc,xd,yd,xe,ye,cx,cy,pt,pr,freq,addsqrtrms;
double speed,no_of_frames,fps,timetotal;
mat object(1,3);
vec vcap(3);
vec ptA(3),ptB(3);
vec pa(3);
vec pit(3);
mat path(2,3);
vec u(3),v(3),w(3),ucap(3);
vec direction(3);
double l=5.;
bool tk=false;
bool ln=false;
bool k=false;
double theta=10*(M_PI/180);
double ma,mb,mc,md,me,r;

#define MAX 100
double angle=atan2(ya,xa);
int state=1;
string s;
void* currentFont;
void setFont(void* font) {
  currentFont = font;
}
void drawString(float x, float y, float z,const char *string) {
  glRasterPos3f(x, y, z);

  for (const char* c = string; *c != '\0'; c++) {
    glutBitmapCharacter(currentFont, *c);  // Updates the position
  }
}

 void initialize()
 {
glClearColor(0.0, 0.0, 1.0,1.0); //background color will change
	
 }
 
 void resize(int width , int height)
 {
if(height ==0)
   height = 1;
if (width == 0)
width = 1;
glViewport(0,0,width,height);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0,10,0,10);
glMatrixMode(GL_MODELVIEW);
//gluPerspective(45.0 , (float)width/(float)height, 0.1, 100.0);
}
void  display()
{
//Clear information from last draw
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
glLoadIdentity(); //Reset the drawing perspective
glPointSize(5);
glBegin(GL_POINTS); //Begin quadrilateral coordinates
//rectangle

glVertex2f(object(0,0),object(0,1));  //assuming length of the rectangle to be 4 units
/*glVertex2f(object(1,0),object(1,1),object(1,2));
glVertex2f(object(2,0),object(2,1),object(2,2));
glVertex2f(object(3,0),object(3,1),object(3,2));
glVertex2f(object(4,0),object(4,1),object(4,2));*/
glEnd(); 
float x_num = (180/M_PI)*atan2(yd,xd);
    char buf[MAX];
  char bufpr[MAX];
    gcvt(x_num, 6, buf);
    gcvt(pr,6,bufpr);
    
drawString(0, 95, 0.,"Amrita Vishwa Vidyapeetham");
drawString(0, 90, 0.,"Angle of Arrival");
drawString(30, 90, 0.,buf);//End quadrilateral coordinates
drawString(0, 85, 0.,"power : ");
drawString(30, 85, 0.,bufpr);//End quadrilateral coordinates

/*
glBegin(GL_TRIANGLES); //Begin triangle coordinates
//Triangle
glVertex2f(x+l, y);
glVertex2f(x+l,x+l);
glVertex2f(x+l, -9.);
glEnd(); //End triangle coordinates*/

glutSwapBuffers();  // Swap front and back buffers (of double buffered mode)
//glFlush();
}
void keyboard(unsigned char key, int x , int y)
{
switch(key)
{
case 27:glutMainLoop();
   break;
default :
break;
}
}
double tempx,tempy;
void rotate(double *x,double *y, double teta,double cx,double cy){

			tempx=*x;
			tempy=*y;
		*x=cx+((tempx-cx)*(cos(teta))-(tempy-cy)*(sin(teta)));
		*y=cy+((tempy-cy)*(cos(teta))+(tempx-cx)*(sin(teta)));
}
/*mat global_cord_real_time(double xi,double yi,double zi,double ux,double uy,double uz,double vx,double vy,double vz,double wx,double wy,double wz){
double 	k
return k;
}*/

vec crossproduct(vec a,vec b)
{
vec c(3);
c[0]=((a[1]*b[2])-(b[1]*a[2]));
c[1]=-((a[0]*b[2])-(b[0]*a[2]));
c[2]=((a[0]*b[1])-(b[0]*a[1]));
return c;

}
void timer(int)
{
	setFont(GLUT_BITMAP_HELVETICA_18);
	glutPostRedisplay();
	glutTimerFunc(1000/60,timer,0);
	///----------------------------------------------------------------///finding direction vectors u,v,w (local)
	double umag;
	ptA[0]=path(0,0);
	ptA[1]=path(0,1);
	ptA[2]=path(0,2);
	ptB[0]=path(1,0);
	ptB[1]=path(1,1);
	ptB[2]=path(1,2);
	cout<<"entered void timer"<<endl;
	//cout<<"ptb are "<<ptB[0]<<ptB[1]<<ptB[2]<<endl;
	//cout<<"sqr(ptB[0]-ptA[0]) : "<<sqr(ptB[0]-ptA[0])<<"sqr(ptB[1]-ptA[1]) : "<<sqr(ptB[1]-ptA[1])<<"sqr(ptB[2]-ptA[2]) : "<<sqr(ptB[2]-ptA[2])<<endl;
	addsqrtrms=sqr(ptB[0]-ptA[0])+sqr(ptB[1]-ptA[1])+sqr(ptB[2]-ptA[2]);
	//cout<<"sqrt : "<<sqrt(addsqrtrms)<<endl;
	//umag=sqrt(pow((ptB[0]-ptA[0]),2)+pow((ptB[1]-ptA[1]),2)+pow((ptB[2]-ptA[2]),2));
	umag=sqrt(addsqrtrms);
	cout<<"umag and length = "<<umag<<endl;
	
	ucap[0]=(ptB[0]-ptA[0])/(umag);
	ucap[1]=(ptB[1]-ptA[1])/(umag);
	ucap[2]=(ptB[2]-ptA[2])/(umag);
	
	timetotal=umag/speed;
	
	no_of_frames=timetotal*fps;
	sf= (umag/no_of_frames);//length of each frame
	
	pa[0]=ptA[0];
	pa[1]=ptA[1];
	pa[2]=ptA[2];
	
	double i=0;
	while(i<=(umag/sf)){
	
	pit=pa+ucap*sf*i;
	i+=0.001;
	cout<<i<<" "<<"sf*i"<<(sf*i)<<"pit : "<<pit<<endl;//ucap*i*no_of_frames<<endl;
	//display pi
	///---------------///
	object(0,0)=pit[0];
	object(0,1)=pit[1];
	object(0,2)=pit[2];
	//cout<<object(0,0)<<object(0,1)<<object(0,2)<<endl;
	///---------------///
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	///----------------------------------------------------------------///

}
	
	

int main(int argc, char** argv)
 {
	 freq=2.5e9;
	 pt=1;
	 /*xd=0;
	 yd=0;
	 xa=xd-(2*l);
ya=yd-(l/2);
	 xb=xa;
yb=ya+l;
xc=xa+l;
yc=ya+l;

xe=xa+l;*/
 
object(0,0)=0; //object(5,3)=xa,ya,za;xb,yb,zb.............
object(0,1)=0;
object(0,2)=0;





xd=0;   //tip of the aircraft
yd=0;
xa=xd-((5*l)/4);
ya=yd-(l/16);
xb=xd-((5*l)/4);
yb=yd+(l/16);
xc=xd-(l/4);
yc=yd+(l/16);
xe=xd-(l/4);
ye=yd-(l/16);

path(0,0)=0;
path(0,1)=0;
path(0,2)=0;
path(1,0)=10;
path(1,1)=10;
path(1,2)=0;

//object from (0,0,0) to (10,10,10)
pit[0]=path(0,0);
pit[1]=path(0,1);
pit[2]=path(0,2);
fps=60;
speed=15;

glutInit(&argc, argv);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  
glutInitWindowSize(800,800);
glutInitWindowPosition(400,400);
glutCreateWindow("Aircraft_trajectory");
glutDisplayFunc(display);
glutKeyboardFunc(keyboard);
glutReshapeFunc(resize);
glutTimerFunc(0,timer,0);
initialize();
glutMainLoop();
return 0;
 }

