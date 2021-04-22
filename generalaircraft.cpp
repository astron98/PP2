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







///---------------------------------------------------------------------------Opengl declarations-----------------------------------------------------------------///
double itr;
int i=0;
double sf,xa,ya,xb,yb,xc,yc,xd,yd,xe,ye,cx,cy,pt,pr,freq,addsqrtrms;
double speed,no_of_frames,fps,timetotal,row;
mat object(1,3);
vec vcap(3);
vec ptA(3),ptB(3);
vec pa(3);
vec pit(3);
mat path(8,3);
vec u(3),v(3),w(3),ucap(3);
vec direction(3);
double l=15.;
bool rot=true;
bool rst=false;
double theta=10*(M_PI/180);
double ma,mb,mc,md,me,r;

#define MAX 100
double rotate_angle;
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
gluOrtho2D(-10,200,-10,200);//left, right, bottom, top
glMatrixMode(GL_MODELVIEW);
//gluPerspective(45.0 , (float)width/(float)height, 0.1, 100.0);
}
void  display()
{

//Clear information from last draw
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
glLoadIdentity();

float x_num = (180/M_PI)*atan2(object(0,1),object(0,0));
    char buf[MAX];
  char bufpr[MAX];
    gcvt(x_num, 6, buf);
    gcvt(pr,6,bufpr);
   
drawString(50, 190, 0.,"Amrita Vishwa Vidyapeetham");
drawString(0, 150, 0.,"AoA");
drawString(50, 150, 0.,buf);//End quadrilateral coordinates
drawString(0, 140, 0.,"power : ");
drawString(80, 140, 0.,bufpr);//End quadrilateral coordinates

glLoadIdentity(); //Reset the drawing perspective
glPointSize(5);
glTranslated(object(0,0),object(0,1),0);
glRotatef(rotate_angle,0,0,1.);
glBegin(GL_POLYGON); //Begin quadrilateral coordinates

//rectangle); //Begin quadrilateral coordinates
//rectangle

  //assuming length of the rectangle to be 4 units
/*glVertex2f(object(1,0),object(1,1));
glVertex2f(object(2,0),object(2,1));
glVertex2f(object(3,0),object(3,1));
glVertex2f(object(0,0),object(0,1));
glVertex2f(object(4,0),object(4,1));
*/
glVertex2f(xa,ya);  //assuming length of the rectangle to be 4 units
glVertex2f(xb,yb);
glVertex2f(xc,yc);
glVertex2f(xd,yd);
glVertex2f(xe,ye);

glEnd();
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
cout<<"entered rotate function call"<<endl;
tempx=*x;
tempy=*y;
*x=cx+((tempx-cx)*(cos(teta))-(tempy-cy)*(sin(teta)));
*y=cy+((tempy-cy)*(cos(teta))+(tempx-cx)*(sin(teta)));
}
/*mat global_cord_real_time(double xi,double yi,double zi,double ux,double uy,double uz,double vx,double vy,double vz,double wx,double wy,double wz){
double k
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
void find_rot_ang(vec ptA,vec ptB)
{
double new_angle=(atan2((ptB[1]-ptA[1]),(ptB[0]-ptA[0])))*((180/M_PI));
cout<<"new angle : "<<new_angle<<endl;
/*if(new_angle<0){
new_angle=360+new_angle;}*/

rotate_angle=new_angle;

