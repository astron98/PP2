#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>
#include<bits/stdc++.h>
#include<fstream>
#include<iostream>
#include<stdlib.h>
#include "trajectory.h"
#include<cmath>
#define PI 3.14159265358979323846
#define Cos(th) cos(PI/180*(th))
#define Sin(th) sin(PI/180*(th))
/*  D degrees of rotation */
#define DEF_D 5

#define SPEED 150.0

using namespace std;


/* GLOBAL VARIABLES */

//trajectory class
Trajectory trajectory;

// Points
Points p,p1,p2;

// Trajectory related variables
bool isInitial = true, newPath = false;
double distanceBetweenPoints = 0.0;
double timeOfPath = 0.0;
double numberOfFrames = 0.0;
double frameindex = 0.0;
double eachFrameLength = 0.0;
vector<double> directionVector;

//initial position of x
double x_position = 0.0, y_position = 0.0, z_position = -20.0;
double angle_x = -60.0, angle_y = 180.0, angle_z = -90.0;
double x_direction = 0, y_direction = 0, z_direction = 0;
int N = 0;

void displayAnimationWindow();
void reshape(int, int);
void timer(int);


// Under development
void updateAngle();
void updateAngles(double Ax, double Ay, double Az){


    double A = sqrt(pow(Ax,2)+pow(Ay,2)+pow(Az,2));

    double currentAngleX = acos(Ax/A)*(180/PI);
    double currentAngleY = acos(Ay/A)*(180/PI);
    double currentAngleZ = acos(Az/A)*(180/PI);

    angle_x = currentAngleX;
    angle_y = currentAngleY;
    angle_z = currentAngleZ;

    // cout<<"The angles are :"<<angle_x<<" "<<angle_y<<" "<<angle_z<<endl;
}

vector<Points> generateArc(Points center, Points start, Points end, double radius) {
    double tx = start.x;

    //h,k = center of the circle
    vector<Points> res;

    while(abs(end.x-tx)>0.1) {
        double ty = center.y + sqrt(pow(radius,2) - pow(tx - center.x,2));
        res.push_back(Points(tx, ty, -20));
        tx-=0.099999999;
    }

    return res;
}

// Load the trajectories from a file
void loadTrajectory(){

//    int n=0;
//    cout<<"Enter the number of points: ";
//    cin>>n;

    vector<Points> points {

    Points(-10.7,-11.32,-20),Points(-2,-11.32,-20),Points(6,-7,-20),Points(7.9,1,-20),/*The arc points are inserted*/Points(2,6,-20),Points(-3,8,-25),Points(-4,6,-24),Points(-7,2,-25),Points(-10.7,-11.32,-20)
    };

    //arc points
    double radius = 6;
    Points center = Points(2,0,-20), startPoint = points[3], endPoint = points[4];

    vector<Points> arcArr = generateArc(center, startPoint, endPoint, radius);

    auto it = points.begin();
    auto itArc = arcArr.begin();
    points.insert(it+4, arcArr.begin()+1, arcArr.end());

//    Points point;
//    for(int i=0;i<n;i++){
//        cout<<"Enter x, y, z for point "<<(i+1)<<endl;
//        cin>>point.x>>point.y>>point.z;
//        points.push_back(point);
//    }

    // Initial position
    x_position = points[0].x;
    y_position = points[0].y;
    z_position = points[0].z;

    trajectory.cpi = 0;
	trajectory.points = points;
	trajectory.points.push_back(points[0]);

	N = points.size();
}

void init(){
    glClearColor(0.7,0.7,0.7,1.0);
    glEnable(GL_DEPTH_TEST);
}
void displayEmptyScreen(){
    glClearColor(0.7,0.7,0.7,1.0);
    glFlush();
}

