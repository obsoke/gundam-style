#ifndef _I_LIGHT_H_
#define _I_LIGHT_H_

/* Light Interface - Modelling Layer
*
* iLight.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include "Frame.h"      // for the Frame class definition
#include "Base.h"       // for the Base class definition

//-------------------------------- iLight -------------------------------------
//
// iLight is the Interface to the Light class
//
enum   LightType;
struct Colour;
class  Frame;

class iLight : public Frame, public iSwitch, public Base {
  virtual bool toggle()              = 0;
  virtual void update()              = 0;
  virtual void suspend()             = 0;
  virtual void release()             = 0;
  virtual void restore()             = 0;
  virtual void Delete() const        = 0;
  friend class Coordinator;
  friend class Design;
  friend class Coordinator;
  friend class Design;
public:
  virtual iLight* clone() const      = 0;
};

iLight* CreateDistantLight(Colour, Colour, Colour, bool = true);

iLight* CreatePointLight(Colour, Colour, Colour, float, bool, float = 1, 
  float = 0, float = 0);

iLight* CreateSpotLight(Colour, Colour, Colour, float, bool, float = 1, 
  float = 0, float = 0, float = 0, float = 0, float = 0);

iLight* Clone(const iLight*);

#endif
