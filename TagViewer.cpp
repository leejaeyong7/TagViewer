/*============================================================================
 * @author     : Jae Yong Lee (leejaeyong7@gmail.com)
 * @file       : TagViewer.cpp
 * @brief      : Definition file for Tag Viewer using OpenGL / GLUT library
 * Copyright (c) Jae Yong Lee / UIUC Fall 2016
 =============================================================================*/
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include <iostream>
#include "TagViewer.h"
//----------------------------------------------------------------------------//
//                                END INCLUDES                                //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                              CLASS DEFINITION                              //
//----------------------------------------------------------------------------//
/**
 * @brief Default Constructor. Initializes all private variables
 * @action initializes all private variables
 */
TagViewer::TagViewer(){
    // initialize OpenGL running Environment
    mouseId = -1;
    mouseDown = false;
    mouse_x = -1;
    mouse_y = -1;

    width = 1;
    height = 1;
};
/**
 * @brief Constructor with window sizes
 * @param w width
 * @param h height
 * @action same as constructor
 */
TagViewer::TagViewer(int w, int h){
    // initialize OpenGL running Environment
    mouseId = -1;
    mouseDown = false;
    mouse_x = -1;
    mouse_y = -1;

    width = w;
    height = h;
    worldCamera.setAspect((double)w / h);
};

/**
 * @brief Destructor. 
 * @action Deletes all memory allocated if exists
 */
TagViewer::~TagViewer(){
    // currently nothing really
};

/**
 * @brief Copy constructor
 * @action Defines new Tagviewer using other Tagviewr instance
 */
TagViewer::TagViewer(const TagViewer & obj){
    mouseId = -1;
    mouseDown = false;
    mouse_x = -1;
    mouse_y = -1;

    width = obj.width;
    height = obj.height;
    // needs to update worldcamera using its own copy constructor..
    /* worldCamera = worldCamera; */
    /* worldCamera.setAspect((double)w / h); */
    
};

/**
 * @brief assignment operator
 * @action Copy constructor using assignment = operator
 */
TagViewer & TagViewer::operator=(const TagViewer & tv){
    //!! needs to write it (Not implemented yet)
    TagViewer * t = new TagViewer();
    return *t;
};

    

/**
 * @brief adds camera node to scene.
 * @param position double array of size 3 representing x,y,z
 * @param rotation double array of size 4 representing x,y,z,w
 * @action adds camera to vector of object nodes used for drawing camera 
 *         frustum in scene
 */
void TagViewer::addCamera(double * position, double * rotation){
    ObjectNode o;
    copy(position, position+3, o.position);
    copy(rotation, rotation+4, o.rotation);
    cameras.push_back(o);
};

/**
 * @brief Sets Tag origin
 * @param position double array of size 3 representing x,y,z
 * @param rotation double array of size 4 representing x,y,z,w
 * @action sets camera orbit origin as this position
 */
void TagViewer::setTagOrigin(double * position, double * rotation){
    copy(position, position+3, Tag.position);
    copy(position, position+3, worldCamera.target);
    copy(rotation, rotation+4, Tag.rotation);
};


/**
 * @brief updates world camera position using r, theta, distance
 * @action sets camera position
 */
void TagViewer::updateWorldCameraPosition(){
    double x = worldCamera.target[0];
    double y = worldCamera.target[1];
    double z = worldCamera.target[2];
    double d = worldCamera.distance;
    double r = worldCamera.r;
    double t = worldCamera.theta;
    
    worldCamera.position[0] = x + d * cos(r) * cos(t);
    worldCamera.position[1] = y + d * sin(r);
    worldCamera.position[2] = z + d * cos(r) * sin(t);
};
    
/**
 * @brief initializes opengl window
 * @param argv,argc for glutinit
 * @action sets up window
 */
void TagViewer::initWindow(int & argv, char** argc){
    // GLUT Window Initialization:
    glutInit (&argv,argc);
    glutInitWindowSize (width, height);
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow ("Tag Viewer");

    // Initialize OpenGL graphics state
    glClearColor(1.0,1.0,1.0,1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_MULTISAMPLE);

};
    
