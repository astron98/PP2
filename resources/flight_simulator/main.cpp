#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>
#include<bits/stdc++.h>
#include "trajectory.h"
#include<cmath>
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
float angle_x = 90.0, angle_y = 90.0, angle_z = 90.0;
float x_direction = 0, y_direction = 0, z_direction = 0;
int N = 0;

void display();
void reshape(int, int);
void timer(int);


// Under development
void updateAngles(float Ax, float Ay, float Az){

    float A = sqrt(pow(Ax,2)+pow(Ay,2)+pow(Az,2));

    angle_x = acos(Ax/A);
    angle_y = acos(Ay/A);
    angle_z = acos(Az/A);

    cout<<"The angles are :"<<angle_x<<" "<<angle_y<<" "<<angle_z<<endl;
}

// Load the trajectories from a file
void loadTrajectory(){

//    int n=0;
//    cout<<"Enter the number of points: ";
//    cin>>n;

    vector<Points> points {
    /*Points(0,0,-20),
    Points(3,1,-25),
    Points(4,3,-30),
    Points(2,5,-25),
    Points(1,3,-20),
    Points(-5,5,-30)*/

    Points(-2, -6, -25),
    Points(6,10,-20),
    Points(8,-5,-12),
    Points(-6,2,-20)
    };
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

	trajectory.points = points;
	trajectory.points.push_back(points[0]);
	N = points.size();
}

void init(){
    glClearColor(0.7,0.7,0.7,1.0);
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(800,500);
    win_id = glutCreateWindow("Flight");



    glutCreateMenu(StartMenu);
    glutAddMenuEntry("Start",1);
    glutAddMenuEntry("Exit",2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(display);
    glutMainLoop();

}

void StartMenu(int n){
    switch(n)
    {
    case 1 :
    	glutDestroyWindow(glutGetWindow());
    	glLoadIdentity();
    	loadTrajectory();
    	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    	glutInitWindowPosition(0,0);
    	glutInitWindowSize(1920,1080);
    	glutCreateWindow("Flight");
    	glutDisplayFunc(display);
    	glutReshapeFunc(reshape);
    	glutTimerFunc(0, timer, 0);
    	init();

    break;

    case 2 :
    	glutDestroyWindow(glutGetWindow());
    break;
    }
    glutPostRedisplay();
}

void display() {

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //will reset the coordinate system.
    glLoadIdentity();

    //Translation is the shifting of the origin. - Translate first and then draw the object
    glTranslatef(x_position,y_position,z_position);
    glRotatef(angle_x,1.0,0.0,0.0);
    glRotatef(angle_y,0.0,1.0,0.0);
    glRotatef(angle_z,0.0,0.0,1.0);

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

    // Path Lines
    glLoadIdentity();
    glBegin(GL_LINE_STRIP);
        for(Points &p:trajectory.points){
            glVertex3f(p.x, p.y, p.z);
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
    gluPerspective(90,1,1.0,500.0);

    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {

    //even the display() function is called 60 times in a second(1000 millisec)
    glutPostRedisplay();

    //1000/60 is the frame rate
    glutTimerFunc(1000/60, timer, 0);

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
        z_direction = (p2.z-p1.z);

        // Update the angles
        //updateAngles(x_direction, y_direction, z_direction);

        isInitial = false;
        newPath = false;
    }


    // Direction is much needed (0,0,0)->(5,10,0)
    if (frameindex < numberOfFrames && !(p2.x-x_position<=0.1 && p2.y-y_position<=0.1 && p2.z-z_direction<=0.1)){
        x_position = x_position + (x_direction*eachFrameLength*frameindex); //0.001
        y_position = y_position + (y_direction*eachFrameLength*frameindex);
        z_position = z_position + (z_direction*eachFrameLength*frameindex);

        frameindex = frameindex + 0.0001;
    }else{

        // Change x,y,z
        x_position = p2.x;
        y_position = p2.y;
        z_position = p2.z;

        trajectory.cpi++;
        isInitial = true;
        newPath = true;
        frameindex = 0.0001;

    }
    cout<<x_position<<" "<<y_position<<" "<<z_position<<endl;

}
