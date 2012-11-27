#ifndef _TEXT_H_
#define _TEXT_H_

/* Text Definition - Modelling Layer
*
* Text.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include "iText.h"       // for the Text Interface
#include "Translation.h" // for MAX_DESC 

//-------------------------------- Text ---------------------------------------
//
// The Text class defines the structure of a text item in the Modelling Layer
//
class iAPIText;
class Frame;
struct Rectf;

class Text : public iText {

  iAPIText* font;                   // points to the font at the API level
  wchar_t   label[MAX_DESC + 1];    // the text string
  wchar_t   value[MAX_DESC + 1];
  iHUD*     hud;                    // points to the parent HUD
  Rectf*    rect;                   // relative rectangle within parent hud
  unsigned _colour;
  unsigned _flags;
  int      _height;
  Text*    topLeft;
  Text*    bottomLeft;
  Text*    topRight;
  Text*    bottomRight;

  // evaluation function and parameters for an evaluated Text
  Frame*  frame;
  Frame** pFrame;
  iSwitch* swtch;
  char     axis;
  unsigned factor;
  const wchar_t* (*intToWCStr)(wchar_t*, const Frame*, char, unsigned); 
  const wchar_t* (*boolToWCStr)(wchar_t*, const iSwitch*);

  Text(const Text& src);
  virtual ~Text();
  void init(Rectf&, const wchar_t*, const wchar_t*, int, unsigned, unsigned);

public:
  Text(Rectf, void*, const wchar_t*, int = 0, const wchar_t* = 0, 
    unsigned = 0, unsigned = 0);
  Text(Rectf, void*, const wchar_t*, const wchar_t* (*)(wchar_t*, 
    const Frame*, char, unsigned), Frame*, char, unsigned = 1, int = 0,
    const wchar_t* = 0, unsigned = 0, unsigned = 0);
  Text(Rectf, void*, const wchar_t*, const wchar_t* (*)(wchar_t*, 
    const Frame*, char, unsigned), Frame**, char, unsigned = 1, int = 0,
    const wchar_t* = 0, unsigned = 0, unsigned = 0);
  Text(Rectf, void*, const wchar_t*, const wchar_t* (*)(wchar_t*, 
    const iSwitch*), iSwitch*, int = 0, const wchar_t* = 0, unsigned = 0, 
    unsigned = 0);
  Text&   operator=(const Text&);
  iText*  clone() const         { return new Text(*this); }
  // execution
  void    set(const wchar_t*);
  void    set(const char*);
  void    set(float);
  void    set(int);
  void    setLabel(const wchar_t*);
  void    setLabel(const char*);
  void    setStyle(int height, unsigned flags = 0);
  void    setColour(unsigned colour);
  void    outline(unsigned colour = 0xFF000000, bool turnOff = false);
  iHUD*   getHUD() const        { return hud; }
  const   wchar_t* text() const { return label; }
  void    render();
  // termination
  void    suspend();
  void    restore();
  void    release();
  void    Delete() const { delete this; }
};

#endif
