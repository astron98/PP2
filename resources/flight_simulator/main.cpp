#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>
#include<bits/stdc++.h>
#include "trajectory.h"

#define PI 3.1415926535898
#define Cos(th) cos(PI/180*(th))
#define Sin(th) sin(PI/180*(th))
/*  D degrees of rotation */
#define DEF_D 5

using namespace std;


//trajectory class
Trajectory trajectory;

//initial position of x
float x_position = 0.0, y_position = 0.0, z_position = -15.0;
int x_direction = 1, y_direction = 1, z_direction = 1;
float angle = 90.0, angle_x = 1.0, angle_y = 1.0, angle_z = 0.0;
int N = 0;

void display();
void reshape(int, int);
void timer(int);

void updateDirections(){
    x_direction = (trajectory.points[trajectory.cpi].x < trajectory.points[trajectory.cpi+1].x)?1:-1;
}

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
    glutInitWindowSize(500,500);

    glutCreateWindow("Flight");

    //callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    init();

    glutMainLoop();
}

void display() {

    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set background color to black and opaque
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //will reset the coordinate system.
    glLoadIdentity();

    //Translation is the shifting of the origin.
    //Translate first and then draw the object

    // x,y=x- Equation. We can use 3d parabola equation to implement path
    glTranslatef(x_position,y_position,z_position);
    glRotatef(angle,angle_x,angle_y,angle_z);

    glShadeModel(GL_SMOOTH);

    //glPointSize(1.0);
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
    gluPerspective(60,1,2.0,50.0);

    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {

    //even the display() function is called 60 times in a second(1000 millisec)
    glutPostRedisplay();

    //1000/60 is the frame rate
    glutTimerFunc(1000/60, timer, 0);

    // Updating the angle

    // Movement

    // Logic
    /*
        1. Find the distance L :GLOBAL (distance)
        2. Speed is constant   :GLOBAL (speed)
        3. t = L/Speed         :GLOBAL (time)
        4. M = t/(1/60)        :GLOBAL (Number of frames)
        5. S = (L/M)           :GLOBAL ()

        d = trajectory.findLine(trajectory.points[trajectory.cpi], trajectory.points[trajectory.cpi+1])
        6. x_position = trajectory.points[trajectory.cpi].x + (d[0]*i*S)
    */
}




