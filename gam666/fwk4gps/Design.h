#ifndef _DESIGN_H_
#define _DESIGN_H_

/* Design Definition - Modelling Layer
 *
 * Design.h
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Coordinator.h" // for the Coordinator class definition
     
//-------------------------------- Design -------------------------------------
//
// The Design class defines the structure of the game design
//
class Design : public Coordinator {

    iObject* parent;    // points to the parent box
    iObject* child;     // points to the child box
    iObject* square;    // points to the square plate
    iObject* sprite;    // points to moving sprite
    iObject* rollLeft;  // points to left spinner
    iObject* spinRight; // points to right spinner

    iLight*  pointLight;   // points to the point light
	iLight*  spotLight;    // points to the spot light
	iLight*  distantLight; // points to the directional light

	iSound*  background;   // points to background sound
	iSound*  discrete;     // points to discrete sound
	iSound*  locall;       // points to local sound on the left
	iSound*  localr;       // points to local sound on the right
	iSound*  objectSnd;    // points to object sound

    Design(const Design& s);            // prevents copying
    Design& operator=(const Design& s); // prevents assignment

  public:
    Design(void*, int);
    void initialize();
    void update();
};

#endif
