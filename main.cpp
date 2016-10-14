/*============================================================================
 * @author     : Jae Yong Lee (leejaeyong7@gmail.com)
 * @file       : main.cpp
 * @brief      : Entry point example for TagViewer
 * Copyright (c) Jae Yong Lee / UIUC Spring 2016
 =============================================================================*/
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include "TagViewer.h"
#include <iostream>
//----------------------------------------------------------------------------//
//                                END INCLUDES                                //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                        GLOBAL VARIABLE DEFINITIONS                         //
//----------------------------------------------------------------------------//
TagViewer * tv = 0;
using namespace std;
//----------------------------------------------------------------------------//
//                      END GLOBAL VARIABLE DEFINITIONS                       //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                           END POINTER DEFINITION                           //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                             CALLBACK FUNCTIONS                             //
//----------------------------------------------------------------------------//
void display(void){
    tv->displayCB();
};
void reshape(GLint w, GLint h){
    tv->reshapeCB(w,h);
};

void keyboard(unsigned char key, int x, int y){
    tv->keyboardCB(key,x,y);
};
void mouse(int button, int state, int x, int y){
    tv->mouseDownCB(button,state,x,y);
};
void motion(int x, int y){
    tv->mouseMoveCB(x,y);
};
void idle(void){
    tv->idleCB();
};
//----------------------------------------------------------------------------//
//                           END CALLBACK FUNCTIONS                           //
//----------------------------------------------------------------------------//

int main(int argv, char** argc){
    tv = new TagViewer(800,600);
    tv->initWindow(argv,argc);
    

    // defining test cases
    double pos1[3] = {10,0,6};
    double pos2[3] = {0,10,2};
    double pos3[3] = {0,0,0};
    double pos4[3] = {0,0,0};
    
    double rot1[4] = {1,1,1,1};
    double rot2[4] = {1,0,1,1};
    double rot3[4] = {0,0,0,1};
    double rot4[4] = {0,0,0,1};
    
    tv->addCamera(pos1,rot1);
    tv->addCamera(pos2,rot2);
    tv->addCamera(pos3,rot3);
    tv->setTagOrigin(pos4,rot4);


    // Register callbacks:
    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    glutKeyboardFunc (keyboard);
    glutMouseFunc (mouse);
    glutMotionFunc (motion);
    glutIdleFunc (idle);   

    while(1){
        tv->renderFrame();
    };
    return 0;
    
}
