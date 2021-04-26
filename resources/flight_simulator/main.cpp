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

#define SPEED 150.0

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
double frameindex = 0.0;
double eachFrameLength = 0.0;
vector<double> directionVector;

//initial position of x
double x_position = 0.0, y_position = 0.0, z_position = -20.0;
double angle_x = 60.0, angle_y = 0.0, angle_z = -90.0,rotAngle = 90.0;
double x_direction = 0, y_direction = 0, z_direction = 0;
int N = 0;

void display();
void reshape(int, int);
void timer(int);


// Under development
void updateAngles(double Ax, double Ay, double Az){


    double A = sqrt(pow(Ax,2)+pow(Ay,2)+pow(Az,2));

    angle_x = angle_x+acos(Ax/A)*10;
    angle_y = angle_y+acos(Ay/A)*5;
    angle_z = angle_z-acos(Az/A)*20;

    cout<<"The angles are :"<<angle_x<<" "<<angle_y<<" "<<angle_z<<endl;
}

void updateAngle(Points p, Points q) {


}


// Load the trajectories from a file
void loadTrajectory(){

//    int n=0;
//    cout<<"Enter the number of points: ";
//    cin>>n;

    vector<Points> points {
    //Points(0,0,-20),Points(15,1,-45),Points(25,3,-55),Points(2,5,-25),Points(1,3,-20),Points(-5,5,-30)
    Points(-10.7,-11.32,-20),Points(-2,-11.32,-20),Points(6,-7,-20),Points(8,0,-20),Points(6,10,-35),Points(3,13,-35),Points(-3,8,-25),Points(-4,6,-24),Points(-7,2,-25),Points(-10.5,-10,-20)
    //Points(-12,-13,-20),Points(-5,-13,-20),Points(5,-7,-20),Points(-4,-4,-20),Points(-6,-8,-20),Points(-10,-11,-20)

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

    trajectory.cpi = 0;
	trajectory.points = points;
	trajectory.points.push_back(points[0]);

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

    glRotatef(angle_x,1.0,0.0,0.0);
    glRotatef(angle_y,0.0,1.0,0.0);
    glRotatef(angle_z,0.0,0.0,1.0);

    glShadeModel(GL_SMOOTH);

    glPushMatrix();
        glScalef(.5,1,.5);
        glColor3f(0.0,0.0,1.0);
        glutSolidCube(1);
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


    // Path Lines
    glLoadIdentity();
    glColor3f(1.0,0.0,0.0);
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

    //projection-window
    gluPerspective(60,1,1.0,500.0);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

bool checkPrecision(double point_i, double point_f){

    bool result=false;
    double difference = (point_i-point_f);
    return (difference<=0.1);

}

bool findMaxDifference(double a, double b){
    return (a<b);
}

/*
double maxDifference = std::max({p2.x-x_position, p2.y-y_position, p2.z-z_position},findMaxDifference);
*/

void timer(int) {

    //even the display() function is called 60 times in a second(1000 millisec)
    glutPostRedisplay();

    //1000/60 is the frame rate
    glutTimerFunc(1000/60, timer, 0);

    // Logic A->B->A
    if(isInitial || newPath){
        if(trajectory.cpi==N) {

            angle_x = 60.0;
            angle_y = 0.0;
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

        isInitial = false;
        newPath = false;

        cout<<"-----PATH START-----"<<endl;
    }

    // Direction is much needed (0,0,0)->(5,10,0)

    // Update the check condition
    if((frameindex<=numberOfFrames) && !(checkPrecision(p2.x,x_position) && checkPrecision(p2.y,y_position) && checkPrecision(p2.z,z_position)))
    {

        cout<<"\n Difference: x-axis: "<<p2.x-x_position<<" y-axis: "<<p2.y-y_position<<" z-axis: "<<p2.z-z_position<<"\n"<<endl;
        cout<<"Current Position: ("<<x_position<<", "<<y_position<<", "<<z_position<<")"<<endl;

        double xRate = (x_direction*eachFrameLength*frameindex),
              yRate = (y_direction*eachFrameLength*frameindex),
              zRate = (z_direction*eachFrameLength*frameindex);

        x_position = x_position + xRate; //0.001
        y_position = y_position + yRate;
        z_position = z_position + zRate;

        frameindex = frameindex + 0.0004;

        cout<<"Next Position: ("<<x_position<<", "<<y_position<<", "<<z_position<<")"<<endl;

    }else{

        // Change x,y,z
        x_position = p2.x;
        y_position = p2.y;
        z_position = p2.z;

        //
        updateAngles(x_direction, y_direction, z_direction);

        trajectory.cpi+=1;
        isInitial = false;
        newPath = true;

        //frameindex = 0.0;

        cout<<"-----PATH END-----"<<endl;

    }

}

