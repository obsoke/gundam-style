#ifndef _LIGHT_H_
#define _LIGHT_H_

/* Light Definition - Modelling Layer
 *
 * Light.h
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iLight.h"   // for the Light Interface
#include "MathDecl.h" // for Colour

//-------------------------------- Light --------------------------------------
//
// The Light class defines the structure of a light source
//
class iAPILight;
enum  RenderState;

class Light : public iLight {

	iAPILight* apiLight;   // points to the api light

	bool       on;         // light is on?
    LightType  type;       // directional, point, or spot?
	bool       turnOn;     // turn on this light?
	bool       turnOff;    // turn off this light?
    int        lastToggle; // time of the last toogle

	Light(const Light&);
	virtual ~Light();

  public:
    static void alloc();
    static void dealloc();
    Light(LightType, Colour, Colour, Colour, float, bool, float = 1, 
	 float = 0, float = 0, float = 0, float = 0, float = 0);
	Light&  operator=(const Light&);
    iLight* clone() const { return new Light(*this); }
    static void setAmbient(const Colour&);
    static void set(RenderState, bool);
    bool isOn() const     { return on; }
	bool toggle();
	void update();
	void suspend();
	void restore();
    void release() { }
    void Delete() const   { delete this; }
};

#endif
