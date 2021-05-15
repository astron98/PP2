#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>
#include<bits/stdc++.h>
#include<iostream>
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

    // cout<<"The angles are :"<<angle_x<<" "<<angle_y<<" "<<angle_z<<endl;
}

// void updateAngle(Points p, Points q) {


// }

vector<Points> generateArc(Points center, Points start, Points end, double radius) {
    double tx = start.x;
    cout<< "generateArc is called"<<endl;
    // double ty=400+sqrt((pow(100,2)-pow((tx-200),2)));
    //h,k = center of the circle
    vector<Points> res;
    // for( ; abs(end.x-tx)<=0.1; tx-=0.01) {
    while(abs(end.x-tx)>0.1) {
        double ty = center.y + sqrt(pow(radius,2) - pow(tx - center.x,2));
        res.push_back(Points(tx, ty, -20));
        tx-=0.099999999;
    }

    for(auto &it:res) {
        std::cout << it.x << ","<< it.y <<"\n";
    } 
    cout<< "generateArc is ended"<<endl;
    return res;
}   

// Load the trajectories from a file
void loadTrajectory(){

//    int n=0;
//    cout<<"Enter the number of points: ";
//    cin>>n;

    vector<Points> points {
    //path 3
    //Points(0,0,-20),Points(15,1,-45),Points(25,3,-55),Points(2,5,-25),Points(1,3,-20),Points(-5,5,-30)
    
    /*path 1*/
    // Points(-10.7,-11.32,-20),
    // Points(-2,-11.32,-20),
    // // new points
    // Points(-0.4,-10.57, -20),   //c
    // Points(1.7, -9.7,-20),  
    // Points(3.7, -8.7,-20),
    // Points(5.5, -7.8,-20),
    // Points(6.5, -6.5,-20),
    // Points(7.2, -5.5,-20),
    // Points(7.19, -5.0,-20),
    // Points(7.15, -4.7,-20),
    // Points(7.11, -3,  -20),  //steep upward
    // Points(7.1, -2.9,-20),  
    // Points(6.7, 0.5, -20),
    // Points(6.2, 4 ,-20),
    // Points(5.7, 6 ,-20),
    // Points(4.5, 8, -20),  //c
    // Points(3.3, 8.2, -20),  //topmost tip
    // Points(2.0, 8.15, -20),
    // Points(1.3, 8.4, -20),
    // Points(-1, 8.6, -20),
    // Points(-2, 7.5, -20),   //c
    // // Points(-2.5,8.1,-20),
    // // Points(-3, 8.0, -20),
    // // Points(-3.3, 7.3, -20),
    // // Points(-3.7, 6.6,-20),
    

    // // Points(6,-7,-20),
    // // Points(8,0,-20),
    // // Points(6,10,-35),
    // // Points(3,13,-35),
    // // Points(-3,8,-25),
    // Points(-4,6,-24),
    // Points(-7,2,-25),
    // Points(-10.5,-10,-20)
    /*path1 end*/

    // path2
        Points(-10.7,-11.32,-20),Points(-2,-11.32,-20),Points(6,-7,-20),Points(7.9,1,-20),/*Points(6,10,-35),*/Points(2,6,-20),Points(-3,8,-25),Points(-4,6,-24),Points(-7,2,-25),Points(-10.7,-11.32,-20)
    //path2 end

    //Points(-12,-13,-20),Points(-5,-13,-20),Points(5,-7,-20),Points(-4,-4,-20),Points(-6,-8,-20),Points(-10,-11,-20)

    };

    //arc points
    double radius = 6;
    Points center = Points(2,0,-20), start = points[3], end = points[4];

    vector<Points> arcArr = generateArc(center, start, end, radius);
    // points.remove(4);
    auto it = points.begin();
    auto itArc = arcArr.begin();
    // for(int i=0;i<arcArr.size();i++) {
        // points.insert(it +3+i+1, arcArr[i]);
        points.insert(it+4, arcArr.begin()+1, arcArr.end());
        // std::cout << points[4+i].x << ","<< points[4+i].y <<"\n";
    // }


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
    double difference = abs(point_i-point_f);
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

        // cout<<"-----PATH START-----"<<endl;
    }

    // Direction is much needed (0,0,0)->(5,10,0)

    // Update the check condition
    if((frameindex<=numberOfFrames) && !(checkPrecision(p2.x,x_position) && checkPrecision(p2.y,y_position) && checkPrecision(p2.z,z_position)))
    {

        // cout<<"\n Difference: x-axis: "<<p2.x-x_position<<" y-axis: "<<p2.y-y_position<<" z-axis: "<<p2.z-z_position<<"\n"<<endl;
        // cout<<"Current Position: ("<<x_position<<", "<<y_position<<", "<<z_position<<")"<<endl;

        double xRate = (x_direction*eachFrameLength*frameindex),
              yRate = (y_direction*eachFrameLength*frameindex),
              zRate = (z_direction*eachFrameLength*frameindex);

        x_position = x_position + xRate; //0.001
        y_position = y_position + yRate;
        z_position = z_position + zRate;

        frameindex = frameindex + 0.0001;

        // cout<<"Next Position: ("<<x_position<<", "<<y_position<<", "<<z_position<<")"<<endl;

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

        // cout<<"-----PATH END-----"<<endl;

    }

}
