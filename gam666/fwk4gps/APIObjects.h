#ifndef _APIOBJECTS_H_
#define _APIOBJECTS_H_

class iAPIWindow;
class iAPIUserInput;
class iAPIDisplay;
class iAPIAudio;

struct APIObjects {
  iAPIWindow*            window;           // points to the window object
  iAPIUserInput*         userInput;        // points to the user input object
  iAPIDisplay*           display;          // points to the display object
  iAPIAudio*             audio;            // points to the audio object

  APIObjects(void* handle, int show);
  ~APIObjects();
};

#endif