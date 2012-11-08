#ifndef _BASE_H_
#define _BASE_H_

/* Base Definition - Modelling Layer
*
* Base.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

//-------------------------------- Base ---------------------------------------
//
// The Base class holds the absolute state for the Modelling Layer
//
class iCoordinator;

class Base {

protected:

  static iCoordinator* coordinator; // points to the Coordinator object

  static unsigned      now;         // current time in system units
  static unsigned      lastUpdate;  // time of the last update
  static bool          active;      // application is active?
  static int           width;       // width of model area
  static int           height;      // height of model area
  static int           frequency;   // current ambient frequency
  static int           volume;      // current volume
};

#endif
