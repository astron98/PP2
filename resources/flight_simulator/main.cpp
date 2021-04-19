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

// Model vertices
Points v1(0.0, 1.0, 0.0),
       v2(0.0,-1.0, 0.0),
       v3(3.0,-1.0, 0.0),
       v4(4.0,0.0,0.0),
       v5(3.0, 1.0, 0.0);

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
float angle_x = 90.0, angle_y = 90.0, angle_z = 90.0,rotAngle = 90.0;
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

void updateAngle(Points p, Points q) {
   float dotProduct = p.x*q.x + p.y*q.y + p.z*q.z;
   float modP =  sqrt(pow(p.x,2)+pow(p.y,2)+pow(p.z,2));
   float modQ =  sqrt(pow(q.x,2)+pow(q.y,2)+pow(q.z,2));

   rotAngle = acos(dotProduct/ (modP * modQ));

}


// Load the trajectories from a file
void loadTrajectory(){

//    int n=0;
//    cout<<"Enter the number of points: ";
//    cin>>n;

    vector<Points> points {
    Points(0,0,-20),
    Points(15,1,-45),
    Points(25,3,-55),
    Points(2,5,-25),
    Points(1,3,-20),
    Points(-5,5,-30)

    /*Points(-2, -6, -25),
    Points(6,10,-20),
    Points(8,-5,-12),
    Points(-6,2,-20)*/
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

	// Initialise points
	p1.x = points[0].x;
	p1.y = points[0].y;
	p1.z = points[0].z;

	p2.x = points[1].x;
	p2.y = points[1].y;
	p2.z = points[1].z;

	N = points.size();
}

void init(){
    glClearColor(0.7,0.7,0.7,1.0);
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char** argv) {

    loadTrajectory();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(200,100);
    glutInitWindowSize(1920,1200);

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

    glPushMatrix();
    //Translation is the shifting of the origin. - Translate first and then draw the object
    glTranslatef(x_position,y_position,z_position);

    glRotatef(angle_x,1.0,0.0,0.0);
    glRotatef(angle_y,0.0,1.0,0.0);
    glRotatef(angle_z,0.0,0.0,1.0);

    glShadeModel(GL_SMOOTH);


    glColor3f(0.0,1.0,1.0);
    glutWireCone(1,2,4,70);

    /*glBegin(GL_POLYGON);
    glColor3f(0.0,0.0,1.0);
    glVertex3f(v1.x,v1.y,v1.z);
    glVertex3f(v2.x,v2.y,v2.z);
    glColor3f(0.0,1.0,1.0);
    glVertex3f(v3.x,v3.y,v3.z);
    glVertex3f(v4.x,v4.y,v4.z);
    glColor3f(0.0,1.0,1.0);
    glVertex3f(v5.x,v5.y,v5.z);
    glEnd();*/

    // Path Lines
    glLoadIdentity();
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINE_STRIP);
        for(Points &p:trajectory.points){
            glVertex3f(p.x, p.y, p.z);
        }
    glEnd();

    //will display the framebuffer on screen
    glPopMatrix();
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

        // Update model vertices

        isInitial = false;
        newPath = false;
    }


    // Direction is much needed (0,0,0)->(5,10,0)
    if (frameindex < numberOfFrames && !(p2.x-x_position<=0.1 && p2.y-y_position<=0.1 && p2.z-z_direction<=0.1)){

        float xRate = (x_direction*eachFrameLength*frameindex),
              yRate = (y_direction*eachFrameLength*frameindex),
              zRate = (z_direction*eachFrameLength*frameindex);

        x_position = x_position + xRate; //0.001
        y_position = y_position + yRate;
        z_position = z_position + zRate;

        // Vertex update

        frameindex = frameindex + 0.0001;
    }else{

        // Change x,y,z
        x_position = p2.x;
        y_position = p2.y;
        z_position = p2.z;

        angle_x -= 30;
        angle_y += 180;

        //
        trajectory.cpi++;
        isInitial = true;
        newPath = true;
        frameindex = 0.0001;

    }
    cout<<x_position<<" "<<y_position<<" "<<z_position<<endl;

}

