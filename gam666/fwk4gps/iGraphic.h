#ifndef _I_GRAPHIC_H_
#define _I_GRAPHIC_H_

/* Graphic Interface - Modelling Layer
 *
 * iGraphic.h
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Base.h"       // for the Base class definition

//-------------------------------- iGraphic ------------------------------
//
// iGraphic is the Interface to the Graphic hierarchy
//
enum PrimitiveType;
struct Colour;
struct Vector;
struct AABB;

class iGraphic : public Base {
    virtual void setWorld(const void*)                        = 0;
    virtual void setRadius(float)                             = 0;
    virtual float getRadius() const                           = 0;
    virtual void beginDraw()                                  = 0;
    virtual void render(int = 0, int = 0, unsigned char = 0) = 0;
    virtual void endDraw()                                    = 0;
    virtual AABB getAABB() const = 0;
    virtual int  width() const                                = 0;
    virtual int  height() const                               = 0;
    virtual void set(void*)                                   = 0;
    virtual void suspend()                                    = 0;
    virtual void restore()                                    = 0;
    virtual void release()                                    = 0;
    virtual void Delete() const                               = 0;
    friend class Coordinator;
    friend class Design;
    friend class Object;
    friend class HUD;
};

iGraphic* CreateGraphic(int = 0, int = 0);
iGraphic* CreateBox(float, float, float, float, float, float, const Colour&);
iGraphic* CreateGrid(float, float, int, const Colour&);
iGraphic* TriangleList(const wchar_t*, const Colour&);
iGraphic* CreateBox(float, float, float, float, float, float, bool f = false);
iGraphic* CreateGrid(float, float, int);
iGraphic* TriangleList(const wchar_t* file);
iGraphic* CreateRectangleList(float, float, float, float);

#endif
