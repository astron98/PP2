//create a square animation.

#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>
#include<bits/stdc++.h>
//#include "trajectory.h"

using namespace std;

class Points{
public:
    float x,y,z;
    Points(float x_1, float y_1, float z_1){

        x=x_1;
        y=y_1;
        z=z_1;

    }
};

class Trajectory{
public:
    vector<Points> points;
    int numberOfPoints;
    Trajectory(vector<Points> &points_input){

        points = points_input;
        numberOfPoints = points.size();

    }
};

void display();
void reshape(int, int);
void timer(int);

void init(){
    glClearColor(0.7,0.7,0.7,1.0);
    glEnable(GL_DEPTH_TEST);
}
int main(int argc, char** argv) {
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

//initial position of x
float x_position = 0.0, y_position = 0.0, z_position = -15.0;
float angle = 0.0;
int state = 1;


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
    glRotatef(angle,0.0,0.0,1.0);

    glShadeModel(GL_SMOOTH);

    //glPointSize(1.0);
    //draw the primistive/ load the model
    glBegin(GL_QUADS);

    //Colors are assigned to vertices
    glColor3f(1.0,0.0,0.0);

    glColor3f(1.0,0.0,0.0);
    glVertex3f(-1.0,1.0,1.0);
    glVertex3f(-1.0,-1.0,1.0);
    glVertex3f(1.0,-1.0,1.0);
    glVertex3f(1.0,1.0,1.0);
    //back
    glColor3f(0.0,1.0,0.0);
    glVertex3f(1.0,1.0,-1.0);
    glVertex3f(1.0,-1.0,-1.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glVertex3f(-1.0,1.0,-1.0);
    //right
    glColor3f(0.0,0.0,1.0);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(1.0,-1.0,1.0);
    glVertex3f(1.0,-1.0,-1.0);
    glVertex3f(1.0,1.0,-1.0);
    //left
    glColor3f(1.0,1.0,0.0);
    glVertex3f(-1.0,1.0,-1.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glVertex3f(-1.0,-1.0,1.0);
    glVertex3f(-1.0,1.0,1.0);
    //top
    glColor3f(0.0,1.0,1.0);
    glVertex3f(-1.0,1.0,-1.0);
    glVertex3f(-1.0,1.0,1.0);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(1.0,1.0,-1.0);
    //bottom
    glColor3f(1.0,0.0,1.0);
    glVertex3f(-1.0,-1.0,-1.0);
    glVertex3f(-1.0,-1.0,1.0);
    glVertex3f(1.0,-1.0,1.0);
    glVertex3f(1.0,-1.0,-1.0);

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
    angle += 0.8;
    if(angle > 360.0){
        angle -=  360.0;
    }


    // Updating trajectory co-ordinates
    switch(state) {
    case 1:
        if(x_position<5){
            x_position+=0.015;
            //y_position = x_position;
            }
        else
            state=-1;
        break;
    case -1:
        if(x_position>-5){
            x_position-=0.015;
            //y_position = x_position;
            }
        else
            state=1;
        break;
    }

}

/*
2D projection:
- always the center will be the orgin

*/



