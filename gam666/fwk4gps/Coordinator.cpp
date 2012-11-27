/* Coordinator Implementation - Modelling Layer
*
* Coordinator.cpp
* fwk4gps version 4.0
* gam666/dps901/gam670/dps905
* June 25 2012
* copyright (c) 2012 Chris Szalwinski 
* ViewFrustum (c) addition by Grace Batumbya 2011
* distributed under TPL - see ../Licenses.txt
*/

#include "Coordinator.h"   // for the Coordinator class definition
#include "iAPIWindow.h"    // for the API Window Interface
#include "iAPIUserInput.h" // for the APIUserInput Interface
#include "iAPIDisplay.h"   // for the APIDisplay Interface
#include "iAPIAudio.h"     // for the APIAudio Interface
#include "iUtilities.h"    // for strcpy, sprintf, strcmp
#include "Camera.h"        // for the Camera class definition
#include "iObject.h"       // for the Object Interface
#include "Texture.h"       // for the Texture class
#include "Light.h"         // for the Light class
#include "iSound.h"        // for the Sound Interface
#include "iGraphic.h"      // for the Graphic Interface
#include "iText.h"         // for the Text Interface
#include "iHUD.h"          // for the HUD Interface
#include "Model.h"         // for macros
#include "MathDef.h"       // for ::projection
#include "Translation.h"   // for CAMERA_SELECT ...
#include "APIObjects.h"

//-------------------------------- Coordinator --------------------------------
//
// The Coordinator object manages the design items of the Modelling Layer
//
iCoordinator* CoordinatorAddress() { return Coordinator::Address(); }

// constructor initializes the reference time and sets the current camera
// and HUD
//
Coordinator::Coordinator(APIObjects* objects) {

  coordinator = this;
  setAPIObjects(objects);

  // timers
  now              = 0;
  lastReset        = 0;
  lastUpdate       = 0;
  lastCameraToggle = 0;
  lastWFrameToggle = 0;
  lastHUDToggle    = 0;
  lastAudioUpdate  = 0;
  framecount       = 0;
  fps              = 0;

  // current camera and HUD
  currentCam = NULL;
  currentHUD = 0;
  wireFrame  = false;
  wBuffering = false;
  zBuffering = true;

  // volume and frequency settings
  frequency = DEFAULT_FREQUENCY;
  volume    = DEFAULT_VOLUME;

  // pointers
  timerText  = nullptr;

  // projection parameters are updated
  fov    = 0.9f;
  nearcp = 1.0f;
  farcp  = 1000.0f;

  updateOnRender = true;  
  displayCursor = false;
}

// setConfiguration retrieves the configuration selection from the user
//
bool Coordinator::setConfiguration() { 

  bool rc = false;

  if (userInput->getConfiguration()) {
    release();
    userInput->configure();         
    window->configure(userInput->getWindowMode(), userInput->getWindowWidth(), userInput->getWindowHeight());
    if (window->setup()) {
      width  = window->getClientWidth();
      height = window->getClientHeight();
      if (userInput->setup()) {
        if (display->setup()) {
          createProjection();
          Light::set(DITHERING, true);
          Light::set(SPECULARITY, true);
          Texture::setAnisotropy(10);
          audio->setup();
          rc = true;
        } else {
          window->release();
          userInput->release();
        }
      }
      else
        window->release();
    }
  }

  // reset timers
  now = window->time();
  lastUpdate = now;
  lastReset  = now;

  return rc;
}

// reset resets the configuration
//
void Coordinator::reset() {

  if (setConfiguration()) {
    // reset the sound files
    for (unsigned i = 0; i < sound.size(); i++) {
      if (sound[i]->relFileName() && 
        strcmp(file((ModelSound)i), sound[i]->relFileName()))
        sound[i]->change(file((ModelSound)i));
    }
  }
}

