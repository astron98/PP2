//create a square animation.

#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>

void display();
void reshape(int, int);
void timer(int);

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(200,100);
    glutInitWindowSize(500,500);

    glutCreateWindow("Window 1");

//    callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);


    glutMainLoop();
}

float x_position = -10; //initial position of x
int state = 1;  //if state = 1, move right, else move left.

void display() {
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
    glLoadIdentity();   //will reset the coordinate system.

//    glPointSize(1.0);
    //draw
    glBegin(GL_POLYGON);

    //the coordinates should always be in anticlock wise.
//    topleft, bottomleft, bottomright, topright
    glVertex2f(x_position, 1.0);
    glVertex2f(x_position, -1.0);
    glVertex2f(x_position+2.0, -1);
    glVertex2f(x_position+2.0,1);
    glEnd();

    glutSwapBuffers();  //will display the framebuffer on screen
}

void reshape(int w, int h) {
    //v port
    glViewport( 0, 0, (GLsizei)w, (GLsizei)h);
    //projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10,10,-10,10);  //projection window
    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {
    glutPostRedisplay();    //even the display() function is called 60 times in a second(1000 millisec)
    glutTimerFunc(1000/60, timer, 0); //1000/60 is the frame rate

    switch(state) {
    case 1:
        if(x_position<8)
            x_position+=0.15;
        else
            state=-1;
        break;
    case -1:
        if(x_position>-10)
            x_position-=0.15;
        else
            state=1;
        break;
    }

}

/*
2D projection:
- always the center will be the orgin

*/



