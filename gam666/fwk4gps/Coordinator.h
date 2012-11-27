#ifndef _COORDINATOR_H_
#define _COORDINATOR_H_

/* Coordinator Definition - Modelling Layer
*
* Coordinator.h
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* distributed under TPL - see ../Licenses.txt
*/

#include <vector>
#include "iCoordinator.h" // for the Coordinator Interface
#include "MathDecl.h"     // for Matrix
#include "Frame.h"

//-------------------------------- Templates for add and remove ---------------
//
// add adds a pointer to *o in vector v
//
template <class T>
void add(std::vector<T>& v, T o) {
  unsigned i;
  bool rc = false;

  for (i = 0; i < v.size() && !rc; i++)
    if (!v[i]) {
      v[i] = o;
      rc = true;
    }
    if (!rc)
      v.push_back(o);
}

// remove removes the pointer to *o from vector v
//
/*template <class T>
bool remove(std::vector<T>& v, T o) {
bool rc = false;

for (unsigned i = 0; i < v.size(); i++) {
if (v[i] == o) {
v[i] = 0;
rc = true;
}
while (v.size() && !v[v.size() - 1])
v.pop_back();

return rc;
}*/

template <class T>
bool remove(std::vector<T>& v, T o) {
  bool rc = false;
  for (int i = ((int)v.size()) - 1; i >= 0 && !rc; --i) {
    if (v[i] == o) {
      v.erase(v.begin() + i);
      rc = true;
    }
  }
  return rc;
}

//-------------------------------- Coordinator --------------------------------
//
// The Coordinator class coordinates all design elements in the Modelling Layer 
//
enum Category;
class iAPIWindow;
class iAPIUserInput;
class iAPIDisplay;
class iAPIAudio;
struct Viewport;
struct APIObjects;

class Coordinator : public iCoordinator {

  iText*                 timerText;        // points to timer's text object
  bool keepgoing;

  Coordinator(const Coordinator& s);            // prevents copying
  Coordinator& operator=(const Coordinator& s); // prevents assignment
  bool setConfiguration();
  void adjustVolume(int);
  void adjustFrequency(int);
  void update();
  void render(Category category);

protected:

  std::vector<iGraphic*> graphic;          // points to graphics
  std::vector<iTexture*> texture;          // points to textures
  std::vector<iLight*>   light;            // points to light sources
  std::vector<iCamera*>  camera;           // points to cameras
  std::vector<iSound*>   sound;            // points to sound sources
  std::vector<iText*>    text;             // points to text items
  std::vector<iHUD*>     hud;              // points to huds
  iHUD*                  mainHUD;
  std::vector<iObject*>  object;           // points to objects


  iAPIWindow*            window;           // points to the window object
  iAPIUserInput*         userInput;        // points to the user input object
  iAPIDisplay*           display;          // points to the display object
  iAPIAudio*             audio;            // points to the audio object

  bool updateOnRender;
  // display device
  float                  nearcp;           // near clipping plane
  float                  farcp;            // far clipping plane
  float                  fov;              // field of view in radians
  Matrix                 view;             // current view transformation
  Matrix                 projection;       // current projection transformation
  Colour                 ambient;          // background lighting
  bool                   wireFrame;        // wire frame mode
  bool                   wBuffering;       // w-buffering is on
  bool                   zBuffering;       // depth-buffering is on

  unsigned               framecount;       // no of frames since 'lastReset'
  unsigned               fps;              // frame rate per sec
  unsigned               lastReset;        // last time framecount reset to 0

  iCamera*               currentCam;       // current camera
  unsigned               lastCameraToggle; // time - most recent cam toggle
  unsigned               lastWFrameToggle; // time - most recent wire frame toggle
  unsigned               currentHUD;       // index - current HUD
  unsigned               lastHUDToggle;    // time - most recent hud toggle
  unsigned               lastAudioUpdate;  // time - most recent audio update
  
  bool displayCursor;                         // display windows cursor

  // configuration
  virtual void initialize()                 { }
  void setProjection(float, float, float);
  virtual void createProjection();
  void setAmbientLight(float, float, float);
  void setTimerText(void* text)     { timerText = (iText*)text; }
  // execution
  void release(Action);
  const wchar_t* file (ModelSound s) const;
  virtual ~Coordinator();

public:
  static iCoordinator* Address() { return coordinator; }
  Coordinator(APIObjects* objects);
  // initialization
  void  add(iObject* o)  { ::add(object, o); }
  void  add(iTexture* t) { ::add(texture, t); }
  void  add(iLight* l)   { ::add(light, l); }
  void  add(iCamera* c)  { ::add(camera, c); }
  void  add(iSound* s)   { ::add(sound, s); }
  void  add(iGraphic* g) { ::add(graphic, g); }
  void  add(iText* t)    { ::add(text, t); }
  void  add(iHUD* h)     { ::add(hud, h); }
  void  reset();
  // execution
  int   run(bool configure = true);
  void  stop();
  void  resize();
  // termination
  void  suspend();
  void  restore();
  void  release();
  void  remove(iObject* o);
  void  remove(iTexture* t) { ::remove(texture, t); }
  void  remove(iLight* l)   { ::remove(light, l); }
  void  remove(iCamera* c)  { ::remove(camera, c); }
  void  remove(iSound* s)   { ::remove(sound, s); }
  void  remove(iGraphic* g) { ::remove(graphic, g); }
  void  remove(iText* t)    { ::remove(text, t); }
  void  remove(iHUD* h)     { ::remove(hud, h); }
  bool pressed(Action a, unsigned deviceNumber = 0) const;
  int  change(Action a, unsigned deviceNumber = 0) const;
  bool ptrPressed() const;
  bool ctrPressed() const;
  virtual void render();
  void setViewport(const Viewport& viewport);
  void setAPIObjects(APIObjects* objects);
  void showCursor(bool show) { displayCursor = show; }
  bool getCursor() { return displayCursor; }
  iHUD* getHUD() { return mainHUD; }
};

#endif