/**
 * @brief renders one frame(should be called in loop)
 */
void TagViewer::renderFrame(){
    glutMainLoopEvent();
};


/**
 * @brief draws frustum using obj's pos / rot and pyramid helper function
 * @param obj object node with position and rotation
 */
void TagViewer::drawFrustum(ObjectNode & obj){
    glTranslated(obj.position[0],obj.position[1],obj.position[2]);
    // multiply rotation matrix using quaternion
    applyQuaternion(obj.rotation);

    // draw frustum
    pyramid();
};

/**
 * @brief draws tag using obj's pos / rot and pyramid helper function
 * @param obj object node with position and rotation
 */
void TagViewer::drawTag(ObjectNode & obj){
    glTranslated(obj.position[0],obj.position[1],obj.position[2]);
    // multiply rotation matrix using quaternion
    applyQuaternion(obj.rotation);

    // draw tag
    square();

};

/**
 * @brief applies quaternion rotation to OpenGL matrix
 * @param quat double[4] array of quaternion
 * @action applies rotation using glMultMat
 */
void TagViewer::applyQuaternion(double * quat){
    // normalize quaternion
    double x = quat[0];
    double y = quat[1]; 
    double z = quat[2]; 
    double w = quat[3]; 
    double n = 1.0 / sqrt(x*x+y*y + z*z + w*w);

    x *= n;
    y *= n;
    z *= n;
    w *= n;
    double m[] = {
        1 - 2*y*y - 2*z*z,   2*x*y - 2*w*z  , 2*x*y + 2*w*y    , 0.0, 
        2*x*y + 2*w*z    , 1 - 2*x*x - 2*z*z, 2*y*z - 2*w*x    , 0.0, 
        2*x*z - 2*w*y    , 2*y*z + 2*w*x    , 1 - 2*x*x - 2*y*y, 0.0, 
        0.0              , 0.0              , 0.0              , 1.0
    };
    // apply quaternion rotation
    glMultMatrixd(m);
};

//----------------------------------------------------------------------------//
//                            END CLASS DEFINITION                            //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                              HELPER FUNCTIONS                              //
//----------------------------------------------------------------------------//
void TagViewer::pyramid()
{
    glBegin( GL_TRIANGLES );

    glColor3f( 0.0f, 1.0f, 1.0f ); glVertex3f( 0.0f, 1.f, 0.0f );
    glColor3f( 0.0f, 1.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, 1.0f );
    glColor3f( 0.0f, 0.0f, 1.0f ); glVertex3f( 1.0f, -1.0f, 1.0f);

    glColor3f( 0.0f, 1.0f, 1.0f ); glVertex3f( 0.0f, 1.0f, 0.0f);
    glColor3f( 0.0f, 1.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, 1.0f);
    glColor3f( 0.0f, 0.0f, 1.0f ); glVertex3f( 0.0f, -1.0f, -1.0f);

    glColor3f( 0.0f, 1.0f, 1.0f ); glVertex3f( 0.0f, 1.0f, 0.0f);
    glColor3f( 0.0f, 1.0f, 0.0f ); glVertex3f( 0.0f, -1.0f, -1.0f);
    glColor3f( 0.0f, 0.0f, 1.0f ); glVertex3f( 1.0f, -1.0f, 1.0f);


    glColor3f( 0.0f, 1.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, 1.0f);
    glColor3f( 0.0f, 1.0f, 0.0f ); glVertex3f( 0.0f, -1.0f, -1.0f);
    glColor3f( 0.0f, 0.0f, 1.0f ); glVertex3f( 1.0f, -1.0f, 1.0f);

    glEnd();

}
void TagViewer::square(){
    glBegin(GL_TRIANGLES);
    // set color
    glColor3f( 1.0f, 0.0f, 0.0f );

    // set vertices
    glVertex3f( 1.0f, 0.0f, 1.0f);
    glVertex3f( 1.0f, 0.0f, -1.0f);
    glVertex3f( -1.0f, 0.0f, 1.0f);

    glVertex3f( -1.0f, 0.0f, 1.0f);
    glVertex3f( 1.0f, 0.0f, -1.0f);
    glVertex3f( -1.0f, 0.0f, -1.0f);
    glEnd();
}
//----------------------------------------------------------------------------//
//                            END HELPER FUNCTIONS                            //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                             CALLBACK FUNCTIONS                             //
//----------------------------------------------------------------------------//
/**
 * @brief displays callback function for drawing scene
 * @action Clears scnee and draws all objects
 */
