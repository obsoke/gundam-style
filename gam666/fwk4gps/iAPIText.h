#ifndef _I_API_TEXT_H_
#define _I_API_TEXT_H_

/* APIText Interface - Translation Layer
*
* iAPIText.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

//-------------------------------- iAPIText -----------------------------------
//
// iAPIText is the Interface to the APIText class
//
struct Rectf;

class iAPIText {
  virtual iAPIText* clone() const                 = 0;
  // execution function
  virtual void draw(const Rectf&, const wchar_t*) = 0;
  virtual void useScreenCoords(bool use = true)   = 0;
  // termination
  virtual void suspend()                          = 0;
  virtual bool restore()                          = 0;
  virtual void release()                          = 0;
  virtual void Delete() const                     = 0;
  virtual void outline(unsigned colour = 0, bool turnOff = false) = 0;
  friend class Text;
};

iAPIText* CreateAPIText(const wchar_t*, int, unsigned, unsigned);

#endif