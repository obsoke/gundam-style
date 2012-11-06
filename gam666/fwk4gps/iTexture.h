#ifndef _I_TEXTURE_H_
#define _I_TEXTURE_H_

/* Texture Interface - Modelling Layer
 *
 * iTexture.h
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Base.h" // for the Base class definition

//-------------------------------- iTexture -----------------------------------
//
// iTexture is the Interface to the Texture class
//
class iTexture : public Base {
    virtual void setFilter(unsigned) const = 0;
	virtual void attach(int = 0, int = 0)  = 0;
    virtual void detach()                  = 0;
    virtual void suspend()                 = 0;
    virtual void release()                 = 0;
    virtual void Delete() const            = 0;
    friend class Coordinator;
    friend class Design;
    friend class Object;
    friend class HUD;
  public:
    virtual void setAddressing(unsigned) const = 0;
    virtual iTexture* clone() const        = 0;
};

iTexture* CreateTexture(const wchar_t*, unsigned = 0u);
iTexture* Clone(const iTexture*);

#endif
