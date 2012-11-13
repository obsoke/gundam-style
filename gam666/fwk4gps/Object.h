#ifndef _OBJECT_H_
#define _OBJECT_H_

/* Object Definition - Modelling Layer
*
* Object.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include "iObject.h" // for the Object Interface

//-------------------------------- Object -------------------------------------
//
// The Object class defines an independent object in the Modelling Layer
//
class iGraphic;

class Object : public iObject  {

  Category      category;      // drawing category
  iGraphic*     graphic;       // points to the object's vertex list
  unsigned char alpha;         // transparency '\x00' to '\xFF'
  Reflectivity  reflectivity;  // material reflectivity
  iTexture*     texture;       // points to attached texture
  unsigned      flags;         // texture sampling flags
  unsigned      texAddr;         // texture sampling flags

protected:
  virtual       ~Object();

public:
  Object(Category, iGraphic*, const Reflectivity* = nullptr);
  Object(Category, iGraphic*, unsigned char);
  Object(const Object&);            
  iObject* clone() const                     { return new Object(*this); }
  // initialization
  void attach(iTexture*);
  // execution
  int  width() const;
  int  height() const;
  float radius() const;
  AABB getAABB() const;
  void setClip(Rect* source);
  void setTextureFilter(unsigned f)      { flags = f; }
  void setTextureAddressing(unsigned f) { texAddr = f; }
  bool belongsTo(Category c) const       { return c == category; }
  void render();
  void Delete() const { delete this; }
  const std::vector<Frame*>& getChildren() { return children; }
};

#endif