#ifndef _HUD_H_
#define _HUD_H_

/* HUD Definition - Modelling Layer
*
* HUD.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include "iHUD.h" // for the HUD Interface

//-------------------------------- HUD ----------------------------------------
//
// The HUD class defines the structure of a Heads Up Display object
//
class HUD : public iHUD {

  bool      on;         // is the HUD being displayed?
  Rectf*    rect;       // bounding box [HUD_MIN, HUD_MIN, HUD_MAX, HUD_MAX]
  iTexture* texture;    // points to the HUD texture
  iGraphic* graphic;    // points to the graphic representation
  unsigned  lastToggle; // time of the last toggle
  void      validate(); // validates HUD size & position

  virtual ~HUD();

public:
  HUD(iGraphic*, float, float, float, float, iTexture*);
  HUD(const HUD&);
  HUD& operator=(const HUD&);
  void* clone() const          { return new HUD(*this); }
  bool isOn() const            { return on; }
  const Rectf& getRect() const { return *rect; }
  bool toggle();
  void update();
  void beginDraw();
  void render();
  void endDraw();
  void restore();
  void Delete() const { delete this; }
};

#endif
