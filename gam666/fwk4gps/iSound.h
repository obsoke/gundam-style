#ifndef _I_SOUND_H_
#define _I_SOUND_H_

/* Sound Interface - Modelling Layer
*
* iSound.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include "Frame.h"      // for the Frame class definition
#include "Base.h"       // for the Base class definition

//-------------------------------- iSound -------------------------------------
//
// iSound is the Interface to the Sound class
//

class iSound : public Frame, public iSwitch, public Base {
  // initialization
  virtual const wchar_t* relFileName() const = 0;
  virtual void change(const wchar_t* f)      = 0;
  virtual void loop(bool)                    = 0;
  // execution
  virtual bool isOn() const                  = 0;
  virtual void render()                      = 0;
  virtual void suspend()                     = 0;
  virtual void restore()                     = 0;
  virtual void release()                     = 0;
  virtual void Delete() const                = 0;
  friend class Coordinator;
  friend class Design;
  friend class Coordinator;
  friend class Design;
public:
  virtual void update()                      = 0;
  virtual bool stop()                        = 0;
  virtual bool toggle()                      = 0;
  virtual iSound* clone() const              = 0;
  virtual bool play()                        = 0;
};

iSound* CreateSound(const wchar_t*, bool = true);
iSound* CreateLocalSound(const wchar_t*, bool = true, float = 0, float = 0);

iSound* Clone(const iSound*);

#endif
