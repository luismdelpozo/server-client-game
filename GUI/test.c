#include <GL/glut.h>
#include <stdio.h>
#include "gui_config.h"

float alpha, beta;
int x0, y0; 
int x_win,  y_win;
double x3D, y3D, z3D;
double x_focus, y_focus;

void focus(void){    
    if(x3D >= 48 && x3D <= 216 && y3D >= 48 && y3D <= 216){
        printf("raton 2 x: %d, y: %d\n", (int) x3D, (int) y3D);
        if (x_focus != x3D || y_focus != y3D){
            int x_ini = (x_focus - 48)/14;
            int y_ini = (y_focus - 48)/14;
            
            //glPushMatrix();
            //glColor3s(0.0f, 0.0f, 1.0f);
            glColor3f(0.0f, 0.0f, 0.0f); 
            glBegin(GL_POLYGON);
                glVertex2d(x_ini * 14 + 48 + 1, y_ini * 14 + 48 + 1);
                glVertex2d(x_ini * 14 + 14 + 48 - 1, y_ini * 14 + 48 + 1);
                glVertex2d(x_ini * 14 + 14 + 48 - 1, y_ini * 14 + 14 + 48 - 1);
                glVertex2d(x_ini * 14 + 48 + 1, y_ini * 14 + 14 + 48 - 1);            
            //glLineWidth(4);
            glEnd();        
            glFlush();
            glutSwapBuffers();
        
            x_ini = (x3D - 48)/14;
            y_ini = (y3D - 48)/14;
            
            //glPushMatrix();
            //glColor3s(0.0f, 0.0f, 1.0f);
            glColor3f(1.0f, 0.0f, 0.0f); 
            glBegin(GL_POLYGON);
                glVertex2d(x_ini * 14 + 48 + 1, y_ini * 14 + 48 + 1);
                glVertex2d(x_ini * 14 + 14 + 48 - 1, y_ini * 14 + 48 + 1);
                glVertex2d(x_ini * 14 + 14 + 48 - 1, y_ini * 14 + 14 + 48 - 1);
                glVertex2d(x_ini * 14 + 48 + 1, y_ini * 14 + 14 + 48 - 1);            
            //glLineWidth(4);
            glEnd();        
            glFlush();
            glutSwapBuffers();

            x_focus = x3D;
            y_focus = y3D;
        }
    }
}

void displayMe(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,264,0,264,-10,10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    // Horizontal lines
    for (int i = 0; i <= MAX_MAP_ROWS; i ++){
        glPushMatrix();
        glColor3f(1.0f, 1.0f, 1.0f); 
        glBegin(GL_LINES);
            glVertex2f(48, 48 + i * 14);
            glVertex2f(216, 48 + i * 14);
        glEnd();
        glPopMatrix();
    }     
    // Vertical lines
    for (int i = 0; i <= MAX_MAP_COLS; i ++){
        glPushMatrix();
        glColor3f(1.0f, 1.0f, 1.0f); 
        glBegin(GL_LINES);
            glVertex2f(48 + i * 14, 216);
            glVertex2f(48 + i * 14, 48);
        glEnd();
        glPopMatrix();
    }    
    glFlush();
    glutSwapBuffers(); 
}

void UnProject(int x, int y)
{
    double modelview[16], projection[16];
    int viewport[4];
    float z = 1;

    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadPixels(x, viewport[3] - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z); //Read the window z value from the z-buffer
    gluUnProject(x, viewport[3] - y, z, modelview, projection, viewport, &x3D, &y3D, &z3D);
}

void onMouse(int button, int state, int x_win_p, int y_win_p) {
    UnProject(x_win_p, y_win_p);
    printf("x: %d, y: %d\n", (int) x3D, (int) y3D);
}

void onMotion(int x, int y) {
    UnProject(x,y);
    printf("raton 1 x: %d, y: %d\n", (int) x3D, (int) y3D);
    focus();
    //glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(700, 700);
    int screen_width = glutGet(GLUT_SCREEN_WIDTH);
    int screen_height = glutGet(GLUT_SCREEN_HEIGHT);
    printf("Width: %d, Height: %d\n", screen_width, screen_height);
    glutInitWindowPosition((screen_width - 700)/2 ,screen_height/2);
    glutCreateWindow("New window");
    //glutFullScreen();
    glutDisplayFunc(displayMe);
    glutMouseFunc(onMouse);
    //glutMotionFunc(onMotion);
    glutPassiveMotionFunc(onMotion);
    glutMainLoop();
    return 0;
}