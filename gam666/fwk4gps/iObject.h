#ifndef _I_OBJECT_H_
#define _I_OBJECT_H_

/* Object Interface - Modelling Layer
*
* iObject.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include "Frame.h" // for the Shape class definition
#include "Base.h"  // for the Base class definition

//-------------------------------- iObject ------------------------------------
//
// iObject is the Interface to the Object class
//
class  iTexture;
class  iGraphic;
struct Matrix;
struct Reflectivity;
enum   Category;
struct AABB;
struct Rect;

class iObject : public Frame, public Base {
  // initialization
  // execution
  virtual int  width() const                       = 0;
  virtual int  height() const                      = 0;
  virtual float radius() const                     = 0;
  virtual void render()                            = 0;
  virtual void setTextureFilter(unsigned)          = 0;
  virtual bool belongsTo(Category category) const  = 0;
  // termination function
  virtual void Delete() const                      = 0;
  friend class Coordinator;
  friend class Design;
  friend class World;
public:
  virtual void setReflectivity(Reflectivity* r)    = 0;
  virtual void setClip(Rect* source)               = 0;
  virtual void setTextureAddressing(unsigned)      = 0;
  virtual void attach(iTexture* t)                 = 0;
  virtual AABB getAABB() const                     = 0;
  virtual iObject* clone() const                   = 0;
  virtual const std::vector<Frame*>& getChildren() = 0;
};

iObject* CreateObject(iGraphic*, const Reflectivity*); 
iObject* CreateObject(iGraphic*, unsigned char a = 0); 
Frame* CreateSkybox(const wchar_t** file, float width = 10000, 
  float height = 10000, float depth = 10000); 
iObject* CreateSprite(iGraphic*, unsigned char a = 0);
iObject* CreateSprite(const wchar_t* file, Rect* destRect, 
  Rect* sourceRect = nullptr, unsigned char a = 0);
iObject* Clone(const iObject*);

#endif