// processMessage processes the next message in the message queue 
// returns false if queue is empty
//
int Coordinator::run(bool configure) {

  int  rc = 0;
  keepgoing = configure ? setConfiguration() : true;

  // configure and initialize the application
  if (keepgoing) {
    Light::alloc();
    initialize();
    now = window->time();
    lastUpdate = now;
    lastReset  = now;
    if (userInput) userInput->reset();
  }

  while (keepgoing) {
    // process all system messages as the first priority
    if (window->processMessage(rc, keepgoing)) 
      ; // intentional
    else if (!active)
      window->wait();
    else {
      // opportunity to render a frame if no messages
      now = window->time();
      // render only if sufficient time has elapsed since the last frame
      if (now - lastUpdate >= UNITS_PER_SEC / FPS_MAX) {
        // render the frame
        render();
        // update the reference time
        lastUpdate = now;
      }
    }
  }

  return rc;
}

void Coordinator::stop()  { 
  keepgoing = false; 
  Light::dealloc(); 
}

// setAmbientLight sets the colour of the background lighting
//
void Coordinator::setAmbientLight(float r, float g, float b) {

  ambient = Colour(r, g, b); 
}

// setProjection sets the angle and clipping planes for the projection matrix
//
void Coordinator::setProjection(float angle, float n, float f) {

  fov    = angle;
  nearcp = n;
  farcp  = f;
}

// pressed returns the on/off status of Action a
//
bool Coordinator::pressed(Action a, unsigned deviceNumber) const { return userInput->pressed(a, deviceNumber); }

// ptrPressed returns the on/off status of Action a
//
bool Coordinator::ptrPressed() const { return userInput->ptrPressed(); }

// ctrPressed returns the on/off status of Action a
//
bool Coordinator::ctrPressed() const { return userInput->ctrPressed(); }

// change returns the change caused by Action a 
//
int Coordinator::change(Action a, unsigned deviceNumber) const { return userInput->change(a, deviceNumber); }

// release turns off Action a
//
void Coordinator::release(Action a) { /*userInput->release(a);*/ }

// file returns the address of the file associated with ModelSound s
//
const wchar_t* Coordinator::file(ModelSound s) const { 

  return userInput->file(s); 
}

// adjustVolume adjusts the volume of the audio system by factor *
// incVolume - positive factor increases the volume, negative factor
// decreases the volume
//
void Coordinator::adjustVolume(int factor) {

  if (factor > 0)
    volume += STEP_VOLUME;
  else if (factor < 0)
    volume -= STEP_VOLUME;
  if (volume > MAX_VOLUME)
    volume = MAX_VOLUME;
  else if (volume < MIN_VOLUME)
    volume = MIN_VOLUME;

  lastAudioUpdate = now;
}

// adjustFrequency adjusts the frequency of the audio system by factor *
// incVolume - positive factor increases the volume, negative factor
// decreases the volume
//
void Coordinator::adjustFrequency(int factor) {

  if (factor < 0) {
    frequency = frequency + STEP_FREQUENCY;
    frequency = frequency < MIN_FREQUENCY ? MIN_FREQUENCY : frequency;
  }
  else if (factor > 0) {
    frequency = frequency + STEP_FREQUENCY;
    frequency = frequency > MAX_FREQUENCY ? MAX_FREQUENCY : frequency;
  }

  lastAudioUpdate = now;
}

