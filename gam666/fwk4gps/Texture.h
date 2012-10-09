#ifndef _TEXTURE_H_
#define _TEXTURE_H_

/* Texture Definition - Modelling Layer
 *
 * Texture.h
 * fwk4gps version 4.0
 * gam666/dps901/gam670/dps905
 * June 25 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iTexture.h" // for the Texture Interface

//-------------------------------- Texture ------------------------------------
//
// The Texture class defines the structure of a texture in the Modelling Layer
//
class iAPITexture;

class Texture : public iTexture {

	iAPITexture* apiTexture; // points to the api texture

  protected:
	virtual ~Texture();

  public:
	Texture(const wchar_t*, unsigned);
	Texture(const Texture&);
	Texture& operator=(const Texture&);
    iTexture* clone() const { return new Texture(*this); }
	// execution
    static void setAnisotropy(int);
    void setFilter(unsigned) const;
    void attach(int, int);
    void detach();
	// termination
	void suspend();
    void release();
    void Delete() const { delete this; }
};

#endif
