/* Camera Implementation - Modelling Layer
*
* Camera.cpp
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include "Camera.h"       // for Camera class definition
#include "iCoordinator.h" // for the Coordinator Interface
#include "MathDef.h"      // for math functions
#include "Model.h"        // for FORWARD_SPEED, ROT_SPEED
#include "Translation.h"  // for CAM_PAN_ ...
#include "Game\Utils.h"

//-------------------------------- Camera -------------------------------------
//
// The Camera class defines the structure of one viewpoint on the scene
//
iCamera* Camera::current = nullptr;
Matrix   Camera::view    = Matrix();

// CreateCamera creates a Camera object
//
iCamera* CreateCamera() {

  return (iCamera*)new Camera();
}

// constructor adds the Camera Instance to the coordinator
//
Camera::Camera(bool useInput) : useInput(useInput) {

  coordinator->add(this);
  current = this;
}

Camera::Camera(const Camera& src) {

  coordinator->add(this);
  *this = src;
}

// pressed retrieves the action state from the coordinator
//
bool Camera::pressed(Action a) const { return coordinator->pressed(a); }

// change retrieves the action state from the coordinator
//
int Camera::change(Action a) const { return coordinator->change(a); }

// update adjusts the camera's viewpoint and heading according
// to the user's interventions
//
void Camera::update() {
  if (useInput) {
    int delta = now - lastUpdate;
    int dx = 0, // pitch up/down
      dy = 0, // yaw left/right
      dz = 0; // advance/retreat
    int rx = 0,
      ry = 0,
      rz = 0;

    // controller input
    int jx = change(POSX);
    int jy = change(POSY);
    int jr = change(RPOSX);
    if (jy)
      rx = -(int)(jy * CTR_SPEED);
    if (jr)
      ry = (int)(jr * CTR_SPEED);
    if (jx)
      rz = (int)(jx * CTR_SPEED);

    // mouse input
    int mx = change(CAM_SMOOTH_YAW);
    int my = change(CAM_SMOOTH_PITCH);
    int mz = change(CAM_SMOOTH_F_B);
    if (mx)
      ry += mx * MOUSE_SPEED;
    if (my)
      rx += my * MOUSE_SPEED;
    if (mz)
      dz += mz * MOUSE_SPEED;

    // keyboard input
    if (pressed(CAM_PAN_LEFT))
      dx -= delta;
    if (pressed(CAM_PAN_RIGHT))
      dx += delta;
    if (pressed(CAM_PAN_LEFT_ALT))
      dx -= delta;
    if (pressed(CAM_PAN_RIGHT_ALT))
      dx += delta;
    if (pressed(CAM_FLY_DOWN))
      dy -= delta;
    if (pressed(CAM_FLY_UP))
      dy += delta;
    if (pressed(CAM_ADVANCE))
      dz += delta;
    if (pressed(CAM_RETREAT))
      dz -= delta;
    if (pressed(CAM_PITCH_UP))
      rx -= delta;
    if (pressed(CAM_PITCH_DOWN))
      rx += delta;
    if (pressed(CAM_YAW_LEFT))
      ry -= delta;
    if (pressed(CAM_YAW_RIGHT))
      ry += delta;
    if (pressed(CAM_ROLL_LEFT))
      rz -= delta;
    if (pressed(CAM_ROLL_RIGHT))
      rz += delta;

    // adjust camera orientation
    if ((rx || ry || rz)) {
      // yaw left/right
      if (ry)
        rotate(orientation('y'), ry * ANG_CAM_SPEED);
      // pitch up/down
      if (rx) 
        rotate(orientation('x'), rx * ANG_CAM_SPEED);
      // roll left/right
      if (rz) 
        rotate(orientation('z'), rz * ANG_CAM_SPEED);
      debug(position());
    }
    // adjust camera position
    if ((dx || dy || dz)) { 
      Vector displacement = 
        (float) dx * CAM_SPEED * orientation('x') +
        (float) dy * CAM_SPEED * orientation('y') + 
        (float) dz * CAM_SPEED * orientation('z');
      translate(displacement.x, displacement.y, displacement.z);
      debug(position());
    }
  }

  current = this;

  // update the view transformation
  Vector p = position();
  Vector h = ::normal(orientation('z'));
  Vector u = ::normal(orientation('y'));
  view = ::view(p, p + h, u);
}

// destructor removes the Camera Instance from the coordinator
//
Camera::~Camera() {
  if (coordinator) coordinator->remove(this);
}
