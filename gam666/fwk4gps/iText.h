#ifndef _I_TEXT_H_
#define _I_TEXT_H_

/* Text Interface - Modelling Layer
 *
 * iText.h
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Base.h" // for the Base class definition

//-------------------------------- iText --------------------------------------
//
// iText is the interface to the Text class of the Modelling Layer
//
struct Rectf;
class Frame;
class iSwitch;
class iHUD;

class iText : public Base {
    virtual void  set(const wchar_t* text) = 0;
	virtual const wchar_t* text() const    = 0;
    virtual iHUD* getHUD() const           = 0;
    virtual void render()                  = 0;
    virtual void suspend()                 = 0;
    virtual void restore()                 = 0;
    virtual void release()                 = 0;
    virtual void Delete() const            = 0;
    friend class Coordinator;
    friend class Design;
  public:
    virtual iText* clone() const           = 0;
};

iText* CreateText(Rectf, void* = 0, const wchar_t* = 0, int = 0, 
 const wchar_t* = 0, unsigned = 0, unsigned = 0);
iText* CreateText(Rectf, void*, const wchar_t*, 
 const wchar_t* (*)(wchar_t*, const Frame*, char, unsigned), Frame*, char, 
 unsigned = 1, int = 0, const wchar_t* = 0, unsigned = 0, unsigned = 0);
iText* CreateText(Rectf, void*, const wchar_t*, 
 const wchar_t* (*)(wchar_t*, const Frame*, char, unsigned), Frame**, char,
 unsigned = 1, int = 0, const wchar_t* = 0, unsigned = 0, unsigned = 0);
iText* CreateText(Rectf, void*, const wchar_t*, 
 const wchar_t* (*)(wchar_t*, const iSwitch*), iSwitch*, int = 0, 
 const wchar_t* = 0, unsigned = 0, unsigned = 0);

iText* Clone(const iText*);


const wchar_t* orient(wchar_t*, const Frame*, char, unsigned = 1u);
const wchar_t* position(wchar_t*, const Frame*, char = ' ', unsigned = 1u);
const wchar_t* onOff(wchar_t*, const iSwitch*);

#endif