void StartMenu(int n){
    switch(n)
    {
        case 1:
            loadTrajectory();


            glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
            glutInitWindowPosition(200,100);
            glutInitWindowSize(1920,1200);

            glutCreateWindow("Flight");
            glutDisplayFunc(displayAnimationWindow);
            glutReshapeFunc(reshape);
            glutTimerFunc(0, timer, 0);
            init();
        break;

        case 2:
            break;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(200,100);
    glutInitWindowSize(1920,1200);

    glutCreateWindow("Menu");



    glutCreateMenu(StartMenu);
    glutAddMenuEntry("Start",1);
    glutAddMenuEntry("Exit",0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(displayEmptyScreen);
    //callback functions
    glutMainLoop();
}

void renderBitMap(double x, double y, double z, void *font, char *title){
    glRasterPos2f(x,y);
    for(char *c=title; *c!='\0';c++){
        glColor3f(0,0,0);
        glutBitmapCharacter(font, *c);
    }
}

void displayAnimationWindow() {

    // Text
    char textBuffer[100] = {0};
    sprintf_s(textBuffer,"Amrita");
    renderBitMap(800,800,-20,GLUT_BITMAP_HELVETICA_18,textBuffer);

    GLfloat Pos[] = {0,3,0,1} ;
    GLfloat Col[] = {1,0,0,1} ;

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //will reset the coordinate system.
    glLoadIdentity();
    glLightfv(GL_LIGHT0,GL_POSITION,Pos);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,Col);

    //Translation is the shifting of the origin. - Translate first and then draw the object
    glTranslatef(x_position,y_position,z_position);
    //gluLookAt(0,1,3,0,0,0,0,1,0) ;

    glRotatef(angle_x,p2.x,0.0,0.0);
    glRotatef(angle_y,0.0,p2.y,0.0);
    glRotatef(angle_z,0.0,0.0,p2.z);

    glShadeModel(GL_SMOOTH);

    glPushMatrix();
        glScalef(.5,1,.5);
        glColor3f(0.0,0.0,1.0);
        glutSolidCube(1);
    glPopMatrix();
	glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,0);
    glBegin(GL_POLYGON);
    glColor3f(0.0,0.0,1.0);
    glVertex3f(0.25,0.4,0.0);
    glVertex3f(0.712,-0.3,0.0);
    glVertex3f(0.712,-0.4,0.0);
    glVertex3f(0.25,-0.4,0.0);
    glEnd();

    glPopMatrix();
    glPushMatrix();
    glTranslatef(0,0,0);
    glBegin(GL_POLYGON);
    glColor3f(0.0,0.0,1.0);
    glVertex3f(-0.25,0.4,0.0);
    glVertex3f(-0.712,-0.3,0.0);
    glVertex3f(-0.712,-0.4,0.0);
    glVertex3f(-0.25,-0.4,0.0);
    glEnd();

    glPopMatrix();
    glPushMatrix();
    glTranslatef(0,0,0);
    glBegin(GL_POLYGON);
    glColor3f(1.0,12.0,1.0);
    glVertex3f(0.0,-0.05,0.0);
    glVertex3f(0.0,-0.381,-0.65);
    glVertex3f(0.0,-0.5,-0.65);
    glVertex3f(0.0,-0.45,0.0);
    glEnd();

    glPopMatrix();

    glPushMatrix();

        glTranslatef(0,0,0);
        glBegin(GL_TRIANGLES);
        glColor3f(0.0,1.0,1.0);

        glVertex3f(0.0,1.0,0.0);
        glVertex3f(-0.25,0.5,0.25);
        glVertex3f(0.25,0.5,0.25);

        glColor3f(0.0,0.0,1.0);// Begin drawing the pyramid with 4 triangles
        glVertex3f(0.0,1.0,0.0);
        glVertex3f(0.25,0.5,0.25);
        glVertex3f(0.25,0.5,-0.25);

        glColor3f(0.0,1.0,1.0);
        glVertex3f(0.0,1.0,0.0);
        glVertex3f(0.25,0.5,-0.25);
        glVertex3f(-0.25,0.5,-0.25);

        glColor3f(0.0,0.0,1.0);
        glVertex3f(0.0,1.0,0.0);
        glVertex3f(-0.25,0.5,-0.25);
        glVertex3f(-0.25,0.5,0.25);
        glEnd();
    glPopMatrix();

    //will display the framebuffer on screen
    glutSwapBuffers();
}

