#ifndef _I_API_GRAPHIC_H_
#define _I_API_GRAPHIC_H_

/* APIGraphic Interface - Translation Layer
*
* iAPIGraphic.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

//-------------------------------- iAPIGraphic --------------------------------
//
// iAPIGraphic is the Interface to the APIGraphic hierarchy
//
enum PrimitiveType;
struct Vector;
struct Rect;

class iAPIGraphic {
  virtual iAPIGraphic* clone() const           = 0;
  virtual void setWorld(const void*)           = 0; 
  virtual void beginDraw()                     = 0;
  virtual void endDraw()                       = 0;
  virtual void render(int, int, unsigned char, 
    Rect* sourceRect = nullptr)                = 0;
  virtual void setReflectivity(const void*)    = 0;
  virtual void draw()                          = 0;
  virtual void suspend()                       = 0;
  virtual void release()                       = 0;
  virtual void Delete() const                  = 0;
  friend class Graphic;
  friend struct APIObjects;
};

iAPIGraphic* CreateAPIGraphic();
iAPIGraphic* CreateAPIVertexList(PrimitiveType, unsigned);

#endif
