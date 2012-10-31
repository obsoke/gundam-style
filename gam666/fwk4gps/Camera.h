#ifndef _CAMERA_H_
#define _CAMERA_H_

/* Camera Definition - Modelling Layer
 *
 * Camera.h
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iCamera.h"  // for the Camera Interface
#include "MathDecl.h" // for Matrix

//-------------------------------- Camera -------------------------------------
//
// The Camera class defines the structure of a viewpoint
//
enum Action;

class Camera : public iCamera {

    static iCamera* current; // points to the current camera
    static Matrix   view;    // the view transformation for the current camera

    bool pressed(Action a) const;
    int  change(Action a) const;
    virtual ~Camera();

  public:
    bool useInput;

    static Frame** getCurrent() { return (Frame**)&current; }
    static void*    getView()    { return &view; }
    Camera(bool useInput = false);
	Camera(const Camera& c);
    void* clone() const          { return new Camera(*this); }
	// execution
    void update();
    void Delete() const { delete this; }
};

#endif
