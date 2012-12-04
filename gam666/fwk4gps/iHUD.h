#ifndef _I_HUD_H_
#define _I_HUD_H_

/* HUD Interface - Modelling Layer
*
* iHUD.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include "Frame.h"      // for the Frame class definition
#include "Base.h"       // for the Base class definition

//-------------------------------- iHUD ---------------------------------------
//
// iHUD is the Interface to the HUD class
//
struct Rectf;
class iTexture;
class iGraphic;

class iHUD : public iSwitch, public Base {
  virtual bool toggle()                = 0;
  virtual void update()                = 0;
  virtual const Rectf& getRect() const = 0;
  virtual void beginDraw()             = 0;
  virtual void render()                = 0;
  virtual void endDraw()               = 0;
  virtual void restore()               = 0;
  virtual void Delete() const          = 0;
  friend class Coordinator;
  friend class Design;
  friend class Text;
  friend class Menu;
};

iHUD* CreateHUD(iGraphic*, float, float, float, float, iTexture* = 0); 

#endif
