#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>
#include<bits/stdc++.h>
#include<iomanip>
#include "trajectory.h"

#define PI 3.1415926535898
#define Cos(th) cos(PI/180*(th))
#define Sin(th) sin(PI/180*(th))
/*  D degrees of rotation */
#define DEF_D 5

#define SPEED 200.0

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
double frameindex = 0.0001;
double eachFrameLength = 0.0;
vector<float> directionVector;

//initial position of x
float x_position = 0.0, y_position = 0.0, z_position = -20.0;
float angle = 90.0, angle_x = 0.0, angle_y = 1.0, angle_z = 0.0;
int x_direction = 0, y_direction = 0, z_direction = 0;
int N = 0;

void display();
void reshape(int, int);
void timer(int);


void takeUserInput(){

    int n=0;
    cout<<"Enter the number of points: ";
    cin>>n;

    vector<Points> points;

    Points point;
    for(int i=0;i<n;i++){
        cout<<"Enter x, y, z for point "<<(i+1)<<endl;
        cin>>point.x>>point.y>>point.z;
        points.push_back(point);
    }

    // Initial position
    x_position = points[0].x;
    y_position = points[0].y;

	trajectory.points = points;
	trajectory.points.push_back(points[0]);
	N = points.size();
}

void init(){
    glClearColor(0.7,0.7,0.7,1.0);
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv) {

    takeUserInput();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(200,100);
    glutInitWindowSize(1000,1000);

    glutCreateWindow("Flight");

    //callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    init();

    glutMainLoop();
}

void display() {

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //will reset the coordinate system.
    glLoadIdentity();

    //Translation is the shifting of the origin.
    //Translate first and then draw the object
    glTranslatef(x_position,y_position,z_position);
    glRotatef(angle,angle_x,angle_y,angle_z);

    glShadeModel(GL_SMOOTH);

    //draw the primistive/ load the model
    glBegin(GL_TRIANGLES);

    //Colors are assigned to vertices
    for (int k=0;k<=360;k+=DEF_D){
      glColor3f(0.0,0.0,1.0);
      glVertex3f(0,0,1);
      glColor3f(0.0,1.0,0.0);
      glVertex3f(Cos(k),Sin(k),0);
      glColor3f(1.0,0.0,0.0);
      glVertex3f(Cos(k+DEF_D),Sin(k+DEF_D),0);
    }

    glEnd();
    //will display the framebuffer on screen
    glutSwapBuffers();
}

void reshape(int w, int h) {
    //v port
    glViewport( 0, 0, (GLsizei)w, (GLsizei)h);

    //projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //projection window
    gluPerspective(60,1,1.0,500.0);

    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {

    //even the display() function is called 60 times in a second(1000 millisec)
    glutPostRedisplay();

    //1000/60 is the frame rate
    glutTimerFunc(1000/60, timer, 0);

    // Updating the angle


    // Logic A->B->A
    if(isInitial || newPath){
        if(trajectory.cpi==N) {
            //stop when the plane complete one cycle on the trajectory.
            return;
        }

        //%N is to create a cycle  in the path array.
        p1 = trajectory.points[trajectory.cpi%N];
        p2 = trajectory.points[(trajectory.cpi+1)%N];

        distanceBetweenPoints = p.euclideanDistance(p1,p2);

        timeOfPath = distanceBetweenPoints/SPEED;
        numberOfFrames = (timeOfPath*60);
        eachFrameLength = distanceBetweenPoints/numberOfFrames;

        //directionVector = trajectory.directionVector(p1,p2);
        x_direction = (p2.x-p1.x);
        y_direction = (p2.y-p1.y);
        isInitial = false;
        newPath = false;
    }


    // Direction is much needed (0,0,0)->(5,10,0)
    if (frameindex < numberOfFrames && !(p2.x-x_position<=0.01 && p2.y-y_position<=0.01)){
        x_position = x_position + (x_direction*eachFrameLength*frameindex); //0.001
        y_position = y_position + (y_direction*eachFrameLength*frameindex);

        frameindex = frameindex + 0.0001;
    }else{

        // Change x,y,z
        x_position = p2.x;
        y_position = p2.y;

        trajectory.cpi++;
        isInitial = true;
        newPath = true;
        frameindex = 0.0001;

    }
    cout<<x_position<<" "<<y_position<<endl;

}