// update updates the current Camera and HUD objects, the lights and the sounds
//
void Coordinator::update() {

  if (currentCam) {
    currentCam->update();
  }
  // adjust framecount and fps
  if (now - lastReset <= UNITS_PER_SEC) 
    framecount++;
  else {
    // recalculate the frame rate
    fps        = framecount * UNITS_PER_SEC / (now - lastReset);
    framecount = 0;
    lastReset  = now;
    if (timerText) {
      wchar_t str[MAX_DESC + 1];
      sprintf(str, fps, L" fps");
      timerText->set(str);
    }
  }
  // toggle and update the current hud
  if (hud.size() && userInput->pressed(HUD_SELECT) &&
    now - lastHUDToggle > KEY_LATENCY) {
      lastHUDToggle = now;
      currentHUD++;
      if (currentHUD == hud.size())
        currentHUD = 0;
  }
  if (hud.size() && hud[currentHUD] && userInput->pressed(HUD_SELECT))
    hud[currentHUD]->toggle();
  if (hud.size() && hud[currentHUD]) 
    hud[currentHUD]->update();

  // update the volume and the frequency
  if (now - lastAudioUpdate > KEY_LATENCY) {

    if (userInput->pressed(AUD_VOLUME_DEC))
      adjustVolume(-1);
    if (userInput->pressed(AUD_VOLUME_INC))
      adjustVolume(1);

    if (userInput->pressed(AUD_FREQ_DEC))
      adjustFrequency(-1);
    else if (userInput->pressed(AUD_FREQ_INC))
      adjustFrequency(1);
  }	

  // update the sound sources
  for (unsigned i = 0; i < sound.size(); i++)
    if (sound[i]) 
      sound[i]->update();

  if (now - lastWFrameToggle > KEY_LATENCY && (pressed(WIRE_FRAME_SELECT) || pressed(Z_BUFFERING_SELECT) || pressed(W_BUFFERING_SELECT))) {
    lastWFrameToggle = now;
    // toggle and update the wire frame state
    if (pressed(WIRE_FRAME_SELECT)) {
      wireFrame = !wireFrame;
      display->set(WIRE_FRAME,  wireFrame);
    }
    // toggle and update the depth buffering state
    if (pressed(Z_BUFFERING_SELECT)) {
      zBuffering = !zBuffering;
      display->set(Z_BUFFERING, zBuffering);
    }
    // toggle and update the w-buffering state
    if (pressed(W_BUFFERING_SELECT)) {
      wBuffering = !wBuffering;
      display->set(W_BUFFERING, wBuffering);
    }
  }
}

// renders draws a complete frame
//
void Coordinator::render() {

  if (updateOnRender) {
    // update the user input devices
    userInput->update();  
    Coordinator::update();
    if (!keepgoing) return;
    // update the model
    update();
    // update the light sources in object space
    for (unsigned i = 0; i < light.size(); i++)
      if (light[i])
        light[i]->update();
    // update the audio
    audio->setVolume(volume);
    audio->setFrequencyRatio(frequency);
    audio->update(Camera::getView());
    // update the sound sources
    for (unsigned i = 0; i < sound.size(); i++)
      if (sound[i]) 
        sound[i]->update();
  }

  // draw the frame 
  view = *((Matrix*)Camera::getView());
  display->beginDrawFrame(&view);
  Light::setAmbient(ambient);
  // render all of the sprite objects
  render(SPRITE);
  // render all of the lit objects - include translucency
  display->set(LIGHTING, false);
  display->set(ALPHA_BLEND, true);
  render(LIT_OBJECT);
  display->set(ALPHA_BLEND, false);
  // render all of the opaque unlit objects
  display->set(LIGHTING, true);
  render(OPAQUE_OBJECT);
  // render all of the translucent unlit objects
  display->set(ALPHA_BLEND, true);
  render(TRANSLUCENT_OBJECT);
  display->set(ALPHA_BLEND, false);
  // render all of the hud and text objects
  display->beginDraw(HUD_ALPHA);
  render(ALL_HUDS);
  display->endDraw();
  // finished the graphics part
  display->endDrawFrame();

  // render all of the sounds
  render(ALL_SOUNDS);
}

// render draws the coordinator elements for the specified Category
//
void Coordinator::render(Category category) {
  ViewFrustum viewFrustum(view * projection);

  switch (category) {
  case ALL_HUDS:
    // draw all huds
    for (unsigned i = 0; i < hud.size(); i++)
      if (hud[i] && hud[i]->isOn())
        hud[i]->render();
    for (unsigned i = 0; i < text.size(); i++)
      if (text[i] && text[i]->getHUD() && text[i]->getHUD()->isOn())
        text[i]->render();
    break;
  case ALL_SOUNDS:
    // render all sounds
    for (unsigned i = 0; i < sound.size(); i++)
      if (sound[i]) 
        sound[i]->render();
    break;
  default:
    // draw objects that only belong to category
    for (unsigned i = 0; i < object.size(); i++)
      if (object[i] && object[i]->belongsTo(category) && 
        (category == SPRITE || viewFrustum.contains(object[i]->position(), 
        object[i]->radius())))
        object[i]->render();
  }
}