void TagViewer::displayCB(void){
    // clear color for drawing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    updateWorldCameraPosition();
    gluLookAt(
        worldCamera.position[0], worldCamera.position[1], worldCamera.position[2],
        worldCamera.target[0], worldCamera.target[1], worldCamera.target[2],
        0.0, 1.0, 0.0);

    // setup camera
    vector<ObjectNode>::iterator it;
    for(it = cameras.begin(); it != cameras.end(); it++){
        glPushMatrix();
        drawFrustum(*it);
        glPopMatrix();
    }
    glPushMatrix();
    drawTag(Tag);
    glPopMatrix();

    // swap buffers to redraw scene
    glutSwapBuffers();
};


/**
 * @brief callback function for reshaping window
 * @param  w width of new window
 * @param  h height of new window
 * @action updates camera aspect, resets perspective matrix / viewport
 */
void TagViewer::reshapeCB(GLint w, GLint h){
    // reset width / height
    width = w;
    height = h;
    worldCamera.setAspect((double)width / height);
    // setup viewport for world camera 
    glViewport(0, 0, width, height);

    // reset perspective projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        worldCamera.getFOVY(),
        worldCamera.getAspect(),
        worldCamera.getNear(),
        worldCamera.getFar());

    // return to model view matrix
    glMatrixMode(GL_MODELVIEW);
};
/**
 * @brief Handles keyboard event callback
 */
void TagViewer::keyboardCB(unsigned char key, int x, int y){
    // not implemented yet
    return;
};

/**
 * @brief mouse down/up event
 */
void TagViewer::mouseDownCB(int button, int state, int x, int y){
    // left button
    if (button == GLUT_LEFT_BUTTON)
    {
        mouseDown = (state == GLUT_DOWN);
        if(mouseDown){
            // down
            mouseId = 0;
            mouse_x = x;
            mouse_y = y;
        } else {
            // up
            mouseId = -1;
            mouse_x = -1;
            mouse_y = -1;
        }
    } else if (button == GLUT_RIGHT_BUTTON){
        // right button
        mouseDown = (state == GLUT_DOWN);
        if(mouseDown){
            // down
            mouseId = 1;
            mouse_x = x;
            mouse_y = y;
        } else {
            //up
            mouseId = -1;
            mouse_x = -1;
            mouse_y = -1;
        }
    } else {
        if(button == 3){
            // wheel up
            worldCamera.distance += 0.3;
            glutPostRedisplay();
        } else if(button == 4) {
            // wheel down
            worldCamera.distance -= 0.3;
            if(worldCamera.distance < 0.1){
                worldCamera.distance = 0.1;
            }
            glutPostRedisplay();
        }
    }
    return;
};

/**
 * @brief mouse move event handler
 */
void TagViewer::mouseMoveCB(int x, int y){
    if(!mouseDown){
        return; 
    }
    double dx = (double)(x - mouse_x)/width;
    double dy = (double)(y - mouse_y)/height;
    
    // add dy to r
    worldCamera.r += dy;
    if(worldCamera.r > PI/2){
        worldCamera.r = PI/2;
    } else if (worldCamera.r < -PI/2){
        worldCamera.r = -PI/2;
    }
    
    // add dx to theta;
    worldCamera.theta += dx;
    if(worldCamera.theta > 2*PI){
        worldCamera.theta  -= 2*PI;
    } else if (worldCamera.theta < 0){
        worldCamera.theta  += 2*PI;
    }

    glutPostRedisplay();
    mouse_x = x;
    mouse_y = y;
    return;
};

/**
 * @brief called every time when nothing is happening
 */
void TagViewer::idleCB(void){
    // not implemented yet
    return;
};
//----------------------------------------------------------------------------//
//                           END CALLBACK FUNCTIONS                           //
//----------------------------------------------------------------------------//