/*if(new_angle<rotate_angle){rotate_angle=new_angle;}
else{rotate_angle = new_angle-rotate_angle;}*/
cout<<"entered rotate function : "<<rotate_angle<<endl;
}
void timer(int)
{
setFont(GLUT_BITMAP_HELVETICA_18);
glutPostRedisplay();
glutTimerFunc(1000/60,timer,0);
///----------------------------------------------------------------///finding direction vectors u,v,w (local)
double umag;//distance btw a and b
ptA[0]=path(row,0);
ptA[1]=path(row,1);
ptA[2]=path(row,2);
ptB[0]=path(row+1,0);
ptB[1]=path(row+1,1);
ptB[2]=path(row+1,2);
//cout<<"entered void timer"<<endl;
//cout<<"ptb are "<<ptB[0]<<ptB[1]<<ptB[2]<<endl;
//cout<<"sqr(ptB[0]-ptA[0]) : "<<sqr(ptB[0]-ptA[0])<<"sqr(ptB[1]-ptA[1]) : "<<sqr(ptB[1]-ptA[1])<<"sqr(ptB[2]-ptA[2]) : "<<sqr(ptB[2]-ptA[2])<<endl;
addsqrtrms=sqr(ptB[0]-ptA[0])+sqr(ptB[1]-ptA[1])+sqr(ptB[2]-ptA[2]);
//cout<<"sqrt : "<<sqrt(addsqrtrms)<<endl;
//umag=sqrt(pow((ptB[0]-ptA[0]),2)+pow((ptB[1]-ptA[1]),2)+pow((ptB[2]-ptA[2]),2));
umag=sqrt(addsqrtrms);
//cout<<"ptA : "<<ptA<<" ptB : "<<ptB<<endl;

ucap[0]=(ptB[0]-ptA[0])/(umag);
ucap[1]=(ptB[1]-ptA[1])/(umag);
ucap[2]=(ptB[2]-ptA[2])/(umag);
//cout<<"ucap= "<<ucap<<endl;
timetotal=umag/speed;

no_of_frames=timetotal*fps;
sf= (umag/no_of_frames);//length of each frame

pa[0]=ptA[0];
pa[1]=ptA[1];
pa[2]=ptA[2];
//cout<<"pa = "<<pa<<endl;
/*if(pa[0]==path(4,0)&&pa[1]==path(4,1)&&pa[2]==path(4,2)){
rst=true;
}*/
if(rst==false && row==8){
object(0,0) = path(0,0);
object(0,1) = path(0,1);
object(0,2) = path(0,2);
rst=true;
}

else if(itr<=(umag/sf) && rot==false && rst==false){

pit=pa+ucap*sf*itr;
itr+=0.5;//speed
//cout<<itr<<" "<<"sf*i"<<(sf*i)<<"pit : "<<pit<<endl;//ucap*i*no_of_frames<<endl;
//display pi
///---------------///
object(0,0) = pit[0];
object(0,1) = pit[1];
object(0,2) = pit[2];
/*cout<<"xd,yd,zd are :  "<<object(0,0)<<"  "<<object(0,1)<<"  "<<object(0,2)<<endl;
cout<<"xa,ya,za are :  "<<object(1,0)<<"  "<<object(1,1)<<"  "<<object(1,2)<<endl;
cout<<"xb,yb,zb are :  "<<object(2,0)<<"  "<<object(2,1)<<"  "<<object(2,2)<<endl;
cout<<"xc,yc,zc are :  "<<object(3,0)<<"  "<<object(3,1)<<"  "<<object(3,2)<<endl;
cout<<"xe,ye,ze are :  "<<object(4,0)<<"  "<<object(4,1)<<"  "<<object(4,2)<<endl;*/



///---------------///
}
else if(itr>=no_of_frames && rot==false && rst==false  ){
pit=ptB;
cout<<"pit equalised to ptB"<<endl;
row+=1;
rot = true;
cout<<endl;
cout<<endl;
cout<<endl;
cout<<endl;
cout<<endl;
}
else if(rot==true){
find_rot_ang(ptA,ptB);
cout<<"rotate entered"<<rotate_angle<<endl;
cout<<"before rotate xd,yd : "<<xd<<"  "<<yd<<endl;
//rotate(&xd,&yd,rotate_angle,0,0);
cout<<"rotated xd,yd : "<<xd<<"  "<<yd<<endl;
cout<<"before rotate xa,ya : "<<xd<<"  "<<yd<<endl;
//rotate(&xa,&ya,rotate_angle,0,0);
cout<<"rotated xa,ya : "<<xa<<"  "<<ya<<endl;
cout<<"before rotate xb,yb : "<<xd<<"  "<<yd<<endl;
//rotate(&xb,&yb,rotate_angle,0,0);
cout<<"rotated xb,yb : "<<xb<<"  "<<yb<<endl;
cout<<"before rotate xc,yc : "<<xd<<"  "<<yd<<endl;
//rotate(&xc,&yc,rotate_angle,0,0);
cout<<"rotated xc,yc : "<<xc<<"  "<<yc<<endl;
cout<<"before rotate xe,ye : "<<xd<<"  "<<yd<<endl;
//rotate(&xe,&ye,rotate_angle,0,0);
cout<<"rotated xe,ye : "<<xe<<"  "<<ye<<endl;
rot=false;
itr=0;
cout<<endl;
cout<<endl;
cout<<endl;
cout<<endl;
cout<<endl;
}
else if(rst==true){

row=0;

/*path(0,0)=0;
path(0,1)=0;
path(0,2)=0;
path(1,0)=50;
path(1,1)=50;
path(1,2)=0;
path(2,0)=80;//x
path(2,1)=50;//y
path(2,2)=0;//z
path(3,0)=100;//x
path(3,1)=80;//y
path(3,2)=0;//z
path(4,0)=140;//x
path(4,1)=80;//y
path(4,2)=0;//z
path(5,0)=140;//x
path(5,1)=80;//y
path(5,2)=0;//z
path(6,0)=50;//x
path(6,1)=50;//y
path(6,2)=0;//z
path(7,0)=0;//x
path(7,1)=0;//y
path(7,2)=0;//z*/
itr=0;
cout<<"path matrix"<<path<<endl;

//object from (0,0,0) to (10,10,10)
pit[0]=path(0,0);
pit[1]=path(0,1);
pit[2]=path(0,2);
rotate_angle=0;
//present_angle=0;
ptA[0]=path(0,0);
ptA[1]=path(0,1);
ptA[2]=path(0,2);
ptB[0]=path(1,0);
ptB[1]=path(1,1);
ptB[2]=path(1,2);
cout<<"pit are in main function:  "<<pit[0]<<"  "<<pit[1]<<"  "<<pit[2]<<endl;
//find_rot_ang(ptA,ptB);
rot = true;
rst=false;
}
r=sqrt(pow((object(0,0)),2)+pow((object(0,1)),2));
pr=pt*1000000*pow((3*pow(10,8)/(freq*4*M_PI*r)),2);
}




