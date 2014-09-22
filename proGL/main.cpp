//
//  main.cpp
//  proGL
//
//  Created by el1ven on 2/9/14.
//  Copyright (c) 2014 el1ven. All rights reserved.
//

#include <GLUT/GLUT.h>
#include <iostream>
#include <string>
#include <GLUI/GLUI.h>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include "Model.h"

using namespace std;

GLUI *glui;
GLUI_Panel *obj_panel;
int   main_window;

Model *mod = new Model("/Users/el1ven/Desktop/meshes/eight.m");

void init(){

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    GLfloat ambient[] = {1.0f, 1.0f, 1.0f, 0.0f};
    GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 0.0f};
    GLfloat specular[] = {1.0f, 1.0f, 1.0f, 0.0f};
    GLfloat position[] = {-300.0f, 300.0f, 300.0f, 0.0f};
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);//指定深度比较中使用的数值
    glShadeModel(GL_SMOOTH);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat light_position[] = { 0.0, 50.0, 300.0, 0.0 };
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
}

void drawAxis(){//画坐标轴函数
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f,0.0f,-10.0f);
    for(GLfloat i = 0; i < 2; i += 1.0f)
        for (GLfloat j = 0; j < 2*3.14; j+= 1.0f)
        {
            glBegin(GL_QUADS);
            glVertex3f(cos(j), sin(j), i);
            glVertex3f(cos(j+1), sin(j+1), i);
            glVertex3f(cos(j+1), sin(j+1), i+1);
            glVertex3f(cos(j), sin(j), i+1);
            glEnd();
        }
}


void myGlutIdle( void )
{
    /* According to the GLUT specification, the current window is
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
    if (glutGetWindow() != main_window){
        glutSetWindow(main_window);
    }
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glutPostRedisplay();
}


/**************************************** myGlutReshape() *************/

void myGlutReshape( int x, int y )
{
    float xy_aspect;
    xy_aspect = (float)x / (float)y;
    glViewport( 0, 0, x, y );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(60, xy_aspect, 0.3,3000);
    gluLookAt(0, 0, 200*xy_aspect, 0, 0, 0, 0, 1, 0);
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    //glScalef(520.0, 520.0, 520.0);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    mod->drawModel();
    drawAxis();
    glutSwapBuffers();
}



/**************************************** main() ********************/

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition( 0, 0 );
    glutInitWindowSize( 500, 500 );
    main_window = glutCreateWindow( "Mesh Viewer" );
    init();
    glutReshapeFunc( myGlutReshape );
    glutDisplayFunc( myGlutDisplay );

    /*
     //create glui user interface
     GLUI *glui = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_RIGHT);
     
     //obj_panel = new GLUI_Rollout(glui, "Properties", false );
     glui->add_statictext( "Mesh Viewer Options:" );
     
     //glui->set_main_gfx_window( main_window );
     
     //GLUI_Master.set_glutIdleFunc( myGlutIdle );*/
    
    glutMainLoop();
    return 0;
}