// resize resizes the window and the user interface
//
void Coordinator::resize() {

  if (active && userInput->getWindowMode()) {
    window->resize();
    createProjection();
  }
}

// suspend suspends all of the design items
//
void Coordinator::suspend() {

  for (unsigned i = 0; i < texture.size(); i++)
    if (texture[i])
      texture[i]->suspend();

  for (unsigned i = 0; i < light.size(); i++)
    if (light[i])
      light[i]->suspend();

  for (unsigned i = 0; i < sound.size(); i++)
    if (sound[i]) 
      sound[i]->suspend();

  for (unsigned i = 0; i < graphic.size(); i++)
    if (graphic[i]) 
      graphic[i]->suspend();

  for (unsigned i = 0; i < text.size(); i++)
    if (text[i])
      text[i]->suspend();

  display->suspend();
  userInput->suspend();
  audio->suspend();
  active = false;
}

// restore restores all of the design items and initializes the timers
//
void Coordinator::restore() {

  now = window->time();
  userInput->restore();
  display->restore();
  createProjection();
  Light::set(DITHERING, true);
  Light::set(SPECULARITY, true);
  audio->restore();

  for (unsigned i = 0; i < light.size(); i++)
    if (light[i])
      light[i]->restore();
  for (unsigned i = 0; i < sound.size(); i++)
    if (sound[i]) 
      sound[i]->restore();
  for (unsigned i = 0; i < hud.size(); i++)
    if (hud[i])
      hud[i]->restore();
  for (unsigned i = 0; i < text.size(); i++)
    if (text[i])
      text[i]->restore();

  lastCameraToggle = now;
  lastHUDToggle    = now;
  lastAudioUpdate  = now;
  lastUpdate       = now;
  active           = true;
}

// release releases the design items
//
void Coordinator::release() {

  for (unsigned i = 0; i < texture.size(); i++)
    if (texture[i])
      texture[i]->release();

  for (unsigned i = 0; i < graphic.size(); i++)
    if (graphic[i]) 
      graphic[i]->release();

  for (unsigned i = 0; i < text.size(); i++)
    if (text[i])
      text[i]->release();

  for (unsigned i = 0; i < sound.size(); i++)
    if (sound[i])
      sound[i]->release();

  Light::dealloc();
  display->release();
  userInput->release();
  audio->release();
  window->release();
}

// destructor deletes all of the coordinator elements
//
Coordinator::~Coordinator() {

  for (unsigned i = 0; i < object.size(); i++)
    if (object[i]) 
      object[i]->Delete();

  Texture::disposeLibrary();

  for (unsigned i = 0; i < light.size(); i++)
    if (light[i])
      light[i]->Delete();

  for (unsigned i = 0; i < camera.size(); i++)
    if (camera[i])
      camera[i]->Delete();

  for (unsigned i = 0; i < sound.size(); i++)
    if (sound[i])
      sound[i]->Delete();

  for (unsigned i = 0; i < graphic.size(); i++)
    if (graphic[i])
      graphic[i]->Delete();

  for (unsigned i = 0; i < text.size(); i++)
    if (text[i]) 
      text[i]->Delete();

  coordinator = nullptr;
}

void Coordinator::setViewport(const Viewport& viewport) {
  display->setViewport(viewport);
}

void Coordinator::createProjection() {
  projection = ::projection(fov, (float)window->getClientWidth() / window->getClientHeight(), nearcp, farcp);
  display->setProjection(&projection);
}

void Coordinator::remove(iObject* o)  { 
  ::remove(object, o);
  const std::vector<Frame*>& children = o->getChildren();
  for (int i = ((int)children.size()) - 1; i >= 0; --i) {
    remove((iObject*)children[i]);
  }
}

void Coordinator::setAPIObjects(APIObjects* objects) {
  window = objects->window;
  display = objects->display;
  audio = objects->audio;
  userInput = objects->userInput;
}