int main(int argc, char** argv)
 {
freq=2.5e9;
pt=1;

xd=0;
yd=0;
xa=xd-((5*l)/4);
ya=yd-(l/16);
xb=xd-((5*l)/4);
yb=yd+(l/16);
xc=xd-(l/4);
yc=yd+(l/16);
xe=xd-(l/4);
ye=yd-(l/16);
row=0;

path(0,0)=0;
path(0,1)=0;
path(0,2)=0;
path(1,0)=8;
path(1,1)=0;
path(1,2)=0;
path(2,0)=15;//x
path(2,1)=2;//y
path(2,2)=0;//z
path(3,0)=24;//x
path(3,1)=4;//y
path(3,2)=0;//z
path(4,0)=30;//x
path(4,1)=6;//y
path(4,2)=0;//z
path(5,0)=36;//x
path(5,1)=10;//y
path(5,2)=0;//z
path(6,0)=45;//x
path(6,1)=15;//y
path(6,2)=0;//z
path(7,0)=90;//x
path(7,1)=47;//y
path(7,2)=0;//z
/*double t=0;
double e=0;
int i=0;
for(i;i<30;i++){
path(i,0)=t;
path(i,1)=0;
path(i,2)=0;
t+=1;
}
for(i;i<100;i++){
path(i,0)=t;
path(i,1)=e;
path(i,2)=0;
t+=10;
e+=1;
}
for(i;i<150;i++){
path(i,0)=t;
path(i,1)=e;
path(i,2)=0;
t+=5;
//e+=0;
}
for(i;i<200;i++){
path(i,0)=t;
path(i,1)=e;
path(i,2)=0;
t-=5;
e-=1;
}
for(i;i<210;i++){
path(i,0)=t;
path(i,1)=e;
path(i,2)=0;
t-=5;
e-=0.7;
}*/



cout<<"path matrix"<<path<<endl;

//object from (0,0,0) to (10,10,10)
pit[0]=path(0,0);
pit[1]=path(0,1);
pit[2]=path(0,2);
rotate_angle=0;
ptA[0]=path(0,0);
ptA[1]=path(0,1);
ptA[2]=path(0,2);
ptB[0]=path(1,0);
ptB[1]=path(1,1);
ptB[2]=path(1,2);
cout<<"pit are in main function:  "<<pit[0]<<"  "<<pit[1]<<"  "<<pit[2]<<endl;
//find_rot_ang(ptA,ptB);
rot = true;
pt=1;
fps=60;
speed=150;
itr=0;
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
