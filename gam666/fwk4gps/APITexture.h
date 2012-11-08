#ifndef _API_TEXTURE_H_
#define _API_TEXTURE_H_

/* APITexture Definition - Translation Layer
*
* APITexture.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include "iAPITexture.h" // for the APITexture Interface
#include "APIBase.h"     // for the APIBase class definition

//-------------------------------- APITexture ---------------------------------
//
// The APITexture class implements a Texture at the API level
//
struct IDirect3DTexture9;

class APITexture : public iAPITexture, public APIBase {

  wchar_t*           file;   // points to file with texture image
  unsigned           key;    // color key
  IDirect3DTexture9* tex;    // interface to the texture COM object
  unsigned           filter; // sampling filter
  unsigned           addressing;

  void setSamplerState(unsigned flags) const;
  void setup(int, int);
  virtual ~APITexture();


public:
  APITexture(const wchar_t*, unsigned);
  APITexture(const APITexture&);
  iAPITexture& operator=(const APITexture&);
  iAPITexture* clone() const { return new APITexture(*this); }
  // execution
  static void setAnisotropy(int);
  void setFilter(unsigned f) { filter = f; }
  void setAddressing(unsigned flags) { addressing = flags; }
  void setAddressingState(unsigned flags);
  void attach(int ,int);
  void detach();
  // suspension
  void suspend();
  // termination
  void release();
  void Delete() const { delete this; }
};

#endif