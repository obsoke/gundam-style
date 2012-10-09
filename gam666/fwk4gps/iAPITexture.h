#ifndef _I_API_TEXTURE_H_
#define _I_API_TEXTURE_H_

/* APITexture Interface - Translation Layer
 *
 * iAPITexture.h
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

//-------------------------------- iAPITexture --------------------------------
//
// iAPITexture is the Interface to the APITexture class
//
class iAPITexture {
    virtual iAPITexture* clone() const = 0;
	// execution
    virtual void setFilter(unsigned)   = 0;
    virtual void attach(int, int)      = 0;
    virtual void detach()              = 0;
	// termination
	virtual void suspend()             = 0;
	virtual void release()             = 0;
	virtual void Delete() const        = 0;
    friend class Texture;
};

iAPITexture* CreateAPITexture(const wchar_t* file, unsigned);

#endif