void reshape(int w, int h) {
    //v port
    glViewport( 0, 0, (GLsizei)w, (GLsizei)h);

    //projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //projection-window
    gluPerspective(60,1,1.0,500.0);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

bool checkPrecision(double point_i, double point_f){

    bool result=false;
    double difference = abs(point_i-point_f);
    return (difference<=0.1);

}

void timer(int) {

    //even the display() function is called 60 times in a second(1000 millisec)
    glutPostRedisplay();

    //1000/60 is the frame rate
    glutTimerFunc(1000/60, timer, 0);

    // Logic A->B->A
    if(isInitial || newPath){
        if(trajectory.cpi==N) {

            angle_x = -60.0;
            angle_y = 180.0;
            angle_z = -90.0;
            //stop when the plane complete one cycle on the trajectory.
            return;
        }

        //%N is to create a cycle  in the path array.
        p1 = trajectory.points[trajectory.cpi];
        p2 = trajectory.points[trajectory.cpi+1];

        distanceBetweenPoints = p.euclideanDistance(p1,p2);

        timeOfPath = distanceBetweenPoints/SPEED;
        numberOfFrames = (timeOfPath*60);
        eachFrameLength = distanceBetweenPoints/numberOfFrames;

        //directionVector = trajectory.directionVector(p1,p2);
        x_direction = (p2.x-p1.x)/distanceBetweenPoints;
        y_direction = (p2.y-p1.y)/distanceBetweenPoints;
        z_direction = (p2.z-p1.z)/distanceBetweenPoints;

        // Update angle

        isInitial = false;
        newPath = false;

        cout<<"-----PATH START-----"<<endl;
    }

    // Direction is much needed (0,0,0)->(5,10,0)

    // Update the check condition
    if((frameindex<=numberOfFrames) && !(checkPrecision(p2.x,x_position) && checkPrecision(p2.y,y_position) && checkPrecision(p2.z,z_position)))
    {

        cout<<"Difference: x-axis: "<<p2.x-x_position<<" y-axis: "<<p2.y-y_position<<" z-axis: "<<p2.z-z_position<<endl;
        cout<<"Current Position: ("<<x_position<<", "<<y_position<<", "<<z_position<<")"<<endl;

        double xRate = (x_direction*eachFrameLength*frameindex),
              yRate = (y_direction*eachFrameLength*frameindex),
              zRate = (z_direction*eachFrameLength*frameindex);

        x_position = x_position + xRate; //0.001
        y_position = y_position + yRate;
        z_position = z_position + zRate;

        frameindex = frameindex + 0.0005;

        cout<<"Next Position: ("<<x_position<<", "<<y_position<<", "<<z_position<<")"<<endl;

    }else{

        // Change x,y,z
        x_position = p2.x;
        y_position = p2.y;
        z_position = p2.z;

        // Update angles
        //updateAngles(x_direction, y_direction, z_direction);
        //angleBetweenVectors(p1,p2);
        updateAngle();

        trajectory.cpi+=1;
        isInitial = false;
        newPath = true;

        cout<<"-----PATH END-----"<<endl;

    }
}

void updateAngle(){
    if(trajectory.cpi == 0){
        angle_x = -50;
        angle_y = 190;
        angle_z = -45;
    }else if(trajectory.cpi == 1){
        angle_x = -50;
        angle_y = 130;
        angle_z = -30;
    }else if(trajectory.cpi == 2){
        angle_x = -50;
        angle_y = 130;
        angle_z = -30;
    }else if(trajectory.cpi > 2 && trajectory.cpi <= N-6){
        angle_x += -5;
        angle_y = -180;
        angle_z = 90;
    }else if(trajectory.cpi == N-5 || trajectory.cpi == N-4){
        angle_x = 40;
        angle_y = 70;
        angle_z = 200;
    }else{
        angle_x = 40;
        angle_y = 30;
        angle_z = 180;
    }
}
