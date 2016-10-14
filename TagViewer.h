/*============================================================================
 * @author     : Jae Yong Lee (leejaeyong7@gmail.com)
 * @file       : TagViewer.h
 * @brief      : Tag viewer class using OpenGL / GLUT
 * Copyright (c) Jae Yong Lee / UIUC Fall 2016
 =============================================================================*/
//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//
#include <GL/freeglut.h>
#include <vector>
#include <iterator>
#include <cmath>
#define PI 3.1415926535
//----------------------------------------------------------------------------//
//                                END INCLUDES                                //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                           NAMESPACE DECLARATIONS                           //
//----------------------------------------------------------------------------//
using namespace std;
//----------------------------------------------------------------------------//
//                         END NAMESPACE DECLARATIONS                         //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                          HELPER CLASS DEFINITION                           //
//----------------------------------------------------------------------------//
// class holding world camera methods
class WorldCamera{
public:
    // initial valued of world camera
    WorldCamera(){
        fovy = 65.0;
        aspect = 1;
        zNear = 0.1;
        zFar = 10000.0;
        r = 0;
        theta = 0;
        distance = 1;
    };
    // setters and getters
    void setFOVY(double _fovy) {fovy = (GLdouble)_fovy;};
    void setAspect(double _aspect) {aspect = (GLdouble)_aspect;};
    void setNear(double _zNear) {zNear = (GLdouble)_zNear;};
    void setFar(double _zFar) {zFar = (GLdouble) _zFar;};

    GLdouble getFOVY() { return fovy; };
    GLdouble getAspect() { return aspect; };
    GLdouble getNear() { return zNear; };
    GLdouble getFar() { return zFar; };

    double r;
    double theta;
    double distance;

    double position[3];
    double target[3];
    double rotation[4];
private:
    GLdouble fovy;
 	GLdouble aspect;
 	GLdouble zNear;
 	GLdouble zFar;
};

struct ObjectNode {
    double position[3];
    double rotation[4];
};
//----------------------------------------------------------------------------//
//                        END HELPER CLASS DEFINITION                         //
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//                              CLASS DEFINITION                              //
//----------------------------------------------------------------------------//
class TagViewer
{
public:
    // define constructor / destructor / copy constructor / assignment operator
    TagViewer();
    TagViewer(int width, int height);
    TagViewer(const TagViewer & obj);
    ~TagViewer();
    TagViewer & operator=(const TagViewer& tv);
    
    // adds camera to display
    void addCamera(double * position, double * rotation);

    // adds Tag position / rotation used as origin
    void setTagOrigin(double * position, double * rotation);

    // sets world camera position based on target
    void updateWorldCameraPosition();
    
    // sets up window and GL drawing environments
    void initWindow(int & argv, char** argc);
    
    // draws scene
    void renderFrame();


    // define GLUT callback functions
    void displayCB(void);
    void reshapeCB(GLint w, GLint h);
    void keyboardCB(unsigned char key, int x, int y);
    void mouseDownCB(int button, int state, int x, int y);
    void mouseMoveCB(int x, int y);
    void idleCB(void);
    

    // World Camera
    WorldCamera worldCamera;
private:
    // holds camera vector
    vector<ObjectNode> cameras;

    // window with / height
    int width;
    int height;

    // mouse handler
    bool mouseDown;
    int mouseId;
    int mouse_x;
    int mouse_y;

    // holds pos/rot for tag
    ObjectNode Tag;

    // OpenGL Drawing Functions
    void drawFrustum(ObjectNode& obj);
    void drawTag(ObjectNode & obj);
    void pyramid();
    void square();
    void applyQuaternion(double *  quat);
    
};
//----------------------------------------------------------------------------//
//                            END CLASS DEFINITION                            //
//----------------------------------------------------------------------------